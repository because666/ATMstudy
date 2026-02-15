#include "Utils.h"
#include <mutex>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

std::string Utils::hashPassword(const std::string& password)
{ 
    size_t hashValue = std::hash<std::string>{}(password);
    size_t hash666 = std::hash<std::string>{}(std::to_string(hashValue));
    return std::to_string(hashValue + hash666);
}//哈希加密生成密码
bool Utils::validateAccountNumber(const std::string& accountNumber){
    // 修复：账户号长度应为19位，根据accountRandom方法生成规则确定
    if(accountNumber.size() == 19){  
        return true;
    }
    else{
        std::cout<<"账户格式不对，请重新输入，谢谢！";
        return false;
    }
}//验证账户是否正确，用在登录时候
std::string Utils::accountRandom(){
    static long long counter = 0;  // 静态计数器
    static std::mutex mtx;  // 静态互斥锁
        
    // 获取当前时间（毫秒级精度）
    auto now = std::chrono::system_clock::now();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()).count();
        
    // 获取今天的日期（年月日）
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm* localTime = std::localtime(&t);
    int dayCode = (localTime->tm_year + 1900) * 10000 + 
                 (localTime->tm_mon + 1) * 100 + 
                 localTime->tm_mday;
        
    // 组合：日期(8位) + 毫秒时间戳后7位 + 序号4位 = 19位
    std::stringstream ss;
    ss << std::setw(8) << std::setfill('0') << dayCode;           // 8位日期
    ss << std::setw(7) << std::setfill('0') << (millis % 10000000); // 7位毫秒
    ss << std::setw(4) << std::setfill('0') << (counter++ % 10000); // 4位序号
        
    std::string account = ss.str();
    return account;
}//生成账户，注册时候使用
bool Utils::validateIDCard(const std::string &ID){
    if(ID.size() == 18){
        return true;
    }
    else{
        return false;
    }
}//验证身份证格式是否正确
bool Utils::validatePassword(const std::string &password){
    if(password.size() == 6){
        return true;
    }
    else{
        return false;
    }
}//用来检查密码格式是否正确