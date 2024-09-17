#include <iostream>

int main() {
    int arr[10] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
    int* ptr = &arr[0]; 
    int fourthElement = *(ptr + 3); 
    std::cout << "The fourth element is: " << fourthElement << "\n";
    return 0;
}