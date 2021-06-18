#include<stdio.h>
int main()
{
	int a[500][500],n,i,j,l,m=0,k,temp;
	int v[500];
	int b[500][500];
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++){
			a[i][j] = i;
			b[i][j]=-1;
		}
	}
	i=0;
	j=0;
	k=0;
	printf("%d ",a[i][j]);
	v[k++] = a[i][j];

	do{
		printf("%d ",a[i][++j]);
		v[k++]=a[i][j];

		for(i=i+1,j=j-1;j>=0;j--,i++){
			printf("%d ",a[i][j]);
			v[k++]=a[i][j];
		}
		i--;
		j++;
		if(i==(n-1))
			break;

		printf("%d ",a[++i][j]);
		v[k++]=a[i][j];

		for(i=i-1,j=j+1 ;i>=0;i--,j++){
			printf("%d ",a[i][j]);
			v[k++]=a[i][j];
		}

		i++;
		j--;
	}while(i+j<n-1);
	do
	{
		printf("%d ",a[++i][j]);
		v[k++]=a[i][j];

		for(i=i-1,j=j+1;j<n;j++,i--){
			printf("%d ",a[i][j]);
			v[k++]=a[i][j];
		}
		i++;
		j--;

		if(j==(n-1)&&i==(n-1))
			break;
		
		printf("%d ",a[++i][j]);
		v[k++]=a[i][j];
		
		for(i=i+1,j=j-1;i<n;i++,j--){
			printf("%d ",a[i][j]);
			v[k++]=a[i][j];
		}
		i--;
		j++;
	}while(i<n-1||j<n-1);
	printf("%d ",a[n-1][n-1]);
	v[k++]=a[n-1][n-1];

	printf("\nzigzag\n");

	for(i=0;i<n*n;i++)
		printf("%d ", v[i]);

	printf("");

	printf("\nundo zigzag\n");


	i=0;
	j=0;
	k=0;

	b[0][0] = v[0];

	do{
		j++;
		b[i][j] = v[++k];
		for(i=i+1,j=j-1;j>=0;j--,i++){
			b[i][j]= v[++k];
		}
		i--;
		j++;
		if(i==(n-1))
			break;

		i++;
		b[i][j] = v[++k];

		for(i=i-1,j=j+1 ;i>=0;i--,j++){
			b[i][j] = v[++k];
		}

		i++;
		j--;


	}while(i+j<n-1);
	do{
		b[++i][j]=v[++k];

		for(i=i-1,j=j+1;j<n;j++,i--){
			b[i][j]=v[++k];
		}
		i++;
		j--;

		if(j==(n-1)&&i==(n-1)){
			break;
		}
		b[++i][j]=v[++k];
		
		for(i=i+1,j=j-1;i<n;i++,j--){
			b[i][j]=v[++k];
		}
		i--;
		j++;
	}while(i<n-1||j<n-1);
	b[n-1][n-1] = v[(n*n)-1];

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			printf("%d ", b[i][j]);
		}
		printf("\n");
	}

	return 0;
}