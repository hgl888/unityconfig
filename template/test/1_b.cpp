//
// Created by hou guoli on 2018/3/28.
//

#include <iostream>
template<typename T>
void print(T const& m) { std::cout << "b.cpp: " << m << '\n'; }

void fb() {
    print('2'); // print<char>
    print(0.1); // print<double>
}

