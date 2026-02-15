#ifndef UTILS_H
#define UTILS_H
// #include"Utils.h"  // 删除这行 - 多余的自包含
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <unordered_set>
#include <mutex>

using namespace std;
class Utils {
public:
    static std :: string hashPassword(const std::string& password);//哈希加密，用来防破解
    static bool validateAccountNumber(const std::string& accountNumber);//验证账号是否合法
    static bool validateIDCard(const std::string& idCard);//身份证号合法性验证
    static std::string accountRandom();//随机生成账号
    static bool validatePassword(const std::string& password);//修复：参数名应为password，不是psassword - 密码合法性验证,在设置密码时候使用
    mutex mtx;
};

#endif // UTILS_H