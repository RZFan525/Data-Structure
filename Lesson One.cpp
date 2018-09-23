#include <iostream>
using namespace std; 
//最大子列和问题 
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
int Max(int a,int b,int c)
{
	return a>b ? a>c? a:c : b>c?b:c;
 } 
int first (int *a,int n)//算法1 
{
	int i,j,sum,summax=0;
	for(i=0;i<n;i++)
	{
		sum=0;
		for(j=i;j<n;j++)
		{
			sum=sum+a[j];
			if(sum>summax)
				summax=sum;
		}
	}
	return summax;
}
int second_main(int *a,int left,int right)//算法二主函数（递归） 
{
	int Lsummax,Rsummax,LBsum=0,RBsum=0,LBsummax=0,RBsummax=0,center,i;
	center=(left+right)/2; 
	if (left==right) //递归初始条件 
	{
		if(a[right]>0)
			return a[right];
		else
			return 0;
	}
	//分开讨论
	//先讨论左右max
	Lsummax= second_main(a,left,center);
	Rsummax= second_main(a,center+1,right);
	//中间max
	for(i=center;i>=left;i--)  //从中间往左边扫描
	{
		LBsum+=a[i];
		if(LBsum>LBsummax)
			LBsummax=LBsum;
	 } 
	for(i=center+1;i<=right;i++)  //从中间往右边扫描 
	{
		RBsum+=a[i];
		if(RBsum>RBsummax)
			RBsummax=RBsum;
	 } 
	 return Max(Lsummax,Rsummax,RBsummax+LBsummax);
}
int second(int *a,int n) //second 接口函数
{
	return second_main(a,0,n-1);
 } 
 int third(int *a,int n) //算法3 (在线处理)
 {
 	int sum=0,summax=0,i;
	for(i=0;i<n;i++)
	{
	 	sum+=a[i];
	 	if (sum>summax)
	 		summax=sum;
	 	else if(sum<0)//如果当前sum为负 不能使后面的增大 抛弃之 
	 		sum=0;
	} 
	return summax;
 }
int main(int argc, char** argv) {
	int n,a[100],i;
	cout<<"Please input number:";
	cin>>n;
	cout<<"Please input sequence:"<<endl;
	for (i=0;i<n;i++)
		cin>>a[i];
	cout<<"result of First is "<<first(a,n)<<endl;
	cout<<"result of Second is "<<second(a,n)<<endl;
	cout<<"result of Third is "<<third(a,n)<<endl;
	return 0;
}
