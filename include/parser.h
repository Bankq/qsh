/*
  Interpret

  parse the input command and invoke run_command

*/

#ifndef _PARSER_H
#define _PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "config.h"
#include "utils.h"
#include "io.h"
#include "run.h"

/* parse user input into a list of command with their arguments
   If there's no pipe, there's only one command
 */
void parse_cmd(cmd_list_t*,char *);

void free_cmd_list(cmd_list_t* );

/* free the mem of a command */
void free_cmd(cmd_t *);

/* print command info (for debugging) */
void print_cmd(cmd_t *);

#endif
