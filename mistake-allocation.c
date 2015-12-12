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

struct example_t {
    int x;
    char *m;
};

int 
main()
{
    printf("sizeof(char) = %ld bytes\n", sizeof(char));
    printf("sizeof(struct example_t) = %ld bytes\n", sizeof(struct example_t));

    /* NOTICE: sizeof(char), it should be sizeof(struct example_t) */
    struct example_t *ex = (struct example_t *)malloc(sizeof(char));
    if(ex == NULL) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    ex->x = 10000;
    ex->m = "message";
    printf("x = %d\n", ex->x);
    printf("m = %s\n", ex->m);
    free(ex);
    return 0;
}