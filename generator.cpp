#include <iostream>

int main() {
    // generate 10M numbers in [2, 10000] 
    for(int i=0; i < 1000; ++i) {
        for(int i=2; i <= 10000; ++i) {
            std::cout << i << std::endl;
        }
    }

    return 0;
}
