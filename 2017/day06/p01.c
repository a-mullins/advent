#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HIST_SIZE 4096
#define BANKS 16
#define DELIMS " \t\r\n"

int main(void) {
    int hist[HIST_SIZE][BANKS] = {0};
    int cur_state = 0;

    {
        char *buf = NULL;
        size_t len = 0;
        if(getline(&buf, &len, stdin) <= 0) {
            fprintf(stderr, "ERR");
            exit(1);
        }

        char *tok = strtok(buf, DELIMS);
        for(int i=0; tok != NULL; i++) {
            hist[0][i] = atoi(tok);
            tok = strtok(NULL, DELIMS);
        }
        free(buf);
    }

    // Generate states until we run out of buffer (or find a repeat).
    for(int limit=0; limit<HIST_SIZE; limit++) {
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

        // remove blocks from bank, redistribute.
        hist[cur_state][max_index] = 0;
        int cur_index = max_index;
        while(blocks > 0) {
            cur_index = (cur_index+1) % BANKS;
            hist[cur_state][cur_index]++;
            blocks--;
        }

        // check if we have seen cur_state before.
        for(int old_state=0; old_state<cur_state; old_state++) {
            int match = false;
            for(int i=0; i<BANKS; i++) {
                match = true;
                if(hist[old_state][i] != hist[cur_state][i]) {
                    match = false;
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
