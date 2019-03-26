#include <iostream>
#include <fstream>

using namespace std;

#define OPTIMISTIC_INPUT_PREFIX "optimistic_input_"
#define PESSIMISTIC_INPUT_PREFIX "pessimistic_input_"
#define RANDOM_INPUT_PREFIX "random_input_"
#define OPTIMISTIC_OUTPUT_PREFIX "optimistic_output_"
#define PESSIMISTIC_OUTPUT_PREFIX "pessimistic_output_"
#define RANDOM_OUTPUT_PREFIX "random_output_"
#define EXTENSION ".txt"
#define AMOUNT_OF_NUMBERS 100000
#define FIRST_PIVOT 1
#define LAST_PIVOT 2
#define MID_PIVOT 3

void swap(int *array, int i, int j) {
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

int fetchPivotPoint(int left, int right, int pivotChoice) {
    switch (pivotChoice) {
        case 1 :
            return left;
        case 2 :
            return right;
        default :
            return (left + right) / 2;
    }
}

int partition(int *array, int left, int right, int pivotChoice) {
    int pivotPoint = fetchPivotPoint(left, right, pivotChoice);
    int pivotValue = array[pivotPoint];
    swap(array, pivotPoint, right);
    int currentPosition = left - 1;
    for (int i = left; i <= right - 1; i++) {
        if (array[i] <= pivotValue) {
            currentPosition++;
            swap(array, currentPosition, i);
        }
    }
    swap(array, currentPosition + 1, right);
    return currentPosition + 1;
}

void quickSort(int *array, int left, int right, int pivotChoice) {
    if (left < right) {
        int i = partition(array, left, right, pivotChoice);
        quickSort(array, left, i - 1, pivotChoice);
        quickSort(array, i + 1, right, pivotChoice);
    }
}

const std::string currentDateTime() {
    time_t now = time(nullptr);
    struct tm tstruct{};
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d||%X", &tstruct);
    return buf;
}

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

int *createAscendingSortedArray(int numbersAmount) {
    int *array = new int[numbersAmount];
    for (int i = 0; i < numbersAmount; i++) {
        array[i] = i;
    }
    return array;
}

int *createDescendingSortedArray(int numbersAmount) {
    int *array = new int[numbersAmount];
    for (int i = 0; i < numbersAmount; i++) {
        array[i] = numbersAmount - i;
    }
    return array;
}

int *createRandomArray(int numbersAmount) {
    int *array = new int[numbersAmount];
    srand((unsigned) time(nullptr));
    for (int i = 0; i < numbersAmount; i++) {
        array[i] = rand() % numbersAmount;
    }
    return array;
}

auto measureTimeExecution(int *array, int numbersAmount, int pivotChoice) {
    chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();
    quickSort(array, 0, numbersAmount - 1, pivotChoice);
    chrono::high_resolution_clock::time_point finishTime = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(finishTime - startTime).count();
}

void
execute(int *ascendingSortedArray, int *descendingSortedArray, int *randomArray, int numbersAmount, int pivotChoice) {
    cout << "Ascending sorted data (milisec): "
         << measureTimeExecution(ascendingSortedArray, numbersAmount, pivotChoice) << endl;
    //saveOutputToFile(ascendingSortedArray, AMOUNT_OF_NUMBERS, OPTIMISTIC_OUTPUT_PREFIX);
    cout << "Descending sorted data (milisec): "
         << measureTimeExecution(descendingSortedArray, numbersAmount, pivotChoice) << endl;
    //saveOutputToFile(descendingSortedArray, AMOUNT_OF_NUMBERS, PESSIMISTIC_OUTPUT_PREFIX);
    cout << "Random data (milisec): " << measureTimeExecution(randomArray, numbersAmount, pivotChoice) << endl;
    //saveOutputToFile(randomArray, AMOUNT_OF_NUMBERS, RANDOM_OUTPUT_PREFIX);
}

int main() {

    int *ascendingSortedArray = createAscendingSortedArray(AMOUNT_OF_NUMBERS);
    //saveInputToFile(ascendingSortedArray, AMOUNT_OF_NUMBERS, OPTIMISTIC_INPUT_PREFIX);
    int *descendingSortedArray = createDescendingSortedArray(AMOUNT_OF_NUMBERS);
    //saveInputToFile(descendingSortedArray, AMOUNT_OF_NUMBERS, PESSIMISTIC_INPUT_PREFIX);
    int *randomArray = createRandomArray(AMOUNT_OF_NUMBERS);
    //saveInputToFile(randomArray, AMOUNT_OF_NUMBERS, RANDOM_INPUT_PREFIX);

    int pivotChoice;
    cout << "Choose pivot:\n1. First element\n2. Last element\n3. Middle element" << endl;
    cin >> pivotChoice;

    switch (pivotChoice) {
        case 1 :
            cout << "First element pivot results:" << endl;
            execute(ascendingSortedArray, descendingSortedArray, randomArray, AMOUNT_OF_NUMBERS, FIRST_PIVOT);
            break;
        case 2 :
            cout << "Last element pivot results:" << endl;
            execute(ascendingSortedArray, descendingSortedArray, randomArray, AMOUNT_OF_NUMBERS, LAST_PIVOT);
            break;
        case 3 :
            cout << "Middle element pivot results:" << endl;
            execute(ascendingSortedArray, descendingSortedArray, randomArray, AMOUNT_OF_NUMBERS, MID_PIVOT);
            break;
        default :
            cout << "Wrong choice!" << endl;
    }

    delete[] ascendingSortedArray;
    delete[] descendingSortedArray;
    delete[] randomArray;

    return 0;
}
