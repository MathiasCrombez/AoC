// AoC 2025 - Day 8
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
    int z;
    int count;
    struct coord * neighbor[MAX_RANGE];
    struct coord * parent;
};

struct ordered_distance {
    double distance;
    int box1;
    int box2;
};

static struct coord junction_boxes[2000];
static int max_size[3] = {0};

static void set_max_size(int size){
    if (size > max_size[0]) {
        max_size[2] = max_size[1];
        max_size[1] = max_size[0];
        max_size[0] = size;
    }
    else if (size > max_size[1]) {
        max_size[2] = max_size[1];
        max_size[1] = size;
    }
    else if (size > max_size[2]) {
        max_size[2] = size;
    }
}

uint64_t process_file(const char *filename, int max_count) {
    int ret = 0;
    int junction_boxes_cnt = 0;
    int max_ordered_distances = max_count;

    memset(junction_boxes, 0, sizeof(junction_boxes));
    memset(max_size, 0, sizeof(max_size));

    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int cnt = 0;
    bool first_number = true;
    char num_str[32] = {0};
    do {
        char c = fgetc(file);

        switch (c)
        {
        case '\n':
        case EOF:
        case 0:
            junction_boxes[junction_boxes_cnt].z = strtol(num_str, NULL, 10);
            cnt = 0;
            first_number = true;
            junction_boxes_cnt++;
            memset(num_str, 0, sizeof(num_str));
            if (c == EOF || c == 0)
            {
                ret = 42;
            }
            break;
        case ',':
            cnt = 0;
            if (first_number)
            {
                junction_boxes[junction_boxes_cnt].x = strtol(num_str, NULL, 10);
                first_number = false;
            }
            else
            {
                junction_boxes[junction_boxes_cnt].y = strtol(num_str, NULL, 10);
            }
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


    struct ordered_distance *raw_distances = calloc(1, sizeof(struct ordered_distance) * junction_boxes_cnt * (junction_boxes_cnt - 1) / 2);
    if (max_count < 0) {
        max_ordered_distances = junction_boxes_cnt * (junction_boxes_cnt - 1) / 4;
        if (max_ordered_distances > junction_boxes_cnt * 10) {
            max_ordered_distances = junction_boxes_cnt * 10;
        }
    }
    struct ordered_distance *ordered_distances = calloc(1, sizeof(struct ordered_distance) * max_ordered_distances);

    for (int box = 0; box < junction_boxes_cnt; box++)
    {
        printf("Junction box %d: (%d, %d, %d)\n", box, junction_boxes[box].x, junction_boxes[box].y, junction_boxes[box].z);
    }
    for (int box = 0; box < junction_boxes_cnt; box++)
    {
        for (int box2 = box + 1; box2 < junction_boxes_cnt; box2++)
        {
            double distance = 0;
            distance = pow(junction_boxes[box2].x - junction_boxes[box].x, 2);
            distance += pow(junction_boxes[box2].y - junction_boxes[box].y, 2);
            distance += pow(junction_boxes[box2].z - junction_boxes[box].z, 2);
            raw_distances[cnt].distance = sqrt(distance);
            raw_distances[cnt].box1 = box;
            raw_distances[cnt].box2 = box2;
            cnt++;
            printf("\r%d", cnt);
        }
    }
    printf("\r\n");
    for (int i = 0; i < max_ordered_distances; i++)
    {
        double min_dist = 10000000.0;
        int min_index = -1;
        for (int j = 0; j < cnt; j++)
        {
            if (raw_distances[j].distance < min_dist)
            {
                min_dist = raw_distances[j].distance;
                min_index = j;
            }
        }
        if (min_index >= 0)
        {
            memcpy(&ordered_distances[i], &raw_distances[min_index], sizeof(struct ordered_distance));
            raw_distances[min_index].distance = 10000000.0;
        }
        printf("\r%d", i);
    }
    free(raw_distances);
    for (int i = 0; i < max_ordered_distances; i++)
    {
        printf("Ordered distance %d: %lf between box %d and box %d\n", i, ordered_distances[i].distance, ordered_distances[i].box1, ordered_distances[i].box2);
    }

    for (int i = 0; i < max_ordered_distances; i++)
    {
        struct coord * box1 = &junction_boxes[ordered_distances[i].box1];
        struct coord * box2 = &junction_boxes[ordered_distances[i].box2];
        struct coord * real_box1 = box1;
        struct coord * real_box2 = box2;
        double min_dist = ordered_distances[i].distance;
        int min_index[2] = {ordered_distances[i].box1, ordered_distances[i].box2};

        if (box1->parent)
            box1 = box1->parent;
        if (box2->parent)
            box2 = box2->parent;
        
        if (box1 == box2)
        {
            // already in the same circuit
           // printf("Skipping distance %lf between box %d and box %d, same circuit\n", min_dist, min_index[0], min_index[1]);
            continue;
        }
        assert(box1->parent == NULL);
        assert(box2->parent == NULL);

//        printf("Processing distance %lf between box %d and box %d\n", min_dist, min_index[0], min_index[1]);
//        printf("Box %d count %d, Box %d count %d\n", min_index[0], box1->count, min_index[1], box2->count);
        if ((box1->count > 0) && (box2->count > 0))
        {
            // both have neighbors, merge parents, 1 into 0 
            int count1 = box1->count;
            int count2 = box2->count;
            for (int n = 0; n < box2->count; n++)
            {
                box1->neighbor[box1->count++] = box2->neighbor[n];
                box2->neighbor[n]->parent = box1;
            }
            box1->neighbor[box1->count++] = box2;
            box2->parent = box1;
            box2->count = 0;
            printf("Merging two circuits %d (%d) and %d (%d)\n", min_index[1], count2, min_index[0], count1);
        }
        else if (box1->count > 0)
        {
            box1->neighbor[box1->count++] = box2;
            box2->parent = box1;
            printf("Merging single box %d into box %d (%d) circuit\n", min_index[1], min_index[0], box1->count - 1);
        }
        else if (box2->count > 0)
        {
            box2->neighbor[box2->count++] = box1;
            box1->parent = box2;
            printf("Merging single box %d into box %d (%d) circuit\n", min_index[0], min_index[1], box2->count - 1);
        }
        else
        {
            box1->neighbor[box1->count++] = box2;
            box2->parent = box1;
            printf("Connecting box %d and box %d\n", min_index[0], min_index[1]);
        }
        if (max_count < 0)
        {
            int orphans = 0;
            for (int b = 0; b < junction_boxes_cnt; b++)
            {
                if (!junction_boxes[b].parent)
                {
                    orphans++;
                }
            }
            if (orphans == 1)
            {
                printf("All boxes connected into single circuit\n");
                printf("Last boxes are %d and %d with distance %lf\n", min_index[0], min_index[1], min_dist);
                printf("Box 1 coordinates: (%d, %d, %d)\n", real_box1->x, real_box1->y, real_box1->z);
                printf("Box 2 coordinates: (%d, %d, %d)\n", real_box2->x, real_box2->y, real_box2->z);
                result = (uint64_t)real_box1->x * (uint64_t)real_box2->x;

                break;
            }
        }
    //    printf("New min dist %lf between box %d and box %d\n", min_dist, min_index[0], min_index[1]);
    }
    free(ordered_distances);

//    for (int box = 0; box < junction_boxes_cnt; box++)
//    {
//        printf("Junction box %d (%d, %d, %d) neighbors (%d): ", box, junction_boxes[box].x, junction_boxes[box].y, junction_boxes[box].z, junction_boxes[box].count + 1);
//        for (int n = 0; n < junction_boxes[box].count; n++)
//        {
//            struct coord * neighbor = junction_boxes[box].neighbor[n];
//            printf("(%d, %d, %d) ", neighbor->x, neighbor->y, neighbor->z);
//        }
//        printf("\n");
//        printf("  Parent: ");
//        if (junction_boxes[box].parent)
//        {
//            printf("(%d, %d, %d)\n", junction_boxes[box].parent->x, junction_boxes[box].parent->y, junction_boxes[box].parent->z);
//        }
//        else
//        {
//            printf("NULL\n");
//        }
//    }
    printf("#################################\n");
    int circuits = 0;
    for (int box = 0; box < junction_boxes_cnt; box++)
    {
        if (junction_boxes[box].parent == NULL)
        {
            printf("circuit of %d boxes\n", junction_boxes[box].count + 1);
            circuits ++;
            set_max_size(junction_boxes[box].count + 1);
        }
        assert(junction_boxes[box].parent == NULL || junction_boxes[box].count == 0);
    }
    printf("Number of circuits: %d\n", circuits);
    printf("Top 3 max sizes: %d, %d, %d\n", max_size[0], max_size[1], max_size[2]);
    if (max_count > 0)
        result = max_size[0] * max_size[1] * max_size[2];

    // process last pattern
    printf("Result: %lu\n\n", result);

    return result;
}


int main(int argc, char *argv[]) {
    uint64_t ret = 0;
    ret = process_file("aoc8_ex.txt", 10);
    assert(ret == 40);
    ret = process_file("aoc8_data.txt", MAX_RANGE);
    assert(ret == 69192);
    ret = process_file("aoc8_ex.txt", -1);
    assert(ret == 25272);
    ret = process_file("aoc8_data.txt", -1);
    assert(ret == 7264308110);
    return 0;
}