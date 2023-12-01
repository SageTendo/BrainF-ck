#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

#define BUFFSIZE 50000
#define TAPESIZE 300000
#define TRUE 1
#define FALSE 0

/**
 * @brief Handles the file specified by the filename.
 * 
 * @param filename The name of the file to be handled.
 * @param fp The file pointer to be set for the file.
 * @return Returns TRUE if the file is successfully opened, otherwise returns FALSE.
 */
int handle_file(char *filename, FILE **fp);

/**
 * @brief Parses the BrainFuck program from the file.
 * 
 * @param fp The file pointer from which the program is to be parsed.
 * @return Returns TRUE if the program is parsed successfully, otherwise returns FALSE.
 */
int parse_program(FILE *fp);

/**
 * @brief Executes the parsed BrainFuck program.
 * 
 * @return Returns TRUE if the program is executed successfully, otherwise returns FALSE.
 */
int execute();

char *program; /**< The BrainFuck program stored as a string. */
int *tape; /**< The array representing the BrainFuck tape. */
int *jumps; /**< The array storing the jump positions for '[' and ']'. */

/**
 * @brief The main function for executing the BrainFuck interpreter.
 * 
 * @param argc The number of command-line arguments.
 * @param argv An array of pointers to the command-line arguments.
 * @return Returns the exit code of the program.
 */
int main(int argc, char* argv[])
{
    FILE *fp;
    int status, exit_code = EXIT_SUCCESS;

    /* Check if the BrainFuck program file is provided as an argument */
    if (argc == 1) {
        printf("Please provide a BrainFuck program...\n");
        return EXIT_FAILURE;
    }
    
    /* Handle the provided file */
    status = handle_file(argv[1], &fp);
    if (!status) {
        return EXIT_FAILURE;
    }

    /* Read in the program from the file */
    status = parse_program(fp);
    if (!status) {
        status = fclose(fp);
        if (status != 0) {
            printf("Failed to close file stream...\n");
        }
        return EXIT_FAILURE;
    }

    /* Execute the BrainFuck program */
    status = execute();
    if (!status) {
        exit_code = EXIT_FAILURE;
    }
    
    /* Free allocated memory and resources */
    fclose(fp);
    free(program);
    free(jumps);
    free(tape);
    return exit_code;
}

/**
 * @brief Handles the provided file.
 * 
 * @param filename The name of the file to be handled.
 * @param fp The file pointer to be set for the file.
 * @return Returns TRUE if the file is successfully opened, otherwise returns FALSE.
 */
int handle_file(char *filename, FILE **fp)
{
    *fp = fopen(filename, "r");
    if (*fp == NULL) {
        printf("Could not parse the provided program...\n");
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief Parses the BrainFuck program.
 * 
 * @param fp The file pointer from which the program is to be parsed.
 * @return Returns TRUE if the program is parsed successfully, otherwise returns FALSE.
 */
int parse_program(FILE *fp)
{
    /* Allocate memory for the program buffer */
    program = (char *) calloc(BUFFSIZE, sizeof(char));
    if (program == NULL) {
        printf("Buffer allocation error...\n");
        return FALSE;
    }

    /* Read characters from the file and store valid BrainFuck instructions */
    char ch;
    int i = 0;
    while ((ch = fgetc(fp)) != EOF) {
        switch (ch) {
            case '+':
            case '-':
            case '[':
            case ']':
            case ',':
            case '.':
            case '>':
            case '<':
                program[i++] = (char) ch;
                break;
            default:
                break;
        }
    }
    
    /* Null-terminate the program */
    program[i] = '\0';
    return TRUE;
}

/**
 * @brief Handles the loops in the program.
 * 
 * @return Returns TRUE if the loops are handled successfully, otherwise returns FALSE.
 */
int handle_loops()
{
    int program_counter;

    /* Allocate memory for the jump array */
    jumps = malloc(sizeof(int) * TAPESIZE);
    if (jumps == NULL) {
        printf("Jump memory allocation error...\n");
        return FALSE;
    }

    /* Initialize the jump array */
    for (int i = 0; i < TAPESIZE; i++) {
        jumps[i] = -1;
    }

    /* Find the positions of '[' and ']' in the program,
    and store their corresponding jump positions */
    program_counter = 0;
    while(program[program_counter] != '\0') {
        switch (program[program_counter]) {
            case '[':
                /* Push the current program counter position onto the stack */
                push(program_counter);
                break;
            case ']':
                /* Store the jump position for the corresponding '[' */
                jumps[pop()] = program_counter;
                break;
        }
        program_counter++;
    }
    
    /* Check for unclosed loops */
    if (peek() != -1) {
        printf("Unclosed brace found at position %d...\n", peek());
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief Executes the BrainFuck program.
 * 
 * @return Returns TRUE if the program is executed successfully, otherwise returns FALSE.
 */
int execute()
{
    int program_counter, tape_index;
    
    /* Allocate memory for the tape */
    tape = (int *) calloc(TAPESIZE, sizeof(int));
    if (tape == NULL) {
        printf("Tape allocation error...\n");
        return FALSE;
    }

    /* Handle loops in the program */
    int status = handle_loops();
    if (!status) {
        return status;
    }
    
    /* Initialize tape index, program counter, and loop count */
    tape_index = 0;
    program_counter = 0;

    /* Execute the BrainFuck instructions */
    while (program[program_counter] != '\0') {
        switch(program[program_counter]) {
            case '[':
                if (tape[tape_index] == 0) {
                    /* Leave while loop, jump to the end of the corresponding ']' */
                    program_counter = jumps[program_counter] + 1;
                    continue;
                }

                /* Push the current program counter position onto the stack */
                push(program_counter);
                break;
            case ']':
                if (tape[tape_index] != 0) {
                    /* Retrieve the last '[' position from the stack, 
                    and jump back to its position + 1*/
                    program_counter = peek() + 1;
                    continue;
                }
                
                /* Pop the last '[' position from the stack, discarding it basically. */
                pop();
                break;
            case '+':
                /* Increment the value at the current tape position */
                if (tape[tape_index] + 1 > 255)
                    tape[tape_index] = 0;
                else
                    tape[tape_index]++;
                break;
            case '-':
                /* Decrement the value at the current tape position */
                if (tape[tape_index] - 1 < 0)
                    tape[tape_index] = 255;
                else
                    tape[tape_index]--;
                break;
            case ',':
                /* Input operation: Read a character from the input */
                tape[tape_index] = getchar();
                break;
            case '.':
                /* Output operation: Print the character at the current tape position */
                putchar(tape[tape_index]);
                break;
            case '>':
                /* Move the tape pointer to the right */
                if (tape_index + 1 > TAPESIZE) 
                    tape_index = 0;
                else
                    tape_index++;
                break;
            case '<':
                /* Move the tape pointer to the left */
                if (tape_index - 1 < 0) 
                    tape_index = 0;
                else
                    tape_index--;
                break;
        }
        program_counter++; /* Move to the next instruction in the program */
    }

    printf("\n"); /* Print a newline at the end of the program execution */
    return TRUE;
}

