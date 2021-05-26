#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void insertionSort(int arr[], int l, int r)
{
    for (int i = l; i < r; i++)
    {
        int temp = arr[i];
        for (int j = i - 1; j >= l; j--)
        {
            if (arr[j] < temp)
                break;

            arr[j + 1] = arr[j];
            arr[j] = temp;
        }
    }
}

void quickSort(int arr[], int l, int r)
{
    if (l >= r-1)
        return;
    if (r - l <= 11)
        return insertionSort(arr, l, r);

    int i = l + rand()%(r-l);
    swap(arr + i, arr + l);
    int j = partition(arr, l, r, i);
    quickSort(arr, l, j);
    quickSort(arr, j+1, r);
}

int partition(int arr[], int l, int r)
{
    int p = arr[l];
    int i = l;
    for (int j = l; j < r; j++) 
        if (arr[j] <= p)
            swap(arr + i++, arr + j);
          
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

int main()
{
    setlocale(LC_ALL, "Russian");
    int n = 1e6+ rand()%(int)(1e6);
    int *a = (int*)malloc(n * sizeof(int));
    int *b = (int*)malloc(n * sizeof(int));
    if (a && b)
    {
        srand(time(NULL));
        for (int i = 0; i < 10; i++)
        {
            for (int i = 0; i < n; i++)
                a[i] = rand();
            memcpy(b, a, n * sizeof(int));

            clock_t begin = clock();
            quickSort(a, 0, n);
            clock_t end = clock();
            printf("%f\t", (float)(end-begin)/CLOCKS_PER_SEC);

            begin = clock();
            qsort(b, n, sizeof(int), compare);
            end = clock();
            printf("%f\t", (float)(end - begin) / CLOCKS_PER_SEC);

            if (checkIsCorrect(a, b, n))
                printf("Верно\n");
            else
                printf("Неверно\n");
        }
    }
    free(a);
    free(b);
}