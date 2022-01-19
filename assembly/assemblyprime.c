#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum
{
    READ,
    PRINT,
    ASSIGN,
    DEFINE,
    ADD,
    MINUS,
    MOD,
    EQ,
    LESS,
    JUMP,
    TERM,
    N_inst
} instruction;

struct memory
{
    instruction inst;
    int md;
    int mx;
    int my;
};

struct memory mem[100] = {
    -1,
};

void read(int m)
{
    int a;
    printf("Enter an integer\n");
    scanf("%d", &a);
    mem[mem[m].md].md = a;
};

void print(int m)
{
    if (mem[mem[m].md].md == 0)
    {
        printf("Not Prime Number!!\n");
    }
    if (mem[mem[m].md].md == 1)
    {
        printf("Prime Number!!\n");
    }
};

void assign(int m)
{
    mem[mem[m].md].md = mem[mem[m].mx].md;
};

void define(int m)
{
    mem[mem[m].md].md = mem[mem[m].mx].md;
};

void add(int m)
{
    mem[mem[m].md].md = mem[mem[m].mx].md + mem[mem[m].my].md;
};

void minus(int m)
{
    mem[mem[m].md].md = mem[mem[m].mx].md - mem[mem[m].my].md;
};

void mod(int m)
{
    mem[mem[m].md].md = mem[mem[m].mx].md % mem[mem[m].my].md;
    if (mem[mem[m].my].md == 0)
    {
        printf("it can not be divided by zero\n");
        exit(0);
    }
};

void eq(int m)
{
    if (mem[mem[m].mx].md == mem[mem[m].my].md)
    {
        mem[mem[m].md].md = 1;
    }
    else
    {
        mem[mem[m].md].md = 0;
    }
};

void less(int m)
{
    if (mem[mem[m].mx].md < mem[mem[m].my].md)
    {
        mem[mem[m].md].md = 1;
    }
    else
    {
        mem[mem[m].md].md = 0;
    }
};

int jump(int m)
{
    if (mem[mem[m].md].md != 0)
    {
        return mem[m].mx;
    }
    else
    {
        return -1;
    }
};

void term()
{
    exit(0);
};

int getfile(char *filename)
{
    char buf[100];
    int i = 0, loc;
    char *memloc;
    char *inst;
    char *md;
    char *mx;
    char *my;
    char *b;
    FILE *fp;
    fp = fopen(filename, "r");

    while (!feof(fp))
    {

        fgets(buf, 100, fp);

        buf[strlen(buf) - 1] = '\0';

        for (int j = 0; j < 5; j++)
        {
            if (j == 0)
            {

                memloc = strtok(buf, ":");
                loc = atoi(memloc);
                printf("loc %d ", atoi(memloc));
                if (loc < 0 || loc > 100)
                {
                    printf("Wrong memory location, fix the file");
                    return 1;
                }
            }
            if (j == 1)
            {
                inst = strtok(NULL, " ");
                if (inst == NULL)
                {
                    break;
                }
                printf("%s ", inst);
                if (strcmp("READ", inst) == 0)
                {
                    mem[loc].inst = READ;
                }
                else if (strcmp("PRINT", inst) == 0)
                {
                    mem[loc].inst = PRINT;
                }
                else if (strcmp("ASSIGN", inst) == 0)
                {
                    mem[loc].inst = ASSIGN;
                }
                else if (strcmp("DEFINE", inst) == 0)
                {
                    mem[loc].inst = DEFINE;
                }
                else if (strcmp("ADD", inst) == 0)
                {
                    mem[loc].inst = ADD;
                }
                else if (strcmp("MINUS", inst) == 0)
                {
                    mem[loc].inst = MINUS;
                }
                else if (strcmp("MOD", inst) == 0)
                {
                    mem[loc].inst = MOD;
                }
                else if (strcmp("EQ", inst) == 0)
                {
                    mem[loc].inst = EQ;
                }
                else if (strcmp("LESS", inst) == 0)
                {
                    mem[loc].inst = LESS;
                }
                else if (strcmp("JUMP", inst) == 0)
                {
                    mem[loc].inst = JUMP;
                }
                else if (strcmp("TERM", inst) == 0)
                {
                    mem[loc].inst = TERM;
                }
                else
                {
                    mem[loc].md = atoi(inst);
                    break;
                }
            }
            if (j == 2)
            {
                md = strtok(NULL, " ");
                if (md == NULL)
                    break;
                mem[loc].md = atoi(md);
                printf("%d ", atoi(md));
            }
            if (j == 3)
            {
                mx = strtok(NULL, " ");
                if (mx == NULL)
                    break;
                mem[loc].mx = atoi(mx);
                printf("%d ", atoi(mx));
            }
            if (j == 4)
            {
                my = strtok(NULL, " ");
                if (my == NULL)
                    break;
                mem[loc].my = atoi(my);
                printf("%d ", atoi(my));
            }
        }
        i++;
        printf("\n");
    }
    fclose(fp);
    return 0;
}
int main()
{
    char buf[100];
    int i, j, k;
    int check;

    printf("enter the file name! \n");
    scanf("%s", buf);
    getchar();

    if (getfile(buf))
    {
        exit(1);
    }
    for (i = 0; i < 100; i++)
    {
        for (j = 0; j < N_inst; j++)
        {
            if (mem[i].md == -1)
            {
                check = -1;
                break;
            }
            if (mem[i].inst == j)
            {
                check = j;
                break;
            }
        }
        if (check == READ)
            read(i);
        if (check == PRINT)
            print(i);
        if (check == ASSIGN)
            assign(i);
        if (check == DEFINE)
            define(i);
        if (check == ADD)
            add(i);
        if (check == MINUS)
            minus(i);
        if (check == MOD)
            mod(i);
        if (check == EQ)
            eq(i);
        if (check == LESS)
            less(i);
        if (check == JUMP)
        {
            k = jump(i);
            if (k != -1)
            {
                i = k - 1;
            }
        }
        if (check == 10)
            break;
    }
    return 0;
}