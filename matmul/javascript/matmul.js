const fs = require("fs");


function matmul(matrix_a, matrix_b_transpose) {
  let total_operations = 0;
  console.log("Result dimensions:", matrix_a.length, matrix_b_transpose.length);
  if (matrix_a[0].length != matrix_b_transpose.length) {
    console.log("Matrix dimensions do not match");
    return undefined;
  }
  const result = [];
  for (let i = 0; i < matrix_a.length; i++) {
    const row = [];
    for (let j = 0; j < matrix_b_transpose[0].length; j++) {
      let sum = 0;
      for (let k = 0; k < matrix_a[0].length; k++) {
        sum += matrix_a[i][k] * matrix_b_transpose[j][k];
        total_operations++;
      }
      row.push(sum);
    }
    result.push(Float32Array.from(row));
  }
  console.log("Total operations:", total_operations);
  return result;
}

function transpose(matrix) {
  for (let j = 0; j < matrix.length; j++) {
    for (let i = 0; i < matrix[0].length; i++) {
      if (i < j) {
        const temp = matrix[i][j];
        matrix[i][j] = matrix[j][i];
        matrix[j][i] = temp;
      }
    }
  }
}

function readFile(filename) {
  return new Promise((resolve, reject) => {
    const filestream = fs.createReadStream(filename);
    let dim_x = -1;
    let dim_y = -1;
    let matrix = [];
    let row = [];
    let character_buffer = "";
    filestream.on("data", (chunk) => {
      const chunk_str = Buffer.from(chunk).toString();
      for (const char of chunk_str) {
        if (char == ",") {
          if (dim_x == -1) {
            dim_x = parseFloat(character_buffer);
          } else {
            row.push(parseFloat(character_buffer));
          }
          character_buffer = "";
        } else if (char == "\n") {
          if (dim_y == -1) {
            dim_y = parseFloat(character_buffer);
          } else {
            row.push(parseFloat(character_buffer));
            matrix.push(Float32Array.from(row));
            row = [];
          }
          character_buffer = "";
        } else {
          character_buffer += char;
        }
      }
    });
    filestream.on("error", (err) => {
      reject(err);
    });
    filestream.on("end", () => {
      if (character_buffer.length > 0) {
        row.push(parseFloat(character_buffer));
      }
      if (row.length > 0) {
        matrix.push(Float32Array.from(row));
      }
      resolve(matrix);
    });
  });
}

function writeFile(filename, matrix) {
  return new Promise((resolve, reject) => {
    const filestream = fs.createWriteStream(filename);
    filestream.on("ready", () => {
      const dim_x = matrix[0].length;
      const dim_y = matrix.length;
      filestream.write(dim_x.toString());
      filestream.write(",");
      filestream.write(dim_y.toString());
      filestream.write("\n");
      for (let i = 0; i < dim_y; i++) {
        for (let j = 0; j < dim_x; j++) {
          filestream.write(matrix[i][j].toString());
          if (j < dim_x - 1) {
            filestream.write(",");
          }
        }
        filestream.write("\n");
      }
      filestream.close();
    });
    filestream.on("error", (err) => {
      reject(err);
    });
    filestream.on("close", () => {
      resolve();
    });
  });
}

async function main(filename_a, filename_b, filename_result) {
  const start_read = new Date();
  const matrix_a = await readFile(filename_a);
  const matrix_b = await readFile(filename_b);
  const end_read = new Date();
  console.log("Read time:", end_read - start_read, "ms");
  const start_transpose = new Date();
  transpose(matrix_b);
  const end_transpose = new Date();
  console.log("Transpose time:", end_transpose - start_transpose, "ms");
  const start_matmul = new Date();
  const result = matmul(matrix_a, matrix_b);
  const end_matmul = new Date();
  console.log("Matmul time:", end_matmul - start_matmul, "ms");
  if (result != undefined) {
    const start_write = new Date();
    console.log("Writing file");
    await writeFile(filename_result, result);
    const end_write = new Date();
    console.log("Write time:", end_write - start_write, "ms");
  }
}

main(process.argv[2], process.argv[3], process.argv[4]);
