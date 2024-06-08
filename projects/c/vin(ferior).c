#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define MAX_ROWS 1000
#define MAX_COLS 300

void clear() { printf("\033[H\033[J"); }

void save(char *filename, FILE *file, char *filedata) {
  truncate(filename, 0);
  fprintf(file, "%s", filedata);
}

void print(int cursor, char *filedata, int filesize) {
  /* printf("%s", filedata); */
  for (int i = 0; i <= filesize; i++) {
    char c = filedata[i];
    if (i == cursor) {
      printf("%s", "█");
    } else {
      printf("%c", c);
    }
  }
}

int main(int argc, char *argv[]) {
  struct termios orig_termios, new_termios;
  tcgetattr(STDIN_FILENO, &orig_termios);
  new_termios = orig_termios;
  new_termios.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

  char *filename = argv[1];
  FILE *file = fopen(filename, "a+");

  if (file == NULL) {
    return 1;
  }

  fseek(file, 0, SEEK_END);
  long filesize = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *filedata = (char *)malloc((filesize + 1) * sizeof(char));

  int rows = 0;
  fread(filedata, sizeof(char), filesize, file);

  int cursor = 0;
  int mode = 0;

  clear();
  print(cursor, filedata, filesize);

  while (1) {
    char c = getchar();
    if (mode == 0) {
      if (c == 'w') {
        save(filename, file, filedata);
      } else if (c == 'a') {
        mode = 1;
      } else {
        if (c == 'q') {
          fclose(file);
          return 0;
        }
        if (c == 'k') {
          int currentLineStart = cursor;
          while (currentLineStart > 0 && filedata[currentLineStart] != '\n') {
            currentLineStart--;
          }

          int previousLineStart = currentLineStart - 1;
          while (previousLineStart > 0 && filedata[previousLineStart] != '\n') {
            previousLineStart--;
          }

          if (previousLineStart >= 0) {
            int previousLineLength = currentLineStart - previousLineStart;
            int offset = cursor - currentLineStart;
            cursor = previousLineStart +
                     ((offset > previousLineLength) ? previousLineLength - 2
                                                    : offset - 1) +
                     1;
          }
        }
        if (c == 'j') {
          int currentLineStart = cursor;
          while (currentLineStart > 0 && filedata[currentLineStart] != '\n') {
            currentLineStart--;
          }

          int nextLineStart = currentLineStart + 1;
          while (nextLineStart < filesize && filedata[nextLineStart] != '\n') {
            nextLineStart++;
          }

          int nextNextLineStart = nextLineStart + 1;
          while (nextNextLineStart < filesize &&
                 filedata[nextNextLineStart] != '\n') {
            nextNextLineStart++;
          }

          if (nextLineStart < filesize) {
            int nextLineLength = nextNextLineStart - nextLineStart;
            int offset = cursor - currentLineStart;
            cursor =
                nextLineStart +
                ((offset > nextLineLength) ? nextLineLength - 2 : offset - 1) +
                1;
            if (filedata[cursor] == '\n') {
              cursor += 1;
            }
          }
        }
        if (c == 'l') {
          cursor += 1;
          if (filedata[cursor] == '\n') {
            cursor += 1;
          }
        }
        if (c == 'h') {
          cursor -= 1;
          if (filedata[cursor] == '\n') {
            cursor -= 1;
          }
        }
        clear();
        print(cursor, filedata, filesize);
      }
    } else if (mode == 1) {
      clear();
      print(cursor, filedata, filesize);
    }
  }

  fclose(file);

  return 0;
}
