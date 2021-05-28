#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

enum PivotType {
    FIRST, LAST, RANDOM, MED_FROM_3
};

enum ArraytType {
    SORTED, REVERSED , RND
};

void swap(void *a, char *b, size_t size)
{   
    void *tmp = malloc(size);
    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);
    
    free(tmp);
}

size_t partition(char* arr, size_t s, size_t n, int (*compFunc)(void const*, void const*))
{
    size_t i = 0;
    for (size_t j = 0; j < n; j++) {
        if ((*compFunc)(arr + j * s, arr) <= 0)
            swap(arr + s * i++, arr + j * s, s);
    }
    swap(arr, arr + (i - 1) * s, s);
    return i - 1;
}

unsigned long long quickSort(void *a, size_t n, size_t sizeOfElem, int (*compFunc)(void const*, void const*), enum PivotType pivotType)
{
    if (n<=1)
        return 0;
    char* arr = (char*)a;
    size_t i;
    unsigned long long comps = 0;
    switch (pivotType)
    {
    case FIRST:
        i = 0;
        break;
    case LAST:
        i = n - 1;
        break;
    case RANDOM:
        i= rand() %n;
        break;
    case MED_FROM_3:
        if ((compFunc(arr, arr + sizeOfElem * n / 2) > 0) != (compFunc(arr, arr + sizeOfElem * (n - 1)) > 0)) {
            i = 0;
            comps = 2;
        }
        else {
            comps = 4;
            if ((compFunc(arr + sizeOfElem * n / 2, arr) > 0) != (compFunc(arr + sizeOfElem * n / 2, arr + sizeOfElem * (n - 1)) > 0))
                i = n / 2;
            else
                i = n - 1;
        }
        break;
    default:
        exit(2);
        break;
    } 
    swap(arr + i * sizeOfElem, arr, sizeOfElem);
    size_t j = partition(arr, sizeOfElem, n, compFunc);
    comps += n;
    comps+=quickSort(arr, j, sizeOfElem, compFunc, pivotType);
    comps+=quickSort(arr+(j+1) * sizeOfElem, n-j-1, sizeOfElem, compFunc, pivotType);
    return comps;
}

int checkIsCorrect(int first[], int second[], int n)
{
    for (int i = 0; i < n; i++)
        if (first[i] != second[i])
            return 0;

    return 1;
}

int compare(const void* a, const void* b)
{
    return *(int*)a - *(int*)b;
}

void createSortedArr(int *arr, int n)
{
    for (int i = 0; i < n; i++)
        arr[i] = i;
}

void createReverseSortedArr(int *arr, int n)
{
    for (int i = 0; i < n; i++)
        arr[i] = n-i;
}

void createRandomArr(int *arr, int n)
{
    for (int i = 0; i < n; i++)
        arr[i] = rand();
}

void test(int *a, int *b, int n, enum pivotType pivot, enum arrayType arrType)
{
    unsigned long long comp = 0;
    for (int i = 0; i < 10; i++)
    {
        switch (arrType)
        {
        case SORTED:
            createSortedArr(a,n);
            break;
        case REVERSED:
            createReverseSortedArr(a,n);
            break;
        case RND:
            createRandomArr(a, n);
            break;
        default:
            break;
        }
        memcpy(b, a, n * sizeof(int));
        comp += quickSort(a, n, sizeof(int),compare, pivot);
        qsort(b, n, sizeof(int), compare);
        if (!checkIsCorrect(a, b, n))
        {
            printf("\tМассив отсортирован неверно\n");
            return;
        }
    }
    printf("\tМассив отсортирован верно 10 раз, в среднем %llu сравнений\n", comp/10);
}

int main()
{
    setlocale(LC_ALL, "Russian");
    int n1=10, n2 = 1000;
    int *a = (int*)calloc(n2, sizeof(int));
    int *b = (int*)calloc(n2, sizeof(int));
    if (!a || !b)
        exit(1);
    srand(time(NULL));
    char outs[4][40] = { "*----Опорный элемент - первый:\n" , "*----Опорный элемент - последний:\n",
    "*----Опорный элемент - случайный:\n", "*----Опорный элемент - медиана из 3-х:\n"};
    char arrs[3][40] = { "\t~~~Отсортированный масив:\n" , "\t~~~Отсортированный обратно массив:\n",
    "\t~~~Случайный массив:\n", };
    for (int i = 0; i < 4; i++)
    {
        printf(outs[i]);
        printf("Массив из %d элементов:\n", n1);
        printf(arrs[2]);
        test(a, b, n1, i, RND);
        printf("Массив из %d элементов:\n", n2);
        for (int j = 0; j < 3; j++) {
            printf(arrs[j]);
            test(a, b, n2, i, j);
        }
        
    }
    free(a);
    free(b);
    return 0;
}