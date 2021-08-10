/*
* ��ѧ�ƻ����� 
*/
#include<stdio.h>
#include<malloc.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1
#define MAX_VERTEX_NUM 100    //���γ�����
#define STACK_INIT_SIZE 100 //�洢�ռ�ĳ�ʼ������
#define STACK_INCREMENT 10    //�洢�ռ�ķ�������
#define PATH "teachingPlan.txt"
#define OUTPATH "outputPlan.txt"
typedef int Status;        //����ֵ���ͣ���ʾ�������״̬
typedef int ElemType;
int numTerm;    //ѧ������ 
float upCredit;    //һѧ��ѧ������
//ͼ���ڽӱ�洢��ʾ	 
typedef char VexType; //�������� 
typedef struct ArcNode {
    int adjvex;              //�ڽӶ����ڶ��������е�λ�� 
    struct ArcNode *nextArc;  //ָ����һ���ڽӶ����ָ��
} ArcNode, *ArcNodeP;
typedef struct VexNode {
    int classId;        //�γ̱��
    char name[20];        //�γ����� 
    float credit;            //ѧ��
    int inDegree;        //�ý������
    int learned;        //1Ϊ��ѧ��0Ϊδѧ 
    ArcNode *firstArc;    //�ڽ������ͷָ�� 
} VexNode, AdjList[MAX_VERTEX_NUM];
typedef struct {
    AdjList vexs;    //�������飬���ڴ��涥����Ϣ 
    int n, e;        //ͼ�ĵ�ǰ�������ͻ���
} ALGraph;            //�ڽӱ����� 
//ջ��˳��洢��ʾ 
typedef struct {
    ElemType *elem;
    ElemType *top;
    int size;
    int increment;
} SqStack;

/*  ���ܺ���  */
//��ʼ��ջ
Status InitStack(SqStack *S) {
    S->elem = (ElemType *) malloc(STACK_INIT_SIZE * sizeof(ElemType));
    if (!S->elem)
        return OVERFLOW;
    S->top = S->elem;
    S->size = STACK_INIT_SIZE;
    S->increment = STACK_INCREMENT;
    return OK;
}

//�ж�ջ��
Status StackEmpty(SqStack S) {
    if (S.top == S.elem)
        return TRUE;
    else
        return FALSE;
}

//��ջ
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

//��ջ   
Status Pop(SqStack *S, ElemType *e) {
    if (S->top == S->elem)
        return ERROR;
    *e = *(--S->top);
}

//����ͼ(�ļ�����) 
Status CreateGraph(ALGraph *G) {
    ArcNode *p;
    int i, j, k;
    FILE *fp;
    char term[10], course[10], credit[16], firstNum[18];
    if (NULL == (fp = fopen(PATH, "r"))) {
        printf("\n�Ҳ����ļ���");
        return ERROR;
    }
    fscanf(fp, "%s%d", term, &numTerm);
    printf("\n%s��%d", term, numTerm);
    fscanf(fp, "%s%d", course, &G->n);
    printf("\n%s��%d", course, G->n);
    fscanf(fp, "\n%s%f", credit, &upCredit);
    printf("\n%s��%.1f\n", credit, upCredit);
    for (i = 1; i <= G->n; i++) {
        printf("\n\n�γ̱�ţ�L%d", i);
        G->vexs[i].classId = i;
        fscanf(fp, "%s", &G->vexs[i].name);
        printf("\n������γ����ƣ�%s", G->vexs[i].name);
        fscanf(fp, "%f", &G->vexs[i].credit);
        printf("\n������ÿγ̵�ѧ�֣�%.1f", G->vexs[i].credit);
        G->vexs[i].inDegree = 0;
        G->vexs[i].learned = 0;
        G->vexs[i].firstArc = NULL;
    }
    fscanf(fp, "%s%d", firstNum, &G->e);
    printf("\n\n%s��%d", firstNum, G->e);
    printf("\nÿ���γ̵����޹�ϵ(���޿γ���ǰ)\n");
    for (i = 1; i <= G->e; i++) {
        fscanf(fp, "%d%d", &j, &k);
        printf("%02d. �������޹�ϵ�������γ̵ı�ţ�L%d,L%d\n", i, j, k);
        p = (ArcNode *) malloc(sizeof(ArcNode));
        if (!p)
            return OVERFLOW;
        p->adjvex = k;
        p->nextArc = G->vexs[j].firstArc;
        G->vexs[j].firstArc = p; //���ڱ�ͷ 
    }
    fclose(fp);
    return OK;
}

// ��ӡ�ڽӱ�
void PrintG(ALGraph G) {
    int i;
    ArcNodeP p;
    printf("\n����ͼ��Ϣ���£�\n(1)%d�����㣺\n", G.n);
    printf("�γ̱��\t�γ�����\t\t�γ�ѧ��\n");
    for (i = 1; i <= G.n; i++)
        printf("L%-4d\t\t%-18s\t%-4.1fѧ��\n", G.vexs[i].classId, G.vexs[i].name, G.vexs[i].credit);
    printf("(2)%d������\n", G.e);
    for (i = 1; i <= G.n; i++) {
        printf("L%d:->", G.vexs[i].classId);
        for (p = G.vexs[i].firstArc; p; p = p->nextArc)
            printf("L%d->", p->adjvex);
        printf("NULL\n");
    }
}

//�󶥵����
void FindInDegree(ALGraph G, int *inDegree) {
    int i;
    ArcNode *p;
    for (i = 1; i <= G.n; i++) {
        inDegree[i] = 0;
    }
    for (i = 1; i <= G.n; i++) {
        p = G.vexs[i].firstArc;
        while (p) {
            inDegree[p->adjvex]++; //���ڽӶ��㣬��ȼ�1 
            p = p->nextArc;
        }
    }
    free(p);
}

//��������ʹ�γ̾���ƽ���� 
void TopologicalSort_avg(ALGraph G) {
	FILE *fp;
	if (NULL == (fp = fopen(OUTPATH, "a"))) {
        printf("\n�Ҳ����ļ���");
        return;
    } 
    int i, j, k, sumClass, max;
    j = 1;
    sumClass = 0; //�γ���Ŀ�ۼ� 
    max = G.n / numTerm + 1; //ʹ�γ̾���ƽ������ÿѧ�ڿγ����Ϊ�γ�����/ѧ����(����ȡ��) 
    int *inDegree;
    inDegree = (int *) malloc(G.n * sizeof(int));
    int count = 0; //�γ̱�����Ŀ
    float sumCredit = 0; //ÿѧ��ѧ���ۼ� 
    ArcNode *p;
    SqStack S;
    FindInDegree(G, inDegree);    //����������
    for (i = 1; i <= G.n; i++) {
        G.vexs[i].inDegree = inDegree[i];
    }
    InitStack(&S);    //��ʼ��ջ 
    printf("\n************ �γ̱��ŷ�������(����ƽ��) ************\n");
    fprintf(fp, "\n************ �γ̱��ŷ�������(����ƽ��) ************\n");
    while (j <= numTerm) {
        sumCredit = 0;
        sumClass = 0;
        for (i = 1; i <= G.n; i++) {
            if ((!G.vexs[i].inDegree) && (!G.vexs[i].learned)) {
                //�����Ϊ0�Ķ�����ջ
                Push(&S, i);
                G.vexs[i].learned = 1;
            }
        }
        printf("\n��%d��ѧ�ڵĿγ��У�", j);
        fprintf(fp, "\n��%d��ѧ�ڵĿγ��У�", j);
        while ((!StackEmpty(S)) && (sumCredit <= upCredit) && (sumClass <= max)) {
            //ջ��Ϊ�գ���ǰѧ��ѧ�ֲ�����һѧ��ѧ�����ޣ���ǰѧ�ڿγ���������ÿѧ�ڿγ������ 
            Pop(&S, &k); //ջ���γ̽���ջ 
            sumCredit += G.vexs[k].credit; //��ѧ��ѧ���ۼ� 
            sumClass++; //��ѧ�ڿγ���Ŀ�ۼ� 
            if (sumCredit <= upCredit && sumClass <= max) { //����ѧ�ڵ�ǰѧ���Բ�����ѧ��ѧ�������ҵ�ǰѧ�ڿγ���������ÿѧ�ڿγ������  
                count++;
                printf("%s  ", G.vexs[k].name); //���Ÿÿγ̣����ÿγ���� 
                fprintf(fp, "%s  ", G.vexs[k].name); 
                for (p = G.vexs[k].firstArc; p; p = p->nextArc) {
                    if (((--G.vexs[p->adjvex].inDegree) == 0) && (G.vexs[p->adjvex].learned == 0)) {
                        //ɾȥ�˴�ѭ������Ķ�����ڽӱߣ������µ�����ȵĶ�����ջ
                        Push(&S, p->adjvex);
                        G.vexs[p->adjvex].learned = 1;
                    }
                }
            } else
                Push(&S, k);//ѧ�ִ���ÿѧ�ڵ�ѧ�����ޣ�����ѹ��ջ 
        }
        j++; //��jѧ�� 
    }
    printf("\n");
    free(inDegree);
    free(p);
    fprintf(fp, "\n");
    fclose(fp);
    if (count < G.n)
        printf("\n�γ̱��ŷ��������������룡\n");
    else
        printf("\nDone���γ̱��ųɹ���\n");
}

//��������ʹ�γ̾���������ǰ��ѧ�ڣ� 
void TopologicalSort_front(ALGraph G) {
	FILE *fp;
	if (NULL == (fp = fopen(OUTPATH, "a"))) {
        printf("\n�Ҳ����ļ���");
        return;
    } 
    int i, j, k, sumClass;
    j = 1;
    int *inDegree;
    inDegree = (int *) malloc(G.n * sizeof(int));
    int count = 0; //�γ̱�����Ŀ
    float sumCredit = 0; //ÿѧ��ѧ���ۼ� 
    ArcNode *p;
    SqStack S;
    FindInDegree(G, inDegree);    //����������
    for (i = 1; i <= G.n; i++) {
        G.vexs[i].inDegree = inDegree[i];
    }
    InitStack(&S);    //��ʼ��ջ 
    printf("\n************ �γ̱��ŷ�������(����ǰ��ѧ��) ************\n");
    fprintf(fp, "\n************ �γ̱��ŷ�������(����ǰ��ѧ��) ************\n");
    while (j <= numTerm) {
        sumCredit = 0;
        for (i = 1; i <= G.n; i++) {
            if ((!G.vexs[i].inDegree) && (!G.vexs[i].learned)) {
                //�����Ϊ0�Ķ�����ջ
                Push(&S, i);
                G.vexs[i].learned = 1;
            }
        }
        printf("\n��%d��ѧ�ڵĿγ��У�", j);
        fprintf(fp, "\n��%d��ѧ�ڵĿγ��У�", j);
        while ((!StackEmpty(S)) && (sumCredit <= upCredit)) {
            //ջ��Ϊ�գ���ǰѧ��ѧ�ֲ�����һѧ��ѧ������
            Pop(&S, &k); //ջ���γ̽���ջ 
            sumCredit += G.vexs[k].credit; //��ѧ��ѧ���ۼ� 
            if (sumCredit <= upCredit) { //����ѧ�ڵ�ǰѧ���Բ�����ѧ��ѧ������ 
                count++;
                printf("%s  ", G.vexs[k].name); //���Ÿÿγ̣����ÿγ���� 
                fprintf(fp, "%s  ", G.vexs[k].name); 
                for (p = G.vexs[k].firstArc; p; p = p->nextArc) {
                    if (((--G.vexs[p->adjvex].inDegree) == 0) && (G.vexs[p->adjvex].learned == 0)) {
                        //ɾȥ�˴�ѭ������Ķ�����ڽӱߣ������µ�����ȵĶ�����ջ
                        Push(&S, p->adjvex);
                        G.vexs[p->adjvex].learned = 1;
                    }
                }
            } else
                Push(&S, k);//ѧ�ִ���ÿѧ�ڵ�ѧ�����ޣ�����ѹ��ջ 
        }
        j++; //��jѧ�� 
    }
    printf("\n");
    free(inDegree);
    free(p);
    fprintf(fp, "\n");
    fclose(fp);
    if (count < G.n)
        printf("\n�γ̱��ŷ��������������룡\n");
    else
        printf("\nDone���γ̱��ųɹ���\n");
}

int main() {
    printf("-----------------���ݽṹ�γ����-----------------\n\n");
    printf("|                  ��ѧ�ƻ�����                  |\n\n");
    printf("|          �༶���������ѧ�뼼��19(1)��         |\n\n");
    printf("|                ѧ�ţ�3119004733                |\n\n");
    printf("|                  �����������                  |\n\n");
    printf("--------------------------------------------------\n\n");
    printf("************ ��ӭʹ�ý�ѧ�ƻ�����ϵͳ ************\n\n");
    int sortWay;    //���ŷ�ʽ 
    int again;
    ALGraph G;
    CreateGraph(&G);
    PrintG(G);
    printf("\n���� ��0�� ���������� 0 �˳�\n");
    scanf("%d", &again);
    while (again) {
        printf("\n��ѡ����ŷ�ʽ��1. �γ̾���ƽ����2. �γ̾���������ǰ��ѧ��\n");
        scanf("%d", &sortWay);
        while (sortWay != 1 && sortWay != 2) {
            printf("��������������ѡ����ŷ�ʽ��1. �γ̾���ƽ����2. �γ̾���������ǰ��ѧ��\n");
            scanf("%d", &sortWay);
        }
        if (sortWay == 1)
            TopologicalSort_avg(G);
        else if (sortWay == 2)
            TopologicalSort_front(G);
        printf("\n���� ��0�� ���������� 0 �˳�\n");
        scanf("%d", &again);
    }
    printf("\n���˳���");
    return 0;
}
