#include "library.h"

#include <iostream>
#include "CBaseTypeUtil.h"

void hello() {
    std::cout << "Hello, World!" << std::endl;
    CBaseTypeUtil::IsPBool(0);
}

//int main(){
//    return 0;
//}