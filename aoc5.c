// AoC 2025 - Day 5
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

static uint64_t result = 0;
#define MAX_RANGE 1024
static uint64_t ranges[MAX_RANGE][2];
static uint64_t full_min = UINT64_MAX;
static uint64_t full_max;

uint64_t process_file(const char *filename, bool step2)
{
    int ret = 0;

    result = 0;
    memset(ranges, 0, sizeof(ranges));
    full_max = 0;
    full_min = UINT64_MAX;

    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }
    char number1[32] = {0};
    char number2[32] = {0};
    int cnt = 0;
    bool first_number = true;
    bool range_done = false;
    int rg_cnt = 0;

    do
    {
        char c = fgetc(file);

        if (c == '\n' || c == EOF || c == 0)
        {
            first_number = true;
            // empty line
            if (cnt == 0)
            {
                range_done = true;
                continue;
            }
            if (!range_done)
            {
                ranges[rg_cnt][0] = strtoull(number1, NULL, 10);
                ranges[rg_cnt][1] = strtoull(number2, NULL, 10);
                memset(number1, 0, sizeof(number1));
                memset(number2, 0, sizeof(number2));
                full_min = (ranges[rg_cnt][0] < full_min) ? ranges[rg_cnt][0] : full_min;
                full_max = (ranges[rg_cnt][1] > full_max) ? ranges[rg_cnt][1] : full_max;
                cnt = 0;
                rg_cnt++;
                if (rg_cnt >= MAX_RANGE)
                {
                    printf("Exceeded max ranges %d\n", MAX_RANGE);
                    return 1;
                }
            }
            else
            {
                if (!step2)
                {
                    uint64_t num = strtoull(number1, NULL, 10);
                    memset(number1, 0, sizeof(number1));
                    for (int r = 0; r < rg_cnt; r++)
                    {
                        if (num >= ranges[r][0] && num <= ranges[r][1])
                        {
                            // ingredient is fresh
                            result++;
                            printf("Number %lu is in range %lu-%lu\n", num, ranges[r][0], ranges[r][1]);
                            break;
                        }
                    }
                }
                else
                {
                    int removed_ranges = 0;
                    for (int r = 0; r < rg_cnt; r++)
                    {
                        if (ranges[r][0] == 0 && ranges[r][1] == 0)
                        {
                            continue;
                        }

                        for (int r2 = 0; r2 < rg_cnt; r2++)
                        {
                            if (r == r2)
                            {
                                continue;
                            }
                            if (ranges[r2][0] == 0 && ranges[r2][1] == 0)
                            {
                                continue;
                            }

                            if (ranges[r][0] <= ranges[r2][0] && ranges[r][1] >= ranges[r2][1])
                            {
                                // range r2 is fully contained in r
                                ranges[r2][0] = 0;
                                ranges[r2][1] = 0;
                                removed_ranges++;
                            }
                            else if (ranges[r][0] <= ranges[r2][0] && ranges[r][1] >= ranges[r2][0])
                            {
                                // range r2 is partially contained in r, like 5-10 and 8-12 become 5-12
                                ranges[r][1] = ranges[r2][1];
                                ranges[r2][0] = 0;
                                ranges[r2][1] = 0;
                                removed_ranges++;
                            }
                            else if (ranges[r][0] <= ranges[r2][1] && ranges[r2][1] <= ranges[r][1])
                            {
                                // range r2 is partially contained in r, like 8-12 and 5-10 become 5-12
                                ranges[r][0] = ranges[r2][0];
                                ranges[r2][0] = 0;
                                ranges[r2][1] = 0;
                                removed_ranges++;
                            }
                        }
                    }
                    printf("Removed %d ranges, now %d ranges\n", removed_ranges, rg_cnt - removed_ranges);
                    for (int r = 0; r < rg_cnt; r++)
                    {
                        if (ranges[r][0] == 0 && ranges[r][1] == 0)
                        {
                            continue;
                        }
                        printf("Range %d: %lu - %lu\n", r, ranges[r][0], ranges[r][1]);
                    }

                    for (int r = 0; r < rg_cnt; r++)
                    {
                        if (ranges[r][0] == 0 && ranges[r][1] == 0)
                        {
                            continue;
                        }
                        result += (ranges[r][1] - ranges[r][0] + 1);
                    }
                    c = EOF;
                }
            }
            cnt = 0;
            if (c == EOF || c == 0)
            {
                ret = 0;
                break;
            }
        }

        else
        {
            if (!range_done)
            {
                if (c == '-')
                {
                    first_number = false;
                    cnt = 0;
                    continue;
                }
                if (first_number)
                {
                    number1[cnt] = c;
                }
                else
                {
                    number2[cnt] = c;
                }
            }
            else
            {
                number1[cnt] = c;
            }
            cnt++;
        }
    } while (1);

    if (ret == 0)
    {
        // process last pattern
        printf("Result: %lu\n", result);
    }

    return result;
}

int main(int argc, char *argv[])
{
    uint64_t ret = 0;
    ret = process_file("aoc5_ex.txt", false);
    assert(ret == 3);
    ret = process_file("aoc5_data.txt", false);
    assert(ret == 525);
    ret = process_file("aoc5_ex.txt", true);
    assert(ret == 14);
    ret = process_file("aoc5_data.txt", true);
    assert(ret == 333892124923577);
    return 0;
}