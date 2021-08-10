/*
*	����ͼ���ڽӱ��ʾ
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
typedef char VexType; //��������
typedef int Status; //����ֵ���ͣ���ʾ�������״̬

// ���������Ͷ��壬���ڱ���
typedef struct QNode {
    int data;
    struct QNode *next;
} QNode, *QueuePtr;
typedef struct {
    QueuePtr front;    //��ͷָ��
    QueuePtr rear;    //��βָ��
} Queue;

// �ڽӱ����Ͷ���
typedef struct ArcNode {
    int adjvex; //�ڽӶ����ڶ��������е�λ��
    struct ArcNode *nextArc; //ָ����һ���ڽӶ����ָ��
} ArcNode, *ArcNodeP;         //�ڽ�����Ľ������
typedef struct VexNode {
    VexType data; //����ֵ
    struct ArcNode *firstArc; //�ڽ������ͷָ��
} VexNode;    //���������Ԫ������
typedef struct {
    VexNode *vertices; //�������飬���ڴ��涥����Ϣ
    int n, e;  //ͼ�ĵ�ǰ�������ͻ���
    int *tags;  //��־���飬��������ͼ�ı����б�Ƕ���������
} DG;        //�ڽӱ�����

// ��ʼ��������
void InitQueue(Queue *Q) {
    Q->front = Q->rear = (QueuePtr) malloc(sizeof(QNode));
    if (!Q->front) return;
    Q->front->next = NULL;
}

// �������п�
Status QueueEmpty(Queue Q) {
    if (Q.front == Q.rear)
        return TRUE;
    return FALSE;
}

// Ԫ��e���
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

//����
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

// ��ӡ�ڽӱ� 
void PrintG(DG G) {
    int i, j;
    ArcNodeP p;
    p = (ArcNodeP) malloc(sizeof(ArcNode));
    printf("\n����ͼ���ڽӱ�\n");
    for (i = 0; i < G.n; i++) {
        printf("%d %c:->", i, G.vertices[i].data);
        for (p = G.vertices[i].firstArc; p; p = p->nextArc)
            printf("%c->", G.vertices[p->adjvex].data);
        printf("NULL\n");
    }
}

// ���Ҷ���v��ͼG�е�λ��
int LocateVex(DG *G, VexType v) {
    int i;
    for (i = 0; i < G->n; i++) {
        if (G->vertices[i].data == v)
            return i;
    }
    return -1;
}

// ����n�������e���ߵ�ͼG
Status CreatDG(DG *G) {
    int i, j, k, n, e;
    VexType head, tail;
    ArcNode *p;
    printf("�����붥������");
    scanf("%d", &n);
    printf("�����뻡����");
    scanf("%d", &e);
    if (n < 0 || e < 0) return ERROR;
    G->n = n;
    G->e = e;
    G->vertices = (VexNode *) malloc(n * sizeof(VexNode));
    G->tags = (int *) malloc(n * sizeof(int));
    printf("\n");
    for (i = 0; i < G->n; i++) {  //��ʼ����־���飬������������
        getchar();
        G->tags[i] = UNVISITED;
        printf("������λ��Ϊ%d�Ķ�����Ϣ��", i);
        scanf("%c", &G->vertices[i].data);
        G->vertices[i].firstArc = NULL;
    }
    printf("\n");
    for (k = 0; k < G->e; k++) {
        getchar();
        printf("�����뻡����Ϣ(��β->��ͷ��Ӣ�Ķ���/�ո���)��");
        scanf("%c%*c%c", &tail, &head);
        i = LocateVex(G, tail);
        j = LocateVex(G, head); //ȷ��head��tail��λ��i��j
        if (i < 0 || j < 0) return ERROR;    //����v��w������
        p = (ArcNode *) malloc(sizeof(ArcNode)); //Ϊ����w����p���
        p->adjvex = j;
        p->nextArc = G->vertices[i].firstArc;
        G->vertices[i].firstArc = p;
    }
    PrintG(*G);
    return OK;
}

// ����ͼG
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

// ȡͼG��k�����ֵ��w
Status GetVex(DG *G, int k, VexType *w) {
    if (k < 0 || k >= G->n)
        return ERROR;    //k���㲻����
    *w = G->vertices[k].data;
    return OK;
}

// ��ͼG��k���㸳ֵw
Status PutVex(DG *G, int k, VexType w) {
    if (k < 0 || k >= G->n)
        return ERROR;    //k���㲻����
    G->vertices[k].data = w;
    return OK;
}

// ���һ���ڽӶ���
int FirstAdjVex(DG *G, int k) {
    if (k < 0 || k >= G->n)
        return -1;        //k���㲻����
    ArcNodeP p;
    p = G->vertices[k].firstArc;    //pָ��k�����Ӧ�ڽ�����ĵ�һ�����
    if (p)
        return p->adjvex; //���ص�һ�����洢�Ķ���λ��
    else
        return -1;    //k�������ڽӶ���
}

// ����һ���ڽӶ���
int NextAdjVex(DG *G, int k, ArcNodeP p) {
    if (k < 0 || k >= G->n)
        return -1;        //k���㲻����
    if (!p)
        return -1;        //ָ��pΪ��
    p = p->nextArc;        //pָ����һ�����
    if (p)
        return p->adjvex;    //����p���洢����һ���ڽӶ����λ��
    else
        return -1;        //k��������һ���ڽӶ���
}

// ���붥��k��m�Ļ�(�ڽ�����ı�ͷ����)
Status AddArc(DG *G, int k, int m) {
    ArcNodeP p;
    if (k < 0 || k >= G->n || m < 0 || m >= G->n)
        return ERROR;
    p = G->vertices[k].firstArc;
    while (p) {    //�жϻ��Ƿ����
        if (m == p->adjvex)
            return ERROR;    //�����ڣ�����
        p = p->nextArc;
    }
    p = (ArcNode *) malloc(sizeof(ArcNode));    //Ϊm�������p���
    if (NULL == p)
        return OVERFLOW;
    p->adjvex = m;
    p->nextArc = G->vertices[k].firstArc;    //��k������ڽ�����ı�ͷ����p���
    G->vertices[k].firstArc = p;
    G->e++;
    return OK;
}

// ɾ������k��m�Ļ�
Status RemoveArc(DG *G, int k, int m) {
    if (G->n == 0)    //G���ޱߣ�����
        return ERROR;
    ArcNodeP p, pre;
    p = G->vertices[k].firstArc;    //pָ���ڽ������ͷ���
    pre = NULL;
    while (p) {
        if (m == p->adjvex) {        //��mΪ�ý�����ڽ������е�λ��
            if (!pre)    //����mΪͷ���
                G->vertices[k].firstArc = p->nextArc;
            else    //����m��ͷ���
                pre->nextArc = p->nextArc;
            free(p);
            G->e--;    //������1
            break;    //ɾ�����˳�ѭ��
        }
        pre = p;
        p = p->nextArc;
        if (!p)    //��k��m�ı߻򻡣�����
            return ERROR;
    }
}

//Visit����
Status Visit(VexType v) {
    printf("%c ", v);
    return OK;
}

// ������ȱ����ݹ��㷨 
Status DFS(DG G, int k, Status(*visit)(VexType)) {
    int i;
    G.tags[k] = VISITED;
    visit(G.vertices[k].data); //�������ֵ
    ArcNodeP p;
    p = (ArcNodeP) malloc(sizeof(ArcNode));
    p = G.vertices[k].firstArc;
    while (p) {
        if (UNVISITED == G.tags[p->adjvex]) //δ�����ö���
            if (ERROR == DFS(G, p->adjvex, visit))
                return ERROR;
        p = p->nextArc;
    }
    return OK;
}

// ������ȱ���ͼG
Status DFSTraverse(DG G, Status(*visit)(VexType)) {
    int i;
    for (i = 0; i<G.n; i++) G.tags[i] = UNVISITED; 
    for (i = 0; i < G.n; i++)
        if (UNVISITED == G.tags[i])
            if (ERROR == DFS(G, i, visit)) return ERROR;
    return OK;
}

// ������ȱ���
Status BFSTraverse(DG G, Status(*visit)(VexType)) {
    int i, j, k;
    ArcNodeP p;
    Queue Q;
    InitQueue(&Q);
    for (i = 0; i < G.n; i++) G.tags[i] = UNVISITED;
    for (i = 0; i < G.n; i++) {
        if (UNVISITED == G.tags[i]) { //i����δ������
            G.tags[i] = VISITED;
            if (ERROR == visit(G.vertices[i].data)) return ERROR;
            EnQueue(&Q, i); //i���������
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
//    printf("------------- �����ʵ�飨����ͼ��-------------\n\n");
//    printf("|                  �����ڽӱ�                 |\n\n");
//    printf("|        �༶���������ѧ�뼼��19(1)��        |\n\n");
//    printf("|              ѧ�ţ�3119004733               |\n\n");
//    printf("|                �����������                 |\n\n");
//    printf("-----------------------------------------------\n\n");
    DG G;
    int select, k, m;
    VexType v, w;
    ArcNodeP p;
    p = (ArcNodeP) malloc(sizeof(ArcNode));
    printf("1. ����ͼ��2. ����ͼ��3. ����λ��4. ȡk����ֵ��\n"
           "5. Ϊk���㸳ֵ��6. ���뻡��7. ɾ������8. ���һ���ڽӶ��㣻\n"
           "9. ����һ���ڽӶ��㣻10. ������ȱ�����11. ������ȱ�����0. �˳�\n"
           "��ѡ����������ȴ���ͼ����");
    scanf("%d", &select);
    do {
        switch (select) {
            case 1:
                printf("\n** 1. ����ͼ **\n");
                CreatDG(&G);
                break;
            case 2:
                printf("\n** 2. ����ͼ **\n");
                if (DestroyDG(&G))
                    printf("��ǰͼ���ٳɹ�����������%d��������%d\n", G.n, G.e);
                break;
            case 3:
                printf("\n** 3. ����λ�� **\n");
                printf("������Ҫ���ҵĶ������Ϣ��");
                getchar();
                scanf("%c", &v);
                printf("�ö���λ��Ϊ��%d\n", LocateVex(&G, v));
                break;
            case 4:
                printf("\n** 4. ȡk����ֵ **\n");
                printf("������Ҫȡ�Ķ����λ��");
                scanf("%d", &k);
                GetVex(&G, k, &v);
                printf("λ��Ϊ%d�Ķ���ֵΪ��%c\n", k, v);
                break;
            case 5:
                printf("\n** 5. Ϊk���㸳ֵ **\n");
                printf("�����븳ֵ�����λ��");
                scanf("%d", &k);
                printf("��������Ҫ����ֵ(char����)��");
                getchar();
                scanf("%c", &v);
                printf("��ֵǰ%d����ֵΪ��%c", k, G.vertices[k].data);
                PutVex(&G, k, v);
                printf("\n��ֵ��%d����ֵΪ��%c", k, v);
                PrintG(G);
                break;
            case 6:
                printf("\n** 6. ���뻡 **");
                PrintG(G);
                printf("��ѡ��Ҫ����Ļ��Ķ˵��λ��(��β->��ͷ���ո����)��");
                scanf("%d %d", &k, &m);
                if (AddArc(&G, k, m))
                    printf("����ɹ���\n");
                else
                    printf("���Ѵ��ڣ�����ʧ�ܣ�\n");
                PrintG(G);
                break;
            case 7:
                printf("\n** 7. ɾ���� **");
                PrintG(G);
                printf("��ѡ��Ҫɾ���Ļ��Ķ˵��λ��(��β->��ͷ���ո����)��");
                scanf("%d %d", &k, &m);
                if (RemoveArc(&G, k, m))
                    printf("ɾ���ɹ���\n");
                else
                    printf("�������ڣ�ɾ��ʧ�ܣ�\n");
                PrintG(G);
                break;
            case 8:
                printf("\n** 8. ���һ���ڽӶ��� **");
                PrintG(G);
                printf("������Ҫ���һ���ڽӶ���Ķ���λ��");
                scanf("%d", &k);
                printf("��һ���ڽӶ����λ��Ϊ��%d\n", FirstAdjVex(&G, k));
                break;
            case 9:
                printf("\n** 9. ����һ�ڽӶ��� **");
                PrintG(G);
                printf("��k�����m����һ���ڽӶ���Ķ���λ��(������k��m���ո����)��");
                scanf("%d %d", &k, &m);
                p = G.vertices[k].firstArc;
                while (p && p->adjvex != m)
                    p = p->nextArc;
                printf("%c����һ���ڽӶ����λ��Ϊ��%d\n", G.vertices[p->adjvex].data, NextAdjVex(&G, k, p));
                break;
            case 10:
                printf("\n** 10. ������ȱ��� **\n");
                printf("������ȱ������У�");
                DFSTraverse(G, Visit);
                printf("\n");
                break;
            case 11:
                printf("\n** 11. ������ȱ��� **\n");
                printf("������ȱ������У�");
                BFSTraverse(G, Visit);
                printf("\n");
        }
        printf("\n1. ����ͼ��2. ����ͼ��3. ����λ��4. ȡk����ֵ��\n"
               "5. Ϊk���㸳ֵ��6. ���뻡��7. ɾ������8. ���һ���ڽӶ��㣻\n"
               "9. ����һ���ڽӶ��㣻10. ������ȱ�����11. ������ȱ�����0. �˳�\n"
               "��ѡ�������");
        scanf("%d", &select);
    } while (select != 0);
    printf("\n**** ���н��� ****");
    return 0;
}
