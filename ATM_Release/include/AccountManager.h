#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include "Utils.h"
#include <iostream>
#include <map>
#include <string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<conio.h>
#include<vector>
#include "Transaction.h"  // 添加交易记录头文件

class User;  // 前向声明，避免循环包含
class AccountManager {
private:
    std::map<std::string, User> users;

public:
    AccountManager() = default;
    ~AccountManager();
    void loadAccounts();//加载账户信息
    void addUser( std::string password, 
        std::string name, std::string idCardr);//添加账户
    void deleteAccount(std::string accountNumber);//删除账户
    void findAccount(std::string accountNumber);//查找账户
    void saveAccounts();//保存账户信息
    bool verifyLogin(std::string accountNumber,std::string password);//验证登录
    void lockAccount(std::string accountNumber);//锁定账户(管理员锁定账户的权限，区别与用户登录超过三次的自动锁定)
    void displayAllAccounts();//显示所有账户
    void unlockAccount(const std::string& accountNumber);//解锁账户
    std::map<std::string, User> allusers;//存放所有账户
    std::string getPassword(const std::string&prompt="请输入六位数密码： ");//获取密码
}  ;
#endif // ACCOUNT_MANAGER_H 