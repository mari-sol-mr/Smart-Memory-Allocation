# Smart-Memory-Allocation

Simple memory manager implements malloc and free using a simple memory allocation model:    
  
Blocks of allocated memory are stored on the heap.   
When memory is free, blocks are only removed from the heap if they are the top of the heap,   
otherwise they remain on the stack as a free memory block.


**Key lesson:**  The importance of memory alignment.

Specifically for the header of the linked list that keeps track of the allocated memory blocks on the heap:
We want the header to be 16-byte aligned in order to satisty the strictest alignment requirement of 
any type. On most modern 64-bit systsems, the strictest data types require 16-byte alignment (e.g. long double).
If the header always takes up a 16-byte block, the pointer to the allocated memory will be at the start of the 
next 16-byte block. This guarantes that in the worst case, if an allocated data type has 16 bytes, it will be
16-byte aligned, meaning that the CPU will be able to fetch it in one cycle. 
