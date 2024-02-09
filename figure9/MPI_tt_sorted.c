  // MergeQuick sort in C w /MPI
#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>


// function to swap elements
void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

// function to find the partition position
int partition(int array[], int low, int high) {
    int j;
  
  // select the rightmost element as pivot
  int pivot = array[high];
  
  // pointer for greater element
  int i = (low - 1);

  // traverse each element of the array
  // compare them with the pivot

    for ( j = low; j < high; j++) {
      if (array[j] <= pivot) {
        // if element smaller than pivot is found
        // swap it with the greater element pointed by i
        i++;
        //kan gjøres mer effektiv ved å legge inn en sjekk om i == j 
        // swap element at i with element at j
        swap(&array[i], &array[j]);
      }
    }

  // swap the pivot element with the greater element at i
  swap(&array[i + 1], &array[high]);
  
  // return the partition point
  return (i + 1);
}

void quickSort(int array[], int low, int high) {
    int i;
  if (low < high) { // sjekker at man har mer enn ett element ii arrayet. 
    

    int pi = partition(array, low, high);
    
    // recursive call on the left of pivot
    quickSort(array, low, pi - 1);
    
    // recursive call on the right of pivot
    quickSort(array, pi + 1, high);
  }
}

// function to print array elements
void printArray(int array[], int size) {
    int i;
  for ( i = 0; i < size; ++i) {
    printf("%d  ", array[i]);
  }
  printf("\n");
}

void merge(int arr[], int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 = r - m; 
  
    // Create temp arrays 
    int L[n1], R[n2]; 
  
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
}

// main function
int main(int argc, char *argv[]) {
    int rank, size, i, j, start_posistion, lengt_subarrays, round;
    double time1, time2;
    
    int nElem = 16777216;
    int data[nElem];

    for (i = 0; i < nElem; ++i) {
        data[i] = i;
    }

    int n = sizeof(data) / sizeof(data[0]);
    round = 0;
    //printf("n = %d ", n);

    //printf("Unsorted Array\n");
    //printArray(data, n);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //the ind thread
    MPI_Comm_size(MPI_COMM_WORLD, &size); //total

    if(n % size != 0){
        printf("\n!ERROR! array should be devisable with #processors \n");
        return 0;
    } 
  
    lengt_subarrays = n / size;
    start_posistion = lengt_subarrays * rank;
    int maxRounds = (int)log2(size);
    //printf("\n this is  maxROunds: %d \n", maxRounds);

    time1 = MPI_Wtime();
    quickSort(data, start_posistion, start_posistion + lengt_subarrays - 1); // n-1 org.
    // deler likt mellom prosessorene
    //barriere??
    while(round < maxRounds){
      if(rank % (int)pow(2,round) == 0){
        if( (rank / (int)pow(2,round)) % 2 == 0 ){ //even number
          MPI_Recv(&data[start_posistion + lengt_subarrays], lengt_subarrays, MPI_INT, rank + (int)pow(2, round), round, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          merge(data, start_posistion, start_posistion + lengt_subarrays -1, start_posistion + (2 * lengt_subarrays) -1);
          //printf("[rank, round] = [%d, %d]", rank, round);

        } else{ // odd number  
          MPI_Send(&data[start_posistion], lengt_subarrays, MPI_INT, rank - (int)pow(2, round), round, MPI_COMM_WORLD);
          //printf("[rank, round] = [%d, %d]", rank, round);
        }
      }
      round ++;
      lengt_subarrays = lengt_subarrays * 2;
    } 
    time2 = MPI_Wtime();

    if (rank ==  0){
      //printf("Sorted array in ascending order: \n");
      //printArray(data, n); 
      printf(" \n [Elapsed time MPI quickMerge SORTED (s), number of prosessors, arraySize] = [%f, %d, %d] \n", time2-time1, size, n);
      
    }
    MPI_Finalize();


  //printf("routine 1, serial [time(sec), n] = [%.9f, %d] \n", elapsed_time, n);

  //kjører akk som i videoen linket i OneNote
} //timing 
 