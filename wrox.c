#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



typedef struct Line {
  int original;
  char* text;
} Line;
Line lines[1000];
int nextLineId = 0;

typedef struct Val {
  enum { Int, Str } type;
  union {
    int i;
    char* s;
  } as;
} Val;

typedef struct Ins {
  enum { Value, Add } type;
  union {
    int i;
    Val v;
  } as;
  int lineId;
  int colStart;
  int colEnd;
} Ins;

typedef struct Program {
  Ins* ins;
  int len;
} Program;

void execute(Program program) {
/*
  static void* insLabels[] = {
    &&ins_val, &&ins_exe
  };
  static void* ops[] = {
    &&op_inc_1, &&op_dec_1
  };
  int f = program.ins;
  #define NEXT_INSTRUCTION() goto *insLabels[(++f)->]
*/
  
}


typedef struct Symbol {
  enum { LParen, RParen, Text } type;
  char* text;
} Symbol;

Program parse(char* input, int len) {
  int maxsymbols = 1000;
  Symbol symbols[maxsymbols];
  int numSym = 0;

  for (int c = 0; c < len; ++c) {
    char ch = input[c];
    if (ch <= ' ') {
      input[c] = '\0';
    } else if (ch == '(' || ch == ')') {
      symbols[numSym++] = (Symbol){ ch == '(' ? LParen : RParen, 0 };
      input[c] = '\0';
    } else {
      symbols[numSym++] = (Symbol){ Text, input + c };
      while (ch != '(' && ch != ')' && ch > ' ')
        ch = input[++c];
      --c;
    }
  }

  int maxins = 1000;
  Ins* ins = malloc(maxins * sizeof(Ins));
  int numIns = 3;

  memcpy(ins, (Ins[]){
    (Ins){Value, .as.v = (Val){Int, .as = { 2 }}, 0, 0, 0},
    (Ins){Value, .as.v = (Val){Int, .as = { 2 }}, 0, 0, 0},
    (Ins){Add, .as.i = 2, 0, 0, 0 }
  }, sizeof(Ins) * 3);

  ins = realloc(ins, numIns * sizeof(Ins));
  return (Program){ ins, numIns };
}


int prompt(char* input, int maxlen) {
  printf("❯ ");
  int n = 0;
  do {
    if ((input[n < maxlen ? n++ : 0] = getchar()) != '\n')
      continue;
    if (n < maxlen)
      return n;
    printf("Input too long.\n");
    return 0;
  } while (true);
}

int main(int argc, char *argv[]) {
  int maxlen = 1000, len = 0;
  char* input = malloc(maxlen);

  if (argc > 1) {
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
      perror(argv[1]);
      return 1;
    }
    char ch;
    while((ch = fgetc(fp)) != EOF) {
      input[len++] = ch;
      if (len == maxlen) {
        printf("Source file too long.\n");
        return 1;
      }
    }

    fclose(fp);
    return 0;
  }

  while (true) {
    if (!len && !(len = prompt(input, maxlen)))
      continue;
    char* inputCopy = malloc(len);
    memcpy(inputCopy, input, len);
    Program program = parse(input, len);
    execute(program);
    free(program.ins);
    len = 0;
  }
  free(input);
}