/*
  Run
  
  The runtime of myshell

  Author: Hang Qian
*/
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

/*
  run the command list (connected with pipe)
*/
int run_cmd(cmd_list_t* cmd_list);

/*
  run builtin command 
*/
void run_builtin_cmd(cmd_t* cmd);
/*
  run external command
  fd_in,fd_out,fd_err is for pipe and redirection
*/
void run_external_cmd(cmd_t* cmd,int fd_in,int fd_out,int fd_err);
/* tell if a command is builtin*/
int is_builtin_cmd(char* name);
/* redirect the command IO */
void redirect_io(cmd_t* cmd,int fd_in,int fd_out,int fd_err);
/* restore the command IO */
void restore_io(int fd_in, int fd_out, int fd_err);

#endif
