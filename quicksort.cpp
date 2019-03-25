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
#define AMOUNT_OF_NUMBERS 10000000
#define FIRST_PIVOT 1
#define LAST_PIVOT 2
#define MID_PIVOT 3

void quickSort(int *arr, int left, int right, int pivotChoice) {

    int i = left, j = right;
    int tmp;
    int pivot;

    // pivot choice
    switch (pivotChoice) {
        case 1 :
            pivot = arr[left];
            break;
        case 2 :
            pivot = arr[right];
            break;
        default :
            pivot = arr[(left + right) / 2];
    }

    // partition
    while (i <= j) {
        while (arr[i] < pivot)
            i++;
        while (arr[j] > pivot)
            j--;
        if (i <= j) {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    };

    // recursion
    if (left < j)
        quickSort(arr, left, j, pivot);
    if (i < right)
        quickSort(arr, i, right, pivot);
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

int *createOptimisticArray(int numbersAmount) {
    int *array = new int[numbersAmount];
    for (int i = 0; i < numbersAmount; i++) {
        array[i] = i;
    }
    return array;
}

int *createPessimisticArray(int numbersAmount) {
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
        array[i] = rand() % (numbersAmount / 100);
    }
    return array;
}

auto measureTimeExecution(int *array, int numbersAmount, int pivotChoice) {
    chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();
    quickSort(array, 0, numbersAmount - 1, pivotChoice);
    chrono::high_resolution_clock::time_point finishTime = chrono::high_resolution_clock::now();

    return chrono::duration_cast<chrono::milliseconds>(finishTime - startTime).count();
}

void execute(int *optimisticArray, int *pessimisticArray, int *randomArray, int numbersAmount, int pivotChoice) {
    cout << "Optimistic (milisec): " << measureTimeExecution(optimisticArray, AMOUNT_OF_NUMBERS, pivotChoice) << endl;
    //saveOutputToFile(optimisticArray, AMOUNT_OF_NUMBERS, OPTIMISTIC_OUTPUT_PREFIX);
    cout << "Pessimistic (milisec): " << measureTimeExecution(pessimisticArray, AMOUNT_OF_NUMBERS, pivotChoice) << endl;
    //saveOutputToFile(pessimisticArray, AMOUNT_OF_NUMBERS, PESSIMISTIC_OUTPUT_PREFIX);
    cout << "Random (milisec): " << measureTimeExecution(randomArray, AMOUNT_OF_NUMBERS, pivotChoice) << endl;
    //saveOutputToFile(randomArray, AMOUNT_OF_NUMBERS, RANDOM_OUTPUT_PREFIX);
}

int main() {

    int *optimisticArray = createOptimisticArray(AMOUNT_OF_NUMBERS);
    //saveInputToFile(optimisticArray, AMOUNT_OF_NUMBERS, OPTIMISTIC_INPUT_PREFIX);
    int *pessimisticArray = createPessimisticArray(AMOUNT_OF_NUMBERS);
    //saveInputToFile(pessimisticArray, AMOUNT_OF_NUMBERS, PESSIMISTIC_INPUT_PREFIX);
    int *randomArray = createRandomArray(AMOUNT_OF_NUMBERS);
    //saveInputToFile(randomArray, AMOUNT_OF_NUMBERS, RANDOM_INPUT_PREFIX);

    int pivotChoice;
    cout << "Choose pivot:\n1. First element\n2. Last element\n3. Middle element" << endl;
    cin >> pivotChoice;

    switch (pivotChoice) {
        case 1 :
            cout << "First element pivot results:" << endl;
            execute(optimisticArray, pessimisticArray, randomArray, AMOUNT_OF_NUMBERS, FIRST_PIVOT);
            break;
        case 2 :
            cout << "Last element pivot results:" << endl;
            execute(optimisticArray, pessimisticArray, randomArray, AMOUNT_OF_NUMBERS, LAST_PIVOT);
            break;
        case 3 :
            cout << "Middle element pivot results:" << endl;
            execute(optimisticArray, pessimisticArray, randomArray, AMOUNT_OF_NUMBERS, MID_PIVOT);
            break;
        default :
            cout << "Wrong choice!" << endl;
    }

    delete[] optimisticArray;
    delete[] pessimisticArray;
    delete[] randomArray;

    return 0;
}
