/* 
   config.h
   
   Purpose: head file of all configurations
   Author: Hang Qian
   Created: 2013-01-30
   Last modified: 2013-01-30

 */

#ifndef _CONFIG_H
#define _CONFIG_H

#define CUR_DIR "."

#define MAXLEN 4096
#define MAXARGS 100
#define BUFSIZE 512
#define TRUE 1
#define FALSE 0
#define BUILTIN_CMD_CNT 3


char *SYS_PATH;
char *PATH;

char** BUILTIN_CMD;

int STDOUT_BACKUP;
int STDIN_BACKUP;
int STDERR_BACKUP;

typedef struct command_t{
  int argc;   
  char** argv;

  char* redirect_input;
  char* redirect_output;
  char* redirect_error;

  struct command_t* prev;
  struct command_t* next;
} cmd_t;

typedef struct command_list_t{
  int count;
  cmd_t* head;
} cmd_list_t;

typedef enum { READ, WRITE, READ_WRITE} pipeo_t;

#endif
