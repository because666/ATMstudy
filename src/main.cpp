#include "../include/AccountManager.h"
#include "../include/ATMSystem.h"
#include "../include/Constants.h"
#include "../include/User.h"
#include "../include/Transaction.h"
#include "../include/Utils.h"
#include <iostream>
#include <windows.h>

using namespace std;
int main(){
    // 设置控制台使用UTF-8编码
    SetConsoleOutputCP(CP_UTF8);
    
    ATMSystem system;
    system.run();
    return 0;
} 



