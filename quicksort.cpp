//QuickSort - sortowanie szybkie
//Jan Wojceichowski
//www.algorytm.org

#include <iostream>

#include <utility>
#include <algorithm>
#include <functional>

// Podzia³ danych na dwie czêœci.
template<typename Iter, typename Compare>
std::pair<Iter, Iter> quickSortPartition(Iter first, Iter last, Compare compare) {
	Iter ll = first + ((last - first) / 2), le = ll + 1, lg = le, lu = ll;
	while(lg != last) {
		if(compare(*lg, *ll)) {
			std::swap(*lg, *le);
			std::swap(*le, *ll);
			++ll;
			++le;
		} else if(compare(*ll, *lg)) {
		} else {
			std::swap(*lg, *le);
			++le;
		}
		++lg;
	}
	while(lu != first) {
		--lu;
		if(compare(*lu, *ll)) {
		} else if(compare(*ll, *lu)) {
			--le;
			--ll;
			std::swap(*lu, *ll);
			std::swap(*ll, *le);
		} else {
			--ll;
			std::swap(*lu, *ll);
		}
	}

	return std::make_pair(ll, le);
}

// Sortowanie z w³asn¹ funkcj¹ porównuj¹c¹.
template<typename Iter, typename Compare>
void quickSort(Iter first, Iter last, Compare compare) {
	if(last - first < 2) {
		return;
	}
	while(1 < last - first) {
		std::pair<Iter, Iter> div = quickSortPartition(first, last, compare);
		if(div.first - first < last - div.second) {
			// Pêtla kontynuuje pracê na drugiej czêœci.
			quickSort(first, div.first, compare);
			first = div.second;
		} else {
			// Pêtla kontynuuje pracê na pierwszej czêœci.
			quickSort(div.second, last, compare);
			last = div.first;
		}
	}
}

// Sortowanie z u¿yciem operatora '<'.
template<typename Iter>
void quickSort(Iter first, Iter last) {
	return quickSort(first, last, std::less<std::iterator_traits<Iter>::value_type>());
}

bool greaterThan(double a, double b) {
	return a > b;
}

int main() {
	std::cout << "Ile liczb checsz posortowac? ";
	unsigned int count;
	std::cin >> count;
	std::cout << "Podaj " << count << " liczb rzeczywistych." << std::endl;
	double* data = new double[count];
	for(unsigned int i = 0; i < count; ++i) {
		std::cin >> data[i];
		std::cin.ignore();
	}

	quickSort(data, data + count);
	std::cout << "Liczby posortowane rosnaco:" << std::endl;
	for(unsigned int i = 0; i < count; ++i) {
		std::cout << data[i] << (i + 1 < count ? " " : "\n");
	}

	quickSort(data, data + count, greaterThan);
	std::cout << "Liczby posortowane malejaco:" << std::endl;
	for(unsigned int i = 0; i < count; ++i) {
		std::cout << data[i] << (i + 1 < count ? " " : "\n");
	}

	delete[] data;
	data = NULL;

	std::cout << "Nacisnij ENTER." << std::endl;
	::getchar();

	return 0;
}
