// Quick sort in C

#include <stdio.h>
#include <time.h>
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
    
    // find the pivot element such that
    // elements smaller than pivot are on left of pivot
    // elements greater than pivot are on right of pivot
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

// main function
int main() {
  //int data[] = {8, 7, 7, 1, 0, 9, 6,5};
  struct timespec start_time, end_time;
    int nElem = 16777216;
    int data[nElem];
    int i;

    for (i = 0; i < nElem; ++i) {
        data[i] = (int)(rand()%100000);
    }
  
  int n = sizeof(data) / sizeof(data[0]);
  
  //printf("Unsorted Array\n");
  //printArray(data, n);
  
  // perform quicksort on data
  clock_gettime(CLOCK_REALTIME, &start_time);
  quickSort(data, 0, n - 1);
  clock_gettime(CLOCK_REALTIME, &end_time);
  double elapsed_time = ((end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec)/1.0e9);
  
  //printf("Sorted array in ascending order: \n");
  //printArray(data, n);
  printf("serial quickSort [time(sec), n] = [%.9f, %d] \n", elapsed_time, n);

  //kjører akk som i videoen linket i OneNote
}