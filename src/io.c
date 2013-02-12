#include "../include/io.h"


void print_promt(){
  fprintf(stdout,"$");
  fflush(stdout);
}

void get_cmd_line(char* cmd){
  char c;
  size_t used = 0;

  BEGIN_READ();
  while (((c = getc(stdin)) != EOF) && (c != '\n')){
    cmd[used++] = c;
  }
  cmd[used] = '\0';
  END_READ();
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
