#include <math.h>
#include <stdio.h>
#include <string.h>


// for each magnitude, there are a constrained number of batteries that can be
// activated because we always want to select the largest possible value
// battery for any particular position.

// Each bank of batteries in the test data contains 16 digits, from which we
// must form a 12 digit number.  So at the beginning, the most significant
// digit, which is the one with index zero, must be selected such that there
// are still 11 digits to its right to choose from.

// We are constrained to positions from 0 to 4, ie 0 to len - 12, inclusive,
// which leaves positions 5, 6, 7, 8, 9, 10, a, b, c, d, e, f available.

// Then for the next position, we are contrained on the left by whatever
// position was chosen + 1, to len - 12, inclusive, and so on. We should
// always greedily choose the largest digits available for any given position
// to maximize the final value.  For example, if we had to form a 2-digit
// number from 121, there is no benefit in choosing the first 1 for our tens
// digit just so we can get the 2 in the ones position, because because 21 >
// 12.


int
main(void)
{
    unsigned long total_joltage = 0;
    char line[128];
    while (NULL != fgets(line, 128, stdin)) {
        unsigned short joltage[12] = {0};
        unsigned short bank[128] = {0};


        // Convert the line into an array of digits.
        size_t len = strlen(line);
        char c;
        for(size_t i = 0; i<len; i++) {
            strncpy(&c, line + i, 1);
            bank[i] = c - 0x30;
        }

        // select the maximum digit from the list of candidates
        size_t joltage_i = 0;
        size_t lmost_unselected_i = 0;
        for (/*nop*/; joltage_i < 12; joltage_i++) {
            unsigned short max = 0;
            size_t max_i = 0;
            //printf("Can select from: ");
            for(size_t i = lmost_unselected_i; i < len - (12-joltage_i); i++) {
                //printf("%d ", bank[i]);
                if (bank[i] > max) {
                    max = bank[i];
                    max_i = i;
                }
            }
            joltage[joltage_i] = max;
            lmost_unselected_i = max_i+1;
        }

        for (size_t i = 0; i < 12; i++) {
            total_joltage += (unsigned long)joltage[i] * (unsigned long)pow(10, 11-i);
        }
    }

    printf("%ld\n", total_joltage);
    return 0;
}
