// AoC 2025 - Day 2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

static uint64_t result = 0;


uint64_t process_pattern(const char *num1, const char *num2, bool step2) {
    uint64_t n1 = strtoull(num1, NULL, 10);
    uint64_t n2 = strtoull(num2, NULL, 10);
    bool found = false;
    uint64_t pattern_result = 0;

    if ((num1[0] == '0') || (num2[0] == '0')) {
        return 0;
    }

    for (uint64_t i = n1; i <= n2; i++)
    {
        char number[32];

        sprintf(number, "%lu", i);
        int len = strlen(number);
        if (!step2)
        {
            /* Remove numbers of odd length */
            if (len % 2 == 1)
            {
                continue;
            }

            found = true;
            for (int j = 0; j < len / 2; j++)
            {
                if (number[j] != number[len / 2 + j])
                {
                    found = false;
                }
            }
        }
        else
        {
            found = false;
            /* For all segments sizes */
            for (int s = 1; s <= len / 2; s++)
            {
                if (len % s != 0)
                {
                    continue;
                }
                // s is segment size
                // len / s is number of segments

                /* For every chars of the first segment */
                bool segment_match = true;
                for (int k = 1; k < len / s; k++)
                {
                    /* Compare the segment */
                    if (strncmp(&number[0], &number[k * s], s) != 0)
                    {
                        segment_match = false;
                        break;
                    }
                }
                if (segment_match)
                {
                    found = true;
                    break;
                }
            }
        }
        if (found)
        {
            pattern_result += i;
        }
    }

    return pattern_result;
}

uint64_t process_file(const char *filename, bool step2) {
    int ret = 0;
    
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }
    char number1[32] = {0};
    char number2[32] = {0};
    int cnt = 0;
    bool first_number = true;

    result = 0;

    do {
        char c = fgetc(file);

        if (c == '\n') 
        {
            continue;
        }

        if (c == ',' || c == EOF || c == 0)
        {
            printf("Processing pattern: %s - %s\n", number1, number2);
            // process pattern
            result += process_pattern(number1, number2, step2);
            cnt = 0;
            memset(number1, 0, sizeof(number1));
            memset(number2, 0, sizeof(number2));
            first_number = true;
            if (c == EOF || c == 0) {
                ret = 0;
                break;
            }
        }
        else
        {
            if (c == '-') {
                first_number = false;
                cnt = 0;
                continue;
            }
            if (first_number) {
                number1[cnt] = c;
            } else {
                number2[cnt] = c;
            }
            cnt++;
        }

        if (cnt >= 128)
        {
            printf("Pattern too long\n");
            ret = 1;
            break;
        }

    } while(1);

    // process last pattern
    printf("Result: %lu\n", result);

    fclose(file);

    return result;
}

int main(int argc, char *argv[]) {
    uint64_t ret = 0;
    ret = process_file("aoc2_ex.txt", false);
    assert(ret == 1227775554);
    ret = process_file("aoc2_data.txt", false);
    assert(ret == 24157613387);
    ret = process_file("aoc2_ex.txt", true);
    assert(ret == 4174379265);
    ret = process_file("aoc2_data.txt", true);
    assert(ret == 33832678380);
    return 0;
}