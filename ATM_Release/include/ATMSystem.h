#ifndef ATMSystem_H
#define ATMSystem_H
#include"AccountManager.h"
#include"User.h"
#include"Utils.h"
#include"AdminManager.h"  
#include<iostream>

class ATMSystem {
public:
    ATMSystem() = default;
    void run();
    bool isRunning = true;
    bool isLoggedIn = false;
    bool isAdminLoggedIn = false;  // 是否为管理员登录
    User* currentUser = nullptr;
    void clearScreen();
    void showlogMenu();
    void showMainMenu( User& user);
    void showAdminLogin();  // 添加管理员登录菜单
    void showAdminMainMenu();  // 添加管理员主菜单
private:
    AccountManager manager;
    User user;
    AdminManager adminManager{manager};  // 添加管理员
};

#endif