# Memory Management in C Programming Language

    THIS DOCUMENT IS IN PROGRESS

This repository contains my personal note's on *the safe way* to manage memory 
in C programming language that I gather from different resources. Suggestions
are welcome.

## Common Mistakes
C is NOT type-safe programming language and C doesn't protect us from doing
stupid things. This is the list of mistakes that we can made while doing the
allocation or deallocation manually:

1. Allocate incorrect number of bytes memory.
2. Forget to fully initialize allocated memory.
3. Deallocate memory twice that lead us to Dangling Pointer problem.

The rest of this document is help us to avoid those mistakes.

## Allocation
Allocation is performed using `malloc` or `calloc` function which defined in 
`stdlib.h`.

    void *malloc(size_t size);
    void *calloc(size_t nmemb, size_t size);

### Allocate Memory for an Object
If we want allocate memory for an object we just use `malloc`

    struct obj_t *myobj = (struct obj_t *)malloc(sizeof(struct obj_t));
    if(myobj == NULL) {
        /* perform error handling here */
    }

`malloc` will returns the pointer to the allocated memory if succeed, 
otherwise it returns `NULL` (for example, if system is run of out memory).

### Allocate Memory for an Array of Objects
We can allocate the block of memory for `10` objects using malloc

    struct obj_t *objs = (struct obj_t *)malloc(10 * sizeof(struct obj_t));

or `calloc` (More recommended)

    struct obj_t *objs = (struct obj_t *)calloc(10, sizeof(struct obj_t));

`calloc` also returns the pointer to the allocated memory if succeed, 
otherwise it returns `NULL`.

### Mistake #1
Consider the following example

    /* mistake-allocation.c */
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
        ex->x = 10000;
        ex->m = "message";
        printf("x = %d\n", ex->x);
        printf("m = %s\n", ex->m);
        free(ex);
        return 0;
    }

This program will get compiled and running smoothly

    $ gcc -Wall -Wextra -ggdb allocation.c
    $ ./a.out 
    sizeof(char) = 1 bytes
    sizeof(struct example_t) = 16 bytes
    x = 10000
    m = message

You will notice here that we allocate memory for only 1 bytes

    struct example_t *ex = (struct example_t *)malloc(sizeof(char));

But we need 16 bytes. Since we writes more than allocated memory, heap is
get corrupted. We produce the mistake #1 this can cause an undefined
behaviour. This is why C is called unsafe because C doesn't protect us
from doing things like this.

On the other hand, we just learn that the argument of `malloc` is not 
checked against the type to which is the result is cast. So if we allocate
memory for an object we need to make sure that the number of bytes is correct.

    struct example_t *ex = (struct example_t *)malloc(sizeof(struct example_t));

To avoid this mistake, we need to check our program using `valgrind`.

    $ valgrind ./a.out 
    ==1374== Memcheck, a memory error detector
    ==1374== Copyright (C) 2002-2013, and GNU GPL'd, by Julian Seward et al.
    ==1374== Using Valgrind-3.10.0 and LibVEX; rerun with -h for copyright info
    ==1374== Command: ./a.out
    ==1374== 
    sizeof(char) = 1 bytes
    sizeof(struct example_t) = 16 bytes
    ==1374== Invalid write of size 4
    ==1374==    at 0x4005D8: main (allocation.c:28)
    ==1374==  Address 0x51de040 is 0 bytes inside a block of size 1 alloc'd
    ==1374==    at 0x4C28C20: malloc (vg_replace_malloc.c:296)
    ==1374==    by 0x4005CF: main (allocation.c:27)
    ==1374== 
    ==1374== Invalid write of size 8
    ==1374==    at 0x4005E2: main (allocation.c:29)
    ==1374==  Address 0x51de048 is 7 bytes after a block of size 1 alloc'd
    ==1374==    at 0x4C28C20: malloc (vg_replace_malloc.c:296)
    ==1374==    by 0x4005CF: main (allocation.c:27)
    ==1374== 
    ==1374== Invalid read of size 4
    ==1374==    at 0x4005EE: main (allocation.c:30)
    ==1374==  Address 0x51de040 is 0 bytes inside a block of size 1 alloc'd
    ==1374==    at 0x4C28C20: malloc (vg_replace_malloc.c:296)
    ==1374==    by 0x4005CF: main (allocation.c:27)
    ==1374== 
    x = 10000
    ==1374== Invalid read of size 8
    ==1374==    at 0x400605: main (allocation.c:31)
    ==1374==  Address 0x51de048 is 7 bytes after a block of size 1 alloc'd
    ==1374==    at 0x4C28C20: malloc (vg_replace_malloc.c:296)
    ==1374==    by 0x4005CF: main (allocation.c:27)
    ==1374== 
    m = message
    ==1374== 
    ==1374== HEAP SUMMARY:
    ==1374==     in use at exit: 0 bytes in 0 blocks
    ==1374==   total heap usage: 1 allocs, 1 frees, 1 bytes allocated
    ==1374== 
    ==1374== All heap blocks were freed -- no leaks are possible
    ==1374== 
    ==1374== For counts of detected and suppressed errors, rerun with: -v
    ==1374== ERROR SUMMARY: 4 errors from 4 contexts (suppressed: 0 from 0)

As you can see 
    
    ...
    ==1374== Invalid write of size 4
    ==1374==    at 0x4005D8: main (allocation.c:28)
    ==1374==  Address 0x51de040 is 0 bytes inside a block of size 1 alloc'd
    ==1374==    at 0x4C28C20: malloc (vg_replace_malloc.c:296)
    ==1374==    by 0x4005CF: main (allocation.c:27)
    ==1374== 
    ...

In line `28` we perform an `Invalid write of size 4` and `Address 0x51de040 is 0 bytes inside a block of size 1 alloc'd`.

    ex->x = 10000;

Since `10000` is integer, he need 4 bytes in the memory and `ex->x` is 0 
bytes so we perform in invalid write here.

