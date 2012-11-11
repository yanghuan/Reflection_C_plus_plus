//============================================================================
// Name        : reflection.cpp
// Author      : YH
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include <typeinfo>


#include <vector>

#include "refection/register.hpp"

using namespace std;

int main() {
    extern void test1();
    test1();
}

