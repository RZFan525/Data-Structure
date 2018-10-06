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


#define LIST_INIT_SIZE 80 // 线性表存储空间的初始分配量
#define LISTINCREMENT 10 // 线性表存储空间的分配增量
typedef struct {
	char sno[10];
	char sname[10];
	int sage;
	char sdept[10];
}StuType;
typedef StuType ElemType;

typedef struct {
	ElemType *elem;    // 存储空间基址
	int length;   // 当前长度
	int listsize;  // 当前分配的存储容量,(以sizeof(ElemType)为单位)
} List;  // 俗称顺序表


Status equal(ElemType, ElemType);

Status InitList(List& L);
int LocateElem(List L, ElemType e,Status (*compare)(ElemType, ElemType)) ;
Status ListInsert(List &L, int i, ElemType e) ;
Status ListDelete (List &L, int i, ElemType &e);
void DestroyList(List &L);
bool ListEmpty(List L );
int ListLength( List L ); 
Status PriorElem( List L, ElemType cur_e, ElemType &pre_e );





Status InitList( List& L ) 
{
  // 构造一个空的线性表 
	L.elem = (ElemType*) malloc (LIST_INIT_SIZE*sizeof (ElemType));
	if (!L.elem) exit(OVERFLOW);
	L.length = 0;
	L.listsize = LIST_INIT_SIZE;
	return OK;
} // InitList

 int LocateElem(List L, ElemType e,Status (*compare)(ElemType, ElemType)) 
 {
   // 在顺序表中查询第一个满足判定条件的数据元素，
   // 若存在，则返回它的位序，否则返回 0
	int i = 1;           // i 的初值为第 1 元素的位序
	ElemType *p = L.elem;     // p 的初值为第 1 元素的存储位置
	while (i <= L.length && !(*compare)(*p++, e))  ++i;
	if (i <= L.length)  return i;
	else  return 0;
} // LocateElem

Status ListInsert(List &L, int i, ElemType e) 
{
         // 在顺序表L的第 i 个元素之前插入新的元素e,
          // i 的合法范围为  1≤i≤L.length+1
	ElemType *newbase;
	if (i < 1 || i > L.length+1) return ERROR; 
										   // 插入位置不合法
	if (L.length >= L.listsize) { 
							  // 当前存储空间已满，增加分配
		newbase = (ElemType *)realloc(L.elem,                                                                 
		(L.listsize+LISTINCREMENT)*sizeof (ElemType));
		if (!newbase) exit(OVERFLOW);  
							   // 存储分配失败
		L.elem = newbase;                // 新基址
		L.listsize += LISTINCREMENT; // 增加存储容量
	}
	ElemType *p, *q = &(L.elem[i-1]);                 // q 指示插入位置
	for (p = &(L.elem[L.length-1]); p >= q;  --p)  
		 *(p+1) = *p;       // 插入位置及之后的元素右移
	*q = e;       // 插入e
	++L.length;   // 表长增1
	return OK;

} // ListInsert           

Status ListDelete (List &L,int i, ElemType &e) 
{

	if ((i < 1) || (i > L.length))  return ERROR; 
									   // 删除位置不合法
	ElemType *q, *p = &(L.elem[i-1]);       // p 为被删除元素的位置
	e = *p;                        // 被删除元素的值赋给 e
	q = L.elem+L.length-1;     // 表尾元素的位置
	for (++p; p <= q; ++p)  *(p-1) = *p;  
							// 被删除元素之后的元素左移
	--L.length;       // 表长减1
	return OK;
} // ListDelete


void DestroyList(List &L)
{
	L.length=0;
	L.listsize=0;
	if (L.elem!=NULL) free(L.elem);
}

bool ListEmpty(List L )
{
	return(L.length==0);
}

int ListLength( List L ) 
{
	return (L.length);
}

Status PriorElem( List L, ElemType cur_e, ElemType &pre_e ) 
{
	int i=LocateElem(L, cur_e,equal);
	if (i==0 || i==1) return ERROR;
	pre_e=L.elem[i-2];
	return OK;
}

Status NextElem( List L, ElemType cur_e, ElemType &next_e ) 
{
	int i=LocateElem(L, cur_e,equal);
	if (i==0 || i==L.length) return ERROR;
	next_e=L.elem[i];
	return OK;
}

Status GetElem( List  L, int i, ElemType &e ) 
{
	if (i<1||i>L.length)
        return ERROR;
	else
        e=L.elem[i-1];
	return OK;
}

void ClearList(List &L) 
{
	L.length=0;
}

Status PutElem( List &L, int i,ElemType &e )
{
	ElemType tmp;
	if (i<1||i>L.length)
        return ERROR;
	tmp=L.elem[i-1];
	L.elem[i-1]=e;
	e = tmp;
	return OK;
}

Status ListTraverse(List  L, void (*visit)(ElemType )) 
{
	int i;
	if (ListEmpty(L))  return OK;
	for ( i=0; i<L.length; i++)
	   (*visit)(L.elem[i]);
	return OK;
}



Status equal(ElemType a, ElemType b)
{
	if (strcmp(a.sno,b.sno)==0)
		return OK;
	else
		return ERROR;
}

void visit(ElemType e)
{
	printf("%s\t%s\t%d\t%s\n",e.sno,e.sname,e.sage,e.sdept);
}



void main()
{
	List L;
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
