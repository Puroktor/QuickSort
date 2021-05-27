#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

enum PivotType {
    FIRST, LAST, RANDOM, MED_FROM_3
};

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
//[l, r)
int quickSort(int arr[], int l, int r, enum PivotType pivotType)
{
    if (l >= r-1)
        return;

    int i, comps=0, m = l + (r-l)/2;
    switch (pivotType)
    {
    case FIRST:
        i = l;
        break;
    case LAST:
        i = r - 1;
    case RANDOM:
        i= l + rand() % (r - l);
        break;
    case MED_FROM_3:
        i = (arr[l] > arr[m]) ? ((arr[r-1] < arr[m]) ? m : ((arr[r-1] < arr[l]) ? r-1 : l)) 
            : ((arr[l] > arr[r-1]) ? l : (arr[m] > arr[r-1]) ? r-1 : m);
        comps = 3; 
        /*
            беру кол-во сравнений по максимуму, все равно +-1 дела координально не изменит,
            а в if рассписывать как-то не хочется) 
        */ 
        break;
    default:
        break;
    } 
    swap(arr + i, arr + l);
    int j = partition(arr, l, r, i);
    comps += r - l;
    comps+=quickSort(arr, l, j, pivotType);
    comps+=quickSort(arr, j+1, r, pivotType);
    return comps;
}

int partition(int arr[], int l, int r)
{
    int p = arr[l];
    int i = l;
    for (int j = l; j < r; j++) {
        if (arr[j] <= p)
            swap(arr + i++, arr + j);
    }
    swap(arr + l, arr + i - 1);
    return i - 1;
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

void test(int *a, int *b,enum pivotType pivot, int n) {
    int comp = 0;
    printf("Массив из %d элементов:\n", n);
    for (int i = 0; i < 10; i++)
    {
        createRandomArr(a, n);
        memcpy(b, a, n * sizeof(int));
        comp += quickSort(a, 0, n, pivot);
        qsort(b, n, sizeof(int), compare);
        if (!checkIsCorrect(a, b, n))
        {
            printf("Массив отсортирован неверно\n");
            return;
        }
    }
    printf("Массив отсортирован верно 10 раз, в среднем %d сравнений\n", comp/10);
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
   
    printf("*----Опорный элемент - первый:\n");
    test(a, b, FIRST, n1);
    test(a, b, FIRST, n2);
    printf("*----Опорный элемент - последний:\n");
    test(a, b, LAST, n1);
    test(a, b, LAST, n2);
    printf("*----Опорный элемент - случайный:\n");
    test(a, b, RANDOM, n1);
    test(a, b, RANDOM, n2);
    printf("*----Опорный элемент - медиана из 3-х:\n");
    test(a, b, MED_FROM_3, n1);
    test(a, b, MED_FROM_3, n2);
    free(a);
    free(b);
}