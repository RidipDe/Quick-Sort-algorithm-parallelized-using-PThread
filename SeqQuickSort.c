
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>

struct timeval startwtime, endwtime;
double seq_time;

int partition( int *a, int low, int high )
  {
  int left, right;
  int pivot_item, pivot;
  pivot_item = a[low];
  pivot =low;
  left=low;
  right = high;
  while ( left < right ) {
    /* Move left while item < pivot */
    while( a[left] <= pivot_item ) left++;
    /* Move right while item > pivot */
    while( a[right] > pivot_item ) right--;
    if ( left < right ){
		int temp=a[left];
		a[left]=a[right];
		a[right]=temp;		
     }
    }
  /* right is final position for the pivot */
  a[low] = a[right];
  a[right] = pivot_item;
  return right;
  }

void QuickSort( int *a, int l, int r)
{
int j;
if( l < r )
{
j = partition( a, l, r);
QuickSort( a, l, j-1);
QuickSort( a, j+1, r);
}
}

int main(int argc, char **argv) {

int size=100;
if (argc > 1)
    {
        size = strtol(argv[1], NULL, 10);
    }
printf("NUmber of elements: %d\n", size);
srandom(42);
    int *values = calloc(size, sizeof(int));
    assert(values && "Allocation failed");
    int i;
    for (i=0 ; i<size ; i++)
    {
        values[i] = random();
    }
int start=0, end=size-1;
printf("Before: Printing first 10 elements\n");
    for (i=0;i<10;i++)
    {
		printf("%d\n",values[i]);
    } 
gettimeofday (&startwtime, NULL);
QuickSort(values, start, end);
gettimeofday (&endwtime, NULL);
printf("After: Printing first 10 elements\n");
    for (i=0;i<10;i++)
    {
		printf("%d\n",values[i]);
    } 
seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6
		      + endwtime.tv_sec - startwtime.tv_sec);
printf("Time taken = %f\n", seq_time);

#ifdef DEBUG
int j;
for (j=0 ; j<size ; j++)
	{
		assert(values[j-1] <= values[j]);
	}
printf("Verified! The array is sorted\n");
#endif
return 0;
}
