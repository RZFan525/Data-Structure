#include <stdio.h>
#include<stdlib.h>

#define OK 1
#define FALSE 0
#define TRUE 1
#define ERROR 0
#define INFEISIBLE -1
#define OVERFLOW -2
typedef int Status;

/**********************迷宫********************************/
#define MAXLEN 10
//迷宫类型
typedef struct 
{ 
	int r; //迷宫行数8
	int c; //迷宫列数8
	int adr[MAXLEN][MAXLEN];//10×10矩阵（包括围墙），可以采用不同的字符表示墙和可通的块
}MazeType;

//通道块
typedef struct 
{ 
	int r; //行号
	int c; //列号
}PosType; 

//当前位置
typedef struct
{
	int ord;
	PosType seat;
	int di;
}SElemType;

typedef SElemType ElemType;

/*定义顺序栈*/
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10
typedef struct
{
	ElemType * base;
	ElemType * top;
	int stacksize;
}SqStack;

Status InitStack(SqStack &S);//构造一个空栈s
Status DestroyStack(SqStack &S);//销毁栈s
Status ClearStack(SqStack &S);//清空栈s
Status StackEmpty(SqStack S);//判断s是否为空，空栈返回TRUE 否则返回FALSE
int StackLength(SqStack S);//返回栈s的长度
Status GetTop(SqStack S, ElemType &e);//若栈不空用e返回栈顶元素，否则返回ERROR
Status Push(SqStack &S,ElemType e);//插入元素e到栈顶
Status Pop(SqStack &S,ElemType &e);//若栈不空则删除s的栈顶元素，用e返回，否则返回FALSE
Status StackTraverse(SqStack S, void(*visit)());//从栈底到栈顶依次对栈中每一个元素调用函数visit()
/***********函数实现**************/
Status InitStack(SqStack &S)//构造一个空栈s
{
	S.base=(ElemType *)malloc(STACK_INIT_SIZE*sizeof(ElemType));
	if(!S.base)
		exit(OVERFLOW);
	S.top=S.base;
	S.stacksize=STACK_INIT_SIZE;
	return OK;
}

Status DestroyStack(SqStack &S)//销毁栈s
{
	S.stacksize=0;
	if(S.base!=NULL)
		free(S.base);
	return OK;
}

Status ClearStack(SqStack &S)//清空栈s
{
	S.top=S.base;
	return OK;
}

Status StackEmpty(SqStack S)//判断s是否为空，空栈返回TRUE 否则返回FALSE
{
	return(S.base==S.top);
}

int StackLength(SqStack S)//返回栈s的长度
{
	return(S.top-S.base);
}

Status GetTop(SqStack S, ElemType &e)//若栈不空用e返回栈顶元素，否则返回ERROR
{
	if(StackEmpty(S))
		return ERROR;
	else
		e=*(S.top-1);
	return OK;
}

Status Push(SqStack &S,ElemType e)//插入元素e到栈顶
{
	if(S.top-S.base>=S.stacksize)
	{
		S.base=(ElemType*)realloc(S.base,(S.stacksize+STACKINCREMENT)*sizeof(ElemType));
		if(!S.base)
			exit(OVERFLOW);
		S.top=S.base+S.stacksize;
		S.stacksize+=STACKINCREMENT;
	}
	*(S.top++)=e;
	return OK;
}

Status Pop(SqStack &S,ElemType &e)//若栈不空则删除s的栈顶元素，用e返回，否则返回FALSE
{
	if(StackEmpty(S))
		return FALSE;
	e=*(--S.top);
	return OK;
}

Status StackTraverse(SqStack S, void(*visit)(ElemType))//从栈底到栈顶依次对栈中每一个元素调用函数visit()
{
	if(StackEmpty(S))
		return FALSE;
	ElemType *p;
	for(p=S.base;p<S.top;p++)
		(*visit)(*p);
	return OK;
}
/*******************栈结束*********************************/

//输入迷宫
MazeType InputMaze()
{
	MazeType maze;
	int i,j;
	printf("请输入迷宫的行和列：\n");
	scanf("%d %d",&maze.r,&maze.c);
	while (maze.r <= 0 || maze.c <= 0)
	{
		printf("请输入正确的的行和列！\n");
		scanf("%d %d", &maze.r, &maze.c);
	}
	printf("请输入迷宫(墙为1，路为0):\n");
	for(i=0;i<maze.r;i++)
		for(j=0;j<maze.c;j++)
		{
			scanf("%d",&maze.adr[i][j]);
		}
	return maze;
}

//下一个位置
PosType NextPos(PosType pos,int k) //k=1东边，2南边，3西边，4北边
{
	switch(k)
	{
	case 1: pos.c++; break;
	case 2: pos.r++; break;
	case 3: pos.c--; break;
	case 4: pos.r--; break;
	default: printf("请输入正确方向！"); exit(1);
	}
	return pos;
}

//判断当前位置是否可以通过
Status Pass(PosType curpos, MazeType maze)
{
	if(maze.adr[curpos.r][curpos.c]==0)
		return TRUE;
	else
		return FALSE;
}

//留下足迹
Status FootPrint(PosType curpos, MazeType &maze) //留下足迹为*
{
	maze.adr[curpos.r][curpos.c]=42;
	return OK;
}

//留下不能通过的标记
Status MarkPrint(PosType curpos, MazeType &maze) //留下标记为#
{
	maze.adr[curpos.r][curpos.c]=35;
	return OK;
}

//打印迷宫
Status printmaze(MazeType maze)  //墙为 □  足迹为* 标记的无法通过的路为#
{
	int i,j;
	for(i=0;i<maze.r;i++)
	{
		for(j=0;j<maze.c;j++)
		{
			printf("%c ",(char)maze.adr[i][j]);
		}
		printf("\n");
	}
	return OK;
}
Status MazePath(MazeType &maze, PosType start, PosType end)
{
	SqStack S;
	PosType curpos;
	int curstep;
	InitStack(S);
	curpos=start;
	curstep=1;
	SElemType e;
	do{
		if(Pass(curpos,maze))
		{
			FootPrint(curpos,maze);
			e.ord=curstep;
			e.seat=curpos;
			e.di=1;
			Push(S,e);
			if(curpos.c==end.c && curpos.r==end.r)
				return TRUE;
			curpos=NextPos(curpos,1);
			curstep++;
		}
		else
		{
			if(!StackEmpty(S))
			{
				Pop(S,e);
				while(e.di==4 && !StackEmpty(S))
				{
					MarkPrint(e.seat,maze);
					Pop(S,e);
				}
				if(e.di<4)
				{
					e.di++;
					Push(S,e);
					curpos=NextPos(e.seat,e.di);
				}
			}
		}
	}while(!StackEmpty(S));
	return (FALSE);
}

void main()
{
	MazeType maze;
	maze=InputMaze();
	PosType start,end;
	int r, c;
	printf("请输入起点坐标:");
	scanf("%d %d",&r,&c);
	start.r = r - 1;
	start.c = c - 1;
	printf("请输入终点坐标:");
	scanf("%d %d",&r,&c);
	end.r = r - 1;
	end.c = c - 1;
	if(MazePath(maze,start,end)==1)
		printmaze(maze); // 墙为'□' 足迹为'*'  不可通的足迹为'#'
	else
		printf("此迷宫无通路!");
	system("pause");
}
