#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *  Project 2
 * Kyle Murphey
 * 04/27/20
 */

#define TRUE 1
#define FALSE 0
#define BESTFIT "BESTFIT"
#define FIRSTFIT "FIRSTFIT"
#define NEXTFIT "NEXTFIT"
#define WORSTFIT "WORSTFIT"

/* unsigned char */
typedef unsigned char byte;

typedef struct node {
    byte* pname;
    int size;
    long int location;
    struct node* next;
    struct node* prev
} node;

void bestfit()
{

}

void firstfit()
{

}

void nextfit()
{

}

void worstfit()
{

}

int main(int argc, char** argv)
{
    FILE* file;
    char* endptr;
    long int size = strtol(argv[2], &endptr, 10);

    // between 2^4 and 2^30
    if (size < 16 || size > 1073741824)
    {
        fprintf(stderr, "invalid allocation size\n");
        exit(-1);
    }
    if ((file = fopen(argv[3], "r")) == NULL)
    {
        fprintf(stderr, "error opening file\n");
        exit(-1);
    }

    /**************************************************************
     *
     * Create dllist here before sending off to algos.
     *
     **************************************************************/

    if (strcmp(argv[1], BESTFIT) == 0)
    {
        bestfit();
    }
    else if (strcmp(argv[1], FIRSTFIT) == 0)
    {
        firstfit();
    }
    else if (strcmp(argv[1], NEXTFIT) == 0)
    {
        nextfit();
    }
    else if (strcmp(argv[1], WORSTFIT) == 0)
    {
        worstfit();
    }
    else
    {
        fprintf(stderr, "invalid method of allocation\n");
        exit(-1);
    }
}
