#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 1605
#define MAX_LABEL_LEN 7
#define MAX_CHILDREN 7
#define DELIMS " \t\r\n()->,"

char *children[MAX_NODES+1] = {NULL};
char *nodes[MAX_NODES+1] = {NULL};

bool includes(char *list[], char *s);
int len(char *list[]);
void push(char *list[], char *s);
void clear(char *list[]);

int main(void) {
  size_t linecap = 80;
  char *line = malloc(sizeof(char) * linecap);
  while(getline(&line, &linecap, stdin) != -1) {
    char *label = strtok(line, DELIMS);
    push(nodes, label);
    char *weight = strtok(NULL, DELIMS);
    char *child = NULL;
    while((child = strtok(NULL, DELIMS)) != NULL) {
      push(children, child);
    }
  }
  free(line);
  
  for(int i=0; i<len(nodes); i++) {
    if(!includes(children, nodes[i])) {
      printf("%s\n", nodes[i]);
      break;
    }
  }

  clear(nodes);
  clear(children);
  return 0;
}

bool includes(char *list[], char *s) {
  if(s == NULL) { return true; }
  for(int i=0; list[i] != NULL; i++) {
    if(strcmp(list[i], s) == 0) {
      return true;
    }
  }
  return false;
}

int len(char* list[]) {
  int i = 0;
  for(/* nop */; list[i] != NULL; i++);
  return i;
}

void push(char *list[], char *s) {
  if(!includes(list, s)) {
    char *buf = malloc(strlen(s)+1);
    strcpy(buf, s);
    list[len(list)] = buf;
  }
  return;
}

void clear(char *list[]) {
  int l = len(list);
  for(int i=0; i<l; i++) {
    free(list[i]);
    list[i] = NULL;
  }
  return;
}
