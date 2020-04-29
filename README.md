# Kyle Murphey, Project 2
****************I COULD NOT GET PROJECT TO RUN ON GRADESCOPE; IT KEPT TIMING OUT****************

Compile: make all or gcc -c project2.c -o project2

Run: ./project2 COMMAND N testfile.txt

Bugs: Invalid data types in the CL input or the file would result in a failing program. There could also be an issue with pointers still pointing to chunks of memory rather than NULL at end of life. Memory is freed but pointers still exist.

I assume that all input will be correctly formatted and the file as well.

main():
   * reads the CL input and sends the file off to the user specified alorithm.
   * runs initial checks on CL input.
    
bestfit():
   * finds the smallest available spot in memory to alloc
   * keeps track of smallest distance as it looks through the dllist.
   * inserts the node into the smallest spot it can fit
   
firstfit():
   * finds the first spot available for allocation
   * starts at the beginning of mem, and travels linearly to find the next open spot
   * places node in the first spot big enough
   
nextfit():
   * finds the next available spot for allocation
   * keeps track of where the last accessed spot was in the list
   * if end of list is reached and no spot in found, it loops back to head
   * places node in first spot it finds
   
worstfit():
   * finds largest available spot for allocation
   * keeps track of largest distance as it looks through th dllist
   * places the node into the largest open memory
   
setHead():
   * set the head of the ddlist

getName():
   * get the process name

release():
   * search though the dllist until node is found and removed
   
list():
   * list the available memory slots
   * list the assigned slots

find():
   * find a specified node

deleteNodes():
   * delete the dllist and free the memory


Sources:
   - Linux man pages
   - https://www.geeksforgeeks.org/doubly-linked-list/
   - https://www.youtube.com/watch?v=aV3s3fkriWY