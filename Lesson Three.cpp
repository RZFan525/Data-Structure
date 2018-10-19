#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
typedef int Status;


typedef struct {
	char sno[10];
	char sname[10];
	int sage;
	char sdept[10];
}StuType;
typedef StuType ElemType;

typedef struct LNode{
	ElemType data;
	struct LNode *next;
}LNode, *LinkList;

/*********************函数************************************/
Status InitList(LinkList &L);
Status DestroyList(LinkList &L);
Status ClearList(LinkList &L);
Status ListEmpty(LinkList L);
int ListLength(LinkList L);
Status GetElem(LinkList L, int i, ElemType &e);
Status PutElem(LinkList &L, int i,ElemType &e );
int LocateElem(LinkList L, ElemType e, Status (*compare(ElemType,ElemType)));
Status PriorElem(LinkList L, ElemType cur_e, ElemType &pre_e);
Status NextElem(LinkList L, ElemType cur_e, ElemType &next_e);
Status ListInsert(LinkList &L, int i, ElemType e);
Status ListDelete(LinkList &L, int i, ElemType &e);
Status ListTraverse(LinkList L, Status (*visit)(ElemType));
Status equal(ElemType a, ElemType b);
void visit(ElemType e);
/*****************************************************************/

//初始化一个链表
Status InitList(LinkList &L)
{
	L=(LinkList)malloc(sizeof(LNode));
	if(!L) exit(OVERFLOW);
	L->next=NULL;
	return OK;
}

//销毁链表
Status DestroyList(LinkList &L)
{
	LinkList p;
	if(L==NULL) return ERROR;
	while(L)
	{
		p=L->next;
		free(L);
		L=p;
	}
	return OK;
}
//清空链表，保留头指针
Status ClearList(LinkList &L)
{
	LinkList p,q;
	if(L==NULL) return ERROR;
	q=L->next;
	while(q)
	{
		p=q->next;
		free(q);
		q=p;
	}
	return OK;
}

//判断链表是否为空 空返回1  非空返回0
Status ListEmpty(LinkList L)
{
	if(L==NULL) return ERROR;
	if (L->next==NULL)
		return TRUE;
	else
		return FALSE;
}

//返回链表的长度
int ListLength(LinkList L)
{
	int Length=0;
	if(L==NULL) return ERROR;
	LinkList p=L->next;
	while(p)
	{
		p=p->next;
		Length++;
	}
	return Length;
}

//将链表L中第i个元素存入e中
Status GetElem(LinkList L, int i, ElemType &e)
{
	int t=0;
	LinkList p=L;
	while(p && t<i)
	{
		p=p->next;
		t++;
	}
	if(!p || t>i) return ERROR;
	e=p->data;
	return OK;
}

//将第i个元素信息与e交换
Status PutElem(LinkList &L, int i,ElemType &e )
{
	LinkList p=L;
	int j=0;
	ElemType tmp;
	while(p && j<i)
	{
		p=p->next;
		j++;
	}
	if(!p || j>i) return ERROR;
	tmp=p->data;
	p->data=e;
	e=tmp;
	return OK;
}
//返回第一个与e满足compare()关系的位序，否则返回0
int LocateElem(LinkList L, ElemType e, Status (*compare)(ElemType,ElemType))
{
	if(L==NULL) return ERROR;
	LinkList p=L;
	int i=0;
	while(p && !((*compare)(e,p->data)))
	{
		p=p->next;
		i++;
	}
	if(!p) return 0;
	return i;
}

//返回cur_e的前驱 存入pre_e中，若不存在则返回0
Status PriorElem(LinkList L, ElemType cur_e, ElemType &pre_e)
{
	if(L==NULL) return ERROR;
	int i;
	i=LocateElem(L,cur_e,equal);
	if(i==0 || i==1)  return ERROR;
	GetElem(L,i-1,pre_e);
	return OK;
}

//返回cur_e的后继 存入pre_e中，若不存在则返回0
Status NextElem(LinkList L, ElemType cur_e, ElemType &next_e)
{
	if(L==NULL) return ERROR;
	int i;
	i=LocateElem(L,cur_e,equal);
	if(i==0 || i==ListLength(L))  return ERROR;
	GetElem(L,i+1,next_e);
	return OK;
}

//将元素e插入L的第i个位置之前
Status ListInsert(LinkList &L, int i, ElemType e)
{
	int j=0;
	LinkList p=L;
	while(p && j<i-1)
	{
		p=p->next;
		j++;
	}
	if(!p || j>i-1) return ERROR;
	LinkList s=(LinkList)malloc(sizeof(LNode));
	s->data=e;
	s->next=p->next;
	p->next=s;
	return OK;
}
//删除第i个结点，并将data放入e中;
Status ListDelete(LinkList &L, int i, ElemType &e)
{
	if(L==NULL) return ERROR;
	int j=0;
	LinkList p=L;
	while(p->next &&j<i-1)
	{
		p=p->next;
		j++;
	}
	if(!p || j>i) return ERROR;
	LinkList q=p->next;
	e=p->next->data;
	p->next=p->next->next;
	free(q);
	return OK;
}
//依次对每个元素调用visit
Status ListTraverse(LinkList L, void (*visit)(ElemType))
{
	if(L==NULL) return ERROR;
	LinkList p=L->next;
	while(p)
	{
		visit(p->data);
		p=p->next;
	}
	return OK;
}

//比较两个学生学号是否相等，相等返回1，否则返回0
Status equal(ElemType a, ElemType b)
{
	if(strcmp(a.sno,b.sno)==0)
		return OK;
	else
		return ERROR;
}

//输出此学生信息
void visit(ElemType e)
{
	printf("%s\t%s\t%d\t%s\n",e.sno,e.sname,e.sage,e.sdept);
}


//主函数
void main()
{
	LinkList L;
	ElemType stu,stu_tmp;
	int func,pos;

	if (!InitList(L)) 
	{
		printf("overflow!\n");
		exit(-1);
	}
	for (int j=0;;j++)
	{
		printf("Please select the function:\n0--Exit\n1--insert student\n2--delete student\n3--update student name\n4--search student with sno\n5--show all student\n6--count the student\n Please input:");
		scanf("%d",&func);
		switch (func)
		{
		case 0: 
			exit(1);
			break;
		case 1: //插入学生信息
			printf("sno: ");
			scanf("%s", &stu.sno);
			printf("sname: ");
			scanf("%s", &stu.sname);
			printf("sage: ");
			scanf("%d", &stu.sage);
			printf("sdept: ");
			scanf("%s", &stu.sdept);
			ListInsert(L,ListLength(L)+1,stu);
			break;
		case 2://删除指定学号的学生信息
			printf("input the sno:");
			scanf("%s",&stu.sno);
			pos=LocateElem(L,stu,equal);
			if (pos>0)
				ListDelete(L,pos,stu);
			else
				printf("The student is not exist!\n");
			break;	
		case 3://更新指定学号的学生姓名
			printf("input the sno of the student to be updated:");
			scanf("%s",&stu.sno);
			pos=LocateElem(L,stu,equal);
			if (pos>0)
			{
				printf("input the new name:");
				scanf("%s",&stu.sname);
				GetElem(L,pos,stu_tmp);
				stu.sage=stu_tmp.sage;
				strcpy(stu.sdept,stu_tmp.sdept);
				PutElem(L,pos,stu);
			}
			else
				printf("The student is not exist!\n");
			break;
		case 4://查找指定学号的学生信息
			printf("input the sno:");
			scanf("%s",&stu.sno);
			pos=LocateElem(L,stu,equal);
			if (pos>0)
			{
				GetElem(L,pos,stu_tmp);
				printf("%s\t%s\t%d\t%s\n",stu_tmp.sno,stu_tmp.sname,stu_tmp.sage,stu_tmp.sdept);
			}
			else
				printf("The student is not exist!\n");
			break;
		case 5: //输出所有学生信息
			ListTraverse(L,visit);
			break;
		case 6: //输出学生数量
			printf("The student count is %d\n",ListLength(L));
			break;
		}
	}
}
