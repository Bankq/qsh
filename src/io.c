#include "../include/io.h"


void print_promt(){

  /* char dir[MAXLEN]; */
  /* if (getcwd(dir,sizeof(dir)) != NULL){ */
  /*   fprintf(stdout,"[%s]:$",dir); */
  /* } */
  /* else{ */
  /*   perror("getcwd() error!\n"); */
  /* } */

  fprintf(stdout,"$");
  fflush(stdout);
}

void get_cmd_line(char* cmd){

  char c;
  size_t used = 0;
  
  /* char* cmd = malloc(MAXLEN); */

  /* __fpurge(stdin); */
  BEGIN_READ();
  while (((c = getc(stdin)) != EOF) && (c != '\n')){
    cmd[used++] = c;
  }
  cmd[used] = '\0';
  END_READ();
  /* return cmd; */
}


void debug_info(const char* info){
  if (info != NULL){
    fprintf(stdout,"%s\n",info);
  }
}


void BEGIN_READ(){
  READING = TRUE;
}

void END_READ(){
  READING=FALSE;
}
