#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "interpreter.h"

void help();

int main(int argc, char *argv[]) {
  FILE *program_file = NULL;
  FILE *program_dump = NULL;
  FILE *in = stdin;
  FILE *out = stdout;
  bool custom_in = false;
  bool custom_out = false;
  bool program_file_loaded = false;
  char c;
  
  while (optind < argc) {
    if ((c = getopt(argc, argv, "ho:i:d:")) != -1) {
      switch (c) {
        case 'h':
          help(argv[0]);
          exit(0);
          break;
        case 'o':
          out = fopen(optarg, "w");
          custom_out = true;
          break;
        case 'i':
          in = fopen(optarg, "r");
          custom_in = true;
          break;
        case 'd':
          program_dump = fopen(optarg, "w");
          break;
      }
    } else {
      program_file = fopen(argv[optind], "r");
      program_file_loaded = true;
      optind++;
    }
    
  }

  if (!program_file_loaded) {
    printf("Missing argument: filename\n");
  } else {
    if(!in) {
      printf("Input file not found.\n");
    }
    if(!out) {
      printf("Output file not found.\n");
    }

    if(program_file) {
      load_program(program_file);

      if(is_program_valid()) {
        if(program_dump) {
          dump_instructions(program_dump);
          fclose(program_dump);
        }
        run(in, out);
      }
      else printf("Invalid syntax: brackets '[' ']' are not balanced.\n");
    
      unload_program();

      fclose(program_file);
    } else {
      printf("File not found.\n");
    }
  }

  if(custom_in) fclose(in);
  if(custom_out) fclose(out);
}

void help(char *program_name) {
  printf("Usage: %s "
  "[-h] [-oid <filename>] "
  "filename"
  "\n",
  program_name);

  printf(
  " -h             view this help message\n"
  " -i <filename>  stdin file\n"
  " -o <filename>  stdout file\n"
  " -d <dilename>  dump cleaned brainfuck instructions\n");
}