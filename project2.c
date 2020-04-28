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

void setHead(node** head, int nameLength, byte pname[17], long unsigned int* totalSize, long unsigned int lpsize)
{
    (**head).location = 0;
    (**head).next = NULL;
    (**head).prev = NULL;
    memset((**head).pname, 0, sizeof((**head).pname));
    for (int letter = 0; letter < nameLength; ++letter)
    {
        (**head).pname[letter] = pname[letter];
    }
    (**head).size = lpsize;
    *totalSize += lpsize;
    printf("ALLOCATED %s %ld\n", pname, (**head).location);
}

void getName(byte* line, byte* pname, int* i)
{
    int j = 0;
    byte name[17] = {0};
    // get the process name
    while (line[*i] != '\n' && line[*i] != '\000')
    {
        name[j] = line[*i];
        ++*i;
        ++j;
    }
    strcpy(pname, name);
}

void release(node** head, byte pname[17], long unsigned int *totalSize)
{
    if (*totalSize == 0)
    {
        printf("FAIL %s %s\n", RELEASE, pname);
        return;
    }

    node* temp;
    temp = *head;
    while (temp->next != NULL)
    {
        if(strcmp(pname, temp->pname) == 0)
        {
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            *totalSize -= temp->size;
            printf("FREE %s %ld %ld\n", temp->pname, temp->size, temp->location);
            free(temp);
            return;
        }
        temp = temp->next;
    }
    if(strcmp(pname, temp->pname) == 0)
    {
        temp->prev->next = temp->next;
        *totalSize -= temp->size;
        printf("FREE %s %ld %ld\n", temp->pname, temp->size, temp->location);
        free(temp);
        return;
    }
    else
    {
        printf("FAIL %s %s\n", RELEASE, pname);
        return;
    }
}

void list(node** head, byte c2[17], long unsigned int totalSize, long unsigned int size)
{
    if (strcmp(c2, AVAILABLE) == 0)
    {
        if (totalSize == 0)
        {
            printf("(%ld, %ld)\n", size, totalSize);
        }
        else if (totalSize == size)
        {
            printf("FULL\n");
        }
        else
        {
            if ((**head).location != 0)
            {
                printf("(%ld, %ld) ", (**head).location, (long unsigned int)0);
            }
            node* temp = *head;
            long unsigned int distance;

            while (temp->next != NULL)
            {
                //distance = (temp->next->location + temp->next->size) - (temp->location + temp->size);
                distance = (temp->next->location) - (temp->location + temp->size);
                if (distance > 0)
                {
                    printf("(%ld, %ld) ", distance, (temp->location + temp->size));
                }
                temp = temp->next;
            }
            if ((temp->location + temp->size) < size)
            {
                distance = size - (temp->location + temp->size);
                printf("(%ld, %ld)", distance, (temp->location + temp->size));
            }
            printf("\n");
        }
    }
    else if (strcmp(c2, ASSIGNED) == 0)
    {
        node* temp = *head;
        if (totalSize == 0)
        {
            printf("NONE\n");
        }
        else
        {
            while (temp->next != NULL)
            {
                printf("(%s, %ld, %ld) ", temp->pname, temp->size, temp->location);
                temp = temp->next;
            }
            printf("(%s, %ld, %ld)\n", temp->pname, temp->size, temp->location);
        }
    }
}

void find(node** head, byte pname[17])
{
    node* temp = *head;

    while (temp->next != NULL)
    {
        if (strcmp(temp->pname, pname) == 0)
        {
            printf("(%s, %ld, %ld)\n", pname, temp->size, temp->location);
            return;
        }
    }
    if (strcmp(temp->pname, pname) == 0)
    {
        printf("(%s, %ld, %ld)\n", pname, temp->size, temp->location);
        return;
    }
    printf("FAULT\n");
}

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
                    *head = (node *) malloc(sizeof(node));
                    setHead(head, nameLength, pname, &totalSize, lpsize);
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
                        newProc->location = (**head).size + (**head).location;
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
                        int firstSpot = FALSE;

                        // loop though the dllist
                        while(newProc->next != NULL)
                        {
                            curDistance = newProc->next->location - (newProc->location + newProc->size);
                            // beginning distance
                            if (shortestDist == 0)
                            {
                                shortestDist = curDistance;
                                if (curDistance >= lpsize)
                                {
                                    location = newProc->location + newProc->size;
                                    *tempNode = *newProc;
                                    spotFound = TRUE;
                                }
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
                        || (spotFound == FALSE && (size - (newProc->location + newProc->size)) >= lpsize))
                        {
                            shortestDist = size - (newProc->location + newProc->size);
                            location = newProc->location + newProc->size;
                            *tempNode = *newProc;
                            spotFound = TRUE;
                        }
                        // check if room before the head
                        if ((**head).location != 0 && (**head).location < shortestDist && (**head).location >= lpsize)
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
                            firstSpot = TRUE;
                        }
                        // check if room was found
                        if ((tempNode->next != NULL || tempNode->prev != NULL) && firstSpot == FALSE)
                        {
                            int placed = FALSE;
                            newProc = *head;
                            while (newProc->next != NULL)
                            {
                                if ((newProc->size + newProc->location) == location)
                                {
                                    tempNode->next = newProc->next;
                                    tempNode->prev = newProc;
                                    newProc->next = tempNode;
                                    placed = TRUE;
                                    break;
                                }
                                newProc = newProc->next;
                            }
                            if ((newProc->size + newProc->location) == location && placed == FALSE)
                            {
                                tempNode->next = newProc->next;
                                tempNode->prev = newProc;
                                newProc->next = tempNode;
                            }
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
            getName(line, pname, &i);

            if (*head == NULL)
            {
                printf("FAIL %s %s\n", RELEASE, pname);
            }
            else
            {
                release(head, pname, &totalSize);
            }
        }
        else if (strcmp(command, LIST) == 0)
        {
            byte c2[17] = {0};
            getName(line, (byte **)&c2, &i);
            list(head, c2, totalSize, size);
        }
        else if (strcmp(command, FIND) == 0)
        {
            byte pname[17] = {0};
            getName(line, (byte **)&pname, &i);
            find(head, pname);
        }
        else
        {
            fprintf(stderr, "invalid command\n");
            exit(-1);
        }
        i = 0;
        memset(command, 0, sizeof(command));
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
                    setHead(head, nameLength, pname, &totalSize, lpsize);
                }
                else if ((*head)->next == NULL && ((size - ((**head).location + (**head).size)) >= lpsize))
                {
                    node* newNode = (node*)malloc(sizeof(node));
                    memset(newNode->pname, 0, sizeof(newNode->pname));
                    (*head)->next = newNode;
                    newNode->prev = *head;
                    newNode->next = NULL;
                    newNode->location = (**head).location + (**head).size;
                    newNode->size = lpsize;
                    for (int letter = 0; letter < nameLength; ++letter)
                    {
                        newNode->pname[letter] = pname[letter];
                    }
                    totalSize += newNode->size;
                    printf("ALLOCATED %s %ld\n", pname, newNode->location);
                }
                else
                {
                    int spotFound = FALSE;
                    node* newNode;
                    node* temp = *head;

                    while (temp->next != NULL)
                    {
                        if ((temp->next->location - (temp->size + temp->location)) >= lpsize)
                        {
                            newNode = (node*)malloc(sizeof(node));
                            memset(newNode->pname, 0, sizeof(newNode->pname));
                            newNode->prev = temp;
                            newNode->next = temp->next;
                            temp->next = newNode;
                            newNode->location = temp->location + temp->size;
                            newNode->size = lpsize;
                            for (int letter = 0; letter < nameLength; ++letter)
                            {
                                newNode->pname[letter] = pname[letter];
                            }
                            totalSize += newNode->size;
                            spotFound = TRUE;
                            printf("ALLOCATED %s %ld\n", pname, newNode->location);
                            break;
                        }
                        temp = temp->next;
                    }
                    if (size - (temp->location + temp->size) >= lpsize && spotFound == FALSE)
                    {
                        newNode = (node*)malloc(sizeof(node));
                        memset(newNode->pname, 0, sizeof(newNode->pname));

                        temp->next = newNode;
                        newNode->prev = temp;
                        newNode->next = NULL;
                        newNode->location = temp->location + temp->size;
                        newNode->size = lpsize;
                        for (int letter = 0; letter < nameLength; ++letter)
                        {
                            newNode->pname[letter] = pname[letter];
                        }
                        totalSize += newNode->size;
                        printf("ALLOCATED %s %ld\n", pname, newNode->location);
                    }
                    else if (spotFound == FALSE)
                    {
                        printf("FAIL %s %s %ld\n", REQUEST, pname, lpsize);
                    }
                }
            }
        }
        else if (strcmp(command, RELEASE) == 0)
        {
            byte pname[17] = {0};
            getName(line, pname, &i);

            if (*head == NULL)
            {
                printf("FAIL %s %s\n", RELEASE, pname);
            }
            else
            {
                release(head, pname, &totalSize);
            }
        }
        else if (strcmp(command, LIST) == 0)
        {
            byte c2[17] = {0};
            getName(line, (byte **)&c2, &i);
            list(head, c2, totalSize, size);
        }
        else if (strcmp(command, FIND) == 0)
        {
            byte pname[17] = {0};
            getName(line, (byte **)&pname, &i);
            find(head, pname);
        }
        else
        {
            fprintf(stderr, "invalid command\n");
            exit(-1);
        }
        i = 0;
        memset(command, 0, sizeof(command));
    }
}

void nextfit(FILE* file, long unsigned int size, node** head)
{
    byte line[50] = {'\n'};
    byte command[10] = {0};
    long unsigned int totalSize = 0;
    node * _curNode = (node*)malloc(sizeof(node));
    memset(_curNode->pname, 0, sizeof(_curNode->pname));

    int i = 0;
    while(fgets(line, sizeof(line), file) != NULL)
    {
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
                    setHead(head, nameLength, pname, &totalSize, lpsize);
                    _curNode = *head;
                }
                else
                {
                    node* newNode = (node*)malloc(sizeof(node));
                    memset(newNode->pname, 0, sizeof(newNode->pname));
                    node* temp = _curNode;
                    int spotFound = FALSE;

                    while (_curNode->next != NULL)
                    {
                        if ((_curNode->next->location - (_curNode->location + _curNode->size)) >= lpsize)
                        {
                            newNode->next = _curNode->next;
                            _curNode->next = newNode;
                            newNode->prev = _curNode;
                            newNode->location = (*_curNode).location + (*_curNode).size;
                            newNode->size = lpsize;
                            for (int letter = 0; letter < nameLength; ++letter)
                            {
                                newNode->pname[letter] = pname[letter];
                            }
                            totalSize += newNode->size;
                            printf("ALLOCATED %s %ld\n", pname, newNode->location);
                            _curNode = newNode;
                            spotFound = TRUE;
                            break;
                        }
                        _curNode = _curNode->next;
                    }

                    if (size - (_curNode->size + _curNode->location) >= lpsize && spotFound == FALSE)
                    {
                        _curNode->next = newNode;
                        newNode->prev = _curNode;
                        newNode->next = NULL;
                        newNode->location = (*_curNode).location + (*_curNode).size;
                        newNode->size = lpsize;
                        for (int letter = 0; letter < nameLength; ++letter)
                        {
                            newNode->pname[letter] = pname[letter];
                        }
                        totalSize += newNode->size;
                        printf("ALLOCATED %s %ld\n", pname, newNode->location);
                        _curNode = newNode;
                    }
                    else if ((**head).location != 0 && (**head).location >= lpsize && spotFound == FALSE)
                    {
                        (*head)->prev = newNode;
                        newNode->prev = NULL;
                        newNode->next = *head;
                        newNode->location = 0;
                        newNode->size = lpsize;
                        for (int letter = 0; letter < nameLength; ++letter)
                        {
                            newNode->pname[letter] = pname[letter];
                        }
                        totalSize += newNode->size;
                        printf("ALLOCATED %s %ld\n", pname, newNode->location);
                        _curNode = newNode;
                    }
                    else
                    {
                        _curNode = temp;
                    }
                }
            }
        }
        else if (strcmp(command, RELEASE) == 0)
        {
            byte pname[17] = {0};
            getName(line, pname, &i);

            if (*head == NULL)
            {
                printf("FAIL %s %s\n", RELEASE, pname);
            }
            else
            {
                release(head, pname, &totalSize);
            }
        }
        else if (strcmp(command, LIST) == 0)
        {
            byte c2[17] = {0};
            getName(line, (byte **)&c2, &i);
            list(head, c2, totalSize, size);
        }
        else if (strcmp(command, FIND) == 0)
        {
            byte pname[17] = {0};
            getName(line, (byte **)&pname, &i);
            find(head, pname);
        }
        else
        {
            fprintf(stderr, "invalid command\n");
            exit(-1);
        }
        i = 0;
        memset(command, 0, sizeof(command));
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
                if (*head == NULL) {
                    *head = (node *) malloc(sizeof(node));
                    setHead(head, nameLength, pname, &totalSize, lpsize);
                }
                else
                {
                    node* newNode = (node*)malloc(sizeof(node));
                    memset(newNode->pname, 0, sizeof(newNode->pname));
                    long unsigned int longestDist = 0;
                    long unsigned int curDistance = 0;
                    long unsigned int location = 1;
                    int headCheck = FALSE;
                    int locationFound = FALSE;
                    newNode = *head;

                    while (newNode->next != NULL)
                    {
                        if (headCheck == FALSE && (**head).location != 0)
                        {
                            curDistance = (**head).location;
                            longestDist = curDistance;
                            location = 0;
                            locationFound = TRUE;
                        }
                        else
                        {
                            curDistance = newNode->next->location - (newNode->size + newNode->location);
                            if (curDistance > longestDist && curDistance >= lpsize)
                            {
                                longestDist = curDistance;
                                location = newNode->size + newNode->location;
                                locationFound = TRUE;
                            }
                        }
                        headCheck = TRUE;
                        newNode = newNode->next;
                    }
                    curDistance = size - (newNode->location + newNode->size);
                    if (curDistance > longestDist && curDistance >= lpsize)
                    {
                        location = newNode->location + newNode->size;
                        locationFound = TRUE;
                    }

                    if (locationFound == TRUE)
                    {
                        node* temp = (node*)malloc(sizeof(node));
                        memset(temp->pname, 0 , sizeof(temp->pname));
                        int placed = FALSE;
                        newNode = *head;
                        while (newNode->next != NULL)
                        {
                            if ((newNode->location + newNode->size) == location)
                            {
                                temp->next = newNode->next;
                                newNode->next = temp;
                                temp->prev = newNode;
                                temp->next->prev = temp;
                                temp->location = location;
                                temp->size = lpsize;
                                for (int letter = 0; letter < nameLength; ++letter)
                                {
                                    temp->pname[letter] = pname[letter];
                                }
                                totalSize += temp->size;
                                printf("ALLOCATED %s %ld\n", pname, temp->location);
                                placed = TRUE;
                                break;
                            }
                            newNode = newNode->next;
                        }
                        if ((newNode->location + newNode->size) == location && placed == FALSE)
                        {
                            temp->next = newNode->next;
                            temp->prev = newNode;
                            newNode->next = temp;
                            temp->location = newNode->location + newNode->size;
                            temp->size = lpsize;
                            for (int letter = 0; letter < nameLength; ++letter)
                            {
                                temp->pname[letter] = pname[letter];
                            }
                            totalSize += newNode->size;
                            printf("ALLOCATED %s %ld\n", pname, temp->location);
                        }
                    }
                    else
                    {
                        printf("FAIL %s %s %ld\n", REQUEST, pname, lpsize);
                    }
                }
            }
        }
        else if (strcmp(command, RELEASE) == 0)
        {
            byte pname[17] = {0};
            getName(line, pname, &i);

            if (*head == NULL)
            {
                printf("FAIL %s %s\n", RELEASE, pname);
            }
            else
            {
                release(head, pname, &totalSize);
            }
        }
        else if (strcmp(command, LIST) == 0)
        {
            byte c2[17] = {0};
            getName(line, (byte **)&c2, &i);
            list(head, c2, totalSize, size);
        }
        else if (strcmp(command, FIND) == 0)
        {
            byte pname[17] = {0};
            getName(line, (byte **)&pname, &i);
            find(head, pname);
        }
        else
        {
            fprintf(stderr, "invalid command\n");
            exit(-1);
        }
        i = 0;
        memset(command, 0, sizeof(command));
    }
}

int main(int argc, char** argv)
{
    FILE* file;
    char* endptr;
    node* head = NULL;

    if (argc != 4)
    {
        fprintf(stderr, "invalid input\n");
        exit(-1);
    }
    long unsigned int size = strtol(argv[2], &endptr, 10);/* 268435456 50*/

    // between 2^4 and 2^30
    if (size < 16 || size > 1073741824)
    {
        fprintf(stderr, "invalid allocation size\n");
        exit(-1);
    }
    if ((file = fopen(argv[3]/*"test2"*/, "r")) == NULL)
    {
        fprintf(stderr, "error opening file\n");
        exit(-1);
    }

    if (strcmp(argv[1]/*"ESTFIT"*/, BESTFIT) == 0)
    {
        bestfit(file, size, &head);
    }
    else if (strcmp(argv[1]/*"IRSTFIT"*/, FIRSTFIT) == 0)
    {
        firstfit(file, size, &head);
    }
    else if (strcmp(argv[1]/*"NXTFIT"*/, NEXTFIT) == 0)
    {
        nextfit(file, size, &head);
    }
    else if (strcmp(argv[1]/*"WORSTFIT"*/, WORSTFIT) == 0)
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
