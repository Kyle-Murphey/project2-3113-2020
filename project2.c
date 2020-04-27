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
#define REQUEST "REQUEST"
#define RELEASE "RELEASE"
#define LIST "LIST"
#define FIND "FIND"
#define AVAILABLE "AVAILABLE"
#define ASSIGNED "ASSIGNED"

/* unsigned char */
typedef unsigned char byte;

typedef struct node {
    byte* pname;
    int size;
    long int location;
    struct node* next;
    struct node* prev
} node;

void bestfit(FILE* file)
{
    byte line[50] = {0};
    byte command[10] = {0};
    byte c2[10] = {0};
    byte c3[10] = {0};

    int i = 0;
    int j = 0;
    while(fgets(line, sizeof(line), file) != NULL)
    {
        // line is a comment
        if (line[0] == '#')
            continue;

        // line is not a comment
        while(line[i] != ' ')
        {
            command[i] = line[i++];
        }
        if (strcmp(command, REQUEST) == 0)
        {

        }
        else if (strcmp(command, RELEASE) == 0)
        {

        }
        else if (strcmp(command, LIST) == 0)
        {

        }
        else if (strcmp(command, FIND) == 0)
        {

        }
        else
        {
            fprintf(stderr, "invalid command\n");
            exit(-1);
        }
        i = 0;
    }
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
    node* head = NULL;

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
        bestfit(file);
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
        fclose(file);
        exit(-1);
    }
    fclose(file);
}
