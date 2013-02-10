#ifndef _RUN_H
#define _RUN_H

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "config.h"
#include "io.h"
#include "parser.h"


void run_cmd(cmd_list_t* cmd_list);
/* run builtin command */
void run_builtin_cmd(cmd_t* cmd);
void run_external_cmd(cmd_t* cmd);
int is_builtin_cmd(char* name);
void redirect_io(cmd_t* cmd);
void restore_io();

#endif
