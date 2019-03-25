#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cstring>
#include <fstream>
#include <unistd.h>
#include <utility>
#include <algorithm>
#include <functional>

using namespace std;

#define OPTIMISTIC_INPUT_PREFIX "optimistic_input_"
#define PESSIMISTIC_INPUT_PREFIX "pessimistic_input_"
#define RANDOM_INPUT_PREFIX "random_input_"
#define OPTIMISTIC_OUTPUT_PREFIX "optimistic_output_"
#define PESSIMISTIC_OUTPUT_PREFIX "pessimistic_output_"
#define RANDOM_OUTPUT_PREFIX "random_output_"
#define EXTENSION ".txt"
#define AMOUNT_OF_NUMBERS 10000

const std::string currentDateTime() {
    time_t now = time(nullptr);
    struct tm tstruct{};
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d||%X", &tstruct);

    return buf;
}

////// quicksort algorithm
// lomuto

int partition ( int array[],int start ,int end) {
    int i = start + 1;
    int piv = array[start] ;            //make the first element as pivot element.
    for(int j =start + 1; j <= end ; j++ )  {
        /*rearrange the array by putting elements which are less than pivot
           on one side and which are greater that on other. */

        if (array[j] < piv) {
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
            i+= 1;
        }
    }
    //put the pivot element in its proper place.
    int temp = array[start];
    array[start] = array[i-1];
    array[i-1] = temp;
    return i-1;                      //return the position of the pivot
}

void quickSortPivFirst (int array[] ,int start , int end) {
    if(start < end) {
        //stores the position of pivot element
        int pivPos = partition (array,start , end ) ;
        quickSortPivFirst (array,start , pivPos -1);    //sorts the left side of pivot.
        quickSortPivFirst (array,pivPos +1 , end) ; //sorts the right side of pivot.
    }
}

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
// hoare
int partitionPivLast(int arr[], int low, int high)
{
    int pivot = arr[low];
    int i = low - 1, j = high + 1;

    while (true)
    {
        // Find leftmost element greater than
        // or equal to pivot
        do
        {
            i++;
        } while (arr[i] < pivot);

        // Find rightmost element smaller than
        // or equal to pivot
        do
        {
            j--;
        } while (arr[j] > pivot);

        // If two pointers met.
        if (i >= j)
            return j;

        swap(arr[i], arr[j]);
    }
}

/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void quickSortPivLast(int arr[], int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partitionPivLast(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSortPivLast(arr, low, pi);
        quickSortPivLast(arr, pi + 1, high);
    }
}

// middle pivot

void quickSortMiddle(int a[], int left, int right)
{
    if(left >= right) return;
    int pivot = a[left + (right - left)/2];
    int leftI = left-1;
    int rightI = right+1;
    while(1)
    {
        while(a[++leftI] < pivot);
        while(a[--rightI] > pivot);
        if(leftI >= rightI)break;
        swap(a[leftI], a[rightI]);
    }
    quickSortMiddle(a,left,rightI);
    quickSortMiddle(a,rightI+1,right);
}

int partitionPivRandom(int arr[], int low, int high)
{
    int pivot = arr[high]; // pivot
    int i = (low - 1); // Index of smaller element

    for (int j = low; j <= high - 1; j++) {

        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot) {

            i++; // increment index of smaller element
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

// Generates Random Pivot, swaps pivot with
// end element and calls the partition function
int partition_r(int arr[], int low, int high)
{
    // Generate a random number in between
    // low .. high
    srand(time(NULL));
    int random = low + rand() % (high - low);

    // Swap A[random] with A[high]
    swap(arr[random], arr[high]);

    return partitionPivRandom(arr, low, high);
}

/* The main function that implements QuickSort
arr[] --> Array to be sorted,
low --> Starting index,
high --> Ending index */
void quickSortPivRandom(int arr[], int low, int high)
{
    if (low < high) {

        /* pi is partitioning index, arr[p] is now
        at right place */
        int pi = partition_r(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSortPivRandom(arr, low, pi - 1);
        quickSortPivRandom(arr, pi + 1, high);
    }
}

/////////////

void saveInputToFile(int *array, int numbersAmount, string inputFilePrefix) {
    string inputFileName =
            "/Volumes/SDXC/quick-sort-test-data/input/" + inputFilePrefix + currentDateTime() + EXTENSION;
    ofstream arrayInputData(inputFileName, ios::out | ios::app);

    if (arrayInputData.is_open()) {
        arrayInputData << "Array Data:" << endl;

        for (int i = 0; i < numbersAmount; i++) {
            arrayInputData << array[i] << " ";
        }

        arrayInputData.close();

        cout << "Copy input array to file done!" << endl;
    } else {
        cout << "Unable to open file!" << endl;
    }
}

void saveOutputToFile(int *array, int numbersAmount, string outputFilePrefix) {
    string outputFileName =
            "/Volumes/SDXC/quick-sort-test-data/output/" + outputFilePrefix + currentDateTime() + EXTENSION;
    ofstream arrayOutputData(outputFileName, ios::out | ios::app);

    if (arrayOutputData.is_open()) {
        arrayOutputData << "Array Data:" << endl;

        for (int i = 0; i < numbersAmount; i++) {
            arrayOutputData << array[i] << " ";
        }

        arrayOutputData.close();

        cout << "Copy output array to file done!" << endl;
    } else {
        cout << "Unable to open file!" << endl;
    }
}

int *createOptimisticArray(int numbersAmount) {
    int *array = new int[numbersAmount];
    for (int i = 0; i < numbersAmount; i++) // wczytywanie liczb do tablicy
    {
        array[i] = i;
    }
    return array;
}

int *createPessimisticArray(int numbersAmount) {
    int *array = new int[numbersAmount];
    for (int i = 0; i < numbersAmount; i++)
    {
        array[i] = numbersAmount - i;
    }
    return array;
}

int* createRandomArray(int arraySize) {
    int* array = new int[arraySize];
    for (int i = 0; i < arraySize; i++) {
        srand(time(NULL));
        array[i] = rand() % arraySize;
    }
    return array;
}

auto measureTimeExecution(int *array, int numbersAmount) {
    chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();
    quickSortPivFirst(array, 0, numbersAmount-1);
    chrono::high_resolution_clock::time_point finishTime = chrono::high_resolution_clock::now();

    return chrono::duration_cast<chrono::milliseconds>(finishTime - startTime).count();
}

int main() {

    int *optimisticArray = createOptimisticArray(AMOUNT_OF_NUMBERS);
    //saveInputToFile(optimisticArray, AMOUNT_OF_NUMBERS, OPTIMISTIC_INPUT_PREFIX);
    int *pessimisticArray = createPessimisticArray(AMOUNT_OF_NUMBERS);
    //saveInputToFile(pessimisticArray, AMOUNT_OF_NUMBERS, PESSIMISTIC_INPUT_PREFIX);
    int *randomArray = createRandomArray(AMOUNT_OF_NUMBERS);
    //saveInputToFile(randomArray, AMOUNT_OF_NUMBERS, RANDOM_INPUT_PREFIX);


    cout << "Optimistic (milisec): " << measureTimeExecution(optimisticArray, AMOUNT_OF_NUMBERS) << endl;
    //saveOutputToFile(optimisticArray, AMOUNT_OF_NUMBERS, OPTIMISTIC_OUTPUT_PREFIX);
    cout << "Pessimistic (milisec): " << measureTimeExecution(pessimisticArray, AMOUNT_OF_NUMBERS) << endl;
    //saveOutputToFile(pessimisticArray, AMOUNT_OF_NUMBERS, PESSIMISTIC_OUTPUT_PREFIX);
    cout << "Random (milisec): " << measureTimeExecution(randomArray, AMOUNT_OF_NUMBERS) << endl;
    //saveOutputToFile(randomArray, AMOUNT_OF_NUMBERS, RANDOM_OUTPUT_PREFIX);

    delete[] optimisticArray;
    delete[] pessimisticArray;
    delete[] randomArray;

    return 0;
}