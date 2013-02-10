#ifndef _UTILS_H
#define _UTILS_H

#include "stdlib.h"
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "io.h"

/* append string b to the end of string a */
void path_concatenate(char* dest,char* front, char* back);
char* path_tailor(char* str,char* substr);
/* tell if a command is path or filename */
int is_path(char* name);

/* 
   parse PATH into a list of pathname 
*/
char** get_path_list(char* PATH);
/* take a filename and path, concatenate into a pathname */
char* get_path_name(char* file, char* path);


#endif
