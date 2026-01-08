// AoC 2025 - Day 4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

static uint64_t result = 0;
#define MAX_DATA_SIZE 1024 * 1024
#define CHARS_TO_PROCESS 3
uint8_t full_data[MAX_DATA_SIZE] = {0};





static uint64_t process_pattern(uint8_t* line, int length, bool first_line, bool last_line) {
    uint64_t line_result = 0;
    int offset = 0;
    if (!first_line) {
        offset += length;
    }

    for (int i = 0; i < length; i++) {
        int cnt = 0;
        if (line[i + offset] == 0) {
            continue;
        }
        // j is line
        for (int j = 0; j < CHARS_TO_PROCESS; j++) {
            /* Ignore out of bounds line*/
            if (first_line && j == 0) {
                continue;
            }
            /* Ignore out of bounds line*/
            if (last_line && j == CHARS_TO_PROCESS - 1) {
                continue;
            }
            // k is column
            for (int k = 0; k < CHARS_TO_PROCESS; k++) {
                /* skip center */
                if (j == 1 && k == 1) {
                    continue;
                }
                /* skip out of bounds */
                if (i == 0 && k == 0) {
                    continue;
                }
                /* skip out of bounds */
                if (i == length - 1 && k == CHARS_TO_PROCESS - 1) {
                    continue;
                }
                // i = 0, j = 0, 

                int index = i + offset + (j - 1) * length + (k - 1);
                //printf("Checking paper[%d] %d | %d = %d\n",i, j - 1, index % length, line[index]);

                if (line[index] == 1 || line[index] == 2) {
                  //  printf("paper[%d] %d | %d\n",i, j - 1, index % length);
                    cnt++;
                }
            }
        }
        if (cnt < 4) {
            // found
            //printf("Found at position %d\n", i);
            // remove the paper to avoid double counting
            line[i + offset] = 2;
            line_result++;
        }
    }

    return line_result;
}


static void display_data(uint8_t *data, int lines, int cols) {
    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (data[i * cols + j] == 1)
            {
                printf("@");
            }
            else if (data[i * cols + j] == 2)
            {
                printf("X");
            }
            else
            {
                printf(".");
            }
        }
        printf("\n");
    }
}


static uint64_t process_full_data(uint8_t *data, int lines, int cols) {
    uint64_t ret = 0;
    //printf("Processing full data: %d lines, %d cols\n", lines, cols);
        //printf("Processing line %d\n", 0);
    ret += process_pattern((uint8_t *)&data[0], cols, true, false);
    for (int i = 1; i < lines - 1; i ++) {
        //printf("Processing line %d\n", i);
        ret += process_pattern((uint8_t*)&data[(i - 1) * cols], cols, false, false);
    }
    //printf("Processing line %d\n", lines - 1);
    ret += process_pattern((uint8_t *)&data[(lines - 2) * cols], cols, false, true);

    return ret;
}



uint64_t process_file(const char *filename, bool step2) {
    int ret = 0;

    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }
    //    uint8_t line[3][256] = {0};
    //    int cnt = 0, prev_cnt = 0;
    //    int line_cnt = 1;
    //    int true_line_cnt = 0;

    size_t file_size = fseek(file, 0, SEEK_END);
    if (file_size > MAX_DATA_SIZE)
    {
        printf("File too big\n");
        return 1;
    }
    fseek(file, 0, SEEK_SET);

    result = 0;
    memset(full_data, 0, sizeof(full_data));

    int line_cnt = 0;
    int col_cnt = 0;
    int current_col = 0;
    int rolls_cnt = 0;
    char data[256] = {0};
    do {
        size_t len = fread(data, 1, 256, file);
        if (len <= 0)
            break;
        for (int i = 0; i < len; i++)
        {
            if (data[i] == '\n' || data[i] == 0 || data[i] == EOF)
            {
                if (col_cnt == 0)
                {
                    col_cnt = current_col;
                }
                line_cnt++;
                current_col = 0;
            } else if (data[i] == '@') {
                rolls_cnt ++;
                full_data[line_cnt * col_cnt + current_col] = 1;
                current_col++;
            } else if (data[i] == '.') {
                full_data[line_cnt * col_cnt + current_col] = 0;
                current_col++;
            } else {
                printf("Unknown character %c at line %d col %d\n", data[i], line_cnt, current_col);
            }
        }
    } while (1);
    line_cnt++;

    if (step2)
    {
        uint64_t ret2 = 0;
        //display_data(full_data, line_cnt, col_cnt);
        do
        {
            ret2 = process_full_data(full_data, line_cnt, col_cnt);
            //display_data(full_data, line_cnt, col_cnt);
            for (int i = 0; i < line_cnt * col_cnt; i++)
            {
                if (full_data[i] == 2)
                {
                    full_data[i] = 0;
                }
            }
            if (ret2 > 0)
            {
                result += ret2;
            }

        } while (ret2);
        //display_data(full_data, line_cnt, col_cnt);
    }
    else
    {
        result = process_full_data(full_data, line_cnt, col_cnt);
    }

    // process last pattern
    printf("Result: %lu / %d\n", result, rolls_cnt);

    return result;
}

int main(int argc, char *argv[])
{
    uint64_t ret = 0;
    ret = process_file("aoc4_ex.txt", false);
    assert(ret == 13);
    ret = process_file("aoc4_data.txt", false);
    assert(ret == 1424);
    ret = process_file("aoc4_ex.txt", true);
    assert(ret == 43);
    ret = process_file("aoc4_data.txt", true);
    assert(ret == 8727);
    return 0;
}