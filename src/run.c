#include "../include/run.h"

int STDOUT_REDIR = -1;
int STDIN_REDIR = -1;
int STDERR_REDIR = -1;

int fid_out;
int fid_in;
int fid_err;


void run_cmd(cmd_list_t* cmd_list){

  cmd_t* cur_cmd = cmd_list->head;

  while (cur_cmd != NULL){

    if (is_builtin_cmd(cur_cmd->argv[0])){
      run_builtin_cmd(cur_cmd);
    }
    else{
      run_external_cmd(cur_cmd);
    }
    cur_cmd = cur_cmd->next;
  }
  return;
}

void run_external_cmd(cmd_t* cmd){

  pid_t child;
  int statloc;



  char **path_list = 0;
  char *path_name = 0;

  redirect_io(cmd);

  if ((child = fork()) < 0){
    perror("ERROR:fork error!\n");
    exit(-1);
  }
  else if ( child == 0){    /* in child */

    int ret = 0;
    path_list = get_path_list(PATH);
    if (is_path(cmd->argv[0])){    /* user input a complete path */
      ret = execv(cmd->argv[0],cmd->argv);
      if (ret == -1){
        /* remain silence */
        fprintf(stdout,"Error: no such file or directory: %s\n",cmd->argv[0]);
      }
    }
    else{
      int i = 0;
      while (path_list[i] != NULL){
        if (path_name != NULL){
          free(path_name);
        }
        path_name = get_path_name(cmd->argv[0],path_list[i++]);
        /* debug_info(path_name); */
        ret = execv(path_name,cmd->argv);
      }
    }
    int i = 0;
    while (path_list[i] != NULL){
      free(path_list[i]);
      i++;
    }

    if (path_list != NULL){
      free(path_list);
    }

  }
  else {
    /* debug_info("IN PARENT"); */
    waitpid(child,&statloc,0);
    if (statloc == 139){
      fprintf(stderr,"Error: command not found: %s\n",cmd->argv[0]);
    }
    restore_io();
  }

  if (path_name != NULL ){
    free(path_name);
  }
  return;
}

void run_builtin_cmd(cmd_t* cmd){
  char *name = cmd->argv[0];
  if (strcmp(name,"exit") == 0){
    FORCE_EXIT = TRUE;
    return;
  }
  else if (strcmp(name,"cd") == 0) {
    char *dir = getenv("HOME");
    /* debug_info(dir); */
    if (cmd->argc > 1){
      dir = malloc(sizeof(char) * (strlen(cmd->argv[1]) + 1));
      strcpy(dir,cmd->argv[1]);
    }
    int i = chdir(dir);
    if (i != 0){
      perror("Error:");
    }
    return;
  }
  else if (strcmp(name,"path") == 0){
    /* print_cmd(cmd); */
    if (cmd->argc == 1){
      /* bare "path" command will printn the current PATH */
      fprintf(stdout,"%s\n",PATH);
      fflush(stdout);
      return;
    }
    else if (cmd->argc == 2){
      /* that's illegal,we do nothing */
      return;
    }
    else if (strcmp(cmd->argv[1],"+") == 0){
      /* path + <dir>: add <dir> to PATH */
      char* tmp = malloc(sizeof(char) * (strlen(PATH) + 1));
      strcpy(tmp,PATH);
      path_concatenate(PATH,tmp,cmd->argv[2]);
      free(tmp);
      return;
    }
    else if (strcmp(cmd->argv[1],"-") == 0){
      PATH = path_tailor(PATH,cmd->argv[2]);
      return;
    }
  }
}

int is_builtin_cmd(char* name){
  /* int i = 0; */
  /* while (i < BUILTIN_CMD_CNT){ */
  /*   if (strcmp(BUILTIN_CMD[i],name) == 0){ */
  /*     return TRUE; */
  /*   } */
  /*   i++; */
  /* } */
  if (strcmp(name,"cd") == 0 ||
      strcmp(name,"exit") == 0 ||
      strcmp(name,"path") == 0)
    return TRUE;
  return FALSE;
}

void redirect_io(cmd_t* cmd){
  if (cmd->redirect_output != 0 ){
    STDOUT_REDIR = dup(1);
    if( (fid_out = open(cmd->redirect_output, O_WRONLY | O_CREAT, 0644)) == -1){
      perror("Error opening the file:");
      exit(-1);
    }
    dup2(fid_out,1);
  }
  if (cmd->redirect_input != 0 ){
    STDIN_REDIR = dup(0);
    if( (fid_in = open(cmd->redirect_input, O_RDONLY,0644)) == -1){
      perror("Error opening the file:");
      exit(-1);
    }
    dup2(fid_in,0);
  }
  if (cmd->redirect_error != 0 ){
    STDIN_REDIR = dup(2);
    if( (fid_err = open(cmd->redirect_error, O_WRONLY | O_CREAT, 0644)) == -1){
      perror("Error opening the file:");
      exit(-1);
    }
    dup2(fid_in,2);
  }
}

void restore_io(){
  if (STDOUT_REDIR != -1){
    dup2(STDOUT_REDIR,1);
    close(fid_out);
    STDOUT_REDIR = -1;
  }
  if (STDIN_REDIR != -1){
    dup2(STDIN_REDIR,0);
    close(fid_in);
    STDIN_REDIR = -1;
  }
  if (STDERR_REDIR != -1){
    dup2(STDERR_REDIR,2);
    close(fid_err);
    STDERR_REDIR = -1;
  }
}
