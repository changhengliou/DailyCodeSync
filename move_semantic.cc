#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct A {
  A() { cout << "A's constructor" << endl; }

  A(const A& rhs) { cout << "A's copy constructor" << endl; }

  A(A&& rhs) { cout << "A's move constructor" << endl; }
};

void move_demo() {
  vector<A> v;
  cout << "==> push_back A():" << endl;
  v.push_back(A());
}

// the rule of three
// 1. destructor
// 2. copy constructor
// 3. copy assignment operator
class ArrayWrapper {
 private:
  int* x;
  size_t size;

 public:
  ArrayWrapper() { ArrayWrapper(64); }

  ArrayWrapper(size_t s) : size(s) {
    std::cout << "normal constructor" << std::endl;
    x = new int[s];
  }

  // copy constructor
  ArrayWrapper(const ArrayWrapper& arr) {
    std::cout << "copy constructor" << std::endl;
    x = new int[arr.size];
    for (int i = 0; i < size; ++i) x[i] = arr.x[i];
  }

  // move constructor
  ArrayWrapper(ArrayWrapper&& arr) : x(arr.x), size(arr.size) {
    std::cout << "move constructor" << std::endl;
    // if not set it to nullptr, when rvalue out of scope, its destructor will
    // call
    arr.x = nullptr;
  }

  // copy assign operator
  ArrayWrapper& operator=(const ArrayWrapper& arr) { *this = arr; }

  // move assign operator
  // ArrayWrapper& operator=(ArrayWrapper&& pt) {}

  ~ArrayWrapper() {
    if (x != nullptr) {
      delete[] x;
      x = nullptr;
    }
  }
};
ArrayWrapper getArr() { return ArrayWrapper{}; }
void printReference(const string& str) { cout << "lValue(Reference)" << endl; }

void printReference(string&& str) { cout << "rValue(Move)" << endl; }

int main() {
  // this is invalid because it is a reference to rvalue
  // Point& ptr = getPtr(2, 3);

  // call move constructor
  ArrayWrapper mv{ArrayWrapper{}};
  // call normal constructor
  // ArrayWrapper nr{64};
  // copy constructor
  // ArrayWrapper cp = nr;

  string s = "Alex";

  printReference(s);       // lvalue
  printReference("Alex");  // rvalue
}
