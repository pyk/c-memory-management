/*
 * This code accompanied section "Allocation"
 * compile:
 *
 *    gcc -Wall -Wextra -ggdb mistake-allocation.c
 *
 * Run valgrind
 *
 *    valgrind ./a.out
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct {
    int x;
    int y;
    char *msg;
} obj;

int 
main()
{
    printf("sizeof(char) = %ld bytes\n", sizeof(char));
    printf("sizeof(obj) = %ld bytes\n", sizeof(obj));

    /* NOTICE: sizeof(char), it should be sizeof(obj) */
    obj *ex = (obj *)malloc(sizeof(char));
    if(ex == NULL) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    ex->x = 1;
    ex->y = 0;
    ex->msg = "message";
    printf("x = %d\n", ex->x);
    printf("y = %d\n", ex->y);
    printf("msg = %s\n", ex->msg);
    free(ex);
    return 0;
}