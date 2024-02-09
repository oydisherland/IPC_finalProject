// Quick sort in C

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#ifdef _OPENMP
#include <omp.h>
#endif

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
//pragma omp paralell for
{
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
}
  // swap the pivot element with the greater element at i
  swap(&array[i + 1], &array[high]);
  
  // return the partition point
  return (i + 1);
}

void quickSort(int array[], int low, int high) {
    int i;
  if (low < high) { // sjekker at man har mer enn ett element ii arrayet. 
    //#pragma omp paralell
    

      // find the pivot element such that
      // elements smaller than pivot are on left of pivot
      // elements greater than pivot are on right of pivot
      int pi = partition(array, low, high);

      // recursive call on the left of pivot
      //#pragma omp task shared(array)
        quickSort(array, low, pi - 1);

      // recursive call on the right of pivot
      //#pragma omp task shared(array)
        quickSort(array, pi + 1, high);

  }
}


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
int main() {
    int nElem = 16777216;
    int data[nElem];
    int i;

    for (i = 0; i < nElem; ++i) {
        data[i] = (int)(rand()%100000);
    }

  struct timespec start_time, end_time;
  double wt1,wt2;
  int n_threads, lenght_subarray, start_posistion, round, maxRounds;
  
  int n = sizeof(data) / sizeof(data[0]);
  
  //printf("Unsorted Array\n");
  //printArray(data, n);
  
  // perform quicksort on data
  clock_gettime(CLOCK_REALTIME, &start_time);
    wt1 = omp_get_wtime();
      //quickSort(data, 0, n - 1);
    #pragma omp parallel 
    {
        n_threads = omp_get_num_threads();
        lenght_subarray = n / n_threads;
        //#pragma omp for schedule(guided) nowait
        //#pragma omp single
        #pragma omp for
        for(i = 0; i < n_threads; i++){
            quickSort(data, lenght_subarray * i, (lenght_subarray * i) + lenght_subarray -1 ); //en loop per processor
        }
    }

    round = 0;
        maxRounds = (int)log2(n_threads);
        while(round < maxRounds){
            //printf("\n round: %d  of max_round: %d\n", round, maxRounds);
            //#pragma omp for nowait schedule(runtime)
            //#pragma omp parallel for
              #pragma omp single
              for(i = 0; i < n; i += lenght_subarray*2){
                  merge(data, i, i + lenght_subarray -1, i + (2* lenght_subarray) -1);
              }
              lenght_subarray = lenght_subarray * 2;
              round++;
        }

    wt2 = omp_get_wtime();
  clock_gettime(CLOCK_REALTIME, &end_time);


  double elapsed_time = ((end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec)/1.0e9);
  
  //printf("Sorted array in ascending order: \n");
  //printArray(data, n);
  //printf(" serial [time(sec), n] = [%.9f, %d] \n", elapsed_time, n);
  printf( "\n OpenMP quickMerge f1, single [sec]= %12.10f sec\n", wt2-wt1 );
}