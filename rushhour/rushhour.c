#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* NOFIX --- */

typedef enum
{
    start,
    left,
    right,
    up,
    down,
    quit,
    N_op
} commands;

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

char *op_str[N_op] = {
    "start",
    "left",
    "right",
    "up",
    "down",
    "quit"};

int n_cars = 0;
car_t *cars = 0x0;
int cells[6][6]; // cells[Y][X]
// A6 -> cells[5][0]
// F4 -> cells[3][5]
// F1 -> cells[0][5]

/* --- NOFIX */

commands
get_op_code(char *s)
{
    if (!strcmp(s, "start"))
        return start;
    if (!strcmp(s, "left"))
        return left;
    if (!strcmp(s, "right"))
        return right;
    if (!strcmp(s, "up"))
        return up;
    if (!strcmp(s, "down"))
        return down;
    if (!strcmp(s, "quit"))
        return quit;
    return N_op;
    // return the corresponding number for the command given as s.
    // FIXME
}

int load_game(char *filename)
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
        return 1;
    }

    fscanf(fp, "%d", &n_cars); //get n_cars
    fgetc(fp);
    if (n_cars < 2 || n_cars > 36)
    {
        printf("the number of car needs to be 2< <36 and type of number\n");
        return 1;
    }

    cars = (car_t *)malloc(sizeof(car_t) * (n_cars + 1)); // malloc N cars.

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
            return 1;
        }

        if (strcmp(direc, "horizontal") != 0 && strcmp(direc, "vertical") != 0)
        {
            printf("wrong direction input in line %d\n", i + 1);
            return 1;
        }
        if (width < 1 || width > 6)
        {
            printf("wrong span input in line %d\n", i + 1);
            return 1;
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
        printf("%d %s %d %d %d %d\n", i, direc, cars[i].y1, cars[i].x1, cars[i].y2, cars[i].x2);

        if ((cars[i].x1 > 5 || cars[i].x1 < 0) || (cars[i].x2 > 5 || cars[i].x2 < 0) || (cars[i].y1 > 5 || cars[i].y1 < 0) || (cars[i].y2 > 5 || cars[i].y2 < 0))
        {
            printf("the car is in the out of the map");
            return 1;
        }
        i++;
    }
    return 0;
    //FIXME
    // load_game returns 0 for a success, or return 1 for a failure.
    // Use fopen, getline, strtok, atoi, strcmp
    // Note that the last character of a line obtained by getline may be '\n'.
}

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
    if (cells[2][5] == 1)
    {
        printf("Done\n");
        exit(0);
    }
    /* The beginning state of board1.txt must be shown as follows: 
 	 + + 2 + + +
 	 + + 2 + + +
	 1 1 2 + + +
	 3 3 3 + + 4
	 + + + + + 4
	 + + + + + 4
	*/

    //FIXME
}

int update_cells()
{
    memset(cells, 0, sizeof(int) * 36); // clear cells before the write.

    for (int i = 0; i < n_cars; i++)
    {
        if (cars[i].dir == horizontal)
        {
            for (int j = 0; j < cars[i].span; j++)
            {
                if (cells[cars[i].y1][(cars[i].x1) + j] != 0)
                {
                    printf("overlapping with other car\n");
                    return 1;
                }
                cells[cars[i].y1][(cars[i].x1) + j] = cars[i].id + 1;
            }
        }
        else
        {
            for (int j = 0; j < cars[i].span; j++)
            {
                if (cells[(cars[i].y1) + j][cars[i].x1] != 0)
                {
                    printf("overlapping with other car\n");
                    return 1;
                }
                cells[(cars[i].y1) + j][cars[i].x1] = cars[i].id + 1;
            }
        }
    }
    return 0;
    //FIXME
    // return 0 for sucess
    // return 1 if the given car information (cars) has a problem
}

int move(int id, int op)
{
    if (id > n_cars)
    {
        printf("the car number should be under %d", n_cars);
        return 1;
    }
    if (op == left)
    {
        if (cars[id - 1].dir == vertical || cars[id - 1].x1 <= 0 || cells[cars[id - 1].y1][cars[id - 1].x1 - 1] != 0)
        {
            printf("can not go to left\n");
            return 1;
        }
        cars[id - 1].x1 += -1;
        cars[id - 1].x2 += -1;
    }
    else if (op == right)
    {
        if (cars[id - 1].dir == vertical || cars[id - 1].x2 >= 5 || cells[cars[id - 1].y2][cars[id - 1].x2 + 1] != 0)
        {
            printf("can not go to right\n");
            return 1;
        }
        cars[id - 1].x1 += 1;
        cars[id - 1].x2 += 1;
    }
    else if (op == up)
    {
        if (cars[id - 1].dir == horizontal || cars[id - 1].y1 <= 0 || cells[cars[id - 1].y1 - 1][cars[id - 1].x1] != 0)
        {
            printf("can not go to up\n");
            return 1;
        }
        cars[id - 1].y1 += -1;
        cars[id - 1].y2 += -1;
    }
    else
    { //down
        if (cars[id - 1].dir == horizontal || cars[id - 1].y2 >= 5 || cells[cars[id - 1].y2 + 1][cars[id - 1].x2] != 0)
        {
            printf("can not go to down\n");
            return 1;
        }
        cars[id - 1].y1 += 1;
        cars[id - 1].y2 += 1;
    }
    return 0;
    //FIXME
    // move returns 1 when the given input is invalid.
    // or return 0 for a success.

    // Update cars[id].x1, cars[id].x2, cars[id].y1 and cars[id].y2
    //   according to the given command (op) if it is possible.

    // The condition that car_id can move left is when
    //  (1) car_id is horizontally placed, and
    //  (2) the minimum x value of car_id is greather than 0, and
    //  (3) no car is placed at cells[cars[id].y1][cars[id].x1-1].
    // You can find the condition for moving right, up, down as
    //   a similar fashion.
}

int main()
{
    char buf[128];
    int op;
    int id;

    while (1)
    {
        scanf("%s", buf);

        switch (op = get_op_code(buf))
        {
        case start:
            scanf("%s", buf);
            if (load_game(buf))
            {
                free(cars);
                break;
            }
            if (update_cells())
            {
                free(cars);
                break;
            }
            display();
            break;
        case left:
        case right:
        case up:
        case down:
            if (cars == 0x0)
            {
                printf("start first\n");
                break;
            }
            scanf("%d", &id);
            if (move(id, op))
            {
                display();
                break;
            }
            if (update_cells())
                break;
            display();
            break;
        case quit:
            exit(0);
        case N_op:
            printf("no such command\n");
            //scanf("128%s",buf);
            break;
            //FIXME
        }
    }
}