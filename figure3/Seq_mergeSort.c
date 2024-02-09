// C program for Merge Sort 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>

#define SIZE 16777216

// Merges two subarrays of arr[]. 
// First subarray is arr[l..m] 
// Second subarray is arr[m+1..r] 
void merge(int arr[], int l, int m, int r) 
{ 
	int i, j, k; 
	int n1 = m - l + 1; 
	int n2 = r - m; 

	// Create temp arrays 
	int* L = (int*)malloc(n1*sizeof(int));
	int* R = (int*)malloc(n2*sizeof(int));

	// Copy data to temp arrays 
	// L[] and R[] 
	for (i = 0; i < n1; i++) 
		L[i] = arr[l + i]; 
	for (j = 0; j < n2; j++) 
		R[j] = arr[m + 1 + j]; 

	// Merge the temp arrays back 
	// into arr[l..r] 
	// Initial index of first subarray 
	i = 0; 

	// Initial index of second subarray 
	j = 0; 

	// Initial index of merged subarray 
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

	// Copy the remaining elements 
	// of L[], if there are any 
	while (i < n1) { 
		arr[k] = L[i]; 
		i++; 
		k++; 
	} 

	// Copy the remaining elements of 
	// R[], if there are any 
	while (j < n2) { 
		arr[k] = R[j]; 
		j++; 
		k++; 
	} 
	free(R);
	free(L);
} 

// l is for left index and r is 
// right index of the sub-array 
// of arr to be sorted 
void mergeSort(int arr[], int l, int r) 
{ 
	if (l < r) { 
		// Same as (l+r)/2, but avoids 
		// overflow for large l and r 
		int m = l + (r - l) / 2; 

		// Sort first and second halves 
		mergeSort(arr, l, m); 
		mergeSort(arr, m + 1, r); 

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
    unsigned long  time = 0;
	unsigned long billion = 1000000000;
    struct timespec ta, tb;
   
	/********** Initialize arr through for loop OR shuffleArray **********/
	int* data = (int*)malloc(SIZE*sizeof(int));
	for(i = 0; i < SIZE; i++){
		data[i] = (int)(rand()%100000);
	}
	//shuffleArray(data);

	/********** Sequential Merge Sort **********/
	clock_gettime(CLOCK_REALTIME, &ta);

	mergeSort(data, 0, SIZE - 1); 

	clock_gettime(CLOCK_REALTIME, &tb);
	time += (billion * (tb.tv_sec - ta.tv_sec) + tb.tv_nsec - ta.tv_nsec);

	printf("WallClockTime = %lu sec\n", time/(1000)/1000000);

	/********** Check for errors **********/
	/* data must be initialized using shuffleArray for this check to work*/

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


