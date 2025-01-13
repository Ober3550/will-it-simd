const fs = require("fs");

let total_operations = 0;
function matmul(a, b) {
  if (a[0].length != b.length) {
    console.log("Matrix dimensions do not match");
    return undefined;
  }
  const result = [];
  for (let i = 0; i < a.length; i++) {
    const row = [];
    for (let j = 0; j < b[0].length; j++) {
      let sum = 0;
      for (let k = 0; k < a[0].length; k++) {
        sum += a[i][k] * b[k][j];
        total_operations++;
      }
      row.push(sum);
    }
    result.push(row);
  }
  return result;
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
            matrix.push(row);
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
        matrix.push(row);
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
      console.log("Dimensions Result:", dim_x, dim_y);
      console.log("Total Operations:", total_operations);
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
  console.log("Reading files");
  const matrix_a = await readFile(filename_a);
  const matrix_b = await readFile(filename_b);
  const end_read = new Date();
  console.log("Read Time:", end_read - start_read, "ms");
  const result = matmul(matrix_a, matrix_b);
  if (result != undefined) {
    const start_write = new Date();
    console.log("Writing file");
    await writeFile(filename_result, result);
    const end_write = new Date();
    console.log("Write Time:", end_write - start_write, "ms");
  }
}

main(process.argv[2], process.argv[3], process.argv[4]);
