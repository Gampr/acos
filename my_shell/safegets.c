#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <errno.h>
#include "header.h"

/* Allocate memory for your string, read it from FILE * file
 * If string can't fit in heap, safe_gets cut it into several pieces
 *
 * Return values:
 * ENOMEM - if haven't enough memory(but safe so much so can safe)
 * ENOENT - if bad reading file
 * 0 - success
 * (*str_pointer) == NULL if you can't use new string
 */
int safe_gets(FILE *file, char **str_pointer) {
    size_t page_size;
    size_t size;
    char *ch;
    int sym;
    size = page_size = (size_t)sysconf(_SC_PAGE_SIZE);
    ch = (*str_pointer) = (char *)malloc(sizeof(char *) * size);
    if(NULL == (*str_pointer))
        return ENOMEM;
    while((sym = fgetc(file)) != '\n') {
        if(EOF == sym) {
            if(!feof(file)) {
                free(*str_pointer);
                (*str_pointer) = NULL;
                return ENOENT;
            }
            break;
        }
        if(((*str_pointer) + (size - 1)) == ch) {
            char *tmp;
            tmp = realloc(*str_pointer, sizeof(char *) * (size + page_size));
            if(tmp == NULL) {
                (*ch) = '\0';
                return ENOMEM;
            }
            (*str_pointer) = tmp;
            ch = *(str_pointer) + size - 1;
            size += page_size;
        }
        (*ch) = sym;
        ++ ch;
    }
    (*ch) = '\0';
    return 0;
}
