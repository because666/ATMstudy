
#include "../include/AccountManager.h"
#include "../include/User.h"
#include <iostream>
#include <windows.h>  // 为了使用Sleep函数
#include <algorithm> // 为了使用std::find
#include <cmath> // 添加cmath头文件以使用isnan和isinf函数

AccountManager::~AccountManager() {
    // 析构函数实现，目前不需要特殊清理
}

void AccountManager::addUser(std::string password,
    std::string name, std::string idCardr) {
        if (!Utils::validatePassword(password)) {
            std::cout << "Invalid password" << std::endl;
            return;
        }
        if (!Utils::validateIDCard(idCardr)) {  // 修复参数名称
            std::cout << "Invalid ID card" << std::endl;
            return;
        }
        
        User user(Utils::accountRandom(), password, name, idCardr);  // 修复参数名称
        std::ofstream file("accounts.txt", std::ios::app);
        if(!file.is_open()){
            std::cout << "数据文件打不开" << std::endl;
            return;
        }
        // 修复：保存哈希后的密码，而不是原始密码
        file << user.getName() << "," << user.getHashedPassword() << "," << user.getAccountNumber() << "," << user.getIDCard() << "," << user.getBalance() << "," << user.getIsLocked() << "," << (user.getIsReportedLost() ? "是" : "否") << "," << user.getFailedAttempts() << "," << user.getDayLimit() << "," << user.getOnceLimit() << "," << user.getDailySpent() << "," << user.getLastTransactionYear() << "," << user.getLastTransactionMonth() << "," << user.getLastTransactionDay() << std::endl;
        file.close();
    }//添加用户
void AccountManager::loadAccounts() {
    std::ifstream file("./accounts.txt");  // 修复路径问题
    if (!file.is_open()) {
        std::cout << "无法打开账户文件进行读取！" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string accountNumber, hashedPassword, name, idCard, balanceStr, 
                    isLockedStr, isReportedLostStr, dayLimitStr, onceLimitStr,
                    dailySpentStr, lastTransactionYearStr, lastTransactionMonthStr, 
                    lastTransactionDayStr, failedAttemptsStr;
        
        if (std::getline(iss, accountNumber, ',') &&
            std::getline(iss, hashedPassword, ',') &&
            std::getline(iss, name, ',') &&
            std::getline(iss, idCard, ',') &&
            std::getline(iss, balanceStr, ',') &&
            std::getline(iss, isLockedStr, ',') &&
            std::getline(iss, isReportedLostStr, ',') &&
            std::getline(iss, dayLimitStr, ',') &&
            std::getline(iss, onceLimitStr, ',') &&
            std::getline(iss, dailySpentStr, ',') &&
            std::getline(iss, lastTransactionYearStr, ',') &&
            std::getline(iss, lastTransactionMonthStr, ',') &&
            std::getline(iss, lastTransactionDayStr, ',') &&
            std::getline(iss, failedAttemptsStr)) {
            
            // 添加安全的数值转换，处理异常情况
            double balance;
            try {
                balance = std::stod(balanceStr);
                // 验证数值是否有效
                if (balance <= 0 || balance == 0.0 || std::isnan(balance) || std::isinf(balance)) {
                    balance = 10000.0;  // 使用默认值
                }
            } catch (...) {
                balance = 10000.0;  // 使用默认值
            }

            bool isLocked = (isLockedStr == "是");
            bool isReportedLost = (isReportedLostStr == "是");

            long long dayLimit;
            try {
                dayLimit = std::stoll(dayLimitStr);
                if (dayLimit <= 0 || std::isnan(dayLimit) || std::isinf(dayLimit)) {
                    dayLimit = 10000;  // 使用默认值
                }
            } catch (...) {
                dayLimit = 10000;  // 使用默认值
            }

            long long onceLimit;
            try {
                onceLimit = std::stoll(onceLimitStr);
                if (onceLimit <= 0 || std::isnan(onceLimit) || std::isinf(onceLimit)) {
                    onceLimit = 5000;  // 使用默认值
                }
            } catch (...) {
                onceLimit = 5000;  // 使用默认值
            }

            double dailySpent;
            try {
                dailySpent = std::stod(dailySpentStr);
                if (dailySpent < 0 || std::isnan(dailySpent) || std::isinf(dailySpent)) {
                    dailySpent = 0;  // 使用默认值
                }
            } catch (...) {
                dailySpent = 0;  // 使用默认值
            }

            int lastTransactionYear;
            try {
                lastTransactionYear = std::stoi(lastTransactionYearStr);
                if (lastTransactionYear < 0 || lastTransactionYear > 9999) {
                    lastTransactionYear = 0;  // 使用默认值
                }
            } catch (...) {
                lastTransactionYear = 0;  // 使用默认值
            }

            int lastTransactionMonth;
            try {
                lastTransactionMonth = std::stoi(lastTransactionMonthStr);
                if (lastTransactionMonth < 0 || lastTransactionMonth > 12) {
                    lastTransactionMonth = 0;  // 使用默认值
                }
            } catch (...) {
                lastTransactionMonth = 0;  // 使用默认值
            }

            int lastTransactionDay;
            try {
                lastTransactionDay = std::stoi(lastTransactionDayStr);
                if (lastTransactionDay < 0 || lastTransactionDay > 31) {
                    lastTransactionDay = 0;  // 使用默认值
                }
            } catch (...) {
                lastTransactionDay = 0;  // 使用默认值
            }

            int failedAttempts;
            try {
                failedAttempts = std::stoi(failedAttemptsStr);
                if (failedAttempts < 0) {
                    failedAttempts = 3;  // 使用默认值
                }
            } catch (...) {
                failedAttempts = 3;  // 使用默认值
            }
            
            // 创建用户对象
            User user(accountNumber, hashedPassword, name, idCard, balance, true);
            user.is_or_not_lockAccount(isLocked ? 2 : 1);  // 2表示锁定，1表示解锁
            user.setIsReportedLost(isReportedLost);
            user.setDayLimit(static_cast<double>(dayLimit));
            user.setOnceLimit(static_cast<double>(onceLimit));
            user.setDailySpent(dailySpent);
            user.setLastTransactionYear(lastTransactionYear);
            user.setLastTransactionMonth(lastTransactionMonth);
            user.setLastTransactionDay(lastTransactionDay);
            user.setFailedAttempts(failedAttempts);
            
            // 读取交易记录数量
            std::string transactionCountStr;
            if (std::getline(iss, transactionCountStr)) {
                int transactionCount;
                try {
                    transactionCount = std::stoi(transactionCountStr);
                    if (transactionCount < 0) transactionCount = 0;
                } catch (...) {
                    transactionCount = 0;  // 默认没有交易记录
                }
                
                // 读取每条交易记录
                for (int i = 0; i < transactionCount; ++i) {
                    std::string transactionLine;
                    if (std::getline(file, transactionLine)) {
                        std::istringstream tiss(transactionLine);
                        std::string type, from, to, amountStr, timestamp, status;
                        
                        if (std::getline(tiss, type, '|') &&
                            std::getline(tiss, from, '|') &&
                            std::getline(tiss, to, '|') &&
                            std::getline(tiss, amountStr, '|') &&
                            std::getline(tiss, timestamp, '|') &&
                            std::getline(tiss, status)) {
                            
                            double amount;
                            try {
                                amount = std::stod(amountStr);
                                if (amount <= 0 || std::isnan(amount) || std::isinf(amount)) continue;
                            } catch (...) {
                                continue;  // 跳过无效交易记录
                            }

                            Transaction transaction;
                            transaction.setTransactionType(type);
                            transaction.setAccountFrom(from);
                            transaction.setAccountTo(to);
                            transaction.setAmount(amount);
                            transaction.setTimestamp(timestamp);
                            transaction.setStatus(status);
                            
                            // 添加到用户交易历史
                            std::vector<Transaction> history = user.getTransactionHistory();
                            history.push_back(transaction);
                            user.setTransactionHistory(history);
                        }
                    }
                }
            }
            
            allusers[accountNumber] = user;
        }
    }
    
    file.close();
}

void AccountManager::saveAccounts() {
    std::ofstream file("./accounts.txt");  // 修复路径问题
    if (!file.is_open()) {
        std::cout << "无法打开账户文件进行写入！" << std::endl;
        return;
    }

    for (const auto& pair : allusers) {
        const User& user = pair.second;
        file << user.getAccountNumber() << ","
             << user.getHashedPassword() << ","
             << user.getName() << ","
             << user.getIDCard() << ","
             << user.getBalance() << ","
             << user.getIsLocked() << ","
             << (user.getIsReportedLost() ? "是" : "否") << ","
             << user.getDayLimit() << ","
             << user.getOnceLimit() << ","
             << user.getDailySpent() << ","
             << user.getLastTransactionYear() << ","
             << user.getLastTransactionMonth() << ","
             << user.getLastTransactionDay() << ","
             << user.getFailedAttempts();
        
        // 保存交易记录数量
        std::vector<Transaction> history = user.getTransactionHistory();
        file << "," << history.size() << "\n";
        
        // 保存每条交易记录
        for (const Transaction& transaction : history) {
            file << transaction.getTransactionType() << "|"
                 << transaction.getAccountFrom() << "|"
                 << transaction.getAccountTo() << "|"
                 << transaction.getAmount() << "|"
                 << transaction.getTimestamp() << "|"
                 << transaction.getStatus() << "\n";
        }
    }

    file.close();
}

void AccountManager::deleteAccount(std ::string accountNumber){
    // 修复：如果找不到用户，应该提示错误
    if(allusers.find(accountNumber)==allusers.end()){ // 修复：使用allusers而不是users
        std::cout << "要删除的账户不存在，要么输入的账户不存在，要么输入的账户已删除" << std::endl;
        return;
    }
    allusers.erase(accountNumber);//删除用户
    saveAccounts();
}
void AccountManager::findAccount(std::string acn){
    // 修复：如果找到了用户，应该显示信息；否则提示不存在
    if(allusers.find(acn)!=allusers.end()){ // 修复：使用allusers而不是users
        std::cout << "用户信息如下：" << std::endl;
        std::cout << "用户名：" << allusers[acn].getName() << std::endl;
        std::cout << "账户号：" << allusers[acn].getAccountNumber() << std::endl;
        std::cout << "身份证号：" << allusers[acn].getIDCard() << std::endl;
        std::cout << "余额：" << allusers[acn].getBalance() << std::endl;
        std::cout << "是否锁定：" << allusers[acn].getIsLocked() << std::endl;
        
    }
    else{
        std::cout << "用户不存在" << std::endl;
    }
}
bool AccountManager::verifyLogin(std::string acn,std::string ps){
    if(!Utils::validateAccountNumber(acn)){
        std::cout << "账户号格式错误" << std::endl;
        return false;
    }
    if(!Utils::validatePassword(ps)){
        std::cout << "密码格式错误" << std::endl;
        return false;
    }


    if(allusers.find(acn)!=allusers.end()){ // 修复：使用allusers而不是users
        // 保存原始用户对象的引用，以便后续更新
        User& user = allusers[acn];
        
        // 检查账户是否被锁定或挂失
        if(user.getIsLocked() == "是" || user.getIsReportedLost()) {
            std::cout << "账户已被锁定或挂失，无法登录" << std::endl;
            return false;
        }
        
        if(user.verifyPassword(ps)){
            std::cout << "登录成功" << std::endl;
            std::cout << "用户信息如下：" << std::endl;
            std::cout << "用户名：" << user.getName() << std::endl;
            std::cout << "账户号：" << user.getAccountNumber() << std::endl;
            std::cout << "身份证号：" << user.getIDCard() << std::endl;
            std::cout << "余额：" << user.getBalance() << std::endl;
            std::cout << "当前登录时间：" << std::time(nullptr) << std::endl; //获取当前时间，为了后面的验证超时登录使用（太难写了，后面写完全部再加上吧）
            
            // 重置失败尝试次数
            user.setFailedAttempts(3);
            saveAccounts(); // 保存账户信息
            
            return true;
        }
        else{
            // 密码验证失败，获取更新后的失败次数
            int remainingAttempts = user.getFailedAttempts();
            std::cout << "密码错误" << std::endl;
            
            if(remainingAttempts <= 0) {
                std::cout << "密码错误次数过多，账户已被锁定" << std::endl;
                saveAccounts(); // 保存锁定状态
            } else {
                std::cout << "您还有 " << remainingAttempts << " 次机会输入密码" << std::endl;
            }
            
            return false;
        }
    }
    else{
        std::cout << "用户不存在" << std::endl;
        return false;
    }
}//验证登录
void AccountManager::lockAccount(std::string acn){
    if(allusers.find(acn)!=allusers.end()){ // 修复：使用allusers而不是users
        allusers[acn].is_or_not_lockAccount(2);  // 使用参数2表示锁定
        std::cout << "账户已锁定" << std::endl;
        saveAccounts(); // 保存更改
    }
    else{
        std::cout << "用户不存在" << std::endl;
    }

}  //锁定账号（不是用户登录超过三次的锁定，是管理员指定的锁定）
void AccountManager::displayAllAccounts(){
    std::cout << "所有用户信息如下：" << std::endl;
    for(auto&user:allusers){ // 修复：使用allusers而不是users
        std::cout << "用户名：" << user.second.getName() << "," << "账户号：" << user.second.getAccountNumber()
        << "," << "身份证号：" << user.second.getIDCard() << "," << "是否锁定：" <<
        user.second.getIsLocked() << std::endl;
    }
}//列出所有用户信息
void AccountManager::unlockAccount(const std::string& accountNumber){
    if(allusers.find(accountNumber)!=allusers.end()){ // 修复：使用allusers而不是users
        allusers[accountNumber].is_or_not_lockAccount(1);  // 使用参数1表示解锁
        saveAccounts(); // 保存更改
    }
    else{
        std::cout << "用户不存在" << std::endl;
    }
}
std ::string AccountManager::getPassword(const std::string&prompt){
    std::cout << prompt;
    std::string password;
    char ch;
    while(true){
        ch=_getch();
        if(ch=='\r'||ch=='\n'){
            std::cout << std::endl;
            break;
            
        }
        else if(ch=='\b'||ch==127){
                if(!password.empty()){
                    std::cout << "\b \b";
                    password.pop_back();
                }
            }//处理密码输入，隐藏输入的密码，避免密码泄露，提高安全性
        else{
            std::cout << '*';
            password.push_back(ch);
        }
    }
    Utils::validatePassword(password);
    return password;
}