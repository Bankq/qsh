/*
  myshell.c
  
  Description: main entrance of myshell
  Author: Hang Qian (hq2124@columbia.edu)
  Created: 2013-01-30
  Last modified: 2013-01-30
 */

#include <stdio.h>
#include <unistd.h>

#include "../include/config.h"

int main(){

  pid_t child;
  if ((child = fork()) == 0){
    /* in child */
    int child_return;
    child_return = execv(PATH,ARGV);
    printf("%d\n",child_return);
    _exit(0);
  }
  else{
    printf("Hello\n");
  }
  return 0;
}
