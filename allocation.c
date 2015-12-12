/*
 * This code accompanied section "Allocation"
 * compile:
 *
 *    gcc -Wall -Wextra -ggdb allocation.c
 *
 * Run valgrind
 *
 *    valgrind ./a.out
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct example_t {
    int x;
    char *m;
};

int 
main()
{
    /* Allocate memory for an object */
    struct example_t *ex = (struct example_t *)malloc(sizeof(struct example_t));
    if(ex == NULL) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    ex->x = 10000;
    ex->m = "message";
    printf("x = %d\n", ex->x);
    printf("m = %s\n", ex->m);
    free(ex);

    /* Allocate memory for an array of objects */
    int nexp = 10;
    struct example_t *exsm = (struct example_t *)malloc(nexp * sizeof(struct example_t));
    if(exsm == NULL) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    int iexp = 0;
    for(iexp = 0; iexp < nexp; iexp++) {
        exsm[iexp].x = iexp;
        exsm[iexp].m = "malloc";
    }

    struct example_t *exsc = (struct example_t *)calloc(nexp, sizeof(struct example_t));
    if(exsc == NULL) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    for(iexp = 0; iexp < nexp; iexp++) {
        exsc[iexp].x = iexp;
        exsc[iexp].m = "calloc";
    }

    for(iexp = 0; iexp < nexp; iexp++) {
        printf("malloc: %2d %s\n", exsm[iexp].x, exsm[iexp].m);
        printf("calloc: %2d %s\n", exsc[iexp].x, exsc[iexp].m);
    }

    free(exsm);
    free(exsc);
    return 0;
}