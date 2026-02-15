#ifndef ADMIN_MANAGER_H
#define ADMIN_MANAGER_H

#include "User.h"
#include "AccountManager.h"
#include "Utils.h"
#include <iostream>
#include <string>
#include <vector>

class AdminManager {
private:
    AccountManager& accountManager;
    std::string adminAccount;
    std::string adminPassword;

public:
    AdminManager(AccountManager& manager);
    ~AdminManager() = default;
    
    // 管理员登录验证
    bool authenticateAdmin(const std::string& account, const std::string& password);
    
    // 显示管理员菜单
    void showAdminMenu();
    
    // 管理员功能
    void viewAllAccounts();           // 查看所有账户
    void searchAccount();             // 搜索特定账户
    void lockAccount();               // 锁定账户
    void unlockAccount();             // 解锁账户
    void deleteAccount();             // 删除账户
    void viewSystemLogs();            // 查看系统日志
    void generateReport();            // 生成报表
    
    // 系统管理功能
    void backupData();                // 备份数据
    void restoreData();               // 恢复数据
};

#endif