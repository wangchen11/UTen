#include <iostream>
#include "../common/TinyLog.h"
#include "../common/BaseUdpRadio.h"
#include "../server/UTenServer.h"

using namespace std;


int main() {
    TLOGD("test test:%s %d", "aaa", 123);
    TLOGD("test test:%s %d", "bbb", 123);
    UTenServer server(9002);
    if (server.isBinded()) {
        std::cout << "binded" << endl;
    } else {
        
        std::cout << "not binded" << endl;
    }
    
    while(server.alive) {
        server.step();
    }
    
    std::cout << "finish" << endl;
    return 0;
}