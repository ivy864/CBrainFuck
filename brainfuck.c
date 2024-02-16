#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAPE_LENGTH 5000
#define PROGRAM_LENGTH 5000
// this could be shorter but maybe some dumbass wants a program thats all loops
#define LOOP_LIST_LENGTH 2500
#define BF_TOKENS "+-.,<>[]"

char *program;
unsigned char *tape;
int loop_count = 0;
int program_len;

int load_program(FILE *program_file, char *program) {
    char *token;
    int i = 0;

    int loops = 0;

    while (fscanf(program_file, "%c", token) != EOF) {

        if (strpbrk(BF_TOKENS, token) != 0) {
            *(program + i) = *token;
            i++;

            if (*token == '[') {
                loops++;
                loop_count++;
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
unsigned char *init_tape(int tape_length) {
    for (int i = 0; i < tape_length; i++) {
        tape[i] = 0;
    }
    return tape;
}

void interpret_program() {
    char instruction;
    int tape_pointer = 0;
    int instruction_pointer = 0;
    // starts at -1 cause fuck you it works
    int loop_pointer = -1;
    int loops[loop_count];

    while (instruction_pointer < program_len) {
        instruction = program[instruction_pointer];
        instruction_pointer++;
        switch (instruction) {
        case '+':
            tape[tape_pointer]++;
            break;
        case '-':
            tape[tape_pointer]--;
            break;
        case '>':
            tape_pointer++;
            break;
        case '<':
            if (tape_pointer == 0) {
                tape_pointer = TAPE_LENGTH;
            }
            tape_pointer--;
            break;
        case '[':
            loop_pointer++;
            loops[loop_pointer] = instruction_pointer;
            break;
        case ']':
            if (tape[tape_pointer] == 0) {
                loop_pointer--;
            }
            else {
                instruction_pointer = loops[loop_pointer];
            }
            break;
        case '.':
            printf("%c", tape[tape_pointer]);
            printf("%d", tape[tape_pointer]);
            break;
        case ',':
            scanf("%c", tape + tape_pointer);
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s [program]\n", argv[0]);
        return 1;
    }

    FILE *program_file = fopen(argv[1], "r");

    if (program_file == NULL) {
        printf("File %s not found!\n", argv[1]);
        return 1;
    }

    tape = malloc(TAPE_LENGTH);

    // get program length
    fseek(program_file, 0, SEEK_END);
    program_len = ftell(program_file);
    fseek(program_file, 0, SEEK_SET);

    program = malloc(program_len * sizeof(char));

    if (program == NULL) {
        printf("Program is too large! Get more ram or something.\n");
        return 1;
    }

    switch (load_program(program_file, program)) {
    case 1:
        printf("Loop closed, but not started!\n");
        return 1;
        break;
    case 2:
        printf("Loop started, but not closed!\n");
        return 1;
        break;
    }
    printf("%s\n", program);

    init_tape(TAPE_LENGTH);

    interpret_program();

    return 0;
}
