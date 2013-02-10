#include "../include/run.h"

int STDOUT_REDIR = -1;
int STDIN_REDIR = -1;
int STDERR_REDIR = -1;


void run_cmd(cmd_list_t* cmd_list){
  if(cmd_list == NULL || cmd_list->count == 0){
    return;
  }

  cmd_t* cur_cmd = cmd_list->head;
  int fd_in = -1, fd_out, fd_err = -1;
  while (cur_cmd != NULL){

    print_cmd(cur_cmd);
    int fd_pipe[2];
    if (cur_cmd->next != NULL){
      /* if there's a next command */
      pipe(fd_pipe);
      fd_out = fd_pipe[1];
    }
    else{
      fd_out = -1;
    }
    if (is_builtin_cmd(cur_cmd->argv[0])){
      run_builtin_cmd(cur_cmd);
    }
    else{
      run_external_cmd(cur_cmd,fd_in,fd_out,fd_err);
    }

    close(fd_in);
    close(fd_out);
    close(fd_err);
    fd_in = fd_pipe[0];
    restore_io(fd_in,fd_out,fd_err);
    cur_cmd = cur_cmd->next;
  }
  return;
}

void run_external_cmd(cmd_t* cmd,int fd_in,int fd_out, int fd_err){

  pid_t child;
  int statloc;

  char *path_name = malloc(sizeof(char) * MAXLEN);
  /* char** path_list = malloc(sizeof(char*) * MAXLEN); */
  /* get_path_list(path_list,PATH); */

  if ((child = fork()) < 0){
    perror("ERROR:fork error!\n");
    exit(-1);
  }
  else if ( child == 0){    /* in child */
    if (fd_in != -1 && fd_in != 0){
      /* STDIN_REDIR = dup(0); */
      dup2(fd_in,0);
      close(fd_in);
    }
    if (fd_out != -1 && fd_out != 1){
      /* STDOUT_REDIR = dup(1); */
      dup2(fd_out,1);
      close(fd_out);
    }
    redirect_io(cmd,fd_in,fd_out,fd_err);
    int ret = 0;
    if (is_path(cmd->argv[0])){    /* user input a complete path */
      ret = execv(cmd->argv[0],cmd->argv);
      if (ret == -1){
        /* remain silence */
        fprintf(stdout,"Error: no such file or directory: %s\n",cmd->argv[0]);
      }
    }
    else{
      char* tmp = malloc(sizeof(char) * (strlen(PATH) + 1));
      strcpy(tmp,PATH);
      char* cur_path;
      int i = 0;
      cur_path = get_a_path(tmp,i);
      while (cur_path != NULL){
        path_name = get_path_name(cmd->argv[0],cur_path);
        printf("%d %s\n",i,path_name);
        ret = execv(path_name,cmd->argv);

        if (tmp != NULL){
          free(tmp);
          tmp = NULL;
        }

        tmp = malloc(sizeof(char) * (strlen(PATH) + 1));
        strcpy(tmp,PATH);
        cur_path = get_a_path(tmp,++i);
      }

    }
  }
  else {
    /* debug_info("IN PARENT"); */
    waitpid(child,&statloc,0);
    if (statloc == 139){
      fprintf(stderr,"Error: command not found: %s\n",cmd->argv[0]);
    }

  }

  if (path_name != NULL){
    free(path_name);
    path_name  = NULL;
  }
  /* if(path_list != NULL){ */
  /*   int i = 0; */
  /*   while (path_list[i] != NULL){ */
  /*     free(path_list[i]); */
  /*     path_list[i] = 0; */
  /*     i++; */
  /*   } */
  /*   free(path_list); */
  /*   path_list = NULL; */
  /* } */
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
      tmp = NULL;
      return;
    }
    else if (strcmp(cmd->argv[1],"-") == 0){
      PATH = path_tailor(PATH,cmd->argv[2]);
      return;
    }
  }
}

int is_builtin_cmd(char* name){
  if (strcmp(name,"cd") == 0 ||
      strcmp(name,"exit") == 0 ||
      strcmp(name,"path") == 0)
    return TRUE;
  return FALSE;
}

void redirect_io(cmd_t* cmd,int fd_in,int fd_out,int fd_err){
  if (cmd->next == NULL  && cmd->redirect_output != 0 ){
    STDOUT_REDIR = dup(1);
    if( (fd_out = open(cmd->redirect_output, O_WRONLY | O_CREAT, 0644)) == -1){
      perror("Error opening the file:");
      exit(-1);
    }
    dup2(fd_out,1);
  }
  if (cmd->prev == NULL && cmd->redirect_input != 0 ){
    STDIN_REDIR = dup(0);
    if( (fd_in = open(cmd->redirect_input, O_RDONLY,0644)) == -1){
      perror("Error opening the file:");
      exit(-1);
    }
    dup2(fd_in,0);
  }
  if (cmd->redirect_error != 0 ){
    STDIN_REDIR = dup(2);
    if( (fd_err = open(cmd->redirect_error, O_WRONLY | O_CREAT, 0644)) == -1){
      perror("Error opening the file:");
      exit(-1);
    }
    dup2(fd_err,2);
  }
}

void restore_io(int fd_in, int fd_out, int fd_err){
  if (STDOUT_REDIR != -1){
    dup2(STDOUT_REDIR,1);
    STDOUT_REDIR = -1;
  }
  if (STDIN_REDIR != -1){
    dup2(STDIN_REDIR,0);
    STDIN_REDIR = -1;
  }
  if (STDERR_REDIR != -1){
    dup2(STDERR_REDIR,2);
    STDERR_REDIR = -1;
  }
}

