#include <iostream>

template <class Type>
bool predicate(Type a, Type b) {
  return a > b;
}

template <class T>
class Type {
 public:
  T val;
  Type operator>(const Type& type) {}
};

// when specific type is used, use this specific implementation
template <>
class Type<char> {};

template <class T, int N>
class MyArr {
 public:
  T arr[N];
};
// usage MyArr<int, 3> arr;

// function template, and passing an array
template <class T, int N>
void print(const T (&array)[N]) {
  for (int i = 0; i < N; ++i) {
    std::cout << array[i] << std::endl;
  }
}
