#ifndef _brainf_interpreter_
#define _brainf_interpreter_

#include <stdio.h>

#define _BF_INTERPRETER_ "BF Interpreter"
#define _BF_INTERPRETER_VERSION_ "0.1.0"

#define DATA_STACK_SIZE 30000
#define true 1
#define false 0

#define ADD_METRICS 0x1

typedef unsigned char bool;
typedef unsigned char byte_t;

void load_program(FILE*);
void unload_program();
void run(FILE* in, FILE* out, byte_t opts);
void cli(FILE *in, FILE *out);
void dump_instructions(FILE *);
bool is_program_valid();
bool is_valid_ins(byte_t c);

#endif