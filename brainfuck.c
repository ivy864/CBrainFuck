#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAPE_LENGTH 5000
#define PROGRAM_LENGTH 5000
// this could be shorter but maybe some dumbass wants a program thats all loops
#define LOOP_LIST_LENGTH 2500
#define BF_TOKENS "+-.,<>[]"

typedef struct {
    char* list;
    int pointer;
} Loops;

int load_program(FILE* program_file, char* program) {
    char* token;
    int i = 0;

    int loops = 0;

    while (fscanf(program_file, "%c", token) != EOF) {

        if (strpbrk(BF_TOKENS, token) != 0) {
            *(program + i) = *token;
            i++;

            if (*token == '[') {
                loops++;
            }
            else if (*token == ']') {
                if (loops <= 0) {
                    return 1;
                }
                loops--;
            }
        }
    }

    if (loops > 0) {
        return 2;
    }

    return 0;
}

void interpret_program (char* program, char* tape) {

}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s [program]\n", argv[0]);
        return 0;
    }

    char* tape = malloc(TAPE_LENGTH * sizeof(char));
    FILE* program_file = fopen(argv[1], "r");
    char* program = malloc(PROGRAM_LENGTH * sizeof(char));
    
    switch (load_program(program_file, program)) {
        case 1:
            printf("Loop closed, but not started!\n");
            return 0;
        break;
        case 2:
            printf("Loop started, but not closed!\n");
            return 0;
        break;
    }
    printf("%s\n", program);

    return 0;
}
