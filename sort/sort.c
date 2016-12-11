//
//  main.c
//  Sorting
//
//  Created by Shiuan Lin on 10/22/15.
//

#include <stdio.h>
#include <stdbool.h>    // Bool
#include <stdlib.h>     // atoi();
#include <math.h>       // Srand(); Rand(); 
#include <time.h>       // Srand();

// #define SHOW_ARRAY_MODE

#define SWAP(a, b) { totalMoves += 3; int temp = a; a = b; b = temp;}
#define MAX_BUFFER_SIZE 20      // 20 digits
#define MAX_INPUT_NUMBER 100000 // rand()%MAX_INPUT_NUMBER


typedef enum sort{
    HEAP, MERGE, SHELL, QUICK,
    INSERT, BUBBLE, MIN, END_ENUM
} Sort;
const char *getSortNames[] = {"HEAP", "MERGE", "SHELL","QUICK","INSERT","BUBBLE","MIN",};

int  getSize();

void initArray(int *refArray, int arrayLength);
void copyArray(int *refArrayRestore, int *refArrayBackUp, int arrayLength);
void showArray(int *refArray, int length);
void resetSort(int *myArray, int *myArrayBackUp, int arrayLength);
void beginSort(int *myArray, int arrayLength, Sort selectedSort);

void bubbleSort(int *refArray, int arrayLength);
void heapCheckChild(int *refArray, int currentIndex,int arrayLength);
void heapSort(int *refArray, int arrayLength);
void insertionSort(int *refArray, int arrayLength);
void quickSortRecursive(int *refArray, int headIndex, int tailIndex);
void quickSort(int *refArray, int arrayLength);
void mergeSort(int *refArray, int arrayLength);
void mergeSortRecursive(int *refArray, int *tempArray,int headIndex, int tailIndex);
int  minSortIndex(int *refArray, int currentIndex, int arrayLength);
void minSort(int *refArray, int arrayLength);
void shellsortHalfLength(int *refArray, int arrayLength);

bool running        = true;
int  totalMoves     = 0;
int  totalCompares  = 0;
int  counter        = 0;

int main(int argc, const char * argv[]) {
    
    int arrayLength;
    int *myArray = NULL;
    int *myArrayBackUp = NULL;
    
    while (running) {
#ifndef SHOW_ARRAY_MODE
        printf("Show array mode is off.\n");
        printf("Only shows the first 10 elements\n");
        printf("Uncomment //#define SHOW_ARRAY_MODE\n");
#endif // SHOW_ARRAY_MODE
#ifdef SHOW_ARRAY_MODE
        printf("Show array mode is on.\n");
        printf("Program will print ALL elements\n");
        printf("Comment #define SHOW_ARRAY_MODE to turn off\n");
#endif // SHOW_ARRAY_MODE
        
        printf("\nCurrent number cap is %d (rand()%%MAX_INPUT_NUMBER)\n", MAX_INPUT_NUMBER);
        printf("Size greater than 100000 may take minutes to run\n");
        printf("Larget number might result repeating 0s when printing\n");
        printf("Enter size of the array:\n");
        
        arrayLength = getSize();
        
        if (running == false) {
            // Get EOF, end loop
            break;
        }
        while (arrayLength == 0) {
            // User did not enter vaild value
            arrayLength = getSize();
        }
        
        myArray         = (int *)calloc(arrayLength, sizeof(int));
        myArrayBackUp   = (int *)calloc(arrayLength, sizeof(int));
        
        initArray(myArray, arrayLength); // Assign random number accround to the size
        showArray(myArray, arrayLength); // Shows array if defined
        copyArray(myArrayBackUp, myArray, arrayLength); // Backup Array with random numbers
        resetSort(myArray, myArrayBackUp, arrayLength); // Calls beginSort
    }
    free(myArray);
    free(myArrayBackUp);
}


int getSize(){
    
    static char buffer[MAX_BUFFER_SIZE];
    int returnNumber = 0;
    if(fgets (buffer , MAX_BUFFER_SIZE , stdin) == NULL){
        // If fgets reads End of File, stop running the program.
        running = false;
        return 0;
    }
    for (counter = 0; counter < MAX_BUFFER_SIZE && buffer[counter] != '\n'; counter++) {
        if (buffer[counter] < '0' || buffer[counter] > '9') {
            // If enter char is not number, show Invalid Input
            printf("\nInvalid Input\n");
            return 0;
        }
    }
    returnNumber = atoi(buffer);
    // Assign buffer to returnNumber
    if (returnNumber < 1) {
        printf("You can not have array of this size\n");
        return 0;
    }
    printf("Entered:%d\n",returnNumber);
    return returnNumber;
}

void resetSort(int *myArray, int *myArrayBackUp, int arrayLength){
    
    Sort selectedSort;
    clock_t begin, end;
    double totalTime;
    
    // Sorts the exect same random numbers over and over
    for (selectedSort = HEAP; selectedSort != END_ENUM; selectedSort++) {
        copyArray(myArray, myArrayBackUp, arrayLength);
        begin = clock();
        beginSort(myArray, arrayLength, selectedSort);
        end = clock();
        totalTime = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Random Array (Checked: %d Moved: %d Time: %.5lf s) Using %s Sort \n", totalCompares, totalMoves, totalTime, getSortNames[selectedSort]);
        showArray(myArray, arrayLength);
        totalCompares = 0; totalMoves = 0;
        }
    
    // Sorts the sorted array
    for (selectedSort = HEAP; selectedSort != END_ENUM; selectedSort++) {
        // Array is sorted, doesn't need to copy the random number back
        begin = clock();
        beginSort(myArray, arrayLength, selectedSort);
        end = clock();
        totalTime = (double)(end - begin) / CLOCKS_PER_SEC;

        printf("Sorted Array (Checked: %d Moved: %d Time: %.5lf s) Using %s Sort \n", totalCompares, totalMoves, totalTime, getSortNames[selectedSort]);
        showArray(myArray, arrayLength);
        totalCompares = 0; totalMoves = 0;
    }
}

void beginSort(int *myArray, int arrayLength, Sort selectedSort){
    switch (selectedSort) {
        case HEAP:
            heapSort(myArray, arrayLength);
            break;
        case MERGE:
            mergeSort(myArray, arrayLength);
            break;
        case SHELL:
            shellsortHalfLength(myArray, arrayLength);
            break;
        case INSERT:
            insertionSort(myArray, arrayLength);
            break;
        case BUBBLE:
            bubbleSort(myArray, arrayLength);
            break;
        case MIN:
            minSort(myArray, arrayLength);
            break;
        case QUICK:
            quickSort(myArray, arrayLength);
            break;
        case END_ENUM:
            break;
    }
}

void initArray(int *refArray, int arrayLength){
    time_t t;
    srand((unsigned)time(&t));
    for (counter = 0; counter < arrayLength; counter++) {
        refArray[counter] = rand()%MAX_INPUT_NUMBER;
    }
}

void copyArray(int *refArrayRestore, int *refArrayBackUp, int arrayLength){
    for (counter = 0; counter < arrayLength; counter++) {
        refArrayRestore[counter] = refArrayBackUp[counter];
    }
}

void showArray(int *refArray, int arrayLength){
#ifdef SHOW_ARRAY_MODE
    for (counter = 0; counter < arrayLength; counter++) {
        printf("%d ", refArray[counter]);
    }
    puts("\n");
#endif // SHOW_ARRAY_MODE
#ifndef SHOW_ARRAY_MODE
    for (counter = 0; counter < 10; counter++) {
        printf("%d ", refArray[counter]);
    }
    puts("\n");
#endif // SHOW_ARRAY_MODE
}


void bubbleSort(int *refArray, int arrayLength){
    int leftPush, currentElement;
    for (leftPush = 0; leftPush < arrayLength; leftPush++) {
        // Start from the very first element, bubble up the smallest element to this location.
        for (currentElement = 0;
             currentElement < (arrayLength -1 - leftPush);
             currentElement++) {
            // Compare current element and the next element, bubble up the smaller one.
            totalCompares ++;
            if (refArray[currentElement] > refArray[currentElement +1]) {
                SWAP(refArray[currentElement], refArray[currentElement +1]);
            }
        }
    }
}


void heapCheckChild(int *refArray, int currentIndex, int arrayLength){
    int parent = currentIndex;
    int child = parent*2 + 1; // Start index is 0, thus add 1;
    int otherChild = child + 1;
    
    while (child < arrayLength) {
        totalCompares++;
        if (otherChild < arrayLength) {
            // If otherChild is >= than the arrayLength, it's already at the end of the array.
            if (refArray[child] < refArray[otherChild]) {
                // Check which child is bigger.
                // If otherChile is larger(right child), child++ to make it right child
                child++;
            }
        }

        totalCompares++;
        if (refArray[parent] > refArray[child]) {
            // Parent is bigger than the bigger child. No swapping need. Exit loop.
            break;
        } else {
            // Child is bigger than the parent. Swap them.
            // Set child as the new parent. And set child to next level.
            // Loop contiunes checking the parent.
            SWAP(refArray[parent], refArray[child]);
            parent = child;
            child = 2 * parent + 1;
            otherChild = child + 1;
        }
    }
}

void heapSort(int *refArray, int arrayLength){
    for (counter = (arrayLength - 2); counter >= 0; counter --) {
        // Build the array into a heap.
        // Make the biggest child to head.
        heapCheckChild(refArray, counter, arrayLength);
    }
    
    for (counter = 0; counter < arrayLength; counter++) {
        SWAP(refArray[0], refArray[arrayLength - counter - 1]);
        // Take the top and swap with the last element
        heapCheckChild(refArray, 0, arrayLength - counter -1);
        // Check the new top and swap with the bigger child
        // And build array with the smallest element as head
    }
}


void insertionSort(int *refArray, int arrayLength){
    totalCompares = -1; //The first case does not have perv case
    int currentIndex, currentElement, checker;
    for (currentIndex = 0;
         currentIndex < arrayLength;
         currentIndex++) {
        checker = refArray[currentIndex];
        // set checker to the current index, and check where it should be inserted to
        currentElement = currentIndex -1;
        // set currentElement (we check against to) to the pervious element of the Index
        totalCompares ++;
        while((currentElement >= 0) && (refArray[currentElement] > checker)){
            // Insert the element to where it should go.
            refArray[currentElement +1] = refArray[currentElement];
            totalMoves ++;
            totalCompares ++;
            currentElement--;
        }
        totalCompares ++;
        if (refArray[currentElement + 1] != checker) {
            // Put back the checker is it is not in the right position
            refArray[currentElement + 1] = checker;
            totalMoves ++;
        }
    }
}



void mergeSortRecursive(int *refArray, int *tempArray,int headIndex, int tailIndex){
    if (headIndex >= tailIndex) {
        // Single element is sorted.
        return;
    }
    
    int length      = tailIndex - headIndex;
    // Get the length of this array.
    int midIndex    = length/2 + headIndex;
    // Get the mid index for futher breakdown
    
    int startLeft   = headIndex;
    int endLeft     = midIndex;
    int startRight  = midIndex+1;
    int endRight    = tailIndex;
    
    // Recursively break the index down to signle element
    mergeSortRecursive(refArray, tempArray, startLeft, endLeft);
    mergeSortRecursive(refArray, tempArray, startRight, endRight);
    
    int Index = headIndex;
    totalCompares++;
    while (startLeft <= endLeft && startRight <= endRight) {
        // Started to pop back, and sort the elements that poped back
        // While index from both start do not reach to the end
        totalCompares++;
        if (refArray[startLeft] < refArray[startRight]) {
            // Assign the smaller element to the head of the temp array.
            // Then increase the index of the temp array and increase the index of the element we just copied.
            tempArray[Index++] = refArray[startLeft++];
            totalMoves++;
        } else {
            tempArray[Index++] = refArray[startRight++];
            totalMoves++;
        }
    } // while startLeft
    
    while (startLeft <= endLeft) {
        // Because one side sometimes has more element left than the other, double check each size.
        totalCompares++;
        totalMoves++;
        tempArray[Index++] = refArray[startLeft++];
    }
    while (startRight <= endRight) {
        totalCompares++;
        totalMoves++;
        tempArray[Index++] = refArray[startRight++];
    }
    
    for (Index = headIndex; Index <= tailIndex; Index++) {
        // Recursively put correct value back to the main array
        totalMoves++;
        refArray[Index] = tempArray[Index];
    }
    
}

void mergeSort(int *refArray, int arrayLength){
    // Helper function that passes in right argument and create a temp array to store values.
    int *tempArray = (int*)malloc(sizeof(int*));
    mergeSortRecursive(refArray, tempArray, 0, arrayLength-1);
    free(tempArray);
}

int minSortIndex(int *refArray, int currentIndex, int arrayLength){
    int min = currentIndex;
    int index;
    for (index = currentIndex; index < arrayLength -1; index++) {
        min = refArray[index] < refArray[min]? index:min;
        // Is this location smaller than our current min? set min to this location
        totalCompares ++;
    }
    return min;
} // Professor Darrell Long's code

void minSort(int *refArray, int arrayLength){
    for (counter = 0; counter < arrayLength; counter++) {
        int min = minSortIndex(refArray, counter, arrayLength);
        if (min != counter) {
            SWAP(refArray[min], refArray[counter]);
        }
    }
} // Professor Darrell Long's code


void quickSortRecursive(int *refArray, int headIndex, int tailIndex){
    int rightEnd, midPivot, leftStart;
    if (headIndex >= tailIndex) {
        // Single element, no need to sort
        return;
    }
    
    midPivot  = refArray[tailIndex];
    leftStart = headIndex;
    rightEnd  = tailIndex - 1;
    
    while (leftStart < rightEnd) {
        while (refArray[leftStart] < midPivot && leftStart < rightEnd) {
            // When left side is smaller than the pivot, it is in the corret side.
            // Increase right index and check against pivot again.
            totalCompares++;
            leftStart++;
        }
        while (refArray[rightEnd] >= midPivot &&  leftStart < rightEnd) {
            // When right side is greater than the pivot, it is in the correct side.
            // When the number is equal to Pivot, assume it is in the correct side
            // Decrease left index and check against pivot again.
            totalCompares++;
            rightEnd--;
        }
        // Find something that's is not in the right side, swap them.
        if (refArray[rightEnd] != refArray[leftStart]) {
            SWAP(refArray[rightEnd], refArray[leftStart]);
        }
    } // while rightEnd < leftStart
    
    if (refArray[leftStart] >= refArray[tailIndex]) {
        // Check if the current Index (left = right) is greater or equal to pivot(tail).
        // SWAP if greater
        SWAP(refArray[tailIndex], refArray[leftStart]);
    } else {
        // The pivot is not in the correct position, however, correct side.
        // Increase left so Recursive call has correct index.
        leftStart++;
    }
    
    quickSortRecursive(refArray, headIndex, leftStart-1);
    // Sort the smaller side.
    quickSortRecursive(refArray, leftStart+1, tailIndex);
    // Sort the larger side.
    
}

void quickSort(int *refArray, int arrayLength){
    // Helper function that passes in right argument
    quickSortRecursive(refArray, 0, arrayLength-1);
}


void shellsortHalfLength(int *refArray, int arrayLength){
    int gap, startWithGap, reducedIndex, temp;
    for (gap = arrayLength/2; gap > 0; gap = gap/2) {
        // Size of the gap is divied by 2 every cycle.
        for (startWithGap = gap;
             startWithGap< arrayLength;
             startWithGap ++) {
            // Start with the gap, increase by 1 until it reach the size of the arrayLength.
            temp = refArray[startWithGap];
            totalCompares++;
            for (reducedIndex = startWithGap - gap;
                 reducedIndex >= 0 && refArray[reducedIndex] > temp;
                 reducedIndex = reducedIndex-gap) {
                totalCompares++;
                totalMoves++;
                refArray[reducedIndex + gap] = refArray[reducedIndex];
                
            } // for reduced index
            totalMoves++;
            if (refArray[reducedIndex + gap] != temp) {
                refArray[reducedIndex + gap] = temp;
            }
        } // for start with gap
    } // for gap
}


