#include "../include/utils.h"

void path_concatenate(char*dest, char* a, char *b){
  /* in case a not long enough pointer passed,
     we re-allocate the mem first */
  if (realloc(dest,(strlen(a)+strlen(b)+2)*sizeof(char)) == NULL ){
    fprintf(stderr,"realloc error in path_concatenate!\n");
    return;
  }
  
  strcpy(dest,a);
  strcat(dest,":");
  strcat(dest,b);
}

char* path_tailor(char* str, char* substr){

  char* m_str = malloc(sizeof(char) * (strlen(str) + 3));
  strcpy(m_str,":");
  strcat(m_str,str);
  strcat(m_str,":");

  char* m_substr = malloc(sizeof(char) * (strlen(substr) + 3));
  strcpy(m_substr,":");
  strcat(m_substr,substr);
  strcat(m_substr,":");

  int len2 = strlen(m_substr);

  char* anchor;
  while ((anchor = strstr(m_str, m_substr)) != NULL){
    memmove(anchor,(anchor + len2 - 1),(strlen(anchor) - len2 + 2));
  }

  char* result = malloc(sizeof(char) * (strlen(m_str) - 1));
  strncpy(result,m_str+1,(strlen(m_str) - 2));
  result[strlen(m_str)] = '\0';
  free(m_str);
  free(m_substr);
  return result;
}


int is_path(char* name){
  int i;
  for (i = 0; i < strlen(name) ; i++){
    if (name[i] == '/'){
      return TRUE;
    }
  }
  return FALSE;
}

char *get_a_path(char* PATH, int n){
  char* s;
  int i = 0;
  const char* delim = ":";
  s = strtok(PATH,delim);
  while (i++ < n){
    s = strtok(NULL,delim);
    if (s == NULL){
      return NULL;
    }
  }
  return s;
}


void get_path_list(char** list,char* PATH){
  int flag = FALSE;

  int count = 0;
  int i = 0;
  char c;

  /* char **list = malloc(sizeof(char*) * MAXLEN); */
  /* list = realloc(list,sizeof(char*)); */

  int tmp_i = 0;
  char* tmp = malloc((sizeof(char) * MAXLEN));
  
  while ((c = PATH[i++]) != '\0'){
    switch(c){
    case ':':
      if (flag){
        flag = FALSE;
        tmp[tmp_i] = '\0';
        list = realloc(list,sizeof(char*) * (count + 1));
        list[count] = malloc(sizeof(char) * (strlen(tmp) + 1));
        strcpy(list[count],tmp);
        count++;
        if (tmp != NULL){
          free(tmp);
          tmp = NULL;
        }
        tmp = malloc(sizeof(char) * MAXLEN);
        /* tmp = calloc(MAXLEN,sizeof(char)); */
        tmp_i = 0;
      }
      break;
    default:
      flag = TRUE;
      tmp[tmp_i++] = c;
    }
  }

  if (flag){
    tmp[tmp_i] = '\0';
    list = realloc(list,sizeof(char*) * (count + 1));
    list[count] = malloc(sizeof(char) * (strlen(tmp) + 1));
    strcpy(list[count],tmp);
    count++;
  }

  if (tmp != NULL) {
    free(tmp);
    tmp = NULL;
  }
  list[count] = NULL;
  /* return list; */
}


char* get_path_name(char* filename, char* path){
  if (filename == NULL || path == NULL){
    return NULL;
  }
  char* path_name;
  if( (path_name = malloc(sizeof(char) * (strlen(filename)+strlen(path) + 2))) == NULL){
    fprintf(stderr,"MALLOC ERROR");
  }
  strcpy(path_name,path);
  if (path[strlen(path) - 1] != '/'){
    strncat(path_name,"/",1);
  }
  strcat(path_name,filename);
  return path_name;
}
