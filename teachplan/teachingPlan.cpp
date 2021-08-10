/*
* 教学计划编制 
*/
#include<stdio.h>
#include<malloc.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1
#define MAX_VERTEX_NUM 100    //最大课程总数
#define STACK_INIT_SIZE 100 //存储空间的初始分配量
#define STACK_INCREMENT 10    //存储空间的分配增量
#define PATH "teachingPlan.txt"
#define OUTPATH "outputPlan.txt"
typedef int Status;        //函数值类型，表示函数结果状态
typedef int ElemType;
int numTerm;    //学期总数 
float upCredit;    //一学期学分上限
//图的邻接表存储表示	 
typedef char VexType; //顶点类型 
typedef struct ArcNode {
    int adjvex;              //邻接顶点在顶点数组中的位序 
    struct ArcNode *nextArc;  //指向下一个邻接顶点的指针
} ArcNode, *ArcNodeP;
typedef struct VexNode {
    int classId;        //课程编号
    char name[20];        //课程名称 
    float credit;            //学分
    int inDegree;        //该结点的入度
    int learned;        //1为已学，0为未学 
    ArcNode *firstArc;    //邻接链表的头指针 
} VexNode, AdjList[MAX_VERTEX_NUM];
typedef struct {
    AdjList vexs;    //顶点数组，用于储存顶点信息 
    int n, e;        //图的当前顶点数和弧数
} ALGraph;            //邻接表类型 
//栈的顺序存储表示 
typedef struct {
    ElemType *elem;
    ElemType *top;
    int size;
    int increment;
} SqStack;

/*  功能函数  */
//初始化栈
Status InitStack(SqStack *S) {
    S->elem = (ElemType *) malloc(STACK_INIT_SIZE * sizeof(ElemType));
    if (!S->elem)
        return OVERFLOW;
    S->top = S->elem;
    S->size = STACK_INIT_SIZE;
    S->increment = STACK_INCREMENT;
    return OK;
}

//判断栈空
Status StackEmpty(SqStack S) {
    if (S.top == S.elem)
        return TRUE;
    else
        return FALSE;
}

//进栈
Status Push(SqStack *S, ElemType e) {
    if ((S->top) - (S->elem) >= S->size) {
        S->elem = (ElemType *) realloc(S->elem, (S->size + S->increment) * sizeof(ElemType));
        if (!S->elem)
            return OVERFLOW;
        S->top = S->elem + S->size;
        S->size += S->increment;
    }
    *(S->top++) = e;
}

//出栈   
Status Pop(SqStack *S, ElemType *e) {
    if (S->top == S->elem)
        return ERROR;
    *e = *(--S->top);
}

//创建图(文件导入) 
Status CreateGraph(ALGraph *G) {
    ArcNode *p;
    int i, j, k;
    FILE *fp;
    char term[10], course[10], credit[16], firstNum[18];
    if (NULL == (fp = fopen(PATH, "r"))) {
        printf("\n找不到文件！");
        return ERROR;
    }
    fscanf(fp, "%s%d", term, &numTerm);
    printf("\n%s：%d", term, numTerm);
    fscanf(fp, "%s%d", course, &G->n);
    printf("\n%s：%d", course, G->n);
    fscanf(fp, "\n%s%f", credit, &upCredit);
    printf("\n%s：%.1f\n", credit, upCredit);
    for (i = 1; i <= G->n; i++) {
        printf("\n\n课程编号：L%d", i);
        G->vexs[i].classId = i;
        fscanf(fp, "%s", &G->vexs[i].name);
        printf("\n请输入课程名称：%s", G->vexs[i].name);
        fscanf(fp, "%f", &G->vexs[i].credit);
        printf("\n请输入该课程的学分：%.1f", G->vexs[i].credit);
        G->vexs[i].inDegree = 0;
        G->vexs[i].learned = 0;
        G->vexs[i].firstArc = NULL;
    }
    fscanf(fp, "%s%d", firstNum, &G->e);
    printf("\n\n%s：%d", firstNum, G->e);
    printf("\n每个课程的先修关系(先修课程在前)\n");
    for (i = 1; i <= G->e; i++) {
        fscanf(fp, "%d%d", &j, &k);
        printf("%02d. 存在先修关系的两个课程的编号：L%d,L%d\n", i, j, k);
        p = (ArcNode *) malloc(sizeof(ArcNode));
        if (!p)
            return OVERFLOW;
        p->adjvex = k;
        p->nextArc = G->vexs[j].firstArc;
        G->vexs[j].firstArc = p; //插在表头 
    }
    fclose(fp);
    return OK;
}

// 打印邻接表
void PrintG(ALGraph G) {
    int i;
    ArcNodeP p;
    printf("\n创建图信息如下：\n(1)%d个顶点：\n", G.n);
    printf("课程编号\t课程名称\t\t课程学分\n");
    for (i = 1; i <= G.n; i++)
        printf("L%-4d\t\t%-18s\t%-4.1f学分\n", G.vexs[i].classId, G.vexs[i].name, G.vexs[i].credit);
    printf("(2)%d条弧：\n", G.e);
    for (i = 1; i <= G.n; i++) {
        printf("L%d:->", G.vexs[i].classId);
        for (p = G.vexs[i].firstArc; p; p = p->nextArc)
            printf("L%d->", p->adjvex);
        printf("NULL\n");
    }
}

//求顶点入度
void FindInDegree(ALGraph G, int *inDegree) {
    int i;
    ArcNode *p;
    for (i = 1; i <= G.n; i++) {
        inDegree[i] = 0;
    }
    for (i = 1; i <= G.n; i++) {
        p = G.vexs[i].firstArc;
        while (p) {
            inDegree[p->adjvex]++; //有邻接顶点，入度加1 
            p = p->nextArc;
        }
    }
    free(p);
}

//拓扑排序（使课程尽量平均） 
void TopologicalSort_avg(ALGraph G) {
	FILE *fp;
	if (NULL == (fp = fopen(OUTPATH, "a"))) {
        printf("\n找不到文件！");
        return;
    } 
    int i, j, k, sumClass, max;
    j = 1;
    sumClass = 0; //课程数目累加 
    max = G.n / numTerm + 1; //使课程尽量平均，令每学期课程最多为课程总数/学期数(向上取整) 
    int *inDegree;
    inDegree = (int *) malloc(G.n * sizeof(int));
    int count = 0; //课程编排数目
    float sumCredit = 0; //每学期学分累加 
    ArcNode *p;
    SqStack S;
    FindInDegree(G, inDegree);    //各结点求入度
    for (i = 1; i <= G.n; i++) {
        G.vexs[i].inDegree = inDegree[i];
    }
    InitStack(&S);    //初始化栈 
    printf("\n************ 课程编排方案如下(尽量平均) ************\n");
    fprintf(fp, "\n************ 课程编排方案如下(尽量平均) ************\n");
    while (j <= numTerm) {
        sumCredit = 0;
        sumClass = 0;
        for (i = 1; i <= G.n; i++) {
            if ((!G.vexs[i].inDegree) && (!G.vexs[i].learned)) {
                //将入度为0的顶点入栈
                Push(&S, i);
                G.vexs[i].learned = 1;
            }
        }
        printf("\n第%d个学期的课程有：", j);
        fprintf(fp, "\n第%d个学期的课程有：", j);
        while ((!StackEmpty(S)) && (sumCredit <= upCredit) && (sumClass <= max)) {
            //栈不为空，当前学期学分不大于一学期学分上限，当前学期课程数不大于每学期课程最大数 
            Pop(&S, &k); //栈顶课程结点出栈 
            sumCredit += G.vexs[k].credit; //该学期学分累加 
            sumClass++; //该学期课程数目累加 
            if (sumCredit <= upCredit && sumClass <= max) { //若该学期当前学分仍不大于学期学分上限且当前学期课程数不大于每学期课程最大数  
                count++;
                printf("%s  ", G.vexs[k].name); //编排该课程，将该课程输出 
                fprintf(fp, "%s  ", G.vexs[k].name); 
                for (p = G.vexs[k].firstArc; p; p = p->nextArc) {
                    if (((--G.vexs[p->adjvex].inDegree) == 0) && (G.vexs[p->adjvex].learned == 0)) {
                        //删去此次循环输出的顶点的邻接边，并将新的零入度的顶点入栈
                        Push(&S, p->adjvex);
                        G.vexs[p->adjvex].learned = 1;
                    }
                }
            } else
                Push(&S, k);//学分大于每学期的学分上限，重新压入栈 
        }
        j++; //第j学期 
    }
    printf("\n");
    free(inDegree);
    free(p);
    fprintf(fp, "\n");
    fclose(fp);
    if (count < G.n)
        printf("\n课程编排发生错误，请检查输入！\n");
    else
        printf("\nDone！课程编排成功！\n");
}

//拓扑排序（使课程尽量集中在前几学期） 
void TopologicalSort_front(ALGraph G) {
	FILE *fp;
	if (NULL == (fp = fopen(OUTPATH, "a"))) {
        printf("\n找不到文件！");
        return;
    } 
    int i, j, k, sumClass;
    j = 1;
    int *inDegree;
    inDegree = (int *) malloc(G.n * sizeof(int));
    int count = 0; //课程编排数目
    float sumCredit = 0; //每学期学分累加 
    ArcNode *p;
    SqStack S;
    FindInDegree(G, inDegree);    //各结点求入度
    for (i = 1; i <= G.n; i++) {
        G.vexs[i].inDegree = inDegree[i];
    }
    InitStack(&S);    //初始化栈 
    printf("\n************ 课程编排方案如下(尽量前几学期) ************\n");
    fprintf(fp, "\n************ 课程编排方案如下(尽量前几学期) ************\n");
    while (j <= numTerm) {
        sumCredit = 0;
        for (i = 1; i <= G.n; i++) {
            if ((!G.vexs[i].inDegree) && (!G.vexs[i].learned)) {
                //将入度为0的顶点入栈
                Push(&S, i);
                G.vexs[i].learned = 1;
            }
        }
        printf("\n第%d个学期的课程有：", j);
        fprintf(fp, "\n第%d个学期的课程有：", j);
        while ((!StackEmpty(S)) && (sumCredit <= upCredit)) {
            //栈不为空，当前学期学分不大于一学期学分上限
            Pop(&S, &k); //栈顶课程结点出栈 
            sumCredit += G.vexs[k].credit; //该学期学分累加 
            if (sumCredit <= upCredit) { //若该学期当前学分仍不大于学期学分上限 
                count++;
                printf("%s  ", G.vexs[k].name); //编排该课程，将该课程输出 
                fprintf(fp, "%s  ", G.vexs[k].name); 
                for (p = G.vexs[k].firstArc; p; p = p->nextArc) {
                    if (((--G.vexs[p->adjvex].inDegree) == 0) && (G.vexs[p->adjvex].learned == 0)) {
                        //删去此次循环输出的顶点的邻接边，并将新的零入度的顶点入栈
                        Push(&S, p->adjvex);
                        G.vexs[p->adjvex].learned = 1;
                    }
                }
            } else
                Push(&S, k);//学分大于每学期的学分上限，重新压入栈 
        }
        j++; //第j学期 
    }
    printf("\n");
    free(inDegree);
    free(p);
    fprintf(fp, "\n");
    fclose(fp);
    if (count < G.n)
        printf("\n课程编排发生错误，请检查输入！\n");
    else
        printf("\nDone！课程编排成功！\n");
}

int main() {
    printf("-----------------数据结构课程设计-----------------\n\n");
    printf("|                  教学计划编制                  |\n\n");
    printf("|          班级：计算机科学与技术19(1)班         |\n\n");
    printf("|                学号：3119004733                |\n\n");
    printf("|                  姓名：黄祖佳                  |\n\n");
    printf("--------------------------------------------------\n\n");
    printf("************ 欢迎使用教学计划编制系统 ************\n\n");
    int sortWay;    //编排方式 
    int again;
    ALGraph G;
    CreateGraph(&G);
    PrintG(G);
    printf("\n输入 非0数 继续，输入 0 退出\n");
    scanf("%d", &again);
    while (again) {
        printf("\n请选择编排方式：1. 课程尽量平均；2. 课程尽量集中在前几学期\n");
        scanf("%d", &sortWay);
        while (sortWay != 1 && sortWay != 2) {
            printf("输入有误！请重新选择编排方式：1. 课程尽量平均；2. 课程尽量集中在前几学期\n");
            scanf("%d", &sortWay);
        }
        if (sortWay == 1)
            TopologicalSort_avg(G);
        else if (sortWay == 2)
            TopologicalSort_front(G);
        printf("\n输入 非0数 继续，输入 0 退出\n");
        scanf("%d", &again);
    }
    printf("\n已退出！");
    return 0;
}
