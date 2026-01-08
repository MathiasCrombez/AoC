// AoC 2025 - Day 7
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

static uint64_t result = 0;
#define MAX_DATA_SIZE 1024 * 1024
#define CHARS_TO_PROCESS 3
char full_data[MAX_DATA_SIZE] = {0};





static void display_data(uint8_t *data, int lines, int cols) {
    printf("%s\n\n", data);
}

uint64_t process_file(const char *filename, bool step2)
{
    int ret = 0;

    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    if (file_size > MAX_DATA_SIZE)
    {
        printf("File too big\n");
        return 1;
    }
    fseek(file, 0, SEEK_SET);
    memset(full_data, 0, sizeof(full_data));
    result = 0;

    int line_cnt = 0;
    int col_cnt = 0;
    int current_col = 0;
    int rolls_cnt = 0;
    size_t len = fread(full_data, 1, file_size, file);

    while (full_data[col_cnt++] != '\n')
        ;
    line_cnt = len / col_cnt;
    if (full_data[len - 1] != '\n') {
        line_cnt++;
    }

    // get S
    while(full_data[current_col++] != 'S');
    current_col--;
    printf("Found S at col %d\n", current_col);

    for (int l = 1; l < line_cnt; l++) {
        for (int c = 0; c < col_cnt - 1; c++) {
            int index = l * col_cnt + c;

            if (full_data[index - col_cnt] == 'S' && full_data[index] == '.') {
                full_data[index] = '|';
            } else if (full_data[index - col_cnt] == '|' && full_data[index] == '.') {
                full_data[index] = '|';
            } else if (full_data[index - col_cnt] == '|' && full_data[index] == '^') {
                full_data[index - 1] = '|';
                full_data[index + 1] = '|';
                if (!step2) {
                    result++;
                }
            }
        }
    }
    display_data(full_data, line_cnt, col_cnt);

    if (step2)
    {
        for (int l = 1; l < line_cnt; l++)
        {
            for (int c = 0; c < col_cnt - 1; c++)
            {
                int index = l * col_cnt + c;
                if (full_data[index] == '^' && full_data[index - col_cnt] != '|')
                {
                    full_data[index] = '.';
                }
            }
        }

        display_data(full_data, line_cnt, col_cnt);
        printf("Max splitters possible: %d\n", (line_cnt - 1) / 2 * ((line_cnt - 1) / 2 + 1) / 2); // real_line * (real_line + 1) / 2

        uint64_t *total = calloc(sizeof(uint64_t), col_cnt);
        total[current_col]++;

        for (int l = 2; l < line_cnt; l += 2)
        {
            for (int c = 0; c < col_cnt - 1; c++)
            {
                int index = l * col_cnt + c;
                if (full_data[index] == '^')
                {
                    total[index % line_cnt - 1] += total[index % line_cnt];
                    total[index % line_cnt + 1] += total[index % line_cnt];
                    total[index % line_cnt] = 0;
                }
            }
            printf("Line %d totals: ", l / 2);
            for (int c = 0; c < col_cnt; c++)
            {
                if (total[c] == 0)
                {
                    continue;
                }
                printf("%lu ", total[c]);
            }
            printf("\n");
        }
        for (int i = 0; i < col_cnt; i++)
        {
            result += total[i];
        }

        // max 1 splitter on line 2
        // max 2 splitters on line 4
        // max 3 splitters on line 6

        printf("Lines: %d, Cols: %d\n", line_cnt, col_cnt);
        printf("Max splitters possible: %d\n", (line_cnt - 1) / 2 * ((line_cnt - 1) / 2 + 1) / 2); // real_line * (real_line + 1) / 2
        // max combinations is 1 -> 2, 2 -> 4, 3 -> 8
        // if we remove a splitter of line 3, we remove 1 or 2 combinations depending on the splitter missing
        // if we remove a splitter of line 2, we remove 3 combinations
        // if we remove a splitter of line 1, we remove 7
        printf("Raw max combinations: %llu\n", (1ULL << ((line_cnt - 1) / 2))); // 2 ^ (real_line)
        printf("True max combinations: %lu\n", result * 2);
    }

    // process last pattern
    printf("Result: %lu\n", result);

    return result;
}

int main(int argc, char *argv[])
{
    uint64_t ret = 0;
    ret = process_file("aoc7_ex.txt", false);
    assert(ret == 21);
    ret = process_file("aoc7_data.txt", false);
    assert(ret == 1628);
    ret = process_file("aoc7_ex.txt", true);
    assert(ret == 40);
    ret = process_file("aoc7_data.txt", true);
    assert(ret == 27055852018812);
    return 0;
}