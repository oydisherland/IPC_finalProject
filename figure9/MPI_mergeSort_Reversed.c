#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include <math.h>
#include <mpi.h>

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


void mergeSort(int arr[], int l, int r) 
{ 
	if (l < r) { 

		int m = l + (r - l) / 2; 

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
int main(int argc, char** argv) 
{ 
    int i, j;
    srand(time(NULL));
    unsigned long  time = 0;
    double w1, w2;
   
   /********** Initialize arr through for loop OR shuffleArray **********/
    int* arr = (int*)malloc(SIZE*sizeof(int));
	for(i = 0; i < SIZE; i++){
		arr[i] = SIZE - 1 - i;
	}
	// shuffleArray(arr);


	int world_rank;
	int world_size;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    /********** Divide the array in equal-sized chunks **********/
	int size = SIZE/world_size;
	
	/********** Send each subarray to each process **********/
	int *sub_array = malloc(SIZE * sizeof(int));

	w1 = MPI_Wtime();

    MPI_Scatter(arr, size, MPI_INT, sub_array, size, MPI_INT, 0, MPI_COMM_WORLD);

	mergeSort(sub_array, 0, size - 1);

	/********** Merge together each subarray **********/
	int totalNumberOfMergeSteps = log2(world_size);
	for(i = 1; i <= totalNumberOfMergeSteps; i++){
		int numberOfMerges = pow(2,i);

		//Receiving processors
		if(world_rank%numberOfMerges == 0){
			int receiveFrom = world_rank + numberOfMerges/2;
			MPI_Recv(sub_array + size*numberOfMerges/2, size*numberOfMerges/2, MPI_INT, receiveFrom, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			merge(sub_array, 0, size*numberOfMerges/2 - 1, size*numberOfMerges-1);
		}
		//Sending processors
		if(world_rank%numberOfMerges == numberOfMerges/2){
			int sendTo = world_rank - (world_rank%numberOfMerges);
			MPI_Send(sub_array, size*numberOfMerges/2, MPI_INT, sendTo, 0, MPI_COMM_WORLD);
		}
	}

	w2 = MPI_Wtime();

	if(world_rank == 0){

		printf("%d, time with %d num of procs is:  %12.4g sec\n", SIZE, world_size, w2-w1);
		

		/********** Check for errors **********/
		/****data in arr must be initialized using shuffle array for this check to work****/

		// int mistakes = 0;
        // for(i = 0; i < SIZE; i++){
        //     if(sub_array[i] != i){
        //         mistakes++;
        //     }
        // }
        // if(mistakes != 0){
        //     printf("NOT CORRECT, mistakes = %d\n", mistakes);
        // }
    }
	
	free(sub_array);
    free(arr);

	/********** Finalize MPI **********/
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	
    return 0; 
}