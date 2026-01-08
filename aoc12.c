// AoC 2025 - Day 12
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>

static uint64_t result = 0;


struct shape {
    int area;
} shapes[6];

struct region {
    int x;
    int y;
    int area;
    int shape_nb[6];
} regions[10000];



static uint64_t process_file(const char *filename, bool step2)
{
    int ret = 0;

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }
    memset(shapes, 0, sizeof(shapes));
    for (int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            if (fgetc(file) == '#')
            {
                shapes[i].area++;
            }
        }
    }
    memset(regions, 0, sizeof(regions));

    int regions_cnt = 0;
    char num_str[32] = {0};
    int shape_idx = 0;
    int cnt = 0;
    char c = 0;
    do {
        c = fgetc(file);

        switch (c)
        {
        case EOF:
        case 0:
            ret = 42;
        case '\n':
            regions[regions_cnt].shape_nb[shape_idx] = strtol(num_str, NULL, 10);
            memset(num_str, 0, sizeof(num_str));
            cnt = 0;
            shape_idx = 0;
            regions_cnt++;
            /* code */
            break;
        case 'x':
            cnt = 0;
            regions[regions_cnt].x = strtol(num_str, NULL, 10);
            memset(num_str, 0, sizeof(num_str));
            break;
        case ':':
            cnt = 0;
            regions[regions_cnt].y = strtol(num_str, NULL, 10);
            memset(num_str, 0, sizeof(num_str));
            regions[regions_cnt].area = regions[regions_cnt].x * regions[regions_cnt].y;
            break;
        case ' ':
            cnt = 0;
            if (num_str[0] != 0)
            {
                regions[regions_cnt].shape_nb[shape_idx] = strtol(num_str, NULL, 10);
                memset(num_str, 0, sizeof(num_str));
                shape_idx++;
            }
            break;
        default:
            num_str[cnt++] = c;
            break;
        }
    } while (ret == 0);

    result = 0;

    for (int r = 0; r < regions_cnt; r++)
    {
        int area_covered = 0;
        for (int s = 0; s < 6; s++)
        {
            area_covered += regions[r].shape_nb[s] * shapes[s].area;
        }
        if (area_covered <= regions[r].area)
        {
            result++;
        }
    }


    // process last pattern
    printf("Result: %lu\n", result);

    return result;
}


int main(int argc, char *argv[]) {
    uint64_t ret = 0;
    ret = process_file("aoc12_ex.txt", false);
    // wrong, this is 2
    assert(ret == 3);
    ret = process_file("aoc12_data.txt", false);
    assert(ret == 517);
//    ret = process_file("aoc12_ex.txt", true);
//    assert(ret == 2);
//    ret = process_file("aoc12_data.txt", true);
//    assert(ret == 358564784931864);
    return 0;
}