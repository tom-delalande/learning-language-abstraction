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

void print(int cursor, char *filedata, int filesize, int mode,
           char commands[2]) {
  for (int i = 0; i <= filesize; i++) {
    char c = filedata[i];
    if (i == cursor) {
      printf("\033[7m%c\033[0m", c);
    } else {
      printf("%c", c);
    }
  }

  if (mode == 0) {
    printf("\nNORMAL");
  }
  if (mode == 1) {
    printf("\nINSERT");
  }
  if (mode == 2) {
    printf("\nCOMMAND");
    printf(":%s", commands);
  }
}

int moveCursorUp(int cursor, char *filedata) {
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
    cursor =
        previousLineStart +
        ((offset > previousLineLength) ? previousLineLength - 1 : offset - 1);
    if (filedata[cursor] == '\n') {
      cursor += 1;
    }
  }
  return cursor;
}

int moveCursorDown(int cursor, char *filedata, int filesize) {
  int currentLineStart = cursor;
  while (currentLineStart > 0 && filedata[currentLineStart] != '\n') {
    currentLineStart--;
  }

  int nextLineStart = currentLineStart + 1;
  while (nextLineStart < filesize && filedata[nextLineStart] != '\n') {
    nextLineStart++;
  }

  int nextNextLineStart = nextLineStart + 1;
  while (nextNextLineStart < filesize && filedata[nextNextLineStart] != '\n') {
    nextNextLineStart++;
  }

  if (nextLineStart < filesize) {
    int nextLineLength = nextNextLineStart - nextLineStart;
    int offset = cursor - currentLineStart;
    cursor = nextLineStart +
             ((offset > nextLineLength) ? nextLineLength - 1 : offset);
    if (filedata[cursor] == '\n') {
      cursor += 1;
    }
  }
  return cursor;
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

  // 0: Normal mode
  // 1: Insert mode
  // 2: Command mode
  int mode = 0;

  int commandsSize = 0;
  char commands[2];

  clear();
  print(cursor, filedata, filesize, mode, commands);

  while (1) {
    char c = getchar();
    if (mode == 0) {
      if (c == ':') {
        mode = 2;
      } else if (c == 'i') {
        mode = 1;
      } else {
        if (c == 'q') {
          fclose(file);
          return 0;
        }
        if (c == 'k') {
          cursor = moveCursorUp(cursor, filedata);
        }
        if (c == 'j') {
          cursor = moveCursorDown(cursor, filedata, filesize);
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
      }
    } else if (mode == 1) {
      if (c == 27) {
        mode = 0;
      } else if (c == 127) {
        for (int i = cursor - 1; i <= filesize; i++) {
          filedata[i] = filedata[i + 1];
        }
        cursor -= 1;
        filesize -= 1;
      } else {
        for (int i = filesize; i >= cursor + 1; i--) {
          filedata[i] = filedata[i - 1];
        }

        filedata[cursor + 1] = c;
        cursor += 1;
        filesize += 1;
      }
    } else if (mode == 2) {
      if (c == 10) {
        for (int i = 0; i < commandsSize; i++) {
          char command = commands[i];
          if (command == 'w') {
            save(filename, file, filedata);
          }
          if (command == 'q') {
            fclose(file);
            return 0;
          }
          mode = 0;
        }
      } else {
        commands[commandsSize] = c;
        commandsSize += 1;
      }
    }
    clear();
    print(cursor, filedata, filesize, mode, commands);
  }

  return 0;
}
