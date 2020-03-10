#include <stdio.h>
#include <stdlib.h>
#define MAXCOST 100
typedef struct Node {
    int row_num;
    int col_num;
    int count;
    struct Node *next_path;
    struct Node *next_cell;
} Node;

int num_rows, num_cols, size, start_row, start_col, end_row, end_col;
int min_cost_iter;
Node* best_path;

void printQueue(Node *prq[MAXCOST + 1]) {
    for (int i = 0; i < 10; i++) {
        if (prq[i] != NULL) {
            Node *tmp = prq[i];
            while (tmp != NULL) {
                Node *currPath = tmp;
                printf("cost:%d\t", i);
                while (currPath != NULL) {
                    printf("%d,%d->", currPath->row_num, currPath->col_num);
                    currPath = currPath->next_cell;
                }
                printf("\n");
                tmp = tmp->next_path;
            }
            printf("\n");
        }
        printf("\n");
    }
}


int countPaths(Node *prev_cell, Node *curr_cell, int maze[][num_cols], int *next_row, int *next_col) {
    int count = 0;
    int curr_row_index = curr_cell->row_num, curr_col_index = curr_cell->col_num, prev_row_index = prev_cell->row_num, prev_col_index = prev_cell->col_num;
    if (curr_row_index != prev_row_index || curr_col_index + 1 != prev_col_index) {
        if (maze[curr_row_index][curr_col_index + 1] != -1) {
            next_row[count] = curr_row_index;
            next_col[count] = curr_col_index + 1;
            count += 1;
        }

    }
    if (curr_row_index != prev_row_index || curr_col_index - 1 != prev_col_index) {
        if (maze[curr_row_index][curr_col_index - 1] != -1) {
            next_row[count] = curr_row_index;
            next_col[count] = curr_col_index - 1;
            count += 1;
        }
    }
    if (curr_row_index + 1 != prev_row_index || curr_col_index != prev_col_index) {
        if (maze[curr_row_index + 1][curr_col_index] != -1) {
            next_row[count] = curr_row_index + 1;
            next_col[count] = curr_col_index;
            count += 1;
        }
    }
    if (curr_row_index - 1 != prev_row_index || curr_col_index != prev_col_index) {
        if (maze[curr_row_index - 1][curr_col_index] != -1) {
            next_row[count] = curr_row_index - 1;
            next_col[count] = curr_col_index;
            count += 1;
        }
    }
    return count;
}

void deletePath(Node *prev_path, Node *curr_path, int i,Node *prq[MAXCOST + 1]) {
    if (prev_path == NULL) {
        prq[i] = curr_path->next_path;
    } else {
        prev_path->next_path = curr_path->next_path;
    }
    Node* tmp=curr_path->next_cell;
    while(tmp!=NULL)
    {
        free(curr_path);
        curr_path=tmp;
        tmp=curr_path->next_cell;
    }
}

Node *createNode(int row, int col) {
    Node *ptr;
    ptr = (Node *) malloc(sizeof(Node));
    ptr->next_path = NULL;
    ptr->count = 0;
    ptr->row_num = row;
    ptr->col_num = col;
    ptr->next_cell = NULL;

    return ptr;
}

Node *copyPath(Node *curr_path) {
    Node *new_path = NULL;
    Node *tmp = NULL;
    while (curr_path != NULL) {
        if (tmp != NULL) {
            tmp->next_cell = (Node *) malloc(sizeof(Node));
            tmp = tmp->next_cell;
        } else {
            tmp = (Node *) malloc(sizeof(Node));
        }
        tmp->count = curr_path->count;
        tmp->col_num = curr_path->col_num;
        tmp->row_num = curr_path->row_num;
        tmp->next_path = curr_path->next_path;
        tmp->next_cell = NULL;
        if (new_path == NULL)
            new_path = tmp;
        curr_path = curr_path->next_cell;

    }

    return new_path;
}

void insertPath(Node *new_path, int index, Node *prq[MAXCOST + 1]) {
    new_path->next_path = prq[index];
    prq[index] = new_path;
}

void updatePos(Node *prev_path, Node *curr_path, int index, Node *prq[MAXCOST + 1]) {
    int prev_index = index - 1;
    if (prev_path == NULL) {
        prq[prev_index] = curr_path->next_path;
    } else {
        prev_path->next_path = curr_path->next_path;
    }
    insertPath(curr_path, index,prq);
}

void work(int count, Node *curr_path, Node *prev_path, int i, int maze[][num_cols], int *next_row, int *next_col,
          Node *curr_cell, int *flag, Node *prq[MAXCOST + 1]) {
    if (count == 0 || curr_path->count == 120) {
        deletePath(prev_path, curr_path, i,prq);
        *flag=1;

    } else {
        curr_path->count += 1;
        int x = 0;
        while (x != count - 1) {
            Node *new_path = copyPath(curr_path);
            Node *new_node = new_path;
            while (new_node->next_cell != NULL) {
                new_node = new_node->next_cell;
            }
            new_node->next_cell = createNode(next_row[x], next_col[x]);
            if (new_node->next_cell->row_num == end_row && new_node->next_cell->col_num == end_col)
                new_path->count = -1;
            int new_index = i + maze[next_row[x]][next_col[x]];
            insertPath(new_path, new_index,prq);
            x += 1;
        }

        curr_cell->next_cell = createNode(next_row[x], next_col[x]);
        if (curr_cell->next_cell->row_num == end_row && curr_cell->next_cell->col_num == end_col)
        {
            curr_path->count = -1;
            if (i<min_cost_iter)
            {
                min_cost_iter=i;
                 best_path=curr_path;
            }
        }
        if (maze[next_row[x]][next_col[x]] == 1) {
            int new_index = i + 1;
            *flag = 1;
            updatePos(prev_path, curr_path, new_index,prq);
        }
    }
}

void iter(int iter_num, int maze[][num_cols],Node *prq[MAXCOST + 1]) {
//    printQueue(prq);
    for (int i = 0; i <= iter_num; i++) {
//        printf("\n%d\n",i);
        Node *prev_path = NULL;
        Node *curr_path = prq[i];
        Node *new_prev_path=NULL;
        int flag = 0;
        while (curr_path != NULL) {
            flag=0;
            if (curr_path->count != -1 && curr_path->count <= iter_num) {
                new_prev_path = prev_path;
                //new_curr_path=curr_path;
                Node *curr_cell = curr_path;
                Node *prev_cell = NULL;
                while (curr_cell->next_cell != NULL) {
                    prev_cell = curr_cell;
                    curr_cell = curr_cell->next_cell;
                }

                if (prev_cell != NULL) {
                    int next_row[3], next_col[3];
                    int count = countPaths(prev_cell, curr_cell, maze, next_row, next_col);
                    work(count, curr_path, prev_path, i, maze, next_row, next_col, curr_cell, &flag,prq);
                } else {
                    Node *useless = (Node *) malloc(sizeof(Node));
                    useless->row_num = -1;
                    useless->col_num = -1;
                    useless->next_path = NULL;
                    useless->next_cell = NULL;
                    useless->count = 0;
                    int next_row[4], next_col[4];
                    int count = countPaths(useless, curr_cell, maze, next_row, next_col);
                    work(count, curr_path, prev_path, i, maze, next_row, next_col, curr_cell, &flag,prq);
                }
            }
            if (flag == 1) {
                if (new_prev_path == NULL) {
                    prev_path = new_prev_path;
                    curr_path = prq[i];
                } else {
                    prev_path = new_prev_path;
                    curr_path = new_prev_path->next_path;
                }
            } else {
                prev_path=curr_path;
                curr_path=curr_path->next_path;
            }
        }
    }
}


void init(Node *prq[MAXCOST + 1]) {
    Node *startNode = (Node *) malloc(sizeof(Node));
    startNode->col_num = start_col;
    startNode->row_num = start_row;
    startNode->next_cell = NULL;
    startNode->next_path = NULL;
    startNode->count = 0;
    prq[0] = startNode;
    for (int i = 1; i < size; i++)
        prq[i] = NULL;

}


void printMaze(int maze[][num_cols]) {
    for (int i = 0; i < num_rows; i++) {
        printf("|");
        for (int j = 0; j < num_cols; j++) {
            if (i == start_row && j == start_col)
                printf(" S ");
            else if (i == end_row && j == end_col)
                printf(" E ");
            else if (maze[i][j] == -1)
                printf(" # ");
            else if (maze[i][j] == 0)
                printf("   ");
            else
                printf(" C ");
            printf("|");
        }
        printf("\n");
    }
}

int main() {
    Node *prq[MAXCOST + 1];
    min_cost_iter=120;
    num_rows = 12;
    num_cols = 10;
    int maze[12][10] = {{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                        {-1, 0,  0,  0,  1,  1,  0,  0,  0,  -1},
                        {-1, 0,  -1, -1, -1, 0,  -1, -1, 1,  -1},
                        {-1, 1,  1,  1,  1,  1,  0,  0,  1,  -1},
                        {-1, -1, 0,  0,  0,  -1, 0,  0,  -1, -1},
                        {-1, 0,  -1, -1, 1,  -1, 1,  -1, -1, -1},
                        {-1, -1, 0,  0,  0,  0,  1,  0,  0,  -1},
                        {-1, 0,  -1, 0,  -1, 1,  1,  -1, 0,  -1},
                        {-1, -1, -1, 0,  -1, 1,  -1, -1, -1, -1},
                        {-1, 0,  1,  1,  -1, 1,  1,  0,  0,  -1},
                        {-1, 0,  -1, 0,  0,  1,  1,  -1, 0,  -1},
                        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};
    start_row = 1;
    start_col = 1;
    end_row = 10;
    end_col = 8;
    size = MAXCOST + 1;
    //printf("num_rows:%d\nnum_cols:%d\n",num_rows,num_cols);
    init(prq);
    //printMaze(maze);
    printQueue(prq);
    for (int i = 0; i < min_cost_iter; i++) {
        printf("iter %d\n", i);
        iter(i, maze,prq);
        printQueue(prq);
    }

    Node *tmp=best_path;
    int count=0;
    printf("Cost : %d \n", min_cost_iter);
    while(tmp!=NULL)
    {
        printf("%d,%d->", tmp->row_num, tmp->col_num);

        tmp=tmp->next_cell;
        count+=1;

    }

    printf("\nSteps : %d \n", count-1);
    return 0;
}
