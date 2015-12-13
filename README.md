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
3. We made a Dangling Pointer.

The rest of this document is help us to avoid those mistakes.

## Allocation & Initialization
Allocation is performed using `malloc` or `calloc` function which defined in 
`stdlib.h`.

    void *malloc(size_t size);
    void *calloc(size_t nmemb, size_t size);

The differences between `malloc` and `calloc` is `malloc` doesn’t initializes 
the allocated memory which possibly contains garbage values and `calloc` 
initializes the allocated memory to zero.

### Allocate Memory for a String
The string allocation can be a tricky because string is terminated with a null
character that doesn’t count in the “length” of the string but does need a 
space.
    
    char *mystr = "hello";
    int lenmystr = strlen(mystr);

    char *str = (char *)malloc((lenmystr + 1) * sizeof(char));
    str = strncpy(str, mystr, lenmystr)
    str[lenstr] = '\0';

Usually we need this to create self-containable object that doesn't 
depend on random pointer which potentially causes a dangling pointer
problem.

### Allocate Memory for a Structure
If we want allocate block of memory for a structure we just use `malloc`

    typedef struct {
        int x;
        int y;
        char *msg;
    } obj;

    obj *myobj = (obj *)malloc(sizeof(obj));
    if(myobj == NULL) {
        /* perform error handling here */
    }

`malloc` returns a pointer to a newly allocated block `sizeof(obj)` bytes 
long, or a `NULL` pointer if the block could not be allocated (for 
example, if system is run of out memory).

`malloc` not initializes the newly allocated memory, so it may contain
garbage values (values from previous operation that are not cleared). So we 
must initialize manually.

    myobj->x = 0;
    myobj->y = 0;
    myobj->msg = "my message"; /* Or maybe we want NULL pointer instead */

Sometimes we want to create an instance of `obj` types with function like this

    obj *obj_new(int x, int y, char *m)
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
    
    obj *o = obj_new(x, y, m);

The implementation above is not safe. Note that we pass `char *m` or `char m[]`as a pointer. Which mean that `o->msg` now pointing to the memory where `m` is
pointed to.

The problem is when the `m` is modified outside this function scope and
it's will reflected to `o->msg`. If memory that pointed by `m` is get freed
then `o->msg` now pointing to the invalid memory, or in other words we have a
dangling pointer problem.

To fix this problem, we need to allocate new block of memory for `o->msg` and 
copy the values of `m`. Like so
    
    ...
    myobj->msg = (char *)malloc((lenm + 1) * sizeof(char));
    strncpy(myobj->msg, m, lenm);
    myobj->msg[lenm] = '\0';

    return myobj;
    ...

This allow us to return a self-contains struct which doen't depend on random
pointer.

### Allocate Memory for an Array of Objects
We can allocate the block of memory for `10` objects using malloc

    obj *objs = (obj *)malloc(10 * sizeof(obj));

or `calloc` (More recommended)

    obj *objs = (obj *)calloc(10, sizeof(obj));

`calloc` also returns the pointer to the allocated memory if succeed, 
otherwise it returns `NULL`.

### Mistake #1
Consider the example in `mistake-allocation.c`. This program will get compiled 
and running smoothly

    $ gcc -Wall -Wextra -ggdb mistake-allocation.c
    $ ./a.out
    sizeof(char) = 1 bytes
    sizeof(obj) = 16 bytes
    x = 1
    y = 0
    msg = message

You will notice here that we allocate memory for only `sizeof(char)` bytes 
which is equal to 1 bytes.

    obj *ex = (obj *)malloc(sizeof(char));

But we need 16 bytes. Since we writes more than allocated memory, heap is
get corrupted. We produce the mistake #1 this can cause an undefined
behaviour. This is why C is called unsafe because C doesn't protect us
from doing things like this.

On the other hand, we just learn that the argument of `malloc` is not 
checked against the type to which is the result is cast. So if we allocate
memory for an object we need to make sure that the number of bytes is correct.

    obj *ex = (obj *)malloc(sizeof(obj));

To avoid this mistake, we need to check our program using `valgrind`.

    $ valgrind ./a.out 
    ...
    sizeof(char) = 1 bytes
    sizeof(obj) = 16 bytes
    ==2170== Invalid write of size 4
    ==2170==    at 0x400773: main (mistake-allocation.c:35)
    ==2170==  Address 0x51de040 is 0 bytes inside a block of size 1 alloc'd
    ==2170==    at 0x4C28C20: malloc (vg_replace_malloc.c:296)
    ==2170==    by 0x40072F: main (mistake-allocation.c:30)
    ==2170== 
    ...

As you can see. In line `mistake-allocation.c:35` we perform an `Invalid write of size 4` and `Address 0x51de040 is 0 bytes inside a block of size 1 alloc'd`.

    ex->x = 1;

Since `1` is integer, he need 4 bytes in the memory and `ex->x` is 0 
bytes so we perform in invalid write here.

## Resources

1. CSE 341: Unsafe languages (C) [link](http://courses.cs.washington.edu/courses/cse341/04wi/lectures/26-unsafe-languages.html)
2. Debugging Memory Problems [link](http://valgrind.org/gallery/linux_mag.html)
3. C – Dynamic memory allocation [link](http://fresh2refresh.com/c/c-dynamic-memory-allocation/)
4. How an uninitialised variable gets a garbage value? [link](http://stackoverflow.com/a/1422774)