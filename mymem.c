#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "mymem.h"
#include <time.h>


/* The main structure for implementing memory allocation.
 * You may change this to fit your implementation.
 */

struct memoryList
{
  // doubly-linked list
  struct memoryList *last;
  struct memoryList *next;

  int size;            // How many bytes in this block?
  char alloc;          // 1 if this block is allocated,
                       // 0 if this block is free.
  void *ptr;           // location of block in memory pool.
};

struct memoryList* first_block(size_t requested);
struct memoryList* best_block(size_t requested);
struct memoryList* worst_block(size_t requested);
struct memoryList* next_block(size_t requested);

strategies myStrategy = NotSet;    // Current strategy


size_t mySize;
void *myMemory = NULL;

static struct memoryList *head;
static struct memoryList *next;


/* initmem must be called prior to mymalloc and myfree.

   initmem may be called more than once in a given exeuction;
   when this occurs, all memory you previously malloc'ed  *must* be freed,
   including any existing bookkeeping data.

   strategy must be one of the following:
		- "best" (best-fit)
		- "worst" (worst-fit)
		- "first" (first-fit)
		- "next" (next-fit)
   sz specifies the number of bytes that will be available, in total, for all mymalloc requests.
*/

void initmem(strategies strategy, size_t sz)
{
	myStrategy = strategy;

	/* all implementations will need an actual block of memory to use */
	mySize = sz;

	if (myMemory != NULL)
  {
    free(myMemory);
  }
  if(head != NULL){
     free(head);
   }

	/* TODO: release any other memory you were using for bookkeeping when doing a re-initialization! */

	myMemory = malloc(sz);
  head = malloc(sizeof(struct memoryList));
  head->size = sz;
  head->alloc = 0;
  head->ptr = myMemory;
  next = head;
  head->last = head;
  head->next = head;

}

/* Allocate a block of memory with the requested size.
 *  If the requested block is not available, mymalloc returns NULL.
 *  Otherwise, it returns a pointer to the newly allocated block.struct memoryList* first_block(size_t requested) {

 *  Restriction: requested >= 1
 */

void *mymalloc(size_t requested)
{
	assert((int)myStrategy > 0);

struct memoryList *memBlock = NULL;
struct memoryList *i = head;
struct memoryList *min;
struct memoryList *max;
struct memoryList *ptrNext;
	switch (myStrategy)
	  {
	  case NotSet:
	           fprintf(stderr, "myalloc: Invalid memory management strategy! %d\n", myStrategy);
             break;
	  case First:
	            //i = head;
              do{
                if(i->alloc == 0 && i->size >= requested){
                  memBlock = i;
                  break;
                }
              }while((i = i->next) != head);
              break;
	  case Best:
	           // i = head;
              min = NULL;
              do{
                if(i->alloc == 0 && i->size >= requested && (!min || i->size < min->size)){
                  min = i;
                }
              }while((i = i->next) != head);
              memBlock = min;
              break;
	  case Worst:
            //  i = head;
              max = NULL;
              do{
                if(i->alloc == 0 && (!max || i->size > max->size)){
                  max = i;
                }
              }while((i = i->next) != head);
              if(max->size >= requested){
                memBlock = max;
              }
              break;
	  case Next:
	            ptrNext = next;
              do{
                if(!(next->alloc) && next->size >= requested){
                  memBlock = next;
                  break;
                }
              }while((next = next->next) != head);
              break;
    default:
            fprintf(stderr, "myalloc: Invalid strategy %d", myStrategy);
	  }
    if(!memBlock){ //No block avaliable
      fprintf(stderr,"myalloc: No avaliable blocks \n");
      return NULL;
    }
    if(memBlock->size > requested){
      struct memoryList *newMemBlock = malloc(sizeof(struct memoryList));
      /*Added new memBlock after the existing memBlock*/
      newMemBlock->next = memBlock->next;
      newMemBlock->next->last = newMemBlock;
      newMemBlock->last = memBlock;
      memBlock->next = newMemBlock;
      newMemBlock->size = memBlock->size - requested;
      newMemBlock->alloc = 0;
      newMemBlock->ptr = memBlock->ptr + requested;
      memBlock->size = requested;
      next = newMemBlock;
    } else {
      next = memBlock->next;
    }
    memBlock->alloc = 1;

	return memBlock->ptr; //pointer that points to memBlock that has been allocated
}


/* Frees a block of memory previously allocated by mymalloc. */
void myfree(void* block)
{

  struct memoryList* ptrHead = head;
  do{
    if(ptrHead->ptr == block){
      break;
    }
  }while((ptrHead = ptrHead->next) != head);

  ptrHead->alloc = 0;
  if(ptrHead != head && ptrHead->last->alloc == 0){
    struct memoryList* temp = ptrHead->last;
    temp->next = ptrHead->next;
    temp->next->last = temp;
    temp->size = temp->size + ptrHead->size;
    if(next == ptrHead){
      next = temp;
    }
    free(ptrHead);
    ptrHead = temp;
  }
  if(ptrHead->next != head && !(ptrHead->next->alloc)){
    struct memoryList* temp2 = ptrHead->next;
    ptrHead->next = temp2->next;
    ptrHead->next->last = ptrHead;
    ptrHead->size = ptrHead->size + temp2->size;
    if(next == temp2){
      next = ptrHead;
    }
    free(temp2);
  }
}

/****** Memory status/property functions ******
 * Implement these functions.
 * Note that when we refer to "memory" here, we mean the
 * memory pool this module manages via initmem/mymalloc/myfree.
 */

/* Get the number of contiguous areas of free space in memory. */
int mem_holes()
{
	int count = 0;
  struct memoryList *i = head;
  do{
    if(i->size <= mySize){
      if(i->alloc == 0){
        count++;
      }
    }
  }while((i = i->next) != head);
  return count;
}

/* Get the number of bytes allocated */
int mem_allocated()
{
	return mySize - mem_free();
}

/* Number of non-allocated bytes */
int mem_free()
{
	int c = 0;
  struct memoryList* i = head;
  do{
    if(i->alloc == 0){
      c = c + i->size;
    }
  }while((i = i->next) != head);
  return c;
}

/* Number of bytes in the largest contiguous area of unallocated memory */
int mem_largest_free()
{
	int maxSize = 0;
  struct memoryList *i = head;
  do{
    if(i->alloc == 0 && i->size > maxSize){
      maxSize = i->size;
    }
  } while((i = i->next) != head);
  return maxSize;
}

/* Number of free blocks smaller than "size" bytes. */
int mem_small_free(int size)
{
	int minCount = 0;
  struct memoryList *i = head;
  do{
    if(i->size <= size){
      if(i->alloc == 0){
        minCount++;
      }
    }
  }while((i = i->next) != head);
  return minCount;
}

char mem_is_alloc(void *ptr)
{
        struct memoryList *i = head;
        while(i->next != head){
          if(ptr < i->next->ptr){
            return i->alloc;
          }
          i = i->next;
        }
        return i->alloc;
}

/*
 * Feel free to use these functions, but do not modify them.
 * The test code uses them, but you may ind them useful.
 */


//Returns a pointer to the memory pool.
void *mem_pool()
{
	return myMemory;
}

// Returns the total number of bytes in the memory pool. */
int mem_total()
{
	return mySize;
}


// Get string name for a strategy.
char *strategy_name(strategies strategy)
{
	switch (strategy)
	{
		case Best:
			return "best";
		case Worst:
			return "worst";
		case First:
			return "first";
		case Next:
			return "next";
		default:
			return "unknown";
	}
}

// Get strategy from name.
strategies strategyFromString(char * strategy)
{
	if (!strcmp(strategy,"best"))
	{
		return Best;
	}
	else if (!strcmp(strategy,"worst"))
	{
		return Worst;
	}
	else if (!strcmp(strategy,"first"))
	{
		return First;
	}
	else if (!strcmp(strategy,"next"))
	{
		return Next;
	}
	else
	{
		return 0;
	}
}


/*
 * These functions are for you to modify however you see fit.  These will not
 * be used in tests, but you may find them useful for debugging.
 */

/* Use this function to print out the current contents of memory. */
void print_memory()
{
	return;
}

/* Use this function to track memory allocation performance.
 * This function does not depend on your implementation,
 * but on the functions you wrote above.
 */
void print_memory_status()
{
	printf("%d out of %d bytes allocated.\n",mem_allocated(),mem_total());
	printf("%d bytes are free in %d holes; maximum allocatable block is %d bytes.\n",mem_free(),mem_holes(),mem_largest_free());
	printf("Average hole size is %f.\n\n",((float)mem_free())/mem_holes());
}

/* Use this function to see what happens when your malloc and free
 * implementations are called.  Run "mem -try <args>" to call this function.
 * We have given you a simple example to start.
 */
void try_mymem(int argc, char **argv) {
        strategies strat;
	void *a, *b, *c, *d, *e;
	if(argc > 1)
	  strat = strategyFromString(argv[1]);
	else
	  strat = First;


	/* A simple example.
	   Each algorithm should produce a different layout. */

	initmem(strat,500);

	a = mymalloc(100);
	b = mymalloc(100);
	c = mymalloc(100);
	myfree(b);
	d = mymalloc(50);
	myfree(a);
	e = mymalloc(25);

	print_memory();
	print_memory_status();

}
