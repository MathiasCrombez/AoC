// AoC 2025 - Day 9
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>

static uint64_t result = 0;
#define MAX_RANGE 1000

struct coord {
    int x;
    int y;
};

struct ordered_area{
    uint64_t area;
    int tile1;
    int tile2;
    bool bad;
};

static struct coord red_tiles[MAX_RANGE];

void display_data(int cnt) {
    printf("Displaying data:\n");
    int div = 1;
    int size=0;
    if (cnt > 100) {
        size = 100;
        div = 1000;
    } else {
        size = 14;
    }
    char * grid = malloc(size * size);
    memset(grid, '.', size * size);
    for (int t = 0; t < cnt; t++) {
        if (abs(red_tiles[t].x - red_tiles[(t-1) % cnt].x) > 5000 || abs(red_tiles[t].y - red_tiles[(t-1) % cnt].y) > 5000) {
        grid[red_tiles[t].x / div + red_tiles[t].y / div * size] = '%';
        }
        else
        grid[red_tiles[t].x / div + red_tiles[t].y / div * size] = '#';
    }
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            printf("%c", grid[y * size + x]);
        }
        printf("\n");
    }
    printf("\n");
    free(grid);
}

static uint64_t process_file(const char *filename, bool step2) {
    int ret = 0;

    memset(red_tiles, 0, sizeof(red_tiles));

    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int cnt = 0;
    char num_str[32] = {0};
    int red_tiles_cnt = 0;
    do {
        char c = fgetc(file);

        switch (c)
        {
        case '\n':
        case EOF:
        case 0:
            red_tiles[red_tiles_cnt].y = strtol(num_str, NULL, 10);
            cnt = 0;
            red_tiles_cnt++;
            memset(num_str, 0, sizeof(num_str));
            if (c == EOF || c == 0)
            {
                ret = 42;
            }
            break;
        case ',':
            cnt = 0;
            red_tiles[red_tiles_cnt].x = strtol(num_str, NULL, 10);
            memset(num_str, 0, sizeof(num_str));
            break;
        default:
            num_str[cnt++] = c;
            break;
        }
    } while (ret == 0);

    fclose(file);

    if (ret != 42) {
        return 1;
    }


    for (int tile = 0; tile < red_tiles_cnt; tile++)
    {
        printf("Red tile %d: (%d, %d)\n", tile, red_tiles[tile].x, red_tiles[tile].y);
    }

//    for (int tile = 0; tile < red_tiles_cnt; tile++)
//    {
//        int cnt_x = 0;
//        int cnt_y = 0;
//        for (int tile2 = tile + 1; tile2 < red_tiles_cnt; tile2++)
//        {
//            if (red_tiles[tile2].x == red_tiles[tile].x) {
//                cnt_x++;
//            }
//            if (red_tiles[tile2].y == red_tiles[tile].y) {
//                cnt_y++;
//            }
//        }
//        if (cnt_x != 1 && cnt_y != 1) {
//            printf("Tile %d has %d same x and %d same y\n", tile, cnt_x, cnt_y);
//        }
//    }

    display_data(red_tiles_cnt);
    struct ordered_area *raw_areas = calloc(1, sizeof(struct ordered_area) * red_tiles_cnt * (red_tiles_cnt - 1) / 2);

    for (int tile = 0; tile < red_tiles_cnt; tile++)
    {
        for (int tile2 = tile + 1; tile2 < red_tiles_cnt; tile2++)
        {
            int x = abs((red_tiles[tile2].x - red_tiles[tile].x)) + 1;
            int y = abs((red_tiles[tile2].y - red_tiles[tile].y)) + 1;
            raw_areas[cnt].area = (uint64_t)x * (uint64_t)y; // Calculate area here if needed
            raw_areas[cnt].tile1 = tile;
            raw_areas[cnt].tile2 = tile2;
           // printf("\r%d", cnt);
            cnt++;
            if (step2) {
                int current_area = cnt - 1;
                if (red_tiles[tile2].x == red_tiles[tile].x || red_tiles[tile2].y == red_tiles[tile].y) {
                    raw_areas[current_area].bad = true;
                    continue;
                }
                int start_tile = tile;
                int end_tile = tile2;

                int corner_top_left_x = red_tiles[start_tile].x > red_tiles[end_tile].x ? red_tiles[end_tile].x : red_tiles[start_tile].x;
                int corner_top_left_y = red_tiles[start_tile].y > red_tiles[end_tile].y ? red_tiles[end_tile].y : red_tiles[start_tile].y;
                // x high y low
                int corner_top_right_x = red_tiles[start_tile].x > red_tiles[end_tile].x ? red_tiles[start_tile].x : red_tiles[end_tile].x;
                int corner_top_right_y = red_tiles[start_tile].y > red_tiles[end_tile].y ? red_tiles[end_tile].y : red_tiles[start_tile].y;

                int corner_bottom_right_x = red_tiles[start_tile].x > red_tiles[end_tile].x ? red_tiles[start_tile].x : red_tiles[end_tile].x;
                int corner_bottom_right_y = red_tiles[start_tile].y > red_tiles[end_tile].y ? red_tiles[start_tile].y : red_tiles[end_tile].y;

                int corner_bottom_left_x = red_tiles[start_tile].x > red_tiles[end_tile].x ? red_tiles[end_tile].x : red_tiles[start_tile].x;
                int corner_bottom_left_y = red_tiles[start_tile].y > red_tiles[end_tile].y ? red_tiles[start_tile].y : red_tiles[end_tile].y;

                int max_x = corner_bottom_right_x;
                int max_y = corner_bottom_right_y;

                int min_x = corner_top_left_x;
                int min_y = corner_top_left_y;

                enum side_e {
                    NONE,
                    TOP,
                    LEFT,
                    BOTTOM,
                    RIGHT
                } side = NONE;
                bool top_found = false;
                bool left_found = false;
                bool bottom_found = false;
                bool right_found = false;

                printf("Processing area between tile %d (%d, %d) and tile %d (%d, %d), corners (%d, %d) and (%d, %d)\n", tile, red_tiles[tile].x, red_tiles[tile].y, tile2, red_tiles[tile2].x, red_tiles[tile2].y, min_x, min_y, max_x, max_y);
                for (int t0 = 0; t0 < red_tiles_cnt + 1; t0++) {
                    int t = (t0 + tile) % red_tiles_cnt;

                    if (t == tile || t == tile2) {
                        side = NONE;
                        continue;
                    }

                    if ((red_tiles[t].x == min_x && red_tiles[t].y == max_y) || (red_tiles[t].y == min_y && red_tiles[t].x == max_x) ||
                        (red_tiles[t].x == min_x && red_tiles[t].y == min_y) || (red_tiles[t].y == max_y && red_tiles[t].x == max_x)){
                        // on corner
                        side = NONE;
                        continue;
                    }

                    if (red_tiles[t].x <= min_x && (red_tiles[t].y >= min_y && red_tiles[t].y <= max_y)) {
                        if (side != NONE && side != BOTTOM && side != LEFT) {
                            printf("Tile %d (%d, %d) is on the wrong side (expected BOTTOM, got %d)\n", t, red_tiles[t].x, red_tiles[t].y, side);
                            raw_areas[current_area].bad = true;
                            break;
                        }
                        if (left_found) {
                            printf("Tile %d (%d, %d) going back\n", t, red_tiles[t].x, red_tiles[t].y);
                            raw_areas[current_area].bad = true;
                            break;
                        }
                        side = LEFT;
                    } else if (red_tiles[t].x >= max_x && (red_tiles[t].y >= min_y && red_tiles[t].y <= max_y)) {
                        if (side != NONE && side != TOP && side != RIGHT) {
                            printf("Tile %d (%d, %d) is on the wrong side (expected TOP, got %d)\n", t, red_tiles[t].x, red_tiles[t].y, side);
                            raw_areas[current_area].bad = true;
                            break;
                        }
                        if (right_found) {
                            printf("Tile %d (%d, %d) going back\n", t, red_tiles[t].x, red_tiles[t].y);
                            raw_areas[current_area].bad = true;
                            break;
                        }
                        side = RIGHT;
                    } else if (red_tiles[t].y <= min_y && (red_tiles[t].x >= min_x && red_tiles[t].x <= max_x)) {
                        if (side != NONE && side != LEFT && side != TOP) {
                            printf("Tile %d (%d, %d) is on the wrong side (expected LEFT, got %d)\n", t, red_tiles[t].x, red_tiles[t].y, side);
                            raw_areas[current_area].bad = true;
                            break;
                        }
                        if (top_found) {
                            printf("Tile %d (%d, %d) going back\n", t, red_tiles[t].x, red_tiles[t].y);
                            raw_areas[current_area].bad = true;
                            break;
                        }
                        side = TOP;
                    } else if (red_tiles[t].y >= max_y && (red_tiles[t].x >= min_x && red_tiles[t].x <= max_x)) {
                        if (side != NONE && side != RIGHT && side != BOTTOM) {
                            printf("Tile %d (%d, %d) is on the wrong side (expected RIGHT, got %d)\n", t, red_tiles[t].x, red_tiles[t].y, side);
                            raw_areas[current_area].bad = true;
                            break;
                        }
                        if (bottom_found) {
                            printf("Tile %d (%d, %d) going back\n", t, red_tiles[t].x, red_tiles[t].y);
                            raw_areas[current_area].bad = true;
                            break;
                        }
                        side = BOTTOM;
                    } else {
                        // outside area
                        side = NONE;
                    }
                    if (red_tiles[t].x > min_x && red_tiles[t].x < max_x && red_tiles[t].y > min_y && red_tiles[t].y < max_y) {
                        printf("Tile %d (%d, %d) is blocking area between tile %d and tile %d\n", t, red_tiles[t].x, red_tiles[t].y, tile, tile2);
                        raw_areas[current_area].bad = true;
                        break;
                    }

//                    int t_minus = t - 1 >= 0 ? t - 1 : red_tiles_cnt - 1;
//                    if ((red_tiles[t_minus].x == max_x && red_tiles[t].x == max_x && red_tiles[t].y > min_y && red_tiles[t].y < red_tiles[t_minus].y)) {
//                        raw_areas[current_area].bad = true;
//                        break;
//                    }
//                    if ((red_tiles[t_minus].y == max_y && red_tiles[t].y == max_y && red_tiles[t].x > min_x && red_tiles[t].x < red_tiles[t_minus].x)) {
//                        raw_areas[current_area].bad = true;
//                        break;
//                    }
//                    if ((red_tiles[t_minus].x == min_x && red_tiles[t].x == min_x && red_tiles[t].y > min_y && red_tiles[t].y > red_tiles[t_minus].y)) {
//                        raw_areas[current_area].bad = true;
//                        break;
//                    }
//                    if ((red_tiles[t_minus].y == min_y && red_tiles[t].y == min_y && red_tiles[t].x > min_x && red_tiles[t].x > red_tiles[t_minus].x)) {
//                        raw_areas[current_area].bad = true;
//                        break;
//                    }
//                    if (red_tiles[t].x >= min_x && red_tiles[t].x <= max_x && red_tiles[t].y >= min_y && red_tiles[t].y <= max_y) {
//                        printf("Tile %d (%d, %d) is blocking area between tile %d and tile %d\n", t, red_tiles[t].x, red_tiles[t].y, tile, tile2);
//                        raw_areas[current_area].bad = true;
//                        break;
//                    }
//                    if (((red_tiles[t].x > max_x && red_tiles[t_minus].x < min_x) || (red_tiles[t].x < min_x && red_tiles[t_minus].x > max_x)) &&
//                         (red_tiles[t].y > min_y && red_tiles[t].y < max_y)) {
//                        // jump over area
//                        printf("Tile %d (%d, %d) is blocking (jump from (%d, %d)) area between tile %d and tile %d\n", t, red_tiles[t].x, red_tiles[t].y, red_tiles[t_minus].x, red_tiles[t_minus].y, tile, tile2);
//                        raw_areas[current_area].bad = true;
//                        break;
//                    }
//                    if (((red_tiles[t].y > max_y && red_tiles[t_minus].y < min_y) || (red_tiles[t].y < min_y && red_tiles[t_minus].y > max_y)) &&
//                         (red_tiles[t].x > min_x && red_tiles[t].x < max_x)) {
//                        printf("Tile %d (%d, %d) is blocking (jump from (%d, %d)) area between tile %d and tile %d\n", t, red_tiles[t].x, red_tiles[t].y, red_tiles[t_minus].x, red_tiles[t_minus].y, tile, tile2);
//                        raw_areas[current_area].bad = true;
//                        break;
//                    }
                }
            }
        }
    }
    int max_index = -1;
    struct ordered_area max_area = {0};
    int valid_areas = 0;
    for (int j = 0; j < cnt; j++)
    {
        if (step2 && raw_areas[j].bad)
        {
            continue;
        }
        valid_areas++;
        if (raw_areas[j].area > max_area.area)
        {
            printf("New max area %lu between tile %d (%d, %d) and tile %d (%d, %d)\n", raw_areas[j].area, raw_areas[j].tile1, red_tiles[raw_areas[j].tile1].x, red_tiles[raw_areas[j].tile1].y, raw_areas[j].tile2, red_tiles[raw_areas[j].tile2].x, red_tiles[raw_areas[j].tile2].y);
            max_area.area = raw_areas[j].area;
            max_area.tile1 = raw_areas[j].tile1;
            max_area.tile2 = raw_areas[j].tile2;
            max_index = j;
        }
    }
    free(raw_areas);
    printf("Total valid areas: %d / %d\n", valid_areas, cnt);

    result = max_area.area;

    // process last pattern
    printf("Biggest area is %lu between tile %d (%d, %d) and tile %d (%d, %d)\n", max_area.area, max_area.tile1, red_tiles[max_area.tile1].x, red_tiles[max_area.tile1].y, max_area.tile2, red_tiles[max_area.tile2].x, red_tiles[max_area.tile2].y);
    printf("Result: %lu\n\n", result);

    return result;
}


int main(int argc, char *argv[]) {
    uint64_t ret = 0;
    ret = process_file("aoc9_ex.txt", false);
    assert(ret == 50);
    ret = process_file("aoc9_data.txt", false);
    assert(ret == 4790063600);
    ret = process_file("aoc9_ex.txt", true);
    assert(ret == 24);
    ret = process_file("aoc9_data.txt", true);
    assert(ret == 1516172795);
    return 0;
}