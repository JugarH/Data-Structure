/*
*	����ͼ���ڽ������ʾ
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
typedef int Status;   //����ֵ���ͣ���ʾ�������״̬

// ���������Ͷ��壬���ڱ���
typedef struct QNode {
    int data;
    struct QNode *next;
} QNode, *QueuePtr;
typedef struct {
    QueuePtr front;   //��ͷָ��
    QueuePtr rear;    //��βָ��
} Queue;

typedef struct {
    VexType *vertices;    //��������
    int **arcs;       //��ϵ����
    int n, e;         //�������ͱ���
    int *tags;        //��־���飬��������ͼ�ı����б�Ƕ���������
} DG;  //�ڽ��������ͣ�DGΪ����ͼ

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

// ��ӡ�ڽ�����
void PrintG(DG G) {
    int i, j;
    printf("\n����ͼ���ڽ����飺\n");
    for (i = 0; i < G.n; i++) {
        printf("%d %c    ", i, G.vertices[i]);
        for (j = 0; j < G.n; j++)
            printf("%d ", G.arcs[i][j]);
        printf("\n");
    }
}

// ���Ҷ���v��ͼG�е�λ��
int LocateVex(DG G, VexType v) {
    int i;
    for (i = 0; i < G.n; i++) {
        if (G.vertices[i] == v)
            return i;
    }
    return -1;
}

// ��ʼ����n���������ޱߵ�����ͼG���ڽ�����洢�ṹ
Status InitDG(DG *G, int n) {
    int i, j;
    G->n = n;
    G->e = 0;
    if (NULL == (G->vertices = (VexType *) malloc(n * sizeof(VexType))))
        return OVERFLOW;
    printf("\n");
    //������������
    for (i = 0; i < G->n; i++) {
        getchar();
        printf("������λ��Ϊ%d�Ķ�����Ϣ��", i);
        scanf("%c", &G->vertices[i]);
    }
    //Ϊ��ϵ�������ռ�
    if (NULL == (G->arcs = (int **) malloc(n * sizeof(int *))))
        return OVERFLOW;
    for (i = 0; i < n; i++) {
        if (NULL == (G->arcs[i] = (int *) malloc(n * sizeof(int))))
            return OVERFLOW;
    }
    //Ϊ��־�������ռ�
    if (NULL == (G->tags = (int *) malloc(n * sizeof(int))))
        return OVERFLOW;
    //��ʼ����־����͹�ϵ����
    for (i = 0; i < n; i++) {
        G->tags[i] = UNVISITED;
        for (j = 0; j < n; j++)
            G->arcs[i][j] = 0;
    }
    return OK;
}

// ����n�������e���ߵ�����ͼG
Status CreatDG(DG *G) {
    int i, j, k, n, e;
    VexType head, tail; //head����ͷ��tail����β
    printf("�����붥������");
    scanf("%d", &n);
    printf("�����뻡����");
    scanf("%d", &e);
    if (n < 0 || e < 0) return ERROR;
    if (InitDG(G, n) != OK) return ERROR;
    G->e = e;
    printf("\n");
    for (k = 0; k < G->e; k++) {
        getchar();  //���ջس��������ַ�
        printf("�����뻡����Ϣ(��β->��ͷ��Ӣ�Ķ���/�ո���)��");
        scanf("%c%*c%c", &tail, &head);
        i = LocateVex(*G, tail);
        j = LocateVex(*G, head); //ȷ��head��tail��λ��i��j
        if (i < 0 || j < 0) return ERROR;    //����v��w������
        G->arcs[i][j] = 1;
    }
    PrintG(*G);
    return OK;
}

// ����ͼG
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

// ȡͼG��k�����ֵ��w
Status GetVex(DG *G, int k, VexType *w) {
    if (k < 0 || k >= G->n)
        return ERROR;    //k���㲻����
    *w = G->vertices[k];
    return OK;
}

// ��ͼG��k���㸳ֵw
Status PutVex(DG *G, int k, VexType w) {
    if (k < 0 || k >= G->n)
        return ERROR;    //k���㲻����
    G->vertices[k] = w;
    return OK;
}

// ��k����ĵ�һ���ڽӶ����λ��
int FirstAdjVex(DG *G, int k) {
    int j;
    for (j = 0; j < G->n; j++) {
        if (G->arcs[k][j] == 1)
            return j;
    }
    return -1;
}

// ��ͼG��k���������m�������һ���ڽӶ����λ��
int NextAdjVex(DG *G, int k, int m) {
    int i;
    if (k == 0 && m == 0) return 0;
    for (i = m + 1; i < G->n; i++)
        if (G->arcs[k][i]) return i;
    return -1;
}

// ���붥��k��m�Ļ�(�ڽ�����ı�ͷ����)
Status AddArc(DG *G, int k, int m) {
    if (k < 0 || m < 0 || k == m) return ERROR;
    if (G->arcs[k][m]) return ERROR;  //���Ѵ���
    else {
        G->arcs[k][m] = 1;
        G->e++;     //���룬��1��������һ
        PrintG(*G);
        return OK;
    }
}

// ɾ������k��m�Ļ�
Status RemoveArc(DG *G, int k, int m) {
    if (G->n == 0)    //G���޻�������
        return ERROR;
    if (G->arcs[k][m]) {
        G->arcs[k][m] = 0;
        G->e--;     //ɾ������0��������һ
        PrintG(*G);
        return OK;
    }
    return ERROR;
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
    visit(G.vertices[k]);    //�������ֵ
    for (i = FirstAdjVex(&G, k); i >= 0; i = NextAdjVex(&G, k, i)) {
        if (UNVISITED == G.tags[i]) //δ�����ö���
            if (ERROR == DFS(G, i, visit))
                return ERROR;
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
    int i, j;
    Queue Q;
    InitQueue(&Q);
    for (i = 0; i < G.n; i++) G.tags[i] = UNVISITED;
    for (i = 0; i < G.n; i++) { //��ÿһ������ѭ��
        if (UNVISITED == G.tags[i]) { //��ǰ����δ������
            G.tags[i] = VISITED;
            if (ERROR == visit(G.vertices[i])) return ERROR;
            EnQueue(&Q, i); //��ǰ���������
            while (!QueueEmpty(Q)) { //���в�Ϊ��
                DeQueue(&Q, &i);
                for (j = 0; j < G.n; j++) {
                    if (UNVISITED == G.tags[j] && G.arcs[i][j] == 1) {
                        if (ERROR == visit(G.vertices[j])) return ERROR;
                        G.tags[j] = VISITED;    //���ҵ��Ķ�����Ϊ�ѷ���
                        EnQueue(&Q, j);  //���ҵ��Ķ��������
                    }
                }
            }
        }
    }
    return OK;
}

int main() {
//    printf("-------------- �����ʵ�飨����ͼ��-------------\n\n");
//    printf("|                 һ���ڽ�����                 |\n\n");
//    printf("|          �༶���������ѧ�뼼��19(1)��       |\n\n");
//    printf("|                ѧ�ţ�3119004733              |\n\n");
//    printf("|                 �����������                 |\n\n");
//    printf("------------------------------------------------\n\n");
    DG G;
    int select, k, m;
    VexType v;
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
                printf("�ö���λ��Ϊ��%d\n", LocateVex(G, v));
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
                printf("��ֵǰ%d����ֵΪ��%c", k, G.vertices[k]);
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
                printf("��k�����m����һ���ڽӶ���Ķ���λ��(����k��m���ո����)��");
                scanf("%d %d", &k, &m);
                printf("%c�����%c����һ���ڽӶ����λ��Ϊ��%d\n", G.vertices[k], G.vertices[m], NextAdjVex(&G, k, m));
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
