#include "interpreter.h"
#include <stdlib.h>
#include <stdio.h>

#define INC '+'
#define DEC '-'
#define SINC '>'
#define SDEC '<'
#define PUT '.'
#define GET ','
#define LSTART '['
#define LEND ']'

static byte_t data_stack[DATA_STACK_SIZE];
static byte_t *instruction_stack;
static size_t ins_stack_size;
static byte_t *buffer;

size_t total_ins;
int depth;
byte_t *ip;
byte_t *sp;

static void init_instruction_stack(size_t size) {
  ins_stack_size = size;
  instruction_stack = (byte_t*) malloc(size + 1);
}

static size_t file_size(FILE *file) {
  size_t s;
  fseek(file, 0L, SEEK_END);
  s = ftell(file);
  rewind(file);
  return s;
}

static void loop_search_end() {
  depth=1;
  do {
    ip++;
    if(*ip == LSTART) depth++;
    if(*ip == LEND) depth--;
  }
  while(depth != 0);
  ip++;
}

static void loop_search_start() {
  depth=1;
  do {
    ip--;
    if(*ip == LSTART) depth--;
    if(*ip == LEND) depth++;
  }
  while(depth != 0);
  ip++;
}

static inline void b_next_ins() {
  ip++;
}

static inline void b_inc() {
  (*sp)++;
  ip++;
}

static inline void b_dec() {
  (*sp)--;
  ip++;
}

static inline void b_sinc() {
  if((size_t)sp < (size_t)data_stack + DATA_STACK_SIZE - 1) sp++;
  ip++;
}

static inline void b_sdec() {
  if(sp > data_stack) sp--;
  ip++;
}

static inline void b_put(FILE *out) {
  putc(*sp, out);
  ip++;
}

static inline void b_get(FILE *in) {
  (*sp) = getc(in);
  ip++;
}

static inline void b_lstart() {
  if(*sp == 0) {
    loop_search_end();
  } else {
    ip++;
  }
}

static inline void b_lend() {
  if(*sp != 0) {
    loop_search_start();
  } else {
    ip++;
  }
}

static bool is_valid_char(char c) {
  switch (c) {
    case INC:
    case DEC:
    case SINC:
    case SDEC:
    case PUT:
    case GET:
    case LSTART:
    case LEND:
      return true;
  }
  return false;
}

static void print_metrics(FILE *out) {
  fprintf(out, 
  "------------------------------------\n"
  "Total instructions loaded:   %ld\n"
  "Total instructions executed: %ld\n"
  "------------------------------------\n",
  ins_stack_size, total_ins);
}

bool is_program_valid() {
  byte_t *p = instruction_stack;
  int d = 0;

  for(;(size_t)p < (size_t) instruction_stack + ins_stack_size; p++) {
    if(*p == LSTART) d++;
    if(*p == LEND) d--;

    if(d < 0) return false;
  }

  return !d;
}

void dump_instructions(FILE *out) {
  for(byte_t *ip = instruction_stack; (size_t)ip != (size_t)instruction_stack + ins_stack_size; ip++ ) {
    putc(*ip, out);
  }
}

void load_program(FILE *program) {
  size_t fs = file_size(program);
  size_t valid_ins = 0;

  buffer = (byte_t*) malloc(fs);

  fread(buffer, 1, fs, program);

  for(size_t i = 0; i < fs; i++) {
    if(is_valid_char(buffer[i])) valid_ins++;
  }

  init_instruction_stack(valid_ins);
  valid_ins = 0;

  for(size_t i = 0; i < fs; i++) {
    if(is_valid_char(buffer[i]))
      instruction_stack[valid_ins++] = buffer[i];
  }
  instruction_stack[valid_ins] = '\0';

  free(buffer);
  ip = instruction_stack;
  sp = data_stack;
  total_ins = 0;
  depth = 0;
}

void unload_program() {
  free(instruction_stack);
}

void run(FILE *in, FILE *out, byte_t opts) {
  while(*(ip) != '\0') {
    total_ins++;
    switch(*ip) {
      case INC:
        b_inc();
        break;
      case DEC:
        b_dec();
        break;
      case SINC:
        b_sinc();
        break;
      case SDEC:
        b_sdec();
        break;
      case PUT:
        b_put(out);
        break;
      case GET:
        b_get(in);
        break;
      case LSTART:
        b_lstart();
        break;
      case LEND:
        b_lend();
        break;
      default:
        b_next_ins();
    }
  }

  if(opts & ADD_METRICS) print_metrics(out);
}