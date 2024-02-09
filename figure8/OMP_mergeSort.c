#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include <math.h>
#include <omp.h>

#define SIZE 16777216


void merge(int arr[], int l, int m, int r) 
{ 
	int i, j, k; 
	int n1 = m - l + 1; 
	int n2 = r - m; 

	int* L = (int*)malloc(n1*sizeof(int));
	int* R = (int*)malloc(n2*sizeof(int));

	for (i = 0; i < n1; i++) 
		L[i] = arr[l + i]; 
	for (j = 0; j < n2; j++) 
		R[j] = arr[m + 1 + j]; 

	i = 0; 
 
	j = 0; 

	k = l; 
	while (i < n1 && j < n2) { 
		if (L[i] <= R[j]) { 
			arr[k] = L[i]; 
			i++; 
		} 
		else { 
			arr[k] = R[j]; 
			j++; 
		} 
		k++; 
	} 

	while (i < n1) { 
		arr[k] = L[i]; 
		i++; 
		k++; 
	} 
	

	while (j < n2) { 
		arr[k] = R[j]; 
		j++; 
		k++; 
	} 
	free(L);
	free(R);
} 

void mergeSortSequential(int arr[], int l, int r)
{
	if (l < r) {
		int m = l + (r - l) / 2;

		mergeSortSequential(arr, l, m);
		mergeSortSequential(arr, m + 1, r);

		merge(arr, l, m, r);
	}
}

void mergeSort(int arr[], int l, int r) { 

    int m = l + (r - l) / 2; 
    int numberOfThreads = omp_get_num_threads();

	/********** sequential mergesort on the lower branches **********/
    if(r-l < SIZE/numberOfThreads){
        mergeSortSequential(arr, l, r);

	
    }else{
	/********** parallel mergesort on the upper branches **********/
        if (l < r) { 
            
            #pragma omp task firstprivate(l,r, m) 
            {
                mergeSort(arr, l, m); 
            }
            #pragma omp task firstprivate(l,r, m)
            {
                mergeSort(arr, m + 1, r); 
            }
            #pragma omp taskwait
        }
        
		merge(arr, l, m, r); 	
    }
} 

/*___________Utility functions___________*/

void printArray(int A[SIZE]) 
{ 
	int i; 
	for (i = 0; i < SIZE; i++) 
		printf("%d ", A[i]); 
	printf("\n"); 
} 

void shuffleArray(int shuffeldArray[SIZE]){
	int i;
	int numbers[SIZE];

	for(i = 0; i < SIZE; i++){
		numbers[i] = i;
	}
	for(i = 0; i < SIZE; i++){
		int randomIndex = rand()%(SIZE - i);
		shuffeldArray[i] = numbers[randomIndex];
		numbers[randomIndex] = numbers[SIZE - i-1];
	}
}

/*___________Driver code___________*/
int main() 
{ 
    int i, j;
    srand(time(NULL));
    double time = 0;
    double wt1, wt2;

	/********** Initialize arr through for loop OR shuffleArray **********/
	int* data = (int*)malloc(SIZE*sizeof(int));
	for(i = 0; i < SIZE; i++){
		data[i] = (int)(rand()%100000);
	}
	// shuffleArray(data);

	/********** Paralell Merge Sort **********/
	wt1=omp_get_wtime();
	#pragma omp parallel
	{
		#pragma omp single
		{
			mergeSort(data, 0, SIZE - 1); 
		}
	}
	wt2=omp_get_wtime();
	time += wt2-wt1;

	printf("WallClockTime OpenMP mergeSort = %12.4g\n", time);
	

	/********** Check for errors **********/
	/* data must be initialized using shuffleArray for this check to work */

	// int mistakes = 0;
	// for(j = 0; j < SIZE; j++){
	//     if(data[j] != j){
	//         mistakes++;
	//     }
	// }
	// if(mistakes != 0){
	//     printf("NOT CORRECT, mistakes = %d\n", mistakes);
	// }

    free(data);
	return 0; 
}


