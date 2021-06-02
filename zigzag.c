#include<stdio.h>
int main()
{
	int a[500][500],n,i,j,l,m=0,k,temp;
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
			a[i][j] = i;
	}
	i=0;
	j=0;
	printf("%d ",a[i][j]);
	do{
		printf("%d ",a[i][++j]);

		for(i=i+1,j=j-1;j>=0;j--,i++)
			printf("%d ",a[i][j]);
		i--;
		j++;
		if(i==(n-1))
			break;

		printf("%d ",a[++i][j]);

		for(i=i-1,j=j+1 ;i>=0;i--,j++)
			printf("%d ",a[i][j]);

		i++;
		j--;
	}while(i+j<=n);
	do
	{
		printf("%d ",a[i][++j]);

		for(i=i-1,j=j+1;j<n;j++,i--)
			printf("%d ",a[i][j]);
		i++;
		j--;

		if(j==(n-1)&&i==(n-1))
			break;
		
		printf("%d ",a[++i][j]);
		
		for(i=i+1,j=j-1;i<n;i++,j--)
			printf("%d ",a[i][j]);
		i--;
		j++;
	}while(i<n||j<n);
	return 0;
}