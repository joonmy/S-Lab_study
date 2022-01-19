#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N_subject 4

typedef enum
{
    vertical,
    horizontal
} direction;

typedef struct
{
    int id;
    int y1, y2;    // y1: the minimum of y, y2: the maximum of y
    int x1, x2;    // x1: the minimum of x, x2: the maximum of x
    int span;      // the number of cells
    direction dir; // the direction of the car
} car_t;

int n_cars = 0;
int cells[6][6];

struct State
{
    car_t *cars;
};

struct Node
{
    struct State *data;
    struct Node *next;
    struct Node *prev;
};

struct _Queue
{
    struct Node *front;
    struct Node *rear;
};

struct _Queue *create_queue()
{
    struct _Queue *Q = malloc(sizeof(struct _Queue));
    Q->front = NULL;
    Q->rear = NULL;
    return Q;
};

int IsEmpty(struct _Queue *pq)
{
    if (pq->front == NULL)
        return 1;
    else
        return 0;
};

void Enqueue(struct _Queue *q, struct State *data)
{
    struct Node *newNode = malloc(sizeof(struct Node));
    newNode->next = NULL;
    //newNode->data = set_copy_State(data);
    newNode->data = data;

    if (IsEmpty(q))
    {
        q->front = newNode;
        q->rear = newNode;
    }
    else
    {
        q->rear->next = newNode;
        q->rear = newNode;
    }
};

void Enqueue_node(struct _Queue *q, struct Node *data)
{

    if (IsEmpty(q))
    {
        q->front = data;
        q->rear = data;
        data->next = NULL;
        data->prev = NULL;
    }
    else
    {
        q->rear->next = data;
        q->rear = data;
        data->next = NULL;
    }
};

struct Node *Dequeue(struct _Queue *q)
{
    struct Node *deleteNode;
    deleteNode = q->front;
    q->front = q->front->next;

    return deleteNode;
};

struct State *set_initial_State(char *filename)
{
    char line[100];
    char *cell;
    char *direc;
    char *span;
    int i;
    FILE *fp;
    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("No such file exist\n");
        return NULL;
    }

    fscanf(fp, "%d", &n_cars); //get n_cars
    fgetc(fp);
    if (n_cars < 2 || n_cars > 36)
    {
        printf("the number of car needs to be 2< <36 and type of number\n");
        return NULL;
    }
    struct State *st = malloc(sizeof(struct State));
    car_t *cars = (car_t *)malloc(sizeof(car_t) * (n_cars + 1)); // malloc N cars.

    i = 0;
    while (!feof(fp))
    {
        if (i >= n_cars)
            break;
        fgets(line, 100, fp);
        line[strlen(line) - 1] = '\0';

        cell = strtok(line, ":");
        direc = strtok(NULL, ":");
        span = strtok(NULL, ":");
        int width = atoi(span);

        if (direc == NULL || span == NULL)
        {
            printf("wrong form of input in line %d [_:_:_]\n", i + 1);
            return NULL;
        }

        if (strcmp(direc, "horizontal") != 0 && strcmp(direc, "vertical") != 0)
        {
            printf("wrong direction input in line %d\n", i + 1);
            return NULL;
        }
        if (width < 1 || width > 6)
        {
            printf("wrong span input in line %d\n", i + 1);
            return NULL;
        }
        cars[i].id = i;
        cars[i].span = width;
        if (!strcmp(direc, "horizontal"))
        {
            cars[i].dir = horizontal;
            cars[i].x1 = cell[0] - 'A';
            cars[i].y2 = '6' - cell[1];
            cars[i].y1 = '6' - cell[1];
            cars[i].x2 = cell[0] - 'A' + width - 1;
        }
        else
        {
            cars[i].dir = vertical;
            cars[i].x1 = cell[0] - 'A';
            cars[i].y2 = '6' - cell[1] + width - 1;
            cars[i].y1 = '6' - cell[1];
            cars[i].x2 = cell[0] - 'A';
        }
        //printf("%d %s %d %d %d %d\n", i, direc, cars[i].y1, cars[i].x1, cars[i].y2, cars[i].x2);

        if ((cars[i].x1 > 5 || cars[i].x1 < 0) || (cars[i].x2 > 5 || cars[i].x2 < 0) || (cars[i].y1 > 5 || cars[i].y1 < 0) || (cars[i].y2 > 5 || cars[i].y2 < 0))
        {
            printf("the car is in the out of the map");
            return NULL;
        }
        i++;
    }
    st->cars = cars;
    return st;
};

struct State *set_copy_State(struct State *p)
{
    struct State *st = malloc(sizeof(struct State));
    car_t *cars = (car_t *)malloc(sizeof(car_t) * (n_cars + 1)); // malloc N cars.

    for (int i = 0; i < n_cars; i++)
    {
        cars[i] = p->cars[i];
    }
    st->cars = cars;
    return st;
};

int update_cells(struct State *p)
{
    memset(cells, 0, sizeof(int) * 36); // clear cells before the write.

    for (int i = 0; i < n_cars; i++)
    {
        if (p->cars[i].dir == horizontal)
        {
            for (int j = 0; j < p->cars[i].span; j++)
            {
                if (cells[p->cars[i].y1][(p->cars[i].x1) + j] != 0)
                {
                    printf("overlapping with other car\n");
                    return 1;
                }
                cells[p->cars[i].y1][(p->cars[i].x1) + j] = p->cars[i].id + 1;
            }
        }
        else
        {
            for (int j = 0; j < p->cars[i].span; j++)
            {
                if (cells[(p->cars[i].y1) + j][p->cars[i].x1] != 0)
                {
                    printf("overlapping with other car\n");
                    return 1;
                }
                cells[(p->cars[i].y1) + j][p->cars[i].x1] = p->cars[i].id + 1;
            }
        }
    }
    return 0;
    //FIXME
    // return 0 for sucess
    // return 1 if the given car information (cars) has a problem
}

int already_visited(struct _Queue *v, struct State *next)
{
    int a;
    struct Node *node;
    node = v->front;

    while (node != NULL)
    {
        a = 0;
        for (int i = 0; i < n_cars; i++)
        {
            if (!((node->data->cars[i].x1 == next->cars[i].x1) && (node->data->cars[i].x2 == next->cars[i].x2) && (node->data->cars[i].y1 == next->cars[i].y1) && (node->data->cars[i].y2 == next->cars[i].y2)))
            {
                a = -1;
            }
        }
        if (a == 0)
        {
            return 0;
        }
        node = node->next;
    }
    return 1;
};

void next_case(struct _Queue *q, struct _Queue *v, int id, struct State *p)
{
    if (p->cars[id].dir == vertical)
    {
        if (!(p->cars[id].dir == horizontal || p->cars[id].y1 <= 0 || cells[p->cars[id].y1 - 1][p->cars[id].x1] != 0)) //Up
        {
            struct State *uped = set_copy_State(p);
            uped->cars[id].y1 += -1;
            uped->cars[id].y2 += -1;
            if (already_visited(v, uped) && already_visited(q, uped))
            {
                Enqueue(q, uped);
                q->rear->prev = v->rear;
            }
        }
        if (!(p->cars[id].dir == horizontal || p->cars[id].y2 >= 5 || cells[p->cars[id].y2 + 1][p->cars[id].x2] != 0))
        {
            struct State *downed = set_copy_State(p);
            downed->cars[id].y1 += 1;
            downed->cars[id].y2 += 1;
            if (already_visited(v, downed) && already_visited(q, downed))
            {
                Enqueue(q, downed);
                q->rear->prev = v->rear;
            }
        }
    }
    else //horizontal
    {
        if (!(p->cars[id].dir == vertical || p->cars[id].x1 <= 0 || cells[p->cars[id].y1][p->cars[id].x1 - 1] != 0)) //left
        {
            struct State *lefted = set_copy_State(p);
            lefted->cars[id].x1 += -1;
            lefted->cars[id].x2 += -1;
            if (already_visited(v, lefted) && already_visited(q, lefted))
            {
                Enqueue(q, lefted);
                q->rear->prev = v->rear;
            }
        }

        if (!(p->cars[id].dir == vertical || p->cars[id].x2 >= 5 || cells[p->cars[id].y2][p->cars[id].x2 + 1] != 0)) // Right
        {
            struct State *righted = set_copy_State(p);
            righted->cars[id].x1 += 1;
            righted->cars[id].x2 += 1;
            if (already_visited(v, righted) && already_visited(q, righted))
            {
                Enqueue(q, righted);
                q->rear->prev = v->rear;
            }
        }
    }
}

int checkfinish(struct _Queue *v)
{
    if ((v->rear->data->cars[0].x2 == 5) && (v->rear->data->cars[0].y2 == 2))
    {
        return 0;
    }
    else
    {
        return 1;
    }
};

void display()
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (cells[i][j] == 0)
                printf("+ ");
            else
                printf("%d ", cells[i][j]);
        }
        printf("\n");
    }
}
void showque(struct _Queue *q)
{
    struct Node *a;
    a = q->front;
    while (a != NULL)
    {
        update_cells(a->data);
        display();
        printf("\n");
        a = a->next;
    }
}
void showtheroot(struct _Queue *q)
{
    struct Node *a;
    a = q->rear;
    while (a != NULL)
    {
        update_cells(a->data);
        display();
        printf("\n");
        a = a->prev;
    }
}

int main()
{
    struct State *initial;
    struct _Queue *v = create_queue();
    struct _Queue *q = create_queue();
    char buf[128];

    printf("Start the game!, Enter the filename to play rush hour\n");
    scanf("%s", buf);
    initial = set_initial_State(buf);

    Enqueue(q, initial);
    while (!IsEmpty(q))
    {
        struct Node *p = Dequeue(q);
        Enqueue_node(v, p);

        if (!checkfinish(v))
        {
            printf("Finish, the car escape!!\n\n");
            break;
        };

        struct State *latest = set_copy_State(p->data);

        if (update_cells(latest))
        {
            printf("overlapping!!\n");
            break;
        }
        for (int i = 0; i < n_cars; i++)
        {
            next_case(q, v, i, latest);
        }
    }
    showtheroot(v);
}