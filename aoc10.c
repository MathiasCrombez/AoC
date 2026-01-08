// Advent of Code 2025 - Day 10
// Used Co-Pilot extensively for the search algorithm
// Abandoned due to performance issues and lack of skills on matrix algebra
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>

static uint64_t result = 0;
#define MAX_RANGE 1000

int lights[MAX_RANGE] = {0};
int joltages[MAX_RANGE] = {0};
int buttons[MAX_RANGE] = {0};


static void display_data(int light_cnt, int button_cnt, int joltage_cnt) {
    printf("Lights: ");
    for (int i = 0; i < light_cnt; i++) {
        printf("%d", lights[i]);
    }
    printf("\nButtons: \n");
    for (int i = 0; i < button_cnt; i++) {
        for (int j = 0; j < light_cnt; j++) {
            printf("%d", buttons[i * light_cnt + j]);
        }
        printf("\n");
    }
    printf("Joltages: ");
    for (int i = 0; i < joltage_cnt; i++) {
        printf("%d ", joltages[i]);
    }
    printf("\n");
}

static int result_lights[MAX_RANGE];
static int result_joltage[MAX_RANGE];
static uint64_t process_line(int light_cnt, int button_cnt, int joltage_cnt, bool step2) {
    uint64_t line_result = UINT64_MAX;

    display_data(light_cnt, button_cnt, joltage_cnt);

    if (step2) {
        // Step 2: Find minimum button presses to reach target joltages
        // Use bounded DFS with aggressive pruning
        
        int press_counts[MAX_RANGE] = {0};
        int current_joltages[MAX_RANGE] = {0};
        
        // Calculate reasonable upper bound for each button
        int max_presses[MAX_RANGE];
        for (int b = 0; b < button_cnt; b++) {
            max_presses[b] = 0;
            for (int j = 0; j < joltage_cnt; j++) {
                if (buttons[b * light_cnt + j] && joltages[j] > max_presses[b]) {
                    max_presses[b] = joltages[j];
                }
            }
        }
        
        // Recursive DFS with pruning
        void search_step2(int btn_idx, int total_presses) {
            if (total_presses >= line_result) return; // prune
            
            if (btn_idx == button_cnt) {
                // Check if we've reached target
                bool matches = true;
                for (int j = 0; j < joltage_cnt; j++) {
                    if (current_joltages[j] != joltages[j]) {
                        matches = false;
                        break;
                    }
                }
                
                if (matches) {
                    line_result = total_presses;
                }
                return;
            }
            
            // Calculate minimum presses needed for this button based on remaining deficit
            int min_needed = 0;
            int max_needed = max_presses[btn_idx];
            
            // Check if we can skip this button
            search_step2(btn_idx + 1, total_presses);
            
            // Try pressing this button
            for (int presses = 1; presses <= max_needed && total_presses + presses < line_result; presses++) {
                // Apply button presses
                press_counts[btn_idx] = presses;
                for (int j = 0; j < light_cnt; j++) {
                    if (buttons[btn_idx * light_cnt + j]) {
                        current_joltages[j] += presses;
                    }
                }
                
                // Early termination: if any joltage exceeded target, stop
                bool exceeded = false;
                for (int j = 0; j < joltage_cnt; j++) {
                    if (current_joltages[j] > joltages[j]) {
                        exceeded = true;
                        break;
                    }
                }
                
                if (!exceeded) {
                    search_step2(btn_idx + 1, total_presses + presses);
                }
                printf("current joltages [");
                for (int j = 0; j < joltage_cnt; j++) {
                    printf("%d ", current_joltages[j]);
                }
                printf("] after pressing button %d %d times\n", btn_idx, presses);
                
                // Undo button presses
                for (int j = 0; j < light_cnt; j++) {
                    if (buttons[btn_idx * light_cnt + j]) {
                        current_joltages[j] -= presses;
                    }
                }
            }
            press_counts[btn_idx] = 0;
        }
        
        search_step2(0, 0);
        
    } else {
        // Step 1: Binary toggle problem
        uint64_t max_combinations = 1ULL << (uint64_t)button_cnt;
        printf("Processing %lu combinations\n", max_combinations);
        
        for (uint64_t combo = 0; combo < max_combinations; combo++) {
            memset(result_lights, 0, sizeof(int) * light_cnt);
        
        for (int b = 0; b < button_cnt; b++) {
            if (combo & (1ULL << (uint64_t)b)) {
                // Button b is pressed - toggle affected lights
                for (int l = 0; l < light_cnt; l++) {
                    if (buttons[b * light_cnt + l]) {
                        result_lights[l] ^= 1;
                    }
                }
            }
        }
        
        // Check if result matches target state in lights[]
        bool matches = true;
        for (int l = 0; l < light_cnt; l++) {
            if (result_lights[l] != lights[l]) {
                matches = false;
                break;
            }
        }
        
        if (matches) {
            // Count number of buttons pressed
            uint64_t num_buttons = 0;
            for (int b = 0; b < button_cnt; b++) {
                if (combo & (1ULL << (uint64_t)b)) {
                    num_buttons++;
                }
            }
            
            if (num_buttons < line_result) {
                line_result = num_buttons;
                printf("Found solution with combo %lu, buttons pressed: %lu\n", combo, num_buttons);
            }
        }
    }
    } // end if step2/else
    
    if (line_result == UINT64_MAX) {
        printf("No solution exists for this line!\n");
        line_result = 0;
    } else {
        printf("Minimum buttons to press: %lu\n", line_result);
    }

    return line_result;
}

static uint64_t process_file(const char *filename, bool step2) {
    int ret = 0;

    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    // reset result
    result = 0;

    int cnt = 0;
    char num_str[32] = {0};
    int light_cnt = 0;
    int button_cnt = 0;
    int joltage_cnt = 0;
    do {
        char c = fgetc(file);

        switch (c)
        {
        case '\n':
        case EOF:
        case 0:
            printf("####################################################################################\n");
            printf("Processing line %d with %d lights, %d buttons, %d joltages\n", (cnt), light_cnt, button_cnt, joltage_cnt);
            cnt ++;
            result += process_line(light_cnt, button_cnt, joltage_cnt, step2);

            light_cnt = 0;
            button_cnt = 0;
            joltage_cnt = 0;
            memset(lights, 0, sizeof(lights));
            memset(buttons, 0, sizeof(buttons));
            memset(joltages, 0, sizeof(joltages));

            if (c == EOF || c == 0)
            {
                ret = 42;
            }
            break;
        case '[':
            while ((c = fgetc(file)) != ']') {
                lights[light_cnt++] = (c == '#') ? 1 : 0;
            }
            break;

        case '(':
            while ((c = fgetc(file)) != ')') {
                // No button over 9
                if (c == ',') {
                    continue;
                }
                buttons[button_cnt * light_cnt + (c - '0')] = 1;
            }
            button_cnt++;
            break;
        case '{':
            joltage_cnt = 0;
            int j = 0;
            while ((c = fgetc(file)) != '}') {
                if (c == ',') {
                    joltages[joltage_cnt++] = strtol(num_str, NULL, 10);
                    j = 0;
                    memset(num_str, 0, sizeof(num_str));
                    continue;
                }
                num_str[j++] = c;
            }
            joltages[joltage_cnt++] = strtol(num_str, NULL, 10);
            break;
        default:
            break;
        }
    } while (ret == 0);

    fclose(file);

    if (ret != 42) {
        return 1;
    }
    printf("Result: %lu\n\n", result);

    return result;
}


int main(int argc, char *argv[]) {
    uint64_t ret = 0;
    ret = process_file("aoc10_ex.txt", false);
    assert(ret == 7);
    ret = process_file("aoc10_data.txt", false);
    assert(ret == 479);
    ret = process_file("aoc10_ex.txt", true);
    assert(ret == 33);
    ret = process_file("aoc10_data.txt", true);
//    assert(ret == 1516172795);
    return 0;
}