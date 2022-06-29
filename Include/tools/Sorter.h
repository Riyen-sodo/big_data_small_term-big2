//
// Created by Flyoung on 2022/6/29.
//

#ifndef INC_3_SORTER_H
#define INC_3_SORTER_H

#include "PublicIncludes.h"
#include "TupleDataComparable.h"


class Sorter {
public:
    /**
 * 交换 a 和 b 的值
 * @tparam T
 * @param a
 * @param b
 */
    template<typename T>
    void swap(T &a, T &b);

    /**
     * 对 arr 中 [left, right] 范围内的数据进行排序
     * @tparam T
     * @param arr
     * @param left
     * @param right
     */
    template<typename T>
    void quickSort(vector<T> &arr, int left, int right);

    /**
     * 对数组进行划分
     * @tparam T
     * @param arr
     * @param left
     * @param right
     * @return
     */
    template<typename T>
    int partition(vector<T> &arr, int left, int right);
};

template<typename T>
void Sorter::swap(T &a, T &b) {
    T c = a;
    a = b;
    b = c;
}

template<typename T>
void Sorter::quickSort(vector<T> &arr, int left, int right) {
    if (right <= left) {
        return;
    }
    int key = partition(arr, left, right);
    quickSort(arr, left, key - 1);
    quickSort(arr, key + 1, right);
}

template<typename T>
int Sorter::partition(vector<T> &arr, int left, int right) {
    int key = left + rand() % (right - left + 1);
    swap(arr[key], arr[right]);
    int i, j;
    i = left;
    for (j = left; j < right; j++) {
        if (arr[j] <= arr[right]) {
            swap(arr[j], arr[i]);
            i++;
        }
    }
    swap(arr[i], arr[right]);
    return i;
}

#endif //INC_3_SORTER_H
