#include <iostream>
#include "../common/TinyLog.h"
#include "../common/BaseUdpRadio.h"
#include "../insider/UTenInsider.h"

using namespace std;


int main() {
    TLOGD("test test:%s %d", "aaa", 123);
    TLOGD("test test:%s %d", "bbb", 123);
    UTenInsider insider;
    
    while(insider.alive) {
        insider.step();
    }
    
    std::cout << "finish" << endl;
    return 0;
}