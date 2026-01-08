// AoC 2025 - Day 11
// Device connection search problem
// Very important: storing the results of a previous search in the device structure is quite useful and reduces computation time drastically
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>

static uint64_t result = 0;
#define MAX_DATA_SIZE 1024 * 1024
#define CHARS_TO_PROCESS 3
char full_data[MAX_DATA_SIZE] = {0};
#define MAX_OUTPUTS 40


struct device {
    char name[16];
    uint32_t hash;
    uint32_t output[MAX_OUTPUTS];
    int visited_output_results[MAX_OUTPUTS];
    int output_cnt;
    bool visited;
};




static void display_data(uint8_t *data, int lines, int cols) {
    printf("%s\n\n", data);
}

static uint32_t hash(const char *str) {
    uint32_t hash = 5381;

    for (int i = 0; i < 3; i++) {
        hash = ((hash << 5) + hash) + str[i]; /* hash * 33 + c */
    }

    return hash;
}

static uint64_t loop_cnt;

uint64_t search(struct device *devices, int device_cnt, struct device *target, uint32_t to, bool step2, int depth) {
    uint64_t total = 0;

    if (target->visited)
    {
        printf("target %s already visited\n", target->name);
        return 0;
    }
    target->visited = true;

    for (int j = 0; j < target->output_cnt; j++)
    {
        if (target->visited_output_results[j] != -1)
        {
            total += target->visited_output_results[j];
            continue;
        }
        if (target->output[j] == to)
        {
            target->visited = false;
            return 1;
        }
        struct device *dev = NULL;
        for (int i = 0; i < device_cnt; i++)
        {
            if (devices[i].visited)
            {
                continue;
            }
            if (target->output[j] == devices[i].hash)
            {
                dev = &devices[i];
                break;
            }
        }
        if (dev && target->output[j] == dev->hash)
        {
            loop_cnt++;
            uint32_t intermediate_total = search(devices, device_cnt, dev, to, step2, depth + 1);
            target->visited_output_results[j] = intermediate_total;
            total += intermediate_total;
        }
    }
    target->visited = false;
    //printf("Leaving target %s with total %lu\n", target->name, total);

    return total;
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
    char c;
    int line_cnt = 0;
    do {
        c = fgetc(file);

        if (c == '\n' || c == EOF || c == 0) 
        {
            line_cnt++;
        }
    } while (c != EOF && c != 0);

    fseek(file, 0, SEEK_SET);

    struct device *devices = calloc(line_cnt, sizeof(struct device));

    char num_str[3] = {0};
    int device_cnt = 0;
    int output_cnt = 0;
    uint32_t hash_you;
    if (!step2)
        hash_you = hash("you");
    else
        hash_you = hash("svr");

    do {
        c = fgetc(file);

        switch (c)
        {
        case '\n':
        case EOF:
        case 0:
            devices[device_cnt].output[output_cnt++] = hash(num_str);
            devices[device_cnt].output_cnt = output_cnt;
            assert(output_cnt < MAX_OUTPUTS);
            output_cnt = 0;
            device_cnt++;
            if (c == EOF || c == 0)
            {
                ret = 42;
            }
            break;
        case ':':
            strncpy(devices[device_cnt].name, num_str, strlen(num_str));
            memset(num_str, 0, sizeof(num_str));
            devices[device_cnt].hash = hash(devices[device_cnt].name);
            
            fgetc(file); // skip space
            break;
        case ' ':
            devices[device_cnt].output[output_cnt++] = hash(num_str);
            devices[device_cnt].output_cnt = output_cnt;
            break;
        default:
            num_str[0] = c;
            num_str[1] = fgetc(file);
            num_str[2] = fgetc(file);
            break;
        }
    } while (ret == 0);

    fclose(file);

    if (ret != 42) {
        return 1;
    }
    struct device *dev_you = NULL;
    struct device *dev_fft = NULL;
    struct device *dev_dac = NULL;
    uint32_t hash_fft = hash("fft");
    uint32_t hash_dac = hash("dac");
    uint32_t hash_out = hash("out");
    uint32_t output_storage[MAX_OUTPUTS] = {0};
    for (int i = 0; i < device_cnt; i++)
    {
        if (devices[i].hash == hash_you) {
            dev_you = &devices[i];
        } else if (devices[i].hash == hash_fft) {
            dev_fft = &devices[i];
        } else if (devices[i].hash == hash_dac) {
            dev_dac = &devices[i];
        }
        memcpy(output_storage, devices[i].output, sizeof(uint32_t) * devices[i].output_cnt);
        printf("Device %s outputs count sort: %d\n", devices[i].name, devices[i].output_cnt);
        for (int k = 0; k < devices[i].output_cnt; k++)
        {
            uint32_t minimum = UINT32_MAX;
            int min_index = -1;
            for (int j = 0; j < devices[i].output_cnt; j++)
            {
                if (output_storage[j] < minimum)
                {
                    minimum = output_storage[j];
                    min_index = j;
                }
            }
            devices[i].output[k] = output_storage[min_index];
            output_storage[min_index] = UINT32_MAX;
        }
        for (int j = 0; j < devices[i].output_cnt; j++)
        {
            devices[i].visited_output_results[j] = -1;
        }
    }
    assert(dev_you != NULL);

    loop_cnt = 0;
    if (!step2)
        result = search(devices, device_cnt, dev_you, hash_out, step2, 0);
    else {
        assert(dev_dac != NULL);
        assert(dev_fft != NULL);
        uint64_t dac_fft = search(devices, device_cnt, dev_dac, hash_fft, step2, 0);
        printf("dac to fft: %lu\n", dac_fft);
        uint64_t fft_dac = search(devices, device_cnt, dev_fft, hash_dac, step2, 0);
        printf("fft to dac: %lu\n", fft_dac);
        result = 0;

        for (int i = 0; i < device_cnt; i++)
        {
            for (int j = 0; j < devices[i].output_cnt; j++)
            {
                devices[i].visited_output_results[j] = -1;
            }
        }
        if (dac_fft > 0)
        {
            uint64_t intermediate_result = dac_fft;
            intermediate_result = search(devices, device_cnt, dev_you, hash_dac, step2, 0);
            dac_fft *= intermediate_result;
            printf("you to dac: %lu\n", intermediate_result);
            for (int i = 0; i < device_cnt; i++)
            {
                for (int j = 0; j < devices[i].output_cnt; j++)
                {
                    devices[i].visited_output_results[j] = -1;
                }
            }
            intermediate_result = search(devices, device_cnt, dev_fft, hash_out, step2, 0);
            dac_fft *= intermediate_result;
            printf("fft to out: %lu\n", intermediate_result);
        }
        for (int i = 0; i < device_cnt; i++)
        {
            for (int j = 0; j < devices[i].output_cnt; j++)
            {
                devices[i].visited_output_results[j] = -1;
            }
        }
        if (fft_dac > 0)
        {
            uint64_t intermediate_result = fft_dac;
            intermediate_result = search(devices, device_cnt, dev_you, hash_fft, step2, 0);
            fft_dac *= intermediate_result;
            printf("you to fft: %lu\n", intermediate_result);
            for (int i = 0; i < device_cnt; i++)
            {
                for (int j = 0; j < devices[i].output_cnt; j++)
                {
                    devices[i].visited_output_results[j] = -1;
                }
            }
            intermediate_result = search(devices, device_cnt, dev_dac, hash_out, step2, 0);
            fft_dac *= intermediate_result;
            printf("dac to out: %lu\n", intermediate_result);
        }
        result = dac_fft + fft_dac;
    }

    printf("loop cnt: %lu\n", loop_cnt);

    // process last pattern
    printf("Result: %lu\n", result);

    return result;
}


int main(int argc, char *argv[]) {
    uint64_t ret = 0;
    ret = process_file("aoc11_ex.txt", false);
    assert(ret == 5);
    ret = process_file("aoc11_data.txt", false);
    assert(ret == 497);
    ret = process_file("aoc11_ex2.txt", true);
    assert(ret == 2);
    ret = process_file("aoc11_data.txt", true);
    assert(ret == 358564784931864);
    return 0;
}