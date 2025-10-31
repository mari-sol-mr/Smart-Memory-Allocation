#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
/* We want the header to be 16-byte aligned in order to satisty the strictest alignment requirement of 
any type. On most modern 64-bit systsems, the strictest data types require 16-byte alignment (e.g. long double).
If the header always takes up a 16-byte block, the pointer to the allocated memory will be at the start of the 
next 16-byte block. This guarantes that in the worst case, if an allocated data type has 16 bytes, it will be
16-byte aligned, meaning that the CPU will be able to fetch it in one cycle. 
*/
typedef char ALIGN[16];

union header {
	struct {
		size_t size;
		unsigned is_free;
		union header *next;
	} s;
	ALIGN stub;
};

typedef union header header_t;

pthread_mutex_t global_malloc_lock = PTHREAD_MUTEX_INITIALIZER;

header_t *head = NULL, *tail = NULL;

header_t *get_free_block(size_t size)
{
	header_t *curr = head;
	while(curr) {
		if (curr->s.is_free && curr->s.size >= size)
			return curr;
		curr = curr->s.next;
	}
	return NULL;
}

void *malloc_cutom(size_t size)
{
    // Size is 0. Do not allocate memory.
    if (!size)
		return NULL;
	

	
	
	pthread_mutex_lock(&global_malloc_lock);
	
	header_t *header = get_free_block(size);
	
    // If a free block was found.
	if (header) {
		header->s.is_free = 0;
		pthread_mutex_unlock(&global_malloc_lock);
		// Return the byte right after the header (the start of the free block.)
		return (void*)(header + 1);
	}
	
	// Otherwise, add more memory to the heap using sbrk.
	size_t total_size = sizeof(header_t) + size;
	void *block = sbrk(total_size);
	
	// Our memory request failed. 
    if (block == (void*) -1) {
    		pthread_mutex_unlock(&global_malloc_lock);
    		return NULL;
    }
    
    // Add the new header to the heap.
    header = block;
    header->s.size = size;
    header->s.is_free = 0;
    header->s.next = NULL;
    
    if (!head) head = header;
    if (tail) tail->s.next = header;
    tail = header;
    
    pthread_mutex_unlock(&global_malloc_lock);
    return (void*)(header + 1);
}


void free(void *block)
{
	if (!block)
	    // Nothing to free.
		return;
	
	pthread_mutex_lock(&global_malloc_lock);

    // Header is one byte before the block pointer.
    header_t *header = (header_t*)block - 1;
    
    void * end_of_block = (char*)block + header->s.size;
    // Check if block is at the end of the heap.
    if (end_of_block == sbrk(0)) {
        
        // Release the block from our list.
        if (head == tail) head = tail = NULL;
        else
        {
            // Remove the last node in the LL.
            header_t *tmp = head;
            while (tmp)
            {
                // Is the next node the tail node?
				if(tmp->s.next == tail) {
				    // Current node is the new tail node. 
					tmp->s.next = NULL;
					tail = tmp;
				}
				tmp = tmp->s.next;
            }
        }
        
        // Release the block from heap memory.
		sbrk(0 - sizeof(header_t) - header->s.size);
		pthread_mutex_unlock(&global_malloc_lock);
		return;
        
    }
    
    // Else, simply set the block to free for later use.
	header->s.is_free = 1;
	pthread_mutex_unlock(&global_malloc_lock);
};



int main()
{
    printf("Hello World");

    return 0;
}

