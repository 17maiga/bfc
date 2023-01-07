#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void generate();
void usage();
FILE *infile, *outfile;

int main(int argc, char **argv)
{
    char *bf_in, *c_out, *exec_out;

    if (argc<4) usage();

    bf_in = argv[1];
    c_out = argv[2];
    exec_out = argv[3];

    infile = fopen(bf_in, "r");

    if (infile != NULL) {
        outfile = fopen(c_out, "w");
        generate(infile);
        fclose(infile);
        fclose(outfile);
        char compile_instructions[] = "gcc -o ";
        strcat(compile_instructions, exec_out);
        strcat(compile_instructions, " ");
        strcat(compile_instructions, c_out);
        system(compile_instructions);
        exit(0);
    }
    exit(1);
}

void generate () {
    int buffer_size = 30000;
    int ptr = 0;
    int char_position = 1;
    int bracket_level = 0;

    char c;
    fprintf(outfile, "/*\n * This C code was automatically generated with the \"bfc compiler\".");
    fprintf(outfile, "\n * Made by 17maiga");
    fprintf(outfile, "\n**/\n#include <stdio.h>\n#include <stdlib.h>\n");
    fprintf(outfile, "\nint main() {\nchar a[%d], *ptr = a;\n", buffer_size);
    while ((c = fgetc(infile)) != EOF) {
        switch (c) {
            case '<': 
                if (ptr > 0) {
                    fprintf(outfile, "ptr--;\n");
                    ptr--;
                } else {
                    printf("Error: Cannot go below 0 at position %d\n", char_position);
                    exit(-1);
                }
                break;
            case '>':
                if (ptr < buffer_size - 1) {
                    fprintf(outfile, "ptr++;\n");
                    ptr++;
                } else {
                    printf("Error: Cannot go above %d at position %d\n", buffer_size, char_position);
                    exit(-1);
                }
                break;
            case '+': fprintf(outfile, "++*ptr;\n"); break;
            case '-': fprintf(outfile, "--*ptr;\n"); break;
            case '.': fprintf(outfile, "putchar(*ptr);\n"); break;
            case ',': fprintf(outfile, "*ptr=getchar();\n"); break;
            case '[':
                bracket_level++;
                fprintf(outfile, "while(*ptr){\n");
                break;
            case ']':
                if (bracket_level == 0) {
                    printf("Error: Unexpected ']' at position %d\n", char_position);
                    exit(-1);
                }

                bracket_level--;
                fprintf(outfile, "};\n");
                break;
            default: break;
        }

        char_position++;
    }

    if (bracket_level != 0) {
        printf("Error: Bracket level at the end of file: %d. Expected: 0\n", bracket_level);
        exit(-1);
    }

    fprintf(outfile, "exit(0);\n}");
}

void usage() {
    printf("The bfc compiler, by 17maiga\n");
    printf("Usage: \"bfc input_file_name.bf output_file_name.c exec_file_name\"\n");
    exit(1);
}
