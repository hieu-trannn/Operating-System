#include <iostream>
using namespace std;

// Declaring  a global variable
int product;

int main()
{
    // Declaring two local variables
    int a = 6, b = 11;

    // Assigning value to the global variable
    product = a * b;

    // Printing the value of the global variable
    cout << "The product of a and b is " << product << endl;
}