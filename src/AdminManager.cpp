#include "../include/AdminManager.h"
#include "../include/Constants.h"
#include <windows.h>

// 构造函数
AdminManager::AdminManager(AccountManager& manager) : 
    accountManager(manager),
    adminAccount(ADMIN_ACCOUNT),
    adminPassword(ADMIN_PASSWORD) {}

// 管理员登录验证
bool AdminManager::authenticateAdmin(const std::string& account, const std::string& password) {
    return (account == adminAccount && password == adminPassword);
}

// 显示管理员菜单
void AdminManager::showAdminMenu() {
    int choice;
    
    while (true) {
        std::cout << "============= 管理员系统 =============" << std::endl;
        std::cout << std::endl;
        std::cout << "           1. 查看所有账户" << std::endl;
        std::cout << "           2. 搜索特定账户" << std::endl;
        std::cout << "           3. 锁定账户" << std::endl;
        std::cout << "           4. 解锁账户" << std::endl;
        std::cout << "           5. 删除账户" << std::endl;
        std::cout << "           6. 查看系统日志" << std::endl;
        std::cout << "           7. 生成报表" << std::endl;
        std::cout << "           8. 数据备份" << std::endl;
        std::cout << "           9. 恢复数据" << std::endl;
        std::cout << "           10. 退出管理员系统" << std::endl;
        std::cout << std::endl;
        std::cout << "=====================================" << std::endl;
        std::cout << "请选择： ";
        
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                viewAllAccounts();
                break;
            case 2:
                searchAccount();
                break;
            case 3:
                lockAccount();
                break;
            case 4:
                unlockAccount();
                break;
            case 5:
                deleteAccount();
                break;
            case 6:
                viewSystemLogs();
                break;
            case 7:
                generateReport();
                break;
            case 8:
                backupData();
                break;
            case 9:
                restoreData();
                break;
            case 10:
                std::cout << "退出管理员系统..." << std::endl;
                Sleep(1000);
                return;
            default:
                std::cout << "输入错误，请重新选择！" << std::endl;
                Sleep(1000);
                break;
        }
        
        std::cout << std::endl;
        system("pause");
        system("cls");
    }
}

// 查看所有账户
void AdminManager::viewAllAccounts() {
    std::cout << "============= 所有账户信息 =============" << std::endl;
    std::cout << std::endl;
    
    if (accountManager.allusers.empty()) {
        std::cout << "暂无账户信息！" << std::endl;
        return;
    }
    
    int count = 0;
    for (const auto& pair : accountManager.allusers) {
        const User& user = pair.second;
        std::cout << "账户号：" << user.getAccountNumber() << std::endl;
        std::cout << "用户名：" << user.getName() << std::endl;
        std::cout << "身份证号：" << user.getIDCard() << std::endl;
        std::cout << "余额：" << user.getBalance() << std::endl;
        std::cout << "是否锁定：" << user.getIsLocked() << std::endl;
        std::cout << "是否挂失：" << (user.getIsReportedLost() ? "是" : "否") << std::endl;
        std::cout << "每日限额：" << user.getDayLimit() << std::endl;
        std::cout << "单笔限额：" << user.getOnceLimit() << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        count++;
    }
    
    std::cout << "共 " << count << " 个账户" << std::endl;
}

// 搜索特定账户
void AdminManager::searchAccount() {
    std::string accountNumber;
    std::cout << "请输入要搜索的账户号：";
    std::cin >> accountNumber;
    
    auto it = accountManager.allusers.find(accountNumber);
    if (it != accountManager.allusers.end()) {
        const User& user = it->second;
        std::cout << std::endl;
        std::cout << "账户信息如下：" << std::endl;
        std::cout << "账户号：" << user.getAccountNumber() << std::endl;
        std::cout << "用户名：" << user.getName() << std::endl;
        std::cout << "身份证号：" << user.getIDCard() << std::endl;
        std::cout << "余额：" << user.getBalance() << std::endl;
        std::cout << "是否锁定：" << user.getIsLocked() << std::endl;
        std::cout << "是否挂失：" << (user.getIsReportedLost() ? "是" : "否") << std::endl;
        std::cout << "每日限额：" << user.getDayLimit() << std::endl;
        std::cout << "单笔限额：" << user.getOnceLimit() << std::endl;
        std::cout << "失败尝试次数：" << user.getFailedAttempts() << std::endl;
        std::cout << "当日已用额度：" << user.getDailySpent() << std::endl;
    } else {
        std::cout << "账户不存在！" << std::endl;
    }
}

// 锁定账户
void AdminManager::lockAccount() {
    std::string accountNumber;
    std::cout << "请输入要锁定的账户号：";
    std::cin >> accountNumber;
    
    auto it = accountManager.allusers.find(accountNumber);
    if (it != accountManager.allusers.end()) {
        it->second.is_or_not_lockAccount(2);  // 2表示锁定
        accountManager.saveAccounts(); // 保存更改
        std::cout << "账户 " << accountNumber << " 已被锁定！" << std::endl;
    } else {
        std::cout << "账户不存在！" << std::endl;
    }
}

// 解锁账户
void AdminManager::unlockAccount() {
    std::string accountNumber;
    std::cout << "请输入要解锁的账户号：";
    std::cin >> accountNumber;
    
    auto it = accountManager.allusers.find(accountNumber);
    if (it != accountManager.allusers.end()) {
        it->second.is_or_not_lockAccount(1);  // 1表示解锁
        accountManager.saveAccounts(); // 保存更改
        std::cout << "账户 " << accountNumber << " 已被解锁！" << std::endl;
    } else {
        std::cout << "账户不存在！" << std::endl;
    }
}

// 删除账户
void AdminManager::deleteAccount() {
    std::string accountNumber;
    std::cout << "请输入要删除的账户号：";
    std::cin >> accountNumber;
    
    auto it = accountManager.allusers.find(accountNumber);
    if (it != accountManager.allusers.end()) {
        accountManager.allusers.erase(it);
        accountManager.saveAccounts(); // 保存更改
        std::cout << "账户 " << accountNumber << " 已被删除！" << std::endl;
    } else {
        std::cout << "账户不存在！" << std::endl;
    }
}

// 查看系统日志（模拟）
void AdminManager::viewSystemLogs() {
    std::cout << "============= 系统日志 =============" << std::endl;
    std::cout << "（这是一个演示功能，实际系统中应实现日志记录）" << std::endl;
    std::cout << "- 系统启动时间：2026-01-10 00:55:43" << std::endl;
    std::cout << "- 管理员登录时间：2026-01-10 00:55:45" << std::endl;
    std::cout << "- 当前在线用户数：" << accountManager.allusers.size() << std::endl;
    std::cout << "- 总交易次数：待实现" << std::endl;
    std::cout << "- 系统运行状态：正常" << std::endl;
}

// 生成报表（模拟）
void AdminManager::generateReport() {
    std::cout << "============= 系统报表 =============" << std::endl;
    std::cout << "总账户数：" << accountManager.allusers.size() << std::endl;
    
    double totalBalance = 0;
    int lockedAccounts = 0;
    int reportedLostAccounts = 0;
    
    for (const auto& pair : accountManager.allusers) {
        const User& user = pair.second;
        totalBalance += user.getBalance();
        if (user.getIsLocked() == "是") lockedAccounts++;
        if (user.getIsReportedLost()) reportedLostAccounts++;
    }
    
    std::cout << "总余额：" << totalBalance << std::endl;
    std::cout << "平均余额：" << (accountManager.allusers.empty() ? 0 : totalBalance / accountManager.allusers.size()) << std::endl;
    std::cout << "锁定账户数：" << lockedAccounts << std::endl;
    std::cout << "挂失账户数：" << reportedLostAccounts << std::endl;
}

// 数据备份
void AdminManager::backupData() {
    // 读取原始账户数据
    std::ifstream src("./accounts.txt");
    std::ofstream dst("./accounts_backup.txt");
    
    if (src.is_open() && dst.is_open()) {
        dst << src.rdbuf();
        std::cout << "数据备份成功！" << std::endl;
    } else {
        std::cout << "数据备份失败！" << std::endl;
    }
    
    if (src.is_open()) src.close();
    if (dst.is_open()) dst.close();
}

// 恢复数据
void AdminManager::restoreData() {
    char confirm;
    std::cout << "确定要恢复数据吗？这将覆盖当前数据！(y/n): ";
    std::cin >> confirm;
    
    if (confirm == 'y' || confirm == 'Y') {
        std::ifstream src("./accounts_backup.txt");
        std::ofstream dst("./accounts.txt");
        
        if (src.is_open() && dst.is_open()) {
            dst << src.rdbuf();
            std::cout << "数据恢复成功！" << std::endl;
            // 重新加载账户数据
            accountManager.allusers.clear();
            accountManager.loadAccounts();
        } else {
            std::cout << "数据恢复失败！" << std::endl;
        }
        
        if (src.is_open()) src.close();
        if (dst.is_open()) dst.close();
    } else {
        std::cout << "已取消数据恢复操作。" << std::endl;
    }
}