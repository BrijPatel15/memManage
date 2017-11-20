Assignment4: Memory Management
==============================

This machine problem will focus on memory.  You will implement your own
version of malloc() and free(), using a variety of allocation strategies.

You will be implementing a memory manager for a block of memory.  You will
implement routines for allocating and deallocating memory, and keeping track of
what memory is in use.  You will implement four strategies for selecting in
which block to place a new requested memory black:

  1) First-fit: select the first suitable block with smallest address.
  2) Best-fit: select the smallest suitable block.
  3) Worst-fit: select the largest suitable block.
  4) Next-fit: select the first suitable block after
     the last block allocated (with wraparound
     from end to beginning).


Here, "suitable" means "free, and large enough to fit the new data".

Here are the functions you will need to implement:

initmem():
  Initialize memory structures.

mymalloc():
  Like malloc(), this allocates a new block of memory.

myfree():
  Like free(), this deallocates a block of memory.

mem_holes():
  How many free blocks are in memory?

mem_allocated():
  How much memory is currently allocated?

mem_free():
  How much memory is NOT allocated?

mem_largest_free():
  How large is the largest free block?

mem_small_free():
  How many small unallocated blocks are currently in memory?

mem_is_alloc():
  Is a particular byte allocated or not?

We have given you a structure to use to implement these functions.  It is a
doubly-linked list of blocks in memory (both allocated and free blocks).  Every
malloc and free can create new blocks, or combine existing blocks.  You may
modify this structure, or even use a different one entirely.  However, do not
change function prototypes or files other than mymem.c.

IMPORTANT NOTE: Regardless of how you implement memory management, make sure
that there are no adjacent free blocks.  Any such blocks should be merged into
one large block.

We have also given you a few functions to help you monitor what happens when you
call your functions.  Most important is the try_mymem() function.  If you run
your code with "mem -try <args>", it will call this function, which you can use
to demonstrate the effects of your memory operations.  These functions have no
effect on test code, so use them to your advantage.

Running your code:

After running "make", run

1) "mem" to see the available tests and strategies.
2) "mem -test <test> <strategy>" to test your code with our tests.
3) "mem -try <args>" to run your code with your own tests
   (the try_mymem function).

You can also use "make test" and "make stage1-test" for testing.  "make
stage1-test" only runs the tests relevant to stage 1.

As in previous assignments, running "mem -test -f0 ..." will allow tests
to run even after previous tests have failed.  Similarly, using "all" for
a test or strategy name runs all of the tests or strategies.  Note that
if "all" is selected as the strategy, the 4 tests are shown as one.

One of the tests, "stress", runs an assortment of randomized tests on each
strategy.  The results of the tests are placed in "tests.out" .  You may want to
view this file to see the relative performance of each strategy.


Stage 1
-------

Implement all the above functions, for the first-fit strategy.  Use "mem -test
all first" to test your implementation.


Stage 2
-------

A) Implement the other three strategies: worst-fit, best-fit, and next-fit.  The
strategy is passed to initmem(), and stored in the global variable "myStrategy".
Some of your functions will need to check this variable to implement the
correct strategy.

You can test your code with "mem -test all worst", etc., or test all 4 together
with "mem -test all all".  The latter command does not test the strategies
separately; your code passes the test only if all four strategies pass.


Questions
=========

1) Why is it so important that adjacent free blocks not be left as such?  What
would happen if they were permitted?

  To avoid fragmentation, because it is harder to fill small holes rather than just combining them
  into one big hole which makes it easier for them to get filled

2) Which function(s) need to be concerned about adjacent free blocks?

  The function mymalloc needs to be concerned about the adjacent free blocks because this function
  holds the logic of where memory gets allocated. The function free also needs to worry about adjacent free
  blocks as well so when it frees a memory location it can combine them together to make a larger hole.

3) Name one advantage of each strategy.
  First-fit is the fastest strategy because it finds the first possible block where the allocation can
  happen and inserts it there.
  Best-fit finds a memory location that is the best fit for the allocation, that means that you have to
  worry less about leaving large holes between two memory locations and the larger allocations have enough
  spaces to fit in.
  Worst-fit selects the largest possible block for the allocation to happen, this means that you don't leave
  behind small holes that are too small to fit anything in them.
  Next-fit is the second fastest algorithm because it tries to insert after last position it allocated to before

4) Run the stress test on all strategies, and look at the results (tests.out).
What is the significance of "Average largest free block"?  Which strategy
generally has the best performance in this metric?  Why do you think this is?
  The average largest free block shows the average of all the large free blocks that are still not allocated,
  so the best performer in this metric would be first fit. This is because the larger the average largest
  free block the better because that means you can find a block easily to allocate into. After tests.out,
  it becomes clear that in all categories, first fit always has the best performance in the metric. This
  would probably be because it finds the first block to insert regardless the size, so the holes you have left
  are likely going to be large.


5) In the stress test results (see Question 4), what is the significance of
"Average number of small blocks"?  Which strategy generally has the best
performance in this metric?  Why do you think this is?
  The average number of small blocks shows the average number of small blocks are left once everything has been
  allocated. In this metric the best performer is worst fit, because worst fit is designed to leave the least amount
  of small holes, because it finds the largest place to perform the allocation. Since a lot of small holes mean
  that you have to perform memory compaction, the strategy that gives us the smallest average number of smaller
  blocks is the best one.

6) Eventually, the many mallocs and frees produces many small blocks scattered
across the memory pool.  There may be enough space to allocate a new block, but
not in one place.  It is possible to compact the memory, so all the free blocks
are moved to one large free block.  How would you implement this in the system
you have built?
  All you would have to do would be to iterate though all of memory and once you find an memory location that is
  not allocated. Once you did that you can simply append it to the end of the linked list (prev of the head)
  this is possible because we have a double-linked list so the previous of the head is the tail. So once all the
  unallocted is at the end of the linked list, all we have to do is join all the holes to make one large free memory
  location.

7) If you did implement memory compaction, what changes would you need to make
in how such a system is invoked (i.e. from a user's perspective)?
  All you would need to do is to check if compaction is needed after the block is removed. Once you remove, you need
  to relink the linked list to make sure it is not broken. Also you would need to check if the memory needs to be compacted
  only after you are unable to allocate a block once. If you check after every allocation you would use a lot of resources
  and the allocation process would be very slow.

8) How would you use the system you have built to implement realloc?  (Brief
explanation; no code)
    The realloc function can delegate some of the tasks it has to perform to other functions, for example when adding a new block
    mymalloc can be used with the desired strategy. Additionally when freeing memory the myfree function could be called as well.
    The function header could be something like
    realloc(void* block, size_t size);


9) Which function(s) need to know which strategy is being used?  Briefly explain
why this/these and not others.
  Only mymalloc needs to know about the strategy that is being used because that function is responsible for picking the strategy
  the other classes only have to implement a certain strategy so they don't have to know about anything else but what they are
  implementing/


10) Give one advantage of implementing memory management using a linked list
over a bit array, where every bit tells whether its corresponding byte is
allocated.
Memory management using a linked list is ideal because a linked list has unlimited size and don't have to worry about running out of memory
but more importantly, in a linked list you can insert anywhere in the list unlike an array. In an array the only strategy you would be able
to implement is the last fit strategy which adds to the end of memory(the opposite of first-fit). But in a linked list implementation you have
the flexibility to insert anywhere so you can implement a wide range of strategies like best fit which inserts in the middle of the linked list.
