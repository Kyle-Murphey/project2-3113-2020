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
    byte pname[17];
    long unsigned int size;
    long unsigned int location;
    struct node* next;
    struct node* prev
} node;

void bestfit(FILE* file, long unsigned int size, node** head)
{
    byte line[50] = {'\n'};
    byte command[10] = {0};
    long unsigned int totalSize = 0;

    int i = 0;
    while(fgets(line, sizeof(line), file) != NULL)
    {
        // line is a comment
        if (line[0] == '#')
            continue;

        // line is not a comment, get the command
        while(line[i] != ' ')
        {
            command[i] = line[i];
            ++i;
        }
        ++i;

        if (strcmp(command, REQUEST) == 0)
        {
            byte pname[17] = {0}; //process name
            byte bpsize[11] = {0}; //process size stored as bytes
            long unsigned int lpsize; //process size
            int nameLength = 0;

            int j = 0;
            // get the process name
            while (line[i] != ' ')
            {
                pname[j] = line[i];
                ++nameLength;
                ++j;
                ++i;
            }
            ++i;
            j = 0;

            // get the size of allocation
            while (line[i] != '\n' && line[i] != '\000')
            {
                bpsize[j] = line[i];
                ++j;
                ++i;
            }
            lpsize = atol(bpsize);

            // check if allocation size is valid
            if (lpsize > size || lpsize < 1) //not valid
            {
                printf("FAIL %s %s %ld\n", REQUEST, pname, lpsize);
            }
            else //valid
            {
                if (*head == NULL)
                {
                    *head = (node*)malloc(sizeof(node));
                    (**head).location = 0;
                    (**head).next = NULL;
                    (**head).prev = NULL;
                    memset((**head).pname, 0, sizeof((**head).pname));
                    for (int letter = 0; letter < nameLength; ++letter)
                    {
                        (**head).pname[letter] = pname[letter];
                    }
                    (**head).size = lpsize;
                    totalSize += lpsize;
                    printf("ALLOCATED %s %ld\n", pname, (**head).location);
                }
                else
                {
                    node* newProc = (node*)malloc(sizeof(node));
                    memset(newProc->pname, 0, sizeof(newProc->pname));

                    if ((*head)->next == NULL && (lpsize + totalSize) < size)
                    {
                        (*head)->next = newProc;
                        newProc->prev = *head;
                        newProc->next = NULL;
                        newProc->location = (**head).size;
                        for (int letter = 0; letter < nameLength; ++letter)
                        {
                            newProc->pname[letter] = pname[letter];
                        }
                        newProc->size = lpsize;
                        totalSize += lpsize;
                        printf("ALLOCATED %s %ld\n", pname, newProc->location);
                    }
                    else
                    {
                        node * tempNode = (node*)malloc(sizeof(node));
                        tempNode->next = NULL;
                        tempNode->prev = NULL;
                        memset(tempNode->pname, 0, sizeof(tempNode->pname));
                        *newProc = **head;
                        long unsigned int curDistance = 0;
                        long unsigned int shortestDist = 0;
                        long unsigned int location = 1;
                        int spotFound = FALSE;

                        // loop though the dllist
                        while(newProc->next != NULL)
                        {
                            curDistance = newProc->next->location - (newProc->location + newProc->size);
                            // beginning distance
                            if (shortestDist == 0)
                            {
                                shortestDist = curDistance;
                            }
                            // smaller distance found
                            if (curDistance < shortestDist && curDistance >= lpsize)
                            {
                                shortestDist = curDistance;
                                location = newProc->location + newProc->size;
                                *tempNode = *newProc;
                                spotFound = TRUE;
                            }
                            newProc = newProc->next;
                        }
                        // check the last case up until the end
                        if ((size - (newProc->location + newProc->size) < shortestDist && (size - (newProc->location + newProc->size)) >= lpsize)
                        || (spotFound == FALSE && (size - (newProc->location + newProc->size))))
                        {
                            shortestDist = size - (newProc->location + newProc->size);
                            location = newProc->location + newProc->size;
                            *tempNode = *newProc;
                            spotFound = TRUE;
                        }
                        // check if room before the head
                        if ((**head).location != 0 && (**head).location < shortestDist && (**head).location >= lpsize && spotFound == FALSE)
                        {
                            shortestDist = (**head).size;
                            tempNode = *head;
                            tempNode->prev = newProc;
                            newProc->next = tempNode;
                            newProc->location = 0;
                            for (int letter = 0; letter < nameLength; ++letter)
                            {
                                newProc->pname[letter] = pname[letter];
                            }
                            newProc->size = lpsize;
                            *head = newProc;
                            totalSize += lpsize;
                            printf("ALLOCATED %s %ld\n", pname, newProc->location);
                        }
                        // check if room was found
                        if (tempNode->next != NULL || tempNode->prev != NULL)
                        {
                            newProc->next = tempNode;
                            tempNode->next = NULL;
                            tempNode->prev = newProc;
                            tempNode->location = location;
                            for (int letter = 0; letter < nameLength; ++letter)
                            {
                                tempNode->pname[letter] = pname[letter];
                            }
                            tempNode->size = lpsize;
                            totalSize += lpsize;
                            printf("ALLOCATED %s %ld\n", pname, tempNode->location);
                        }
                        else
                        {
                            printf("FAIL %s %s %ld\n", REQUEST, pname, lpsize);
                        }
                    }
                }
            }
        }
        else if (strcmp(command, RELEASE) == 0)
        {
            byte pname[17] = {0};
            int j = 0;
            // get the process name
            while (line[i] != '\n' && line[i] != '\000')
            {
                pname[j] = line[i];
                ++i;
                ++j;
            }
            if (*head == NULL)
            {
                printf("FAIL %s %s\n", RELEASE, pname);
            }
            else
            {

            }
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

void firstfit(FILE* file, long unsigned int size, node** head)
{
    byte line[50] = {'\n'};
    byte command[10] = {0};
    long unsigned int totalSize = 0;

    int i = 0;
    while(fgets(line, sizeof(line), file) != NULL) {
        // line is a comment
        if (line[0] == '#')
            continue;

        // line is not a comment, get the command
        while (line[i] != ' ') {
            command[i] = line[i];
            ++i;
        }
        ++i;

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

void nextfit(FILE* file, long unsigned int size, node** head)
{
    byte line[50] = {'\n'};
    byte command[10] = {0};
    long unsigned int totalSize = 0;

    int i = 0;
    while(fgets(line, sizeof(line), file) != NULL) {
        // line is a comment
        if (line[0] == '#')
            continue;

        // line is not a comment, get the command
        while (line[i] != ' ') {
            command[i] = line[i];
            ++i;
        }
        ++i;

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

void worstfit(FILE* file, long unsigned int size, node** head)
{
    byte line[50] = {'\n'};
    byte command[10] = {0};
    long unsigned int totalSize = 0;

    int i = 0;
    while(fgets(line, sizeof(line), file) != NULL) {
        // line is a comment
        if (line[0] == '#')
            continue;

        // line is not a comment, get the command
        while (line[i] != ' ') {
            command[i] = line[i];
            ++i;
        }
        ++i;

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

int main(int argc, char** argv)
{
    FILE* file;
    char* endptr;
    long unsigned int size = /*strtol(argv[2], &endptr, 10);*/ 268435456;
    node* head = NULL;

    // between 2^4 and 2^30
    if (size < 16 || size > 1073741824)
    {
        fprintf(stderr, "invalid allocation size\n");
        exit(-1);
    }
    if ((file = fopen(/*argv[3]*/"test2", "r")) == NULL)
    {
        fprintf(stderr, "error opening file\n");
        exit(-1);
    }

    /**************************************************************
     *
     * Create dllist here before sending off to algos.
     *
     **************************************************************/

    if (strcmp(/*argv[1]*/"BESTFIT", BESTFIT) == 0)
    {
        bestfit(file, size, &head);
    }
    else if (strcmp(/*argv[1]*/"FIRSTFIT", FIRSTFIT) == 0)
    {
        firstfit(file, size, &head);
    }
    else if (strcmp(/*argv[1]*/"NEXTFIT", NEXTFIT) == 0)
    {
        nextfit(file, size, &head);
    }
    else if (strcmp(/*argv[1]*/"WORSTFIT", WORSTFIT) == 0)
    {
        worstfit(file, size, &head);
    }
    else
    {
        fprintf(stderr, "invalid method of allocation\n");
        fclose(file);
        exit(-1);
    }
    fclose(file);
}
