#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <sys/time.h>

struct timeval startwtime, endwtime;
double seq_time;

// Macro for swapping two values.
#define SWAP(x,y) do {\
    __typeof__(x) tmp = x;\
    x = y;\
    y = tmp;\
} while(0)

//Partition
int partition(int *array, int left, int right, int pivot)
{
    int pivotValue = array[pivot];
    SWAP(array[pivot], array[right]);
    int storeIndex = left;
    int i;
    for (i=left ; i<right ; i++)
    {
        if (array[i] <= pivotValue)
        {
            SWAP(array[i], array[storeIndex]);
            storeIndex++;
        }
    }
    SWAP(array[storeIndex], array[right]);
    return storeIndex;
}
//Print
void dump(int *array, int size)
{
	int i;
    for (i=0 ; i<size ; i++)
    {
        printf("[%d] %d\n", i, array[i]);
    }
}
//quickSort
void quicksort(int *array, int left, int right)
{
     if (right > left)
     {
        int pivotIndex = left + (right - left)/2;
        pivotIndex = partition(array, left, right, pivotIndex);
        quicksort(array, left, pivotIndex-1);
        quicksort(array, pivotIndex+1, right);
     }
}
//Structure
struct qsort_starter
{
    int *array;
    int left;
    int right;
    int depth;
};

void parallel_quicksort(int *array, int left, int right, int depth);

void* quicksort_thread(void *init)
{
    struct qsort_starter *start = init;
    parallel_quicksort(start->array, start->left, start->right, start->depth);
    return NULL;
}
//Parallel quick sort
void parallel_quicksort(int *array, int left, int right, int depth)
{
    if (right > left)
    {
        int pivotIndex = left + (right - left)/2;
        pivotIndex = partition(array, left, right, pivotIndex);
        if (depth-- > 0)
        {
            struct qsort_starter arg = {array, left, pivotIndex-1, depth};
            pthread_t thread;
            int ret = pthread_create(&thread, NULL, quicksort_thread, &arg);
            assert((ret == 0) && "Thread creation failed");
            parallel_quicksort(array, pivotIndex+1, right, depth);
            pthread_join(thread, NULL);
        }
        //If the depth is 1 then just do regular quick sort
        else
        {	
			quicksort(array, left, pivotIndex-1);
            quicksort(array, pivotIndex+1, right);
        }
    }
}

int main(int argc, char **argv)
{
    int depth =3;
    if (argc > 1)
    {
        depth = strtol(argv[1], NULL, 10);
    }
    int size = 1000;
    if (argc > 2)
    {
        size = strtol(argv[2], NULL, 10);
    }
    printf("Depth supplied: %d\n", depth);
    printf("Size of the array supplied: %d\n", size);
    srandom(42);
    int *values = calloc(size, sizeof(int));
    assert(values && "Allocation failed");
    int i;
    for (i=0 ; i<size ; i++)
    {
        values[i] = random();
    }
    printf("*First 10 elements before sorting*\n");
    for (i=0;i<10;i++)
    {
		printf("%d\n",values[i]);
    } 
    gettimeofday (&startwtime, NULL);
	parallel_quicksort(values, 0, size-1, depth);
	gettimeofday (&endwtime, NULL);
    
    printf("First 10 elements after the sorting\n");
    for (i=0;i<10;i++)
    {
		printf("%d\n",values[i]);
    } 
    seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6
		      + endwtime.tv_sec - startwtime.tv_sec);
	printf("Time taken = %f\n", seq_time);
//Test
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
