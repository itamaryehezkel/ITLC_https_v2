#ifndef ITL_H
#define ITL_H

#define ITL_C     "./code.itl"
#define DEBUG_LEVEL 0
#define TOKEN_DUMP if(DEBUG_LEVEL & 1)
#define TRACE_PARSER if(DEBUG_LEVEL & 2)
#define TRACE_AST if(DEBUG_LEVEL & 4)
#define VAR_DUMP if(DEBUG_LEVEL & 8)
#define TRACE_VARS if(DEBUG_LEVEL & 16)
#define DUMP_PATHS if(DEBUG_LEVEL & 32)
#define DUMP_SQL_GETTER if(DEBUG_LEVEL & 64)
#define TRACE_SYS_FILE_READ_CALLS if(DEBUG_LEVEL & 128)


#include "xxhash.h"
#include "structs.h"
#include "pp.c"
#include "tokens.c"
#include "parser.c"
#include "ast.c"

Token * tokens;
char * itlc;
Node * program; // = parse_program(tokens, itlc);
/*
cat ~/.bash_profile
cat ~/.bash_login
cat ~/.profile
cat ~/.bashrc
sudo grep -r "Last login" /etc/
sudo grep -r "lastlog" /home/itamar/

*/

unsigned char * load_itl(){
    fprintf(stdout, "\e[90mLoaded ITLC from: %s\e[0m\n", ITL_C);
    FILE* file = fopen(ITL_C, "r");
    if (!file) {
        perror("Failed to open file");
        return "";
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        perror("fseek failed");
        fclose(file);
        return "";
    }

    long size = ftell(file);
    if (size < 0) {
        perror("ftell failed");
        fclose(file);
        return "";
    }
    rewind(file);

    unsigned char* buffer = (unsigned char*)calloc(size/sizeof(unsigned char), sizeof(unsigned char));;
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(file);
        return "";
    }

    size_t read = fread(buffer, 1, size, file);
    fclose(file);

    if (read != (size_t)size) {
        fprintf(stderr, "Only read %zu of %ld bytes\n", read, size);
        free(buffer);
        return "";
    }

    return buffer;
}

void load_program(){
    itlc = load_itl();
    tokens = tokenise(itlc);
    program = parse_program(tokens, itlc);

}

#endif
