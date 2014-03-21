#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <errno.h>
#include "header.h"

int safe_gets(FILE *file, char **str_pointer) {
    size_t size = (size_t)sysconf(_SC_PAGE_SIZE);
    char *ch;
    int sym;
    ch = (*str_pointer) = (char *)malloc(sizeof(char *) * size);
    if(NULL == (*str_pointer))
        return ENOMEM;
    while((sym = fgetc(file)) != '\n') {
        if(EOF == sym) {
            if(!feof(file)) {
                free(*str_pointer);
                str_pointer = NULL;
                return ENOENT;
            }
            break;
        }
        if((char *)(str_pointer + (size - 1)) == ch) {
            char *tmp;
            tmp = realloc(*str_pointer, 2*size);
            if(tmp == NULL) {
                (*ch) = '\0';
                return ENOMEM;
            }
            free(*str_pointer);
            (*str_pointer) = tmp;
            ch = (char *)(str_pointer + (size - 1));
            size *= 2;
        }
        (*ch) = sym;
        ++ ch;
    }
    (*ch) = '\0';
    return 0;
}
