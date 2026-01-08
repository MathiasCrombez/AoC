// AoC 2025 - Day 3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

static uint64_t result = 0;


static uint64_t process_pattern(const char *num, bool step2) {
    bool found = false;
    uint8_t max[12] = {0};
    int joltage_digits = 0;

    if (!step2)
    {
        joltage_digits = 2;

        for (int i = 0; i < strlen(num); i++)
        {
            char c = num[i];
            int n = (int)(c - '0');
            if (n > max[0] && i != strlen(num) - 1)
            {
                max[0] = n;
                max[1] = 0; // will be updated next
            }
            else if (n > max[1])
            {
                max[1] = n;
            }
        }
    }
    else
    {
        joltage_digits = 12;
        for (int i = 0; i < strlen(num); i++)
        {
            char c = num[i];
            int n = (int)(c - '0');

            bool updated = false;
            for (int j = 0; j < joltage_digits - 1; j++)
            {
                if (n > max[j] && i < strlen(num) - (joltage_digits - 1 - j))
                {
                    printf("Updating max[%d] from %d to %d\n", j, max[j], n);
                    max[j] = n;
                    memset(&max[j + 1], 0, sizeof(uint8_t) * (joltage_digits - 1 - j));
                    updated = true;
                    break;
                }
            }
            if (!updated)
            {
                if (n > max[joltage_digits - 1])
                {
                    max[joltage_digits - 1] = n;
                }
            }
        }
    }
    uint64_t val = 0;
    for (int i = 0; i < joltage_digits - 1; i++) {
        uint64_t power = 1;
        for (int j = 0; j < (joltage_digits - i - 1); j++) {
            power *= 10;
        }
        val += (uint64_t)max[i] * power;
    }
    val += max[joltage_digits - 1];
    printf("%s : %lu\n", num, val);

    return val;
}

uint64_t process_file(const char *filename, bool step2) {
    int ret = 0;
    
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    char line[128] = {0};
    int cnt = 0;

    result = 0;

    do {
        char c = fgetc(file);

        if (c == '\n' || c == EOF || c == 0) 
        {
            if (cnt == 0) {
                if (c == EOF || c == 0) {
                    ret = 0;
                    break;
                }
            }
            result += process_pattern(line, step2);
            memset(line, 0, sizeof(line));
            cnt = 0;
            continue;
        }
        else
        {
            line[cnt] = c;
            cnt++;
        }


    } while(1);

    if (ret == 0) {
        // process last pattern
        printf("Result: %lu\n", result);
    }


    return result;
}


int main(int argc, char *argv[]) {
    uint64_t ret = 0;
    ret = process_file("aoc3_ex.txt", false);
    assert(ret == 357);
    ret = process_file("aoc3_data.txt", false);
    assert(ret == 16993);
    ret = process_file("aoc3_ex.txt", true);
    assert(ret == 3121910778619);
    ret = process_file("aoc3_data.txt", true);
    assert(ret == 168617068915447);
    return 0;
}