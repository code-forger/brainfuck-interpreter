#include <stdio.h>
#include <stdint.h>

char instruction_buffer[30000] = {0};
uint32_t instruction_pointer;
uint32_t loop_stack[30000] = {0};
uint32_t loop_stack_pointer;
uint8_t cells[3000000] = {0};
uint32_t data_pointer;

void run_brainfuck()
{
    instruction_pointer = loop_stack_pointer = data_pointer = 0;

    while(instruction_buffer[instruction_pointer])
    {
        // printf( "ip %d: ", instruction_pointer );
        // printf( "i %c | ", instruction_buffer[instruction_pointer] );
        // printf( "lsp %d: ", loop_stack_pointer );
        // printf( "ls %d | ", loop_stack[loop_stack_pointer] );
        // printf( "dp %d: ", data_pointer );
        // printf( "c %d: ", cells[data_pointer] );
        // printf( "\n" );

        switch(instruction_buffer[instruction_pointer])
        {
            case '+':
                cells[data_pointer]++;
                break;
            case '-':
                cells[data_pointer]--;
                break;
            case '>':
                data_pointer++;
                break;
            case '<':
                data_pointer--;
                break;
            case '.':
                putchar(cells[data_pointer]);
                break;
            case ',':
                cells[data_pointer] = getchar();
                break;
            case '[':
                if(cells[data_pointer])
                {
                    loop_stack[++loop_stack_pointer] = instruction_pointer;
                }
                else
                {
                    int skip_loop_stack = 0;
                    while(instruction_buffer[++instruction_pointer] != ']' || skip_loop_stack)
                    {
                        if(instruction_buffer[instruction_pointer] == '[')
                        {
                            skip_loop_stack++;
                        }
                        else if(instruction_buffer[instruction_pointer] == ']')
                        {
                            skip_loop_stack--;
                        }
                    }
                }
                break;
            case ']':
                if(cells[data_pointer])
                {
                    instruction_pointer = loop_stack[loop_stack_pointer];
                }
                else
                {
                    loop_stack_pointer--;
                }
                break;
        }
        instruction_pointer++;
        if (instruction_pointer == 30000)
        {
            break;
        }
    }
    return;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("usage: %s <filename.bf>\n", argv[0]);
        return 0;
    }

    FILE *file = fopen(argv[1], "r");

    if(file == 0)
    {
        printf( "Could not open file\n" );
        return 0;
    }

    instruction_pointer = 0;
    char c;
    while((c = fgetc(file)) != EOF && instruction_pointer < 30000)
    {
        if ( c == '+' || c == '-' || c == '>' || c == '<' ||
             c == '[' || c == ']' || c == '.' || c == ',' )
        {
            instruction_buffer[instruction_pointer] = c;
            instruction_pointer++;
        }
    }
    fclose(file);

    if (instruction_pointer == 30000)
    {
        printf( "Too many characters in your file\n" );
        return 0;
    }

    run_brainfuck();

    printf("\n"); // since most BF programs wont bother to end their outbut with a new line
    return 0;
}