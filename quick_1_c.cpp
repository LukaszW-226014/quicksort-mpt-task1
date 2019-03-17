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
#define AMOUNT_OF_NUMBERS 10000000

const std::string currentDateTime() {
    time_t now = time(nullptr);
    struct tm tstruct{};
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d||%X", &tstruct);

    return buf;
}

template<typename Iter, typename Compare>
pair<Iter, Iter> quickSortPartition(Iter first, Iter last, Compare compare) {
    Iter ll = first + ((last - first) / 2), le = ll + 1, lg = le, lu = ll;
    while (lg != last) {
        if (compare(*lg, *ll)) {
            swap(*lg, *le);
            swap(*le, *ll);
            ++ll;
            ++le;
        } else if (compare(*ll, *lg)) {
        } else {
            swap(*lg, *le);
            ++le;
        }
        ++lg;
    }
    while (lu != first) {
        --lu;
        if (compare(*lu, *ll)) {
        } else if (compare(*ll, *lu)) {
            --le;
            --ll;
            swap(*lu, *ll);
            swap(*ll, *le);
        } else {
            --ll;
            swap(*lu, *ll);
        }
    }

    return std::make_pair(ll, le);
}

// Sortowanie z wlasna funkcja porownujaca.
template<typename Iter, typename Compare>
void quickSort(Iter first, Iter last, Compare compare) {
    if (last - first < 2) {
        return;
    }
    while (1 < last - first) {
        pair<Iter, Iter> div = quickSortPartition(first, last, compare);
        if (div.first - first < last - div.second) {
            // Petla kontynuuje prace na drugiej czesci.
            quickSort(first, div.first, compare);
            first = div.second;
        } else {
            // Petla kontynuuje prace na pierwszej czesci.
            quickSort(div.second, last, compare);
            last = div.first;
        }
    }
}

// Sortowanie z uzyciem operatora '<'.
template<typename Iter>
void quickSort(Iter first, Iter last) {
    return quickSort(first, last, less<typename iterator_traits<Iter>::value_type>());
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
    for (int i = 0; i < numbersAmount; i++) // wczytywanie liczb do tablicy
    {
        array[i] = i;
    }
    return array;
}

int *createPessimisticArray(int numbersAmount) {
    int *array = new int[numbersAmount];
    for (int i = 0; i < numbersAmount; i++) // wczytywanie liczb do tablicy
    {
        array[i] = numbersAmount - i;
    }
    return array;
}

int *createRandomArray(int numbersAmount) {
    int *array = new int[numbersAmount];
    srand((unsigned) time(nullptr));
    for (int i = 0; i < numbersAmount; i++) // wczytywanie liczb do tablicy
    {
        array[i] = rand() % (numbersAmount / 100);
    }
    return array;
}

auto measureTimeExecution(int *array, int numbersAmount) {
    chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();
    quickSort(array, array + numbersAmount);
    chrono::high_resolution_clock::time_point finishTime = chrono::high_resolution_clock::now();

    return chrono::duration_cast<chrono::milliseconds>(finishTime - startTime).count();
}

int main() {
//    int numbersAmount;
//    cout << "Enter size of array: ";
//    cin >> numbersAmount;

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

    delete[] optimisticArray; // zwolnienie tablicy zaalokowanej dynamicznie
    delete[] pessimisticArray;
    delete[] randomArray;

    return 0;
}