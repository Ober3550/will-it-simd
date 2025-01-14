const std = @import("std");

const MatmulError = error{ MissingFilename, IncorrectDimensions, DataOutsideDimensionBounds, DataParseFloatFail };
const Matrix = struct {
    dimensions: [2]u32,
    data: []f32,
};

pub fn matrix_multiply(matrix_a: Matrix, matrix_b: Matrix, allocator: *const std.mem.Allocator) !Matrix {
    if (matrix_a.dimensions[1] != matrix_b.dimensions[0]) {
        return MatmulError.IncorrectDimensions;
    }
    const result_dimensions: [2]u32 = [_]u32{ matrix_a.dimensions[0], matrix_b.dimensions[1] };
    std.debug.print("Allocating result matrix with dimensions: {d}, {d}\n", .{ result_dimensions[0], result_dimensions[1] });
    var result_matrix = try allocator.alloc(f32, result_dimensions[0] * result_dimensions[1]);
    for (0..result_dimensions[0]) |i| {
        for (0..result_dimensions[1]) |j| {
            var sum: f32 = 0.0;
            for (0..matrix_a.dimensions[1]) |k| {
                sum += matrix_a.data[i * matrix_a.dimensions[0] + k] * matrix_b.data[k * matrix_b.dimensions[0] + j];
            }
            result_matrix[i * result_dimensions[0] + j] = sum;
        }
    }
    std.debug.print("Matrix multiplication done\n", .{});
    return Matrix{ .dimensions = result_dimensions, .data = result_matrix };
}

pub fn isWhitespace(c: u8) bool {
    return (c == ' ') or (c == '\t') or (c == '\n') or (c == '\r');
}

pub fn trim(str: []const u8) []const u8 {
    var start: u64 = 0;
    while ((start < str.len) and (isWhitespace(str[start]))) {
        start += 1;
    }
    var end: u64 = str.len;
    while ((end > 0) and (isWhitespace(str[end - 1]))) {
        end -= 1;
    }
    return str[start..end];
}

pub fn read_matrix(filename: []const u8, allocator: *const std.mem.Allocator) !Matrix {
    const file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    var buf_reader = std.io.bufferedReader(file.reader());
    var in_stream = buf_reader.reader();

    var buf: [4096]u8 = undefined;
    var first_line: bool = true;
    var dimensions: [2]u32 = undefined;
    var matrix: []f32 = undefined;
    var row: u32 = 0;
    var col: u32 = 0;
    while (try in_stream.readUntilDelimiterOrEof(&buf, '\n')) |line| {
        var iterator = std.mem.tokenizeScalar(u8, line, ',');
        if (first_line) {
            const dim_x = trim(iterator.next().?);
            const dim_y = trim(iterator.next().?);
            dimensions[0] = try std.fmt.parseInt(u32, dim_x, 0);
            dimensions[1] = try std.fmt.parseInt(u32, dim_y, 0);
            matrix = try allocator.alloc(f32, dimensions[0] * dimensions[1]);
            first_line = false;
            continue;
        }
        while (iterator.next()) |value| {
            matrix[row * dimensions[0] + col] = try std.fmt.parseFloat(f32, value);
            col += 1;
        }
        col = 0;
        row += 1;
    }
    return Matrix{ .dimensions = dimensions, .data = matrix };
}

pub fn writeMatrix(matrix: Matrix, filename: []const u8) !void {
    const file = try std.fs.cwd().createFile(filename, .{});
    defer file.close();

    var buf_writer = std.io.bufferedWriter(file.writer());
    try buf_writer.writer().print("{},{}\n", .{ matrix.dimensions[0], matrix.dimensions[1] });
    for (0..matrix.dimensions[0]) |i| {
        for (0..matrix.dimensions[1]) |j| {
            if (j == matrix.dimensions[1] - 1) {
                try buf_writer.writer().print("{d:0.}", .{matrix.data[i * matrix.dimensions[0] + j]});
            } else {
                try buf_writer.writer().print("{d:0.},", .{matrix.data[i * matrix.dimensions[0] + j]});
            }
        }
        try buf_writer.writer().print("\n", .{});
        try buf_writer.flush();
    }
    try buf_writer.flush();
}

pub fn printMatrix(matrix: Matrix) void {
    // Print matrixes that are loaded
    std.debug.print("Matrix dimensions: {}, {}\n", .{ matrix.dimensions[0], matrix.dimensions[1] });
    for (0..matrix.dimensions[0]) |i| {
        for (0..matrix.dimensions[1]) |j| {
            if (j == matrix.dimensions[1] - 1) {
                std.debug.print("{d:0.}", .{matrix.data[i * matrix.dimensions[0] + j]});
            } else {
                std.debug.print("{d:0.},", .{matrix.data[i * matrix.dimensions[0] + j]});
            }
        }
        std.debug.print("\n", .{});
    }
}

pub fn main() !void {
    // Initialize an allocator
    // Can be any one of them, this is the most basic one
    const allocator = std.heap.c_allocator;

    // Initialize arguments
    // Then deinitialize at the end of scope
    var argsIterator = try std.process.ArgIterator.initWithAllocator(allocator);
    defer argsIterator.deinit();

    // Skip executable
    _ = argsIterator.next();

    var matrix_a: Matrix = undefined;
    var matrix_b: Matrix = undefined;
    if (argsIterator.next()) |filename_a| {
        matrix_a = try read_matrix(filename_a, &allocator);
    }
    if (argsIterator.next()) |filename_b| {
        matrix_b = try read_matrix(filename_b, &allocator);
    }
    // printMatrix(matrix_a);
    // printMatrix(matrix_b);
    const matrix_result = try matrix_multiply(matrix_a, matrix_b, &allocator);
    // printMatrix(matrix_result);
    if (argsIterator.next()) |filename_result| {
        try writeMatrix(matrix_result, filename_result);
    }
}
