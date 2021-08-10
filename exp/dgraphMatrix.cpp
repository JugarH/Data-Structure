/*
*	有向图的邻接数组表示
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
typedef int Status;   //函数值类型，表示函数结果状态

// 链队列类型定义，用于遍历
typedef struct QNode {
    int data;
    struct QNode *next;
} QNode, *QueuePtr;
typedef struct {
    QueuePtr front;   //队头指针
    QueuePtr rear;    //队尾指针
} Queue;

typedef struct {
    VexType *vertices;    //顶点数组
    int **arcs;       //关系数组
    int n, e;         //顶点数和边数
    int *tags;        //标志数组，可用于在图的遍历中标记顶点访问与否
} DG;  //邻接数组类型，DG为有向图

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

// 打印邻接数组
void PrintG(DG G) {
    int i, j;
    printf("\n有向图的邻接数组：\n");
    for (i = 0; i < G.n; i++) {
        printf("%d %c    ", i, G.vertices[i]);
        for (j = 0; j < G.n; j++)
            printf("%d ", G.arcs[i][j]);
        printf("\n");
    }
}

// 查找顶点v在图G中的位序
int LocateVex(DG G, VexType v) {
    int i;
    for (i = 0; i < G.n; i++) {
        if (G.vertices[i] == v)
            return i;
    }
    return -1;
}

// 初始化含n个顶点且无边的有向图G的邻接数组存储结构
Status InitDG(DG *G, int n) {
    int i, j;
    G->n = n;
    G->e = 0;
    if (NULL == (G->vertices = (VexType *) malloc(n * sizeof(VexType))))
        return OVERFLOW;
    printf("\n");
    //建立顶点数组
    for (i = 0; i < G->n; i++) {
        getchar();
        printf("请输入位序为%d的顶点信息：", i);
        scanf("%c", &G->vertices[i]);
    }
    //为关系数组分配空间
    if (NULL == (G->arcs = (int **) malloc(n * sizeof(int *))))
        return OVERFLOW;
    for (i = 0; i < n; i++) {
        if (NULL == (G->arcs[i] = (int *) malloc(n * sizeof(int))))
            return OVERFLOW;
    }
    //为标志数组分配空间
    if (NULL == (G->tags = (int *) malloc(n * sizeof(int))))
        return OVERFLOW;
    //初始化标志数组和关系数组
    for (i = 0; i < n; i++) {
        G->tags[i] = UNVISITED;
        for (j = 0; j < n; j++)
            G->arcs[i][j] = 0;
    }
    return OK;
}

// 创建n个顶点和e条边的有向图G
Status CreatDG(DG *G) {
    int i, j, k, n, e;
    VexType head, tail; //head：弧头，tail：弧尾
    printf("请输入顶点数：");
    scanf("%d", &n);
    printf("请输入弧数：");
    scanf("%d", &e);
    if (n < 0 || e < 0) return ERROR;
    if (InitDG(G, n) != OK) return ERROR;
    G->e = e;
    printf("\n");
    for (k = 0; k < G->e; k++) {
        getchar();  //吸收回车，接收字符
        printf("请输入弧的信息(弧尾->弧头，英文逗号/空格间隔)：");
        scanf("%c%*c%c", &tail, &head);
        i = LocateVex(*G, tail);
        j = LocateVex(*G, head); //确定head和tail的位序i和j
        if (i < 0 || j < 0) return ERROR;    //顶点v或w不存在
        G->arcs[i][j] = 1;
    }
    PrintG(*G);
    return OK;
}

// 销毁图G
Status DestroyDG(DG *G) {
    int i;
    if (G) {
        free(G->vertices);
        for (i = 0; i < G->n; i++) {
            free(G->arcs[i]);
        }
        free(G->arcs);
    }
    G->n = 0;
    G->e = 0;
    return OK;
}

// 取图G的k顶点的值到w
Status GetVex(DG *G, int k, VexType *w) {
    if (k < 0 || k >= G->n)
        return ERROR;    //k顶点不存在
    *w = G->vertices[k];
    return OK;
}

// 对图G的k顶点赋值w
Status PutVex(DG *G, int k, VexType w) {
    if (k < 0 || k >= G->n)
        return ERROR;    //k顶点不存在
    G->vertices[k] = w;
    return OK;
}

// 求k顶点的第一个邻接顶点的位序
int FirstAdjVex(DG *G, int k) {
    int j;
    for (j = 0; j < G->n; j++) {
        if (G->arcs[k][j] == 1)
            return j;
    }
    return -1;
}

// 求图G中k顶点相对于m顶点的下一个邻接顶点的位序
int NextAdjVex(DG *G, int k, int m) {
    int i;
    if (k == 0 && m == 0) return 0;
    for (i = m + 1; i < G->n; i++)
        if (G->arcs[k][i]) return i;
    return -1;
}

// 插入顶点k到m的弧(邻接链表的表头插入)
Status AddArc(DG *G, int k, int m) {
    if (k < 0 || m < 0 || k == m) return ERROR;
    if (G->arcs[k][m]) return ERROR;  //弧已存在
    else {
        G->arcs[k][m] = 1;
        G->e++;     //插入，置1，弧数加一
        PrintG(*G);
        return OK;
    }
}

// 删除顶点k到m的弧
Status RemoveArc(DG *G, int k, int m) {
    if (G->n == 0)    //G中无弧，返回
        return ERROR;
    if (G->arcs[k][m]) {
        G->arcs[k][m] = 0;
        G->e--;     //删除，置0，弧数减一
        PrintG(*G);
        return OK;
    }
    return ERROR;
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
    visit(G.vertices[k]);    //输出顶点值
    for (i = FirstAdjVex(&G, k); i >= 0; i = NextAdjVex(&G, k, i)) {
        if (UNVISITED == G.tags[i]) //未遍历该顶点
            if (ERROR == DFS(G, i, visit))
                return ERROR;
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
    int i, j;
    Queue Q;
    InitQueue(&Q);
    for (i = 0; i < G.n; i++) G.tags[i] = UNVISITED;
    for (i = 0; i < G.n; i++) { //对每一个顶点循环
        if (UNVISITED == G.tags[i]) { //当前顶点未被访问
            G.tags[i] = VISITED;
            if (ERROR == visit(G.vertices[i])) return ERROR;
            EnQueue(&Q, i); //当前顶点入队列
            while (!QueueEmpty(Q)) { //队列不为空
                DeQueue(&Q, &i);
                for (j = 0; j < G.n; j++) {
                    if (UNVISITED == G.tags[j] && G.arcs[i][j] == 1) {
                        if (ERROR == visit(G.vertices[j])) return ERROR;
                        G.tags[j] = VISITED;    //将找到的顶点标记为已访问
                        EnQueue(&Q, j);  //将找到的顶点入队列
                    }
                }
            }
        }
    }
    return OK;
}

int main() {
//    printf("-------------- 设计性实验（有向图）-------------\n\n");
//    printf("|                 一、邻接数组                 |\n\n");
//    printf("|          班级：XXXXXXXXXXXXXXXXX       |\n\n");
//    printf("|                学号：XXXXXXXXXX             |\n\n");
//    printf("|                 姓名：XXX                 |\n\n");
//    printf("------------------------------------------------\n\n");
    DG G;
    int select, k, m;
    VexType v;
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
                printf("该顶点位序为：%d\n", LocateVex(G, v));
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
                printf("赋值前%d顶点值为：%c", k, G.vertices[k]);
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
                printf("求k相对于m的下一个邻接顶点的顶点位序(输入k、m，空格隔开)：");
                scanf("%d %d", &k, &m);
                printf("%c相对于%c的下一个邻接顶点的位序为：%d\n", G.vertices[k], G.vertices[m], NextAdjVex(&G, k, m));
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
