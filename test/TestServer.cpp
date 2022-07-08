#include <iostream>
#include "../common/BaseUdpRadio.h"

using namespace std;


int main() {
    BaseUdpRadio radio(9002);
    if (radio.isBinded()) {
        std::cout << "binded" << endl;
    } else {
        
        std::cout << "not binded" << endl;
    }
    
    while(radio.step());
    
    std::cout << "finish" << endl;
    return 0;
}