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

typedef struct {
    int x;
    int y;
    char *msg;
} obj;

/* obj_new_unsafe: initialize new instance of OBJ that depends
 * on random block of memory pointed by M */
obj *obj_new_unsafe(int x, int y, char *m)
{
    obj *myobj = (obj *)malloc(sizeof(obj));
    if(myobj == NULL) {
        return NULL;
    }

    myobj->x = x;
    myobj->y = y;
    myobj->msg = m;

    return myobj;
}

/* obj_new_safe: initialize new instance of OBJ that self-contains
 * and doesn't depend on random pointer */
obj *obj_new_safe(int x, int y, char *m)
{
    obj *myobj = (obj *)malloc(sizeof(obj));
    if(myobj == NULL) {
        return NULL;
    }

    myobj->x = x;
    myobj->y = y;

    int lenm = strlen(m);
    myobj->msg = (char *)malloc((lenm + 1) * sizeof(char));
    strncpy(myobj->msg, m, lenm);
    myobj->msg[lenm] = '\0';

    return myobj;
}

int 
main()
{
    /* string allocation */
    /* mystr can be from STDIN or another sources */
    char *mystr = "hello";
    int lenmystr = strlen(mystr);

    char *str = (char *)malloc((lenmystr + 1) * sizeof(char));
    str = strncpy(str, mystr, lenmystr);
    str[lenmystr] = '\0';

    printf("mystr = str =>  %s = %s\n", mystr, str);
    free(str);

    /* Allocate sizeof(obj) bytes of memory */
    obj *mobj = (obj *)malloc(sizeof(obj));
    if(mobj == NULL) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    mobj->x = 1;
    mobj->y = 0;
    mobj->msg = "message";

    printf("Properly initialized; we have values as expected: \n");
    printf("x = %d\n", mobj->x);
    printf("y = %d\n", mobj->y);
    printf("m = %s\n", mobj->msg);
    free(mobj);
    mobj = NULL;

    /* Garbage values */
    obj *gobj = (obj *)malloc(sizeof(obj));
    if(gobj == NULL) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("Gargage values, we don't even expect to have this values: \n");
    printf("x = %d\n", gobj->x);
    printf("y = %d\n", gobj->y);
    printf("m = %s\n", gobj->msg);
    free(gobj);
    gobj = NULL;

    /* Using calloc */
    obj *cobj = (obj *)calloc(1, sizeof(obj));
    if(cobj == NULL) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("Calloc: \n");
    printf("x = %d\n", cobj->x);
    printf("y = %d\n", cobj->y);
    printf("m = %s\n", cobj->msg);
    free(cobj);
    cobj = NULL;

    /* Unsafe */
    char rmsg1[6] = {'h', 'e', 'l', 'l', 'o', '\0'};
    obj *unobj = obj_new_unsafe(0, 1, rmsg1);
    printf("Unsafe, dangling pointer problem: \n");
    printf("x = %d\n", unobj->x);
    printf("y = %d\n", unobj->y);
    printf("m = %s\n", unobj->msg);

    /* we try to modify the memory pointed by rmsg1; or even freed if 
     * the pointer is allocated */
    rmsg1[0] = 'C';
    rmsg1[1] = 'H';
    rmsg1[2] = 'A';
    rmsg1[3] = 'N';
    rmsg1[4] = 'G';
    rmsg1[5] = 'E';
    /* this will be reflected in uobj->msg */
    printf("m = %s\n", unobj->msg);
    free(unobj);

    /* safe */
    char rmsg2[6] = {'h', 'e', 'l', 'l', 'o', '\0'};
    obj *sobj = obj_new_safe(0, 1, rmsg2);
    printf("Safe version: \n");
    printf("x = %d\n", sobj->x);
    printf("y = %d\n", sobj->y);
    printf("m = %s\n", sobj->msg);

    rmsg2[0] = 'C';
    rmsg2[1] = 'H';
    rmsg2[2] = 'A';
    rmsg2[3] = 'N';
    rmsg2[4] = 'G';
    rmsg2[5] = 'E';

    /* the chanfe is not reflected on sobj->msg */
    printf("m = %s\n", sobj->msg);
    free(sobj->msg);
    free(sobj);

    // int nexp = 10;
    // obj *exsm = (obj *)malloc(nexp * sizeof(obj));
    // if(exsm == NULL) {
    //     fprintf(stderr, "error: %s\n", strerror(errno));
    //     exit(EXIT_FAILURE);
    // }
    // int iexp = 0;
    // for(iexp = 0; iexp < nexp; iexp++) {
    //     exsm[iexp].x = iexp;
    //     exsm[iexp].m = "malloc";
    // }

    // obj *exsc = (obj *)calloc(nexp, sizeof(obj));
    // if(exsc == NULL) {
    //     fprintf(stderr, "error: %s\n", strerror(errno));
    //     exit(EXIT_FAILURE);
    // }
    // for(iexp = 0; iexp < nexp; iexp++) {
    //     exsc[iexp].x = iexp;
    //     exsc[iexp].m = "calloc";
    // }

    // for(iexp = 0; iexp < nexp; iexp++) {
    //     printf("malloc: %2d %s\n", exsm[iexp].x, exsm[iexp].m);
    //     printf("calloc: %2d %s\n", exsc[iexp].x, exsc[iexp].m);
    // }

    // free(exsm);
    // free(exsc);
    return 0;
}