#include "../include/parser.h"


void parse_cmd(cmd_list_t* cmd_list,char* cmd_line){
  /* debug_info(cmd_line); */
  /* debug_info("here"); */
  /* cmd_list_t* cmd_list = malloc(sizeof(cmd_list_t)); */

  int i = 0;
  char c;

  char* temp = malloc(MAXLEN);
  int temp_i = 0;
  int in_arg = FALSE;
  int in_redir_stdout = FALSE;
  int in_redir_stdin = FALSE;
  int in_redir_stderr_1= FALSE;
  int in_redir_stderr_2 = FALSE;

  cmd_t *cur_cmd = malloc(sizeof(cmd_t));

  cur_cmd->argc = 0;
  cur_cmd->argv = 0;
  cur_cmd->redirect_input = 0;
  cur_cmd->redirect_output = 0;
  cur_cmd->redirect_error = 0;
  cur_cmd->prev = NULL;
  cur_cmd->next = NULL;


  cmd_list->head = cur_cmd;/* init first command */
  cmd_list->count = 1;

  while(TRUE){
    c = cmd_line[i++];
    if (c == '\0'){
      /* end */
      if (in_arg){
        temp[temp_i] = '\0';
        temp_i = 0;
        if (in_redir_stderr_2){
          cur_cmd->redirect_error = realloc(cur_cmd->redirect_error,
                                             sizeof(char) * (strlen(temp) + 1));
          strcpy(cur_cmd->redirect_error,temp);
        }
        else if (in_redir_stdout){
          cur_cmd->redirect_output = realloc(cur_cmd->redirect_output,
                                             sizeof(char) * (strlen(temp) + 1));
          strcpy(cur_cmd->redirect_output,temp);
        }
        else if (in_redir_stdin){
          cur_cmd->redirect_input = realloc(cur_cmd->redirect_input,
                                            sizeof(char) * (strlen(temp) + 1));
          strcpy(cur_cmd->redirect_input,temp);
        }
        else if (in_redir_stdin){
          cur_cmd->redirect_error = realloc(cur_cmd->redirect_error,
                                            sizeof(char) * (strlen(temp) + 1));
          strcpy(cur_cmd->redirect_error,temp);
        }
        else{
          cur_cmd->argv = realloc(cur_cmd->argv, 
                                  (sizeof(char*) * (cur_cmd->argc + 1)));
          cur_cmd->argv[cur_cmd->argc] = malloc(sizeof(char) * (strlen(temp) + 1));
          strcpy(cur_cmd->argv[cur_cmd->argc],temp);
          cur_cmd->argc++;
        }
      }
        break;
    }
    switch(c){
    case ' ':
      if  (!in_arg){
        continue;
      }

      temp[temp_i] = '\0';
      temp_i = 0;
      in_arg = FALSE;
      if (in_redir_stderr_2){
        debug_info(temp);
        in_redir_stderr_1 = FALSE;
        in_redir_stderr_2 = FALSE;
        cur_cmd->redirect_error = realloc(cur_cmd->redirect_error,
                                           sizeof(char) * (strlen(temp) + 1));
        strcpy(cur_cmd->redirect_error,temp);
      }
      else if (in_redir_stdout){
        /* temp stores the redirect output filename */
        in_redir_stdout = FALSE;
        cur_cmd->redirect_output = realloc(cur_cmd->redirect_output,
                                           sizeof(char) * (strlen(temp) + 1));
        strcpy(cur_cmd->redirect_output,temp);
      }
      else if (in_redir_stdin){
        in_redir_stdin = FALSE;
        cur_cmd->redirect_input = realloc(cur_cmd->redirect_input,
                                           sizeof(char) * (strlen(temp) + 1));
        strcpy(cur_cmd->redirect_input,temp);
      }
      else{
        cur_cmd->argv = realloc(cur_cmd->argv, 
                                (sizeof(char*) * (cur_cmd->argc + 1)));
        cur_cmd->argv[cur_cmd->argc] = malloc(sizeof(char) * (strlen(temp) + 1));
        strcpy(cur_cmd->argv[cur_cmd->argc],temp);
        cur_cmd->argc++;
      }
      if (temp != NULL){
        free(temp); /*erase temp*/
      }
      temp = malloc(MAXLEN);
      break;

    case '>':
      /* hey! standard output redirect! */
      if (!in_redir_stderr_1){
        in_redir_stdout = TRUE;
        if (in_arg){

          in_arg = FALSE;

          temp[temp_i] = '\0';
          temp_i = 0;
          cur_cmd->argv = realloc(cur_cmd->argv, (sizeof(char*) * (cur_cmd->argc + 1)));
          cur_cmd->argv[cur_cmd->argc] = malloc(sizeof(char) * (strlen(temp) + 1));
          strcpy(cur_cmd->argv[cur_cmd->argc],temp);
          cur_cmd->argc++;
          if (temp != NULL){
            free(temp); /*erase temp*/
          }
          temp = malloc(MAXLEN);
        }
      }
      else {
        in_redir_stderr_2 = TRUE;
      }
      break;

    case '<':
      /* hey! standard input redirect! */
      in_redir_stderr_1 = FALSE;
      in_redir_stderr_2 = FALSE;
      in_redir_stdin = TRUE;
      if (in_arg){
        in_arg = FALSE;
        temp[temp_i] = '\0';
        temp_i = 0;
        cur_cmd->argv = realloc(cur_cmd->argv, (sizeof(char*) * (cur_cmd->argc + 1)));
        cur_cmd->argv[cur_cmd->argc] = malloc(sizeof(char) * (strlen(temp) + 1));
        strcpy(cur_cmd->argv[cur_cmd->argc],temp);
        cur_cmd->argc++;
        if (temp != NULL){
          free(temp); /*erase temp*/
        }
        temp = malloc(MAXLEN);
      }
      break; 

    case '2':
      /* hey! potential standard error redirect! */
      if (in_arg){
        /*ls2> ... is illegal*/
        temp[temp_i++] = c;
      }
      else{
        in_redir_stderr_1 = TRUE;
      }
      break;

    default:
      /* deal with characters */
      if (!in_redir_stderr_2){
        in_redir_stderr_1 = FALSE;
      }
      in_arg = TRUE;
      temp[temp_i++] = c;
    }
  }

  if (temp != NULL){
    free(temp);
  }

  /* return cmd_list; */
}


void free_cmd_list(cmd_list_t* list){
  cmd_t* tmp;
  while (list->head != NULL){
    tmp = list->head;
    list->head = list->head->next;
    free_cmd(tmp);
  }
}

void free_cmd(cmd_t* cmd){
  int i = 0;
  while(i < cmd->argc){
    if(cmd->argv[i] != NULL){
      free(cmd->argv[i]);
    }
    i++;
  }
  if(cmd->argv != NULL){
    free(cmd->argv);
  }
  if (cmd->redirect_input != 0){
    free(cmd->redirect_input);
  }
  if (cmd->redirect_output != 0){
    free(cmd->redirect_output);
  }
  if(cmd->redirect_error != 0){
    free(cmd->redirect_error);
  }
  free(cmd);
}

void print_cmd(cmd_t* cmd){
  if (cmd != NULL){
    if (cmd->argv[0] != NULL){
      printf("NAME:%s\n",cmd->argv[0]);
    }
    printf("ARGC:%d\n",cmd->argc);
    int i = 0;
    printf("ARGV:");
    while (i != cmd->argc){
      printf("%s,",cmd->argv[i]);
      i++;
    }
    printf("\n");
    if (cmd->redirect_output != 0){
      printf("OUT:%s\n",cmd->redirect_output);
    }
    if (cmd->redirect_input != 0){
      printf("IN:%s\n",cmd->redirect_input);
    }
    if (cmd->redirect_error != 0){
      printf("ERR:%s\n",cmd->redirect_error);
    }
  }

}
