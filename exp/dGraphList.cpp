/*
*	有向图的邻接表表示
*/
#include<stdio.h>
#include<malloc.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1
#define UNVISITED 0
#define VISITED 1
typedef char VexType; //顶点类型
typedef int Status; //函数值类型，表示函数结果状态

// 链队列类型定义，用于遍历
typedef struct QNode {
    int data;
    struct QNode *next;
} QNode, *QueuePtr;
typedef struct {
    QueuePtr front;    //队头指针
    QueuePtr rear;    //队尾指针
} Queue;

// 邻接表类型定义
typedef struct ArcNode {
    int adjvex; //邻接顶点在顶点数组中的位序
    struct ArcNode *nextArc; //指向下一个邻接顶点的指针
} ArcNode, *ArcNodeP;         //邻接链表的结点类型
typedef struct VexNode {
    VexType data; //顶点值
    struct ArcNode *firstArc; //邻接链表的头指针
} VexNode;    //顶点数组的元素类型
typedef struct {
    VexNode *vertices; //顶点数组，用于储存顶点信息
    int n, e;  //图的当前顶点数和弧数
    int *tags;  //标志数组，可用于在图的遍历中标记顶点访问与否
} DG;        //邻接表类型

// 初始化链队列
void InitQueue(Queue *Q) {
    Q->front = Q->rear = (QueuePtr) malloc(sizeof(QNode));
    if (!Q->front) return;
    Q->front->next = NULL;
}

// 链队列判空
Status QueueEmpty(Queue Q) {
    if (Q.front == Q.rear)
        return TRUE;
    return FALSE;
}

// 元素e入队
Status EnQueue(Queue *Q, int e) {
    QueuePtr p;
    p = (QueuePtr) malloc(sizeof(QNode));
    if (NULL == p) return OVERFLOW;
    p->data = e;
    p->next = NULL;
    if (NULL == Q->front) Q->front = p;
    else Q->rear->next = p;
    Q->rear = p;
    return OK;
}

//出队
Status DeQueue(Queue *Q, int *e) {
    QueuePtr p;
    if (NULL == Q->front) return ERROR;
    p = Q->front;
    e = &p->data;
    Q->front = p->next;
    if (Q->rear == p) Q->rear = NULL;
    free(p);
    return OK;
}

// 打印邻接表 
void PrintG(DG G) {
    int i, j;
    ArcNodeP p;
    p = (ArcNodeP) malloc(sizeof(ArcNode));
    printf("\n有向图的邻接表：\n");
    for (i = 0; i < G.n; i++) {
        printf("%d %c:->", i, G.vertices[i].data);
        for (p = G.vertices[i].firstArc; p; p = p->nextArc)
            printf("%c->", G.vertices[p->adjvex].data);
        printf("NULL\n");
    }
}

// 查找顶点v在图G中的位序
int LocateVex(DG *G, VexType v) {
    int i;
    for (i = 0; i < G->n; i++) {
        if (G->vertices[i].data == v)
            return i;
    }
    return -1;
}

// 创建n个顶点和e条边的图G
Status CreatDG(DG *G) {
    int i, j, k, n, e;
    VexType head, tail;
    ArcNode *p;
    printf("请输入顶点数：");
    scanf("%d", &n);
    printf("请输入弧数：");
    scanf("%d", &e);
    if (n < 0 || e < 0) return ERROR;
    G->n = n;
    G->e = e;
    G->vertices = (VexNode *) malloc(n * sizeof(VexNode));
    G->tags = (int *) malloc(n * sizeof(int));
    printf("\n");
    for (i = 0; i < G->n; i++) {  //初始化标志数组，建立顶点数组
        getchar();
        G->tags[i] = UNVISITED;
        printf("请输入位序为%d的顶点信息：", i);
        scanf("%c", &G->vertices[i].data);
        G->vertices[i].firstArc = NULL;
    }
    printf("\n");
    for (k = 0; k < G->e; k++) {
        getchar();
        printf("请输入弧的信息(弧尾->弧头，英文逗号/空格间隔)：");
        scanf("%c%*c%c", &tail, &head);
        i = LocateVex(G, tail);
        j = LocateVex(G, head); //确定head和tail的位序i和j
        if (i < 0 || j < 0) return ERROR;    //顶点v或w不存在
        p = (ArcNode *) malloc(sizeof(ArcNode)); //为顶点w分配p结点
        p->adjvex = j;
        p->nextArc = G->vertices[i].firstArc;
        G->vertices[i].firstArc = p;
    }
    PrintG(*G);
    return OK;
}

// 销毁图G
Status DestroyDG(DG *G) {
    ArcNode *p, *pre;
    int i;
    for (i = 0; i < G->n; i++) {
        p = G->vertices[i].firstArc;
        while (p != NULL) {
            pre = p;
            p = p->nextArc;
            free(pre);
        }
        G->vertices[i].firstArc = NULL;
        G->vertices[i].data = 0;
    }
    G->n = 0;
    G->e = 0;
    return OK;
}

// 取图G的k顶点的值到w
Status GetVex(DG *G, int k, VexType *w) {
    if (k < 0 || k >= G->n)
        return ERROR;    //k顶点不存在
    *w = G->vertices[k].data;
    return OK;
}

// 对图G的k顶点赋值w
Status PutVex(DG *G, int k, VexType w) {
    if (k < 0 || k >= G->n)
        return ERROR;    //k顶点不存在
    G->vertices[k].data = w;
    return OK;
}

// 求第一个邻接顶点
int FirstAdjVex(DG *G, int k) {
    if (k < 0 || k >= G->n)
        return -1;        //k顶点不存在
    ArcNodeP p;
    p = G->vertices[k].firstArc;    //p指向k顶点对应邻接链表的第一个结点
    if (p)
        return p->adjvex; //返回第一个结点存储的顶点位序
    else
        return -1;    //k顶点无邻接顶点
}

// 求下一个邻接顶点
int NextAdjVex(DG *G, int k, ArcNodeP p) {
    if (k < 0 || k >= G->n)
        return -1;        //k顶点不存在
    if (!p)
        return -1;        //指针p为空
    p = p->nextArc;        //p指向下一个结点
    if (p)
        return p->adjvex;    //返回p结点存储的下一个邻接顶点的位序
    else
        return -1;        //k顶点无下一个邻接顶点
}

// 插入顶点k到m的弧(邻接链表的表头插入)
Status AddArc(DG *G, int k, int m) {
    ArcNodeP p;
    if (k < 0 || k >= G->n || m < 0 || m >= G->n)
        return ERROR;
    p = G->vertices[k].firstArc;
    while (p) {    //判断弧是否存在
        if (m == p->adjvex)
            return ERROR;    //弧存在，返回
        p = p->nextArc;
    }
    p = (ArcNode *) malloc(sizeof(ArcNode));    //为m顶点分配p结点
    if (NULL == p)
        return OVERFLOW;
    p->adjvex = m;
    p->nextArc = G->vertices[k].firstArc;    //在k顶点的邻接链表的表头插入p结点
    G->vertices[k].firstArc = p;
    G->e++;
    return OK;
}

// 删除顶点k到m的弧
Status RemoveArc(DG *G, int k, int m) {
    if (G->n == 0)    //G中无边，返回
        return ERROR;
    ArcNodeP p, pre;
    p = G->vertices[k].firstArc;    //p指向邻接链表的头结点
    pre = NULL;
    while (p) {
        if (m == p->adjvex) {        //若m为该结点在邻接数组中的位序
            if (!pre)    //顶点m为头结点
                G->vertices[k].firstArc = p->nextArc;
            else    //顶点m非头结点
                pre->nextArc = p->nextArc;
            free(p);
            G->e--;    //边数减1
            break;    //删除后退出循环
        }
        pre = p;
        p = p->nextArc;
        if (!p)    //无k到m的边或弧，返回
            return ERROR;
    }
}

//Visit函数
Status Visit(VexType v) {
    printf("%c ", v);
    return OK;
}

// 深度优先遍历递归算法 
Status DFS(DG G, int k, Status(*visit)(VexType)) {
    int i;
    G.tags[k] = VISITED;
    visit(G.vertices[k].data); //输出顶点值
    ArcNodeP p;
    p = (ArcNodeP) malloc(sizeof(ArcNode));
    p = G.vertices[k].firstArc;
    while (p) {
        if (UNVISITED == G.tags[p->adjvex]) //未遍历该顶点
            if (ERROR == DFS(G, p->adjvex, visit))
                return ERROR;
        p = p->nextArc;
    }
    return OK;
}

// 深度优先遍历图G
Status DFSTraverse(DG G, Status(*visit)(VexType)) {
    int i;
    for (i = 0; i<G.n; i++) G.tags[i] = UNVISITED; 
    for (i = 0; i < G.n; i++)
        if (UNVISITED == G.tags[i])
            if (ERROR == DFS(G, i, visit)) return ERROR;
    return OK;
}

// 广度优先遍历
Status BFSTraverse(DG G, Status(*visit)(VexType)) {
    int i, j, k;
    ArcNodeP p;
    Queue Q;
    InitQueue(&Q);
    for (i = 0; i < G.n; i++) G.tags[i] = UNVISITED;
    for (i = 0; i < G.n; i++) {
        if (UNVISITED == G.tags[i]) { //i顶点未被访问
            G.tags[i] = VISITED;
            if (ERROR == visit(G.vertices[i].data)) return ERROR;
            EnQueue(&Q, i); //i顶点入队列
            while (OK == DeQueue(&Q, &i)) {
                p = G.vertices[i].firstArc;
                while (p) {
                    if (UNVISITED == G.tags[j]) {
                        if (ERROR == visit(j)) return ERROR;
                        G.tags[p->adjvex] = VISITED;
                        EnQueue(&Q, p->adjvex);
                    }
                    p = p->nextArc;
                }
            }
        }
    }
    return OK;
}

int main() {
//    printf("------------- 设计性实验（有向图）-------------\n\n");
//    printf("|                  二、邻接表                 |\n\n");
//    printf("|        班级：计算机科学与技术19(1)班        |\n\n");
//    printf("|              学号：3119004733               |\n\n");
//    printf("|                姓名：黄祖佳                 |\n\n");
//    printf("-----------------------------------------------\n\n");
    DG G;
    int select, k, m;
    VexType v, w;
    ArcNodeP p;
    p = (ArcNodeP) malloc(sizeof(ArcNode));
    printf("1. 创建图；2. 销毁图；3. 查找位序；4. 取k顶点值；\n"
           "5. 为k顶点赋值；6. 插入弧；7. 删除弧；8. 求第一个邻接顶点；\n"
           "9. 求下一个邻接顶点；10. 深度优先遍历；11. 广度优先遍历；0. 退出\n"
           "请选择操作（请先创建图）：");
    scanf("%d", &select);
    do {
        switch (select) {
            case 1:
                printf("\n** 1. 创建图 **\n");
                CreatDG(&G);
                break;
            case 2:
                printf("\n** 2. 销毁图 **\n");
                if (DestroyDG(&G))
                    printf("当前图销毁成功！顶点数：%d，边数：%d\n", G.n, G.e);
                break;
            case 3:
                printf("\n** 3. 查找位序 **\n");
                printf("请输入要查找的顶点的信息：");
                getchar();
                scanf("%c", &v);
                printf("该顶点位序为：%d\n", LocateVex(&G, v));
                break;
            case 4:
                printf("\n** 4. 取k顶点值 **\n");
                printf("请输入要取的顶点的位序：");
                scanf("%d", &k);
                GetVex(&G, k, &v);
                printf("位序为%d的顶点值为：%c\n", k, v);
                break;
            case 5:
                printf("\n** 5. 为k顶点赋值 **\n");
                printf("请输入赋值顶点的位序：");
                scanf("%d", &k);
                printf("请输入所要赋的值(char类型)：");
                getchar();
                scanf("%c", &v);
                printf("赋值前%d顶点值为：%c", k, G.vertices[k].data);
                PutVex(&G, k, v);
                printf("\n赋值后%d顶点值为：%c", k, v);
                PrintG(G);
                break;
            case 6:
                printf("\n** 6. 插入弧 **");
                PrintG(G);
                printf("请选择要插入的弧的端点的位序(弧尾->弧头，空格隔开)：");
                scanf("%d %d", &k, &m);
                if (AddArc(&G, k, m))
                    printf("插入成功！\n");
                else
                    printf("弧已存在，插入失败！\n");
                PrintG(G);
                break;
            case 7:
                printf("\n** 7. 删除弧 **");
                PrintG(G);
                printf("请选择要删除的弧的端点的位序(弧尾->弧头，空格隔开)：");
                scanf("%d %d", &k, &m);
                if (RemoveArc(&G, k, m))
                    printf("删除成功！\n");
                else
                    printf("弧不存在，删除失败！\n");
                PrintG(G);
                break;
            case 8:
                printf("\n** 8. 求第一个邻接顶点 **");
                PrintG(G);
                printf("请输入要求第一个邻接顶点的顶点位序：");
                scanf("%d", &k);
                printf("第一个邻接顶点的位序为：%d\n", FirstAdjVex(&G, k));
                break;
            case 9:
                printf("\n** 9. 求下一邻接顶点 **");
                PrintG(G);
                printf("求k相对于m的下一个邻接顶点的顶点位序(请输入k、m，空格隔开)：");
                scanf("%d %d", &k, &m);
                p = G.vertices[k].firstArc;
                while (p && p->adjvex != m)
                    p = p->nextArc;
                printf("%c的下一个邻接顶点的位序为：%d\n", G.vertices[p->adjvex].data, NextAdjVex(&G, k, p));
                break;
            case 10:
                printf("\n** 10. 深度优先遍历 **\n");
                printf("深度优先遍历序列：");
                DFSTraverse(G, Visit);
                printf("\n");
                break;
            case 11:
                printf("\n** 11. 广度优先遍历 **\n");
                printf("广度优先遍历序列：");
                BFSTraverse(G, Visit);
                printf("\n");
        }
        printf("\n1. 创建图；2. 销毁图；3. 查找位序；4. 取k顶点值；\n"
               "5. 为k顶点赋值；6. 插入弧；7. 删除弧；8. 求第一个邻接顶点；\n"
               "9. 求下一个邻接顶点；10. 深度优先遍历；11. 广度优先遍历；0. 退出\n"
               "请选择操作：");
        scanf("%d", &select);
    } while (select != 0);
    printf("\n**** 运行结束 ****");
    return 0;
}
