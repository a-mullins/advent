#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HIST_SIZE 4096
#define BANKS 16

static const char *DELIMS=" \t\r\n";

int main(void) {
  int hist[HIST_SIZE][BANKS];
  memset(&hist, -1, sizeof(int) * HIST_SIZE * BANKS);
  int cur_state = 0;

  // GET LINE
  {
    size_t line_buf_size = 0;
    char *line_buf = NULL;
    if(getline(&line_buf, &line_buf_size, stdin) < -1) {
      fprintf(stderr, "ERR");
      exit(1);
    }
    

    char *tok = NULL;
    tok = strtok(line_buf, DELIMS);
    for(int i=0; tok != NULL; i++) {
      hist[0][i] = atoi(tok);
      tok = strtok(NULL, DELIMS);
    }
    free(line_buf);
    
    for(int i=0; i<BANKS; i++) {
      printf("%d ", hist[0][i]);
    }
    puts("");
  }

  for(int limit=0; limit<HIST_SIZE; limit++) {    // NEXT STATE
    // find biggest block count & index of
    memcpy(&hist[cur_state+1], &hist[cur_state], sizeof(int) * BANKS);
    cur_state++;
    int blocks = 0;
    int max_index = 0;
    for(int i = 0; i < BANKS; i++) {
      if(hist[cur_state][i] > blocks) {
        max_index = i;
        blocks = hist[cur_state][i];
      }    
    }
    printf("max blocks %d @ %d\n", blocks, max_index);
  
    hist[cur_state][max_index] = 0;
    int cur_index = max_index;
    while(blocks > 0) {
      cur_index = ++cur_index % BANKS;
      hist[cur_state][cur_index]++;
      blocks--;
    }

    for(int i=0; i<BANKS; i++) {
      printf("%d ", hist[cur_state][i]);
    }
    puts("");

    // check if i have seen cur_state before.
    for(int old_state=0; old_state<cur_state; old_state++) {
      int match = 0;
      for(int i=0; i<BANKS; i++) {
        match = 1;
        if(hist[old_state][i] != hist[cur_state][i]) {
          match = 0;
          break;
        }
      }
      if(match) {
        printf("match found, iter %d matches %d: ", cur_state, old_state);
        for(int i=0; i<BANKS; i++) {
          printf("%d ", hist[cur_state][i]);
        }
        printf("\nloop size: %d\n", cur_state - old_state);
        exit(0);
      }
    }
  }
  return 0;
}
