#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <algorithm>
#include <random>
#include <memory>

#define BYTE_MASK 0xFF
#define BYTE_SIZE 0xFF

using namespace std;

void CountingSort(int *arr, int *outArr, int nElements, int maskByte)
{
	int byteOffset = 8 * maskByte;
	int mask = BYTE_MASK << byteOffset;
	if (maskByte == 3)
		mask &= ~(1UL << 32);

	long long countArr[BYTE_SIZE + 1 ] = {0};

	for (int i = 0; i < nElements; i++)
	{
		int v = arr[i] & mask;
		v = v >> byteOffset;
		countArr[v]++;
	}


	for (int i = 1; i <= BYTE_SIZE; i++)
		countArr[i] += countArr[i - 1];

	for (int i = nElements - 1; i >= 0; i--)
	{
		int countIndex = (arr[i] & mask) >> byteOffset;
		outArr[(countArr[countIndex]--) - 1] = arr[i];
	}

}

void RadixSort(int *arr, int nElements)
{
	int *tempArr = (int *)malloc(nElements * sizeof(int));
	
	for (int i = 0; i < sizeof(int); i++)
	{
		CountingSort(arr, tempArr, nElements, i);
		memcpy(arr, tempArr, nElements * sizeof(int));
	}
	free(tempArr);
}

int main()
{
	

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(1, 0xffffffff);

    int nSize = 10000000;

    int *arr = (int *)malloc(sizeof(int) * nSize);

    for (int i = 0; i < nSize; i++)
    {
        arr[i] = dist(rng);
    }
   
    RadixSort(arr, nSize);

    ofstream output("output.txt");
    if(!output.is_open())
    {
        free(arr);
        return -1;
        
    }
	
    for(int i = 0; i < nSize; i++)
    output << arr[i] << '\n';

    output.close();
    
    free(arr);

	return 0;
}
