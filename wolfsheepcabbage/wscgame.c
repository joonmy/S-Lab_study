#include <stdio.h>
#include <stdlib.h>
#define N_subject 4

typedef enum
{
    left,
    right
} loc;

struct State
{
    loc person;
    loc wolf;
    loc sheep;
    loc cabbage;
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
    newNode->data = data;

    if (IsEmpty(q))
    {
        q->front = newNode;
        q->rear = newNode;
    }
    else
    {
        q->rear->next = newNode;
        //newNode->prev = q->rear;
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
        //newNode->prev = q->rear;
        q->rear = data;
        data->next = NULL;
    }
};

struct Node *Dequeue(struct _Queue *q)
{
    struct Node *deleteNode;

    /*if (IsEmpty(q))
    {
        printf("Queue is empty!");
    }*/
    deleteNode = q->front;
    q->front = q->front->next;

    return deleteNode;
};

struct State *setState(loc p, loc w, loc s, loc c)
{
    struct State *pt = malloc(sizeof(struct State));
    pt->person = p;
    pt->wolf = w;
    pt->sheep = s;
    pt->cabbage = c;
    return pt;
};

int checkfinish(struct _Queue *v)
{
    if (v->rear->data->person == right && v->rear->data->wolf == right && v->rear->data->sheep == 1 && v->rear->data->cabbage == 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
};
int already_visited(struct _Queue *v, struct State *next)
{
    struct Node *node;
    node = v->front;

    while (node != NULL)
    {
        if ((node->data->person == next->person) && (node->data->wolf == next->wolf) && (node->data->sheep == next->sheep) && (node->data->cabbage == next->cabbage))
        {
            return 0; //visited
        }
        node = node->next;
    }
    return 1;
};
void showtheroot(struct _Queue *q)
{
    struct Node *a;
    a = q->rear;
    while (a != NULL)
    {
        printf("%d %d %d %d\n", a->data->person, a->data->wolf, a->data->sheep, a->data->cabbage);
        a = a->prev;
    }
}
void showque(struct _Queue *q)
{
    struct Node *a;
    a = q->front;
    while (a != NULL)
    {
        printf("%d %d %d %d ->", a->data->person, a->data->wolf, a->data->sheep, a->data->cabbage);
        a = a->next;
    }
}
int main()
{
    struct State *initial = setState(left, left, left, left);  //초기 상태 all left 로 설정
    struct State *goal = setState(right, right, right, right); // 비교 위한 최종 결과 all right 로 설정
    struct _Queue *v = create_queue();
    struct _Queue *q = create_queue(); // 큐 생성, 헤더 리턴
    //struct State *v = malloc(sizeof(struct State) * (N_subject * N_subject + 1)); // visited node 정보 담을 struct State 배열 생성.

    Enqueue(q, initial);
    while (!IsEmpty(q)) //BFS start
    {
        struct Node *p = Dequeue(q);

        Enqueue_node(v, p);

        if (!checkfinish(v))
        {
            printf("Bingo\n");
            break;
        };

        struct State *latest = setState(p->data->person, p->data->wolf, p->data->sheep, p->data->cabbage); //마지막에 pop한 node의 data 부분만 담음.
        latest->person = !(latest->person);
        if (((latest->wolf == latest->sheep) && (latest->person != latest->wolf)) || ((latest->sheep == latest->cabbage) && (latest->person != latest->sheep)))
        {
        }
        else
        {
            if (already_visited(v, latest) && already_visited(v, latest)) //한번도 방문한적 없다면,
            {
                struct State *cpy = setState(latest->person, latest->wolf, latest->sheep, latest->cabbage);
                Enqueue(q, cpy); // prev 포인팅 및 push 둘다
                q->rear->prev = v->rear;
            }
        }
        for (int i = 0; i < N_subject - 1; i++)
        {
            struct State *next = setState(latest->person, latest->wolf, latest->sheep, latest->cabbage);
            if (i == 0)
            {
                next->wolf = !(next->wolf);
            }
            else if (i == 1)
            {
                next->sheep = !(next->sheep);
            }
            else
            {
                next->cabbage = !(next->cabbage);
            }

            if ((next->wolf == next->sheep) && (next->person != next->wolf))
            {
                if (i == 0)
                {
                    next->wolf = !(next->wolf);
                }
                else if (i == 1)
                {
                    next->sheep = !(next->sheep);
                }
                else
                {
                    next->cabbage = !(next->cabbage);
                }
                continue;
            }
            if ((next->sheep == next->cabbage) && (next->person != next->sheep))
            {
                if (i == 0)
                {
                    next->wolf = !(next->wolf);
                }
                else if (i == 1)
                {
                    next->sheep = !(next->sheep);
                }
                else
                {
                    next->cabbage = !(next->cabbage);
                }
                continue;
            }
            if (already_visited(v, next) && already_visited(q, next)) //방문한적이 없다면,
            {
                struct State *cp = setState(next->person, next->wolf, next->sheep, next->cabbage);
                Enqueue(q, cp); // prev 포인팅 및 push 둘다
                q->rear->prev = v->rear;
            }
        }
    }
    //showque(q);
    //showque(v);
    showtheroot(v);
}