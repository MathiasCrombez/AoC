// AoC 2025 - Day 6
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

static uint64_t result = 0;
#define MAX_DATA_SIZE 1024 * 1024
#define CHARS_TO_PROCESS 3




uint64_t process_file(const char *filename, bool step2)
{
    int ret = 0;

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }
    result = 0;

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, -1, SEEK_CUR);
    // take into account missing final newline
    if (fgetc(file) != '\n') {
        file_size += 1;
    }
    if (file_size > MAX_DATA_SIZE)
    {
        printf("File too big\n");
        return 1;
    }

    fseek(file, 0, SEEK_SET);
    int col_cnt = 0;
    do {
        char c = fgetc(file);
        col_cnt++;
        if (c == '\n') {
            break;
        }
        /* code */
    } while (1);
    
    int line_cnt = file_size / col_cnt;

    printf("File size: %lu, lines: %d, cols: %d\n", file_size, line_cnt, col_cnt);

    fseek(file, 0, SEEK_SET);

    int current_col_sz = 0;
    uint64_t cur_res = 0;
    // columns by column
    do
    {
        int column = 0;
        char op = 0;
        // get operation
        fseek(file, (line_cnt - 1) * col_cnt, SEEK_CUR);
        current_col_sz = 0;
        op = fgetc(file);
        switch(op) {
            case '*':
            case '+':
                result += cur_res;
                cur_res = 0;
                break;
            case '\n':
            case EOF:
            default:
                printf("Final position: %lu (%ld)\n", ftell(file), ftell(file) % col_cnt);
                result += cur_res;
                printf("Result: %lu\n", result);
                return result;
        }
        {
            char c;
            do
            {
                c = fgetc(file);
                current_col_sz++;
            } while (c == ' ' || c == '\n');
            fseek(file, -(col_cnt * (line_cnt - 1)), SEEK_CUR);
            if (c == EOF) {
                current_col_sz++;
                fseek(file, 2, SEEK_CUR);
            }
            fseek(file, -current_col_sz, SEEK_CUR);
        }
        if (!step2) {
            fseek(file, -1, SEEK_CUR);
        }
        printf("position %ld (%ld) (col sz %d)\n", ftell(file), ftell(file) % col_cnt, current_col_sz);
        // position to first line, left most col
        int real_current_col_sz = current_col_sz - 1; // minus the space
        if (step2)
        {
            fseek(file, real_current_col_sz - 1, SEEK_CUR);
            for (int col = 0; col < real_current_col_sz; col++)
            {
                char num_str[32] = {0};
                fseek(file, -1, SEEK_CUR);
                int position = ftell(file);
                char c;
                int cnt = 0;
                // position to the left most col without space
                for (int i = 0; i < line_cnt - 1; i++)
                {
                    c = fgetc(file);
                    if (c != ' ' && c != '\n' && c != EOF)
                    {
                        num_str[cnt++] = c;
                    }
                    // skip to next line, same column
                    fseek(file, col_cnt - 1, SEEK_CUR);
                }
                printf("Read number string: '%s'\n", num_str);
                //  printf("Read number: %s (position %ld)\n", num_str, ftell(file) % col_cnt);
                if (cur_res == 0)
                {
                    cur_res = strtoull(num_str, NULL, 10);
                    printf("%lu ", cur_res);
                }
                else
                {
                    uint64_t num = strtoull(num_str, NULL, 10);
                    switch (op)
                    {
                    case '*':
                        printf("* %lu ", num);
                        cur_res *= num;
                        break;
                    case '+':
                        printf("+ %lu ", num);
                        cur_res += num;
                        break;
                    }
                }
                position = ftell(file) - position;
                fseek(file, -position, SEEK_CUR);
                printf("position after number: %ld (%ld)\n", ftell(file), ftell(file) % col_cnt);
            }
            printf("\n");
            printf("Column %d result: %lu\n", column, cur_res);
            fseek(file, current_col_sz, SEEK_CUR);
        }
        else
        {
            for (int l = 0; l<line_cnt - 1; l++)
            {
                char num_str[32] = {0};
                int cnt = 0;
                int position = ftell(file);
                for (int col = 0; col < real_current_col_sz; col++)
                {
                    char c = fgetc(file);
                    if (c != ' ' && c != '\n' && c != EOF)
                    {
                        num_str[cnt++] = c;
                    }
                }
                printf("Read number string: '%s'\n", num_str);
                //  printf("Read number: %s (position %ld)\n", num_str, ftell(file) % col_cnt);
                if (cur_res == 0)
                {
                    cur_res = strtoull(num_str, NULL, 10);
                    printf("%lu ", cur_res);
                }
                else
                {
                    uint64_t num = strtoull(num_str, NULL, 10);
                    switch (op)
                    {
                    case '*':
                        printf("* %lu ", num);
                        cur_res *= num;
                        break;
                    case '+':
                        printf("+ %lu ", num);
                        cur_res += num;
                        break;
                    }
                }
                position = ftell(file) - position;
                fseek(file, -position, SEEK_CUR);
                printf("position after number: %ld (%ld)\n", ftell(file), ftell(file) % col_cnt);
                fseek(file, col_cnt, SEEK_CUR);
            }
        }
        if (!step2) {
            fseek(file, real_current_col_sz + 1, SEEK_CUR);
            fseek(file, -(line_cnt - 1) * col_cnt, SEEK_CUR);
        }
        printf("position after col %d: %ld (%ld)\n", column, ftell(file), ftell(file) % col_cnt);
        column++;
    } while (1);

    printf("Bad Result: %lu\n", result);

    return ret;
}

int main(int argc, char *argv[])
{
    uint64_t ret = 0;
    ret = process_file("aoc6_ex.txt", false);
    assert(ret == 4277556);
    ret = process_file("aoc6_data.txt", false);
    assert(ret == 4951502530386);
    ret = process_file("aoc6_ex.txt", true);
    assert(ret == 3263827);
    ret = process_file("aoc6_data.txt", true);
    assert(ret == 8486156119946);
    return 0;
}