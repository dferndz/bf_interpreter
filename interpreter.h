#ifndef _brainf_interpreter_
#define _brainf_interpreter_

#include <stdio.h>

#define DATA_STACK_SIZE 30000
#define true 1
#define false 0

typedef unsigned char bool;
typedef unsigned char byte_t;

void load_program(FILE*);
void unload_program();
void run(FILE* in, FILE* out);
void cli(FILE *in, FILE *out);
void dump_instructions(FILE *);
bool is_program_valid();
bool is_valid_ins(byte_t c);

#endif