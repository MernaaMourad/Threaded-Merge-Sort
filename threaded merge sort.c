#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int *Arr;
int arrSize;
//arguments passed in thread creation to mergesort
typedef struct arguments{ 
int lo;
int hi;
}arguments;


void Merge(int lo,int mid,int hi){
int i,j,k;
int leftSizeArr=mid-lo+1; //size of left array
int rightSizeArr=hi-mid;  //size of right array
//defining arrays
int leftArr[leftSizeArr];
int rightArr[rightSizeArr];

//filling values in left temp array
for (i=0;i<leftSizeArr;i++)
{
leftArr[i]=Arr[i+lo];
}

//filling values in right temp array
for (j=0;j<rightSizeArr;j++)
{
rightArr[j]=Arr[mid+j+1]; //starts from middle +1 in the original array
}
i=j=0; //i will loop on left array and j will loop onn right array
k=lo; //k will loop on the merged array

while(i<leftSizeArr&&j<rightSizeArr){
//left array and right array both not empty
if(leftArr[i]<rightArr[j])
{
Arr[k]=leftArr[i];
i++;
}
else //right array element smaller than that of left array
{
Arr[k]=rightArr[j];
j++;
}
k++;//wether left array or the right array has the smaller value they will both add to the array of merged elements so its index will be incremented
}


//if one of the arrays ended the other array might still have elements
//if left array still has elements and right array finished
while(i<leftSizeArr)
{Arr[k]=leftArr[i];
i++;
k++;}//add rest of elements of left array
//if right array still has elements and left array finished 
while(j<rightSizeArr)
{Arr[k]=rightArr[j];
j++;
k++;}//add rest of elements of right array 
}

void *mergeSort (void *args) //function that creates threads
{
arguments *data=(arguments *)args; //casting
int lo=data->lo;
int hi=data->hi;
if (lo<hi)//if left index smaller than right index
{
int middle=(lo+hi)/2;
pthread_t th1,th2; // 2 threads to be created for left and right arrays
arguments *leftData=(arguments *)malloc(sizeof(arguments));
arguments *rightData=(arguments *)malloc(sizeof(arguments));
leftData->lo=lo;
leftData->hi=middle;
rightData->lo=middle+1;
rightData->hi=hi;
//creation of threads and calling of mergesort
		pthread_create(&(th1), NULL, mergeSort, leftData);
		pthread_create(&(th2), NULL, mergeSort, rightData);
	
//wait of threads to finish	
		pthread_join(th1, NULL);
		pthread_join(th2, NULL);
	
free(rightData);
free(leftData);
Merge(lo,middle,hi);

}
}




int main()
{
char filename[1024] ;
printf("Please enter file name: ");
fgets(filename,1024,stdin);//gets filename from user
strtok(filename,"\n");//remove new line due to usage of fgets
FILE *fptr;
fptr=fopen(filename,"r"); //open file to read data
if(fptr==NULL)
{
printf("Error can not open the file\n");//NO file with such name
        exit(1);
}
fscanf(fptr,"%d",&arrSize) ;//scan first line in file to determine size of array that is going to be sorted
Arr=malloc(arrSize*sizeof(int)); //create the array with size scanned from file
printf("Unsorted array of size  %d : \n",arrSize);
//scaning all elements needed to be sorted from file and adding them to an array
int i;
//printf("Unsorted array of size  %d : \n",arrSize);
for (i=0;i<arrSize;i++){
//printf("Unsorted array of size  %d : \n",arrSize);
fscanf(fptr,"%d",&Arr[i]) ;// insert elements from file that are seperated by spaces into array 
printf("%d ",Arr[i]); //print elements of unsorted array
}
fclose(fptr);


pthread_t MainTh; //parent thread
arguments *args=(arguments *)malloc(sizeof(arguments));
args->lo=0;
args->hi=arrSize-1;
pthread_create(&MainTh,NULL,mergeSort,(void *)args);//creating 1st thread
pthread_join(MainTh,NULL);//wait 
free(args);
//print sorted array
printf("\nSorted array of size  %d : \n",arrSize);
for (i=0;i<arrSize;i++)
{
printf("%d ",Arr[i]);
}
printf("\n ");


FILE *fp;
fp=fopen("output-sort.txt","w"); //open file to rite data
if(fptr==NULL)
{
printf("Error can not create the file\n");
        exit(1);
}
//printing in file sorted array
fprintf(fp,"Sorted array: ");
for (i=0;i<arrSize;i++)
{
fprintf(fp,"%d ",Arr[i]);
}
fprintf(fp,"\n ");
fclose(fp);
return 0;
}