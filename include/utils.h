/*
  Utils

  Author: Hang Qian
*/

#ifndef _UTILS_H
#define _UTILS_H

#include "stdlib.h"
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "io.h"

/* 
   path_concatenate:

   Purpose:append string b to the end of string a 
   Input:
        char* dest: destination;
        char* front: first string;
        char* back: second sting;
   Output:
        no output;
*/
void path_concatenate(char* dest,char* front, char* back);

/* 
   path_tailor:

   Purpose:remove all path vars from PATH
   Input:
        char* str: current PATH
        char* substr: path var to remove
   Output:
        new PATH;
*/

char* path_tailor(char* str,char* substr);


/* is_path

   Purpose: tell if a string is a valid path
   Input: 
        char* pathname;
   Output:
        1 valid
        0 invalid
*/
int is_path(char* name);

/* 
   get_a_path
   Purpose: get a path var from PATH
   Input: 
        PATH: current PATH
        n: the nth path var
   Output:
       path var
*/
char* get_a_path(char* PATH,int n);

/* take a filename and path, concatenate into a pathname */
char* get_path_name(char* file, char* path);


#endif
