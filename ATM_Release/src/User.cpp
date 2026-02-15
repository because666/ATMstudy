#include "User.h"
#include "Utils.h"
#include "AccountManager.h"
#include <iostream>
#include <ctime>
#include <cmath>

User::User(const std::string& accNum, const std::string& pwd, 
         const std::string& name, const std::string& idCard, 
          double initBalance) 
         : accountNumber(accNum), name(name), idCard(idCard), 
           hashedPassword(Utils::hashPassword(pwd)), balance(initBalance) {
    // 初始化交易日期为0，表示需要重置
    lastTransactionYear = 0;
    lastTransactionMonth = 0;
    lastTransactionDay = 0;
}

// 实现新构造函数，直接使用已哈希的密码
User::User(const std::string& accNum, const std::string& hashedPwd, 
         const std::string& name, const std::string& idCard, 
         double initBalance, bool isHashed) 
         : accountNumber(accNum), name(name), idCard(idCard), 
           balance(initBalance) {
    if (isHashed) {
        // 如果密码已经是哈希过的，直接赋值
        hashedPassword = hashedPwd;
    } else {
        // 否则进行哈希处理
        hashedPassword = Utils::hashPassword(hashedPwd);
    }
    // 初始化交易日期为0，表示需要重置
    lastTransactionYear = 0;
    lastTransactionMonth = 0;
    lastTransactionDay = 0;
}

User::~User() {
    // 析构函数实现
}

void User::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        // 添加交易记录
        addTransactionRecord("存款", amount);
    }
    else{
        std::cout<<"存款金额不对，请重新尝试，谢谢！"<<std::endl;
    }
}//存款

void User::withdraw(double amount) {
    // 检查单笔限额和每日限额
    if (amount > 0 && balance >= amount && is_or_not_onceLimit(amount) && is_or_not_dayLimit(amount)) {
        balance -= amount;
        addDailySpent(amount);  // 更新当日已用额度
        // 添加交易记录
        addTransactionRecord("取款", amount);
    }
    else{
        std::cout<<"取款金额不正确或者取款达到限额，请重试"<<std::endl;
    }
}//取款

bool User::transferTo(User& receiver, double amount) {
    // 检查单笔限额和每日限额，以及账户锁定状态
    bool isok=true;
    if (amount > 0 && balance >= amount && 
        is_or_not_onceLimit(amount) && is_or_not_dayLimit(amount) && 
        !isLocked && !receiver.isLocked) {
        withdraw(amount);
        receiver.deposit(amount);
        // 添加交易记录
        addTransactionRecord("转账支出", amount, receiver.getAccountNumber());
        receiver.addTransactionRecord("转账收入", amount, accountNumber);
        isok=true;
        return isok;
    }
    else{
        isok=false;
        return isok;
    }
}//转账有单日限额和单笔限额，每次只能取100的整数倍

void User::is_or_not_lockAccount(int a) {
    switch(a){
        case 1:
            isLocked = false;
            break;
        case 2:
            isLocked = true;
            break;
    }
}//锁定账户
bool User::verifyPassword(const std::string& pwd) {
    // 检查账户是否已经锁定或挂失
    if (isLocked || isReportedLost) {
        return false;  // 如果账户已锁定或挂失，直接返回false
    }
    
    // 检查密码是否正确
    if(Utils::hashPassword(pwd) == hashedPassword && failedAttempts > 0) {
        failedAttempts = 3;  // 登录成功时重置尝试次数
        return true;
    }
    else {
        // 密码错误或尝试次数已用完，减少尝试次数
        failedAttempts--;
        if (failedAttempts <= 0) {
            is_or_not_lockAccount(2);  // 锁定账户（使用参数2）
        }
        return false;
    }
}//验证密码，有自动锁定功能
bool User::changePassword(const std::string& oldPwd, const std::string& newPwd) {
    bool isok=true;
    if (verifyPassword(oldPwd)&&Utils::validatePassword(oldPwd)&&Utils::validatePassword(newPwd)) {
        hashedPassword = Utils::hashPassword(newPwd);
        isok=true;
        return isok;
    }
    else{
        isok=false;
        return isok;
    }
}//修改密码
void User::reportLostAccount() {
    isReportedLost = true;
}//挂失账户
void User::applyUnlockAccount(AccountManager& manager) {
    if (isLocked) {
        manager.unlockAccount(accountNumber);
    }
    else {
        std::cout << "账户未锁定" << std::endl;
    }
}//申请解锁账户
std::string  User::getAccountNumber() const{
    return accountNumber;
}//获取账户编号
double User::getBalance() const{
    return balance;
}//获取余额
std::string User::getHashedPassword() const{
    return hashedPassword;
}//获取密码
std::string User::getIDCard() const{
    return idCard;
}//获取身份证
std::string User::getName() const{
    return name;
}//获取姓名

std::string User::getIsLocked() const{
    if(isLocked){
        return "是";
    }
    else{
        return "否";
    }
}//获取是否锁定

void User::modifyAccount(std::string accountNumber,std::string name){
    this->accountNumber = accountNumber;
    this->name = name;
}//修改用户信息，暂时只想到名字可能会修改

bool User:: is_or_not_dayLimit(double amount){
    // 获取当前日期
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int currentDay = ltm->tm_mday;
    int currentMonth = ltm->tm_mon + 1;
    int currentYear = ltm->tm_year + 1900;
    
    // 如果上次交易日期不是今天，则重置当日交易金额
    if (lastTransactionYear != currentYear || 
        lastTransactionMonth != currentMonth || 
        lastTransactionDay != currentDay) {
        dailySpent = 0; // 重置当日已用额度
        lastTransactionYear = currentYear;
        lastTransactionMonth = currentMonth;
        lastTransactionDay = currentDay;
    }
    
    // 检查加上本次金额后是否会超过日限额
    return (dailySpent + amount <= dayLimit);
}

bool User:: is_or_not_onceLimit(double amount){
    // 检查单笔限额和是否为100的倍数
    if(amount <= onceLimit && amount >= 100 && fmod(amount, 100) == 0){
        return true;
    }
    else{
        return false;
    }
}

// 添加交易后更新日消费额的方法
void User::addDailySpent(double amount) {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int currentDay = ltm->tm_mday;
    int currentMonth = ltm->tm_mon + 1;
    int currentYear = ltm->tm_year + 1900;
    
    // 如果上次交易日期不是今天，则重置当日交易金额
    if (lastTransactionYear != currentYear || 
        lastTransactionMonth != currentMonth || 
        lastTransactionDay != currentDay) {
        dailySpent = 0; // 重置当日已用额度
        lastTransactionYear = currentYear;
        lastTransactionMonth = currentMonth;
        lastTransactionDay = currentDay;
    }
    
    dailySpent += amount;
}

// 设置每日限额
bool User::setDayLimit(double  newLimit) {
    // 可以添加验证，比如限制最大限额等
    dayLimit = newLimit;
    return true;
}

// 设置单笔限额
bool User::setOnceLimit(double newLimit) {
    // 可以添加验证，比如限制最大单笔限额等
    onceLimit = newLimit;
    return true;
}

// 获取每日限额
double User::getDayLimit() const {
    return dayLimit;
}

// 获取单笔限额
double User::getOnceLimit() const {
    return onceLimit;
}

// 交易记录相关方法
void User::addTransactionRecord(const std::string& type, double amount, const std::string& targetAccount) {
    Transaction transaction(type, accountNumber, amount, targetAccount);
    transactionHistory.push_back(transaction);
}

void User::viewTransactionHistory() const {
    std::cout << "========== 交易记录 ==========" << std::endl;
    if (transactionHistory.empty()) {
        std::cout << "暂无交易记录" << std::endl;
    } else {
        for (size_t i = 0; i < transactionHistory.size(); ++i) {
            std::cout << "记录 " << (i+1) << ":" << std::endl;
            transactionHistory[i].displayTransaction();
        }
    }
    std::cout << "=============================" << std::endl;
}