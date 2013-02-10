/*
  io

  contains all functions with I/O

*/

#ifndef _IO_H
#define _IO_H

#include "stdlib.h"
#include <unistd.h>
#include <stdio.h>
/* #include <stdio_ext.h> */
#include <errno.h>


#include "utils.h"

int FORCE_EXIT;
int READING;

void print_promt();
void get_cmd_line(char *);

void BEGIN_READ(void);
void END_READ(void);

void debug_info(const char* info);

#endif
