# Memory Management in C Programming Language

    THIS DOCUMENT IS IN PROGRESS

This repository contains my personal note's on *the safe way* to manage memory 
in C programming language that I gather from different resources. Suggestions
are welcome.

## Common Mistakes
C is NOT type-safe programming language and C doesn't protect us from doing
stupid things. This is the list of mistakes that we can made while doing the
allocation or deallocation manually:

1. Corrupt the heap by using a piece of memory as an incorrect type.
2. Allocate incorrect number of bytes memory.
3. Forget to fully initialize allocated memory.
4. Deallocate memory twice that lead us to Dangling Pointer problem.

The rest of this document is help us to avoid those mistakes.