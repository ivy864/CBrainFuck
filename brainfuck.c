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

void load_program(FILE* program_file, char* program) {
    char* token;
    int i = 0;

    Loops loop_list;

    loop_list.list = malloc(LOOP_LIST_LENGTH * sizeof(char));
    loop_list.pointer = 0;

    while (fscanf(program_file, "%c", token) != EOF) {

        if (strpbrk(BF_TOKENS, token) != 0) {
            printf("%d\n", i);
            *(program + i) = *token;
            i++;
        }
    }

    free(loop_list.list);
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
    
    load_program(program_file, program);
    printf("%s\n", program);

    return 0;
}
