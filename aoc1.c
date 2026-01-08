// AoC 2025 - Day 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

static uint64_t result = 0;

#define MAX_SIZE 100

uint64_t process_file(const char *filename, bool step2) {
    int ret = 0;
    
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }
    char number[32] = {0};
    int cnt = 0;

    result = 0;
    int direction = 0;

    int dial = 50;

    do {
        char c = fgetc(file);

        switch (c)
        {
            case EOF:
            case 0:
                ret = 1;
            case '\n':
                // process pattern
                cnt = 0;
                int n = strtoull(number, NULL, 10);
                int next_dial;
                int offset = n / MAX_SIZE;
                n = n % MAX_SIZE;

                if (direction > 0) {
                    next_dial = (dial + n) % MAX_SIZE;
                    if (step2)
                    {
                        // If dial is set to 0, it was already counted before
                        result += !!(dial && (dial + n >= MAX_SIZE)) + offset;
                    }
                    else
                    {
                        result += !next_dial;
                    }
                } else {
                    next_dial = (dial - n + MAX_SIZE) % MAX_SIZE;
                    if (step2)
                    {
                        // If dial was set to 0, it was already counted before
                        result += !!(dial && ((dial - n) <= 0)) + offset;
                    }
                    else
                    {
                        result += !next_dial;
                    }
                }
                dial = next_dial;
                memset(number, 0, sizeof(number));

                break;
            case 'L':
                direction = -1;
                break;
            case 'R':
                direction = 1;
                break;
            
            default:
                number[cnt++] = c;
                break;
        }

    } while(ret == 0);

    // process last pattern
    printf("Result: %lu\n", result);

    fclose(file);

    return result;
}

int main(int argc, char *argv[]) {
    uint64_t ret = 0;
    ret = process_file("aoc1_ex.txt", false);
    assert(ret == 3);
    ret = process_file("aoc1_data.txt", false);
    assert(ret == 1023);
    ret = process_file("aoc1_ex.txt", true);
    assert(ret == 6);
    ret = process_file("aoc1_data.txt", true);
    assert(ret == 5899);
    return 0;
}