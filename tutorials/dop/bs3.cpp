#include <iostream>

using namespace std;

struct Secure
{
    int x;
    Secure operator<(const Secure & y) const { return Secure {int(x < y.x)}; }
    friend ostream & operator<<(ostream & os, const Secure & y) { return os << y.x; }

    Secure operator*(const Secure & y) const { return Secure {int(x * y.x)}; }
    Secure operator!() const { return Secure {int(!x)}; }
    Secure operator+(const Secure & y) const { return Secure {int(x + y.x)}; }
};

void swap(Secure & x, Secure & y, Secure cond)
{
    auto t = x ;
    x = cond * y + !cond * x;
    y = cond * t + !cond * y;
}

void bubbleSort(Secure arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            //if (arr[j + 1] < arr[j])
            swap(arr[j], arr[j + 1], (arr[j + 1] < arr[j]));
}

void printArray(Secure arr[], int size)
{
    for (int i = 0; i < size; i++) cout << ' ' << arr[i];
    cout << '\n';
}

int main()
{
    Secure arr[] = {64, 34, 25, 12, 22, 90, 11};
    int n = sizeof(arr) / sizeof(arr[0]);

    cout << "Original array :"; printArray(arr, n);
    bubbleSort(arr, n);
    cout << "Sorted  array  :"; printArray(arr, n);
}