#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include"Utils.h"
#include"Transaction.h"  // 添加交易记录头文件

class AccountManager;  // 前向声明，避免循环依赖

class User {
private:
    // 私有成员
    std::string accountNumber;    // 账号（唯一标识，19位）
    std::string name;            // 姓名
    std::string idCard;          // 身份证号（18位）
    std::string hashedPassword;  // 加密后的密码
    double balance;              // 余额（初始10000元）
    bool isLocked=false;               // 是否被锁定（密码错误3次）
    bool isReportedLost=false;         // 是否挂失
    int failedAttempts=3;          // 连续失败尝试次数
    long long dayLimit=100000;// 每日限额，可以用户自行设置
    long long onceLimit=1000;// 单笔限额，可以用户自行设置
    // 新增与每日限额相关的成员变量
    double dailySpent = 0;              // 当日已用额度
    int lastTransactionYear = 0;         // 上次交易年份
    int lastTransactionMonth = 0;        // 上次交易月份
    int lastTransactionDay = 0;          // 上次交易日期
    // 新增交易记录列表
    std::vector<Transaction> transactionHistory;  // 交易历史记录

public:
    User() = default;
    User(const std::string& accNum, const std::string& pwd, 
         const std::string& name, const std::string& idCard, 
        double initBalance = 10000.0);//构造函数，输入账号、明文密码、姓名、身份证号、初始余额
    // 新增构造函数，直接使用已哈希的密码
    User(const std::string& accNum, const std::string& hashedPwd, 
         const std::string& name, const std::string& idCard, 
         double initBalance, bool isHashed); // 专门用于数据加载，isHashed为true表示密码已哈希
    ~User() ;
    
    void deposit(double amount);//存款
    void withdraw(double amount);//取款
    bool transferTo(User& receiver, double amount);//转账 
    bool verifyPassword(const std::string& pwd);//验证密码
    bool changePassword(const std::string& oldPwd, const std::string& newPwd);//修改密码
    void is_or_not_lockAccount(int isLocked);//锁定账户
    void reportLostAccount();//挂失账户
    void applyUnlockAccount(AccountManager& manager);//申请解锁账户
    std::string getName() const;//获取用户名，用于数据写入和读取，登录可能使用
    std::string getAccountNumber() const;//获取账号，用于数据写入和读取，登录可能使用
    std::string getIDCard() const;//获取身份证号，用于数据写入和读取，登录可能使用
    double getBalance() const;//获取余额，用于数据写入和读取，登录可能使用
    std::string getIsLocked() const;//获取是否被锁定，用于数据写入和读取，登录可能使用
    std::string getHashedPassword() const;//获取加密后的密码，用于数据写入和读取，登录可能使用
    void modifyAccount(std::string accountNumber,std::string name);//修改账户信息（只能修改名字,为了简化流程，防止用户换名和改名，注意：修改账户信息时，不能修改账户编号和身份证号码，不能修改密码和余额）
    bool is_or_not_dayLimit(double amount);//判断是否超过每日限额
    bool is_or_not_onceLimit(double amount);//判断是否超过单笔限额，且每次交易金额为100的倍数
    void addDailySpent(double amount); // 更新当日已用额度
    // 添加修改限额的方法
    bool setDayLimit(double newLimit); // 设置每日限额
    bool setOnceLimit(double newLimit); // 设置单笔限额
    double getDayLimit() const; // 获取每日限额
    double getOnceLimit() const; // 获取单笔限额
    
    // 交易记录相关方法
    void addTransactionRecord(const std::string& type, double amount, const std::string& targetAccount = "");
    void viewTransactionHistory() const; // 查看交易记录
    std::vector<Transaction> getTransactionHistory() const { return transactionHistory; }
    void setTransactionHistory(const std::vector<Transaction>& transactions) { transactionHistory = transactions; }
    
    // 添加获取和设置其他属性的方法，用于数据保存和加载
    bool getIsReportedLost() const { return isReportedLost; }
    void setIsReportedLost(bool lost) { isReportedLost = lost; }
    int getFailedAttempts() const { return failedAttempts; }
    void setFailedAttempts(int attempts) { failedAttempts = attempts; }
    double getDailySpent() const { return dailySpent; }
    void setDailySpent(double spent) { dailySpent = spent; }
    int getLastTransactionYear() const { return lastTransactionYear; }
    void setLastTransactionYear(int year) { lastTransactionYear = year; }
    int getLastTransactionMonth() const { return lastTransactionMonth; }
    void setLastTransactionMonth(int month) { lastTransactionMonth = month; }
    int getLastTransactionDay() const { return lastTransactionDay; }
    void setLastTransactionDay(int day) { lastTransactionDay = day; }
};

#endif