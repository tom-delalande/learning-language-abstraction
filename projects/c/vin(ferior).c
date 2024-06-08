#include <stdio.h>

#define MAX_ROWS 100
#define MAX_COLS 100

void clear() { printf("\033[H\033[J"); }

void print(int cursorRow, int cursorCol, int numRows,
           char filedata[MAX_ROWS][MAX_COLS]) {

  int rowCounter = 0;
  for (int i = 0; i <= numRows; i++) {
    char *data = filedata[i];
    if (rowCounter == cursorRow) {
      char clonedData[MAX_COLS];
      for (int j = 0; j < MAX_COLS; j++) {
        clonedData[j] = data[j];
      }
      clonedData[cursorCol] = '|';
      printf("%s", clonedData);
    } else {
      printf("%s", filedata[i]);
    }
    rowCounter++;
  }
}

void write() {}

int main(int argc, char *argv[]) {
  char *filename = argv[1];
  FILE *file = fopen(filename, "a+");

  if (file == NULL) {
    return 1;
  }

  fseek(file, 0, SEEK_SET);
  char filedata[MAX_ROWS][MAX_COLS];

  int rows = 0;
  while (fgets(filedata[rows], MAX_COLS, file) != NULL && rows < MAX_ROWS) {
    rows++;
  }

  int column = 0;
  int row = 0;
  int mode = 0;

  clear();
  print(row, column, rows, filedata);

  while (1) {
    char c = getchar();
    if (mode == 0) {
      if (c == 'w') {
        fseek(file, 0, SEEK_SET);
        for (int i = 0; i < rows; i++) {
          fprintf(file, "%s\n", filedata[i]);
        }
      } else if (c == 'a') {
        mode = 1;
      } else {
        if (c == 'q') {
          fclose(file);
          return 0;
        }
        if (c == 'k') {
          row -= 1;
        }
        if (c == 'j') {
          row += 1;
        }
        if (c == 'l') {
          column += 1;
        }
        if (c == 'h') {
          column -= 1;
        }
        clear();
        print(row, column, rows, filedata);
      }
    } else if (mode == 1) {
      if (c == '~') {
        mode = 0;
      } else {
        filedata[row][column] = c;
        column += 1;
        clear();
        print(row, column, rows, filedata);
      }
    }
  }

  fclose(file);

  return 0;
}
