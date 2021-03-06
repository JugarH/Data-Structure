# Data-Structure
### 1、数据结构实验
- **设计要求：** 分别采用邻接数组和邻接表为存储结构，实现抽象数据类型有向图DG。
- **所用软件：** Dev-C++
- **数据对象：** 有限顶点集V和有限边集E
- **数据关系：** R={<v,w> | v、w∈V ∧ <v,w>∈E，E表示从顶点v指向顶点w的有向边的集合}
- **基本操作：**
  - InitDG(*G, n)  
操作结果：初始化含n个顶点且无边的有向图G
  - CreatDG(*G)  
操作结果：创建一个含n个顶点和e条边的有向图G
  - LocateVex(G, v)  
初始条件：有向图G已存在，v为顶点值  
操作结果：查找顶点v在有向图G中的位序
  - DestroyDG(*G)  
初始条件：有向图G已存在  
操作结果：销毁有向图G
  - GetVex(G, k, *w)  
初始条件：有向图G已存在，0≤k≤n-1  
操作结果：取有向图G的k顶点的值到w
  - PutVex(*G, k, w)  
初始条件：有向图G已存在，0≤k≤n-1  
操作结果：对有向图G的k顶点赋值w
  - FirstAdjVex(*G, k)  
初始条件：有向图G已存在，0≤k≤n-1  
操作结果：返回k顶点的第一个邻接顶点的位序
  - NextAdjVex(*G, k, m)  
初始条件：有向图G已存在，0≤k、m≤n-1  
操作结果：返回有向图G中k顶点相对于m顶点的下一个邻接顶
点的位序
  - AddArc(*G, k, m)  
初始条件：有向图G已存在，0≤k、m≤n-1  
操作结果：在有向图G中插入k顶点到m顶点的弧，弧数加一
  - RemoveArc(*G, k, m)  
初始条件：有向图G已存在且弧集非空，0≤k、m≤n-1  
操作结果：删除有向图G中k顶点到m顶点的弧，弧数减一
  - DFSTraverse(G, visit(v))  
初始条件：有向图G已存在  
操作结果：输出有向图G的深度优先遍历序列
  - BFSTraverse(G, visit(v))  
初始条件：有向图G已存在  
操作结果：输出有向图G的广度优先遍历序列
- 存储结构定义
```c
#include<stdio.h>
#include<malloc.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1
#define UNVISITED 0
#define VISITED 1

typedef char VexType;  //顶点类型
typedef int Status;     //函数值类型，表示函数结果状态

（1）邻接数组存储结构
	typedef struct {
    	VexType *vertices;  //顶点数组
    	int **arcs;       	//关系数组
    	int n, e;         	//顶点数和边数
    	int *tags;        	//标志数组，可用于在图的遍历中标记顶点访问与否
} DG;

（2）邻接表存储结构
typedef struct ArcNode {
    int adjvex;   			  //邻接顶点在顶点数组中的位序
    struct ArcNode *nextArc;  //指向下一个邻接顶点的指针
} ArcNode, *ArcNodeP;       //邻接链表的结点类型
typedef struct VexNode {
    VexType data;           //顶点值
    struct ArcNode *firstArc;  //邻接链表的头指针
} VexNode;    //顶点数组的元素类型
typedef struct {
    VexNode *vertices;  //顶点数组，用于储存顶点信息
    int n, e;				 //图的当前顶点数和弧数
    int *tags;          //标志数组，可用于在图的遍历中标记顶点访问与否
} DG; 

（3）链队列存储结构（用于广度优先遍历算法）
typedef struct QNode {
    int data;
    struct QNode *next;
} QNode, *QueuePtr;
typedef struct {
    QueuePtr front;    //队头指针
    QueuePtr rear;     //队尾指针
} Queue;
```
### 2、数据结构课程设计
- **运行环境：** Dev-C++
- **问题描述：**  
大学的每个专业都要制定教学计划。假设任何专业都有固定的学习年限，每学年含两学期，每学期的时间长度和学分上限值均相等。每个专业开设的课程都是确定的，而且课程在开设时间的安排必须满足先修关系。每门课程有哪些先修课程是确定的，可以有任意多门，也可以没有。每门课恰好占一个学期。试在这样的前提下设计一个教学计划编制程序。
- **基本要求：**  
（1）将专业课程信息以及课程先修关系信息表示为有向图，并手动输入或使用文件导入信息至系统中。输入参数包括：学期总数，一学期的学分上限，每门课的课程编号、学分和课程先修关系；  
（2）按照给定的先修关系，给出一种教学计划编制方案，即输出每学期要上的课程列表。若根据给定的条件问题无解，则报告适当的信息；否则将教学计划输出到用户指定的文件中；  
（3）在（2）的基础上，若每学期有学分上限，给出一种或多种教学计划编制方案。
- **测试数据：**  
学期总数 6，课程总数 15，一学期学分上限 11；  
下表为专业课程信息以及课程之间的先修关系  
课程编号	课程名称	学分	先修课程  
L1	高等数学(上)	4.5	无  
L2	高等数学(下)	4.5	高等数学(上)  
L3	线性代数	2.0	高等数学(下)  
L4	计算机系统导论	1.0	无  
L5	程序设计	3.0	无  
L6	离散数学	4.0	程序设计  
L7	数据结构	3.0	程序设计，离散，线性代数  
L8	汇编语言	2.5	程序设计，计算机系统导论  
L9	数字逻辑与系统设计	2.5	计算机系统导论  
L10	计算机组成原理	3.0	汇编语言，数字逻辑与系统设计  
L11	操作系统	3.0	数据结构，计算机组成原理，程序设计  
L12	数据库系统	3.0	数据结构，离散数学，操作系统  
L13	编译原理	3.0	数据结构，程序设计，汇编语言  
L14	软件工程	2.5	计算机系统导论，程序设计  
L15	计算机网络	3.0	操作系统，计算机组成原理  
