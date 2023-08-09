// simplememorypool.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "MemoryPool.h"
#include <chrono>

struct MyStruct {
    int a;
    int b;
    double c;
    double d;
};
void NoPool()
{
    auto start = std::chrono::steady_clock::now();
    for (unsigned int i = 0; i < 1000000; i++)
    {
        MyStruct* p = new MyStruct;
        delete p;
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "Elapsed Time NoPool  = " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
}
void WithPool() {
    MemoryPool<MyStruct> m = MemoryPool<MyStruct>(1000000);
    auto start = std::chrono::steady_clock::now();
    for (unsigned int i = 0; i < 1000000; i++)
    {
        MyStruct* p = (MyStruct*)m.Alloc();
        m.Free(p);
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "Elapsed Time WithPool  = " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
}
int main()
{
    try
    {
        NoPool();
        WithPool();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
