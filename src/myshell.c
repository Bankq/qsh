/*
  myshell.c
  
  Description: main entrance of myshell
  Author: Hang Qian (hq2124@columbia.edu)

*/
 
#include "../include/myshell.h"


/*
  init:

  Load environment for our shell
  Print the promt and here we go!

 */

void init(){
  
  FORCE_EXIT = FALSE;
  READING = FALSE;

  /* get system user path */
  SYS_PATH = getenv("PATH");

  if ((PATH = malloc((strlen(SYS_PATH)+3)*sizeof(char))) == NULL){
    perror("MEMORY ALLOCATION FAIL!\n");
    exit(-1);
  }
  /* concatenate with current directory */
  path_concatenate(PATH,CUR_DIR,SYS_PATH);
  
  BUILTIN_CMD = malloc(sizeof(char*) * BUILTIN_CMD_CNT);
  BUILTIN_CMD[0] = "cd";
  BUILTIN_CMD[1] = "path";
  BUILTIN_CMD[2] = "exit";

}  

int main(){

  init();
  char *cmd_line = malloc(sizeof(char) * MAXLEN);
  while (!FORCE_EXIT){
    print_promt();
    get_cmd_line(cmd_line);
    cmd_list_t *cmd_list = malloc(sizeof(cmd_list_t));
    parse_cmd(cmd_list,cmd_line);
    run_cmd(cmd_list);
    free_cmd_list(cmd_list);
  }
  if (cmd_line != NULL){
    free(cmd_line);
    cmd_line = NULL;
  }
  return 0;
}
