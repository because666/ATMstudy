#include "Transaction.h"
#include <iostream>
#include <iomanip>

// 构造函数
Transaction::Transaction(const std::string& type, 
                         const std::string& from, 
                         double amt, 
                         const std::string& to) 
    : transactionType(type), accountFrom(from), amount(amt), accountTo(to) {
    // 生成当前时间戳
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    timestamp = std::string(buf);
    status = "成功";  // 默认交易成功
}

// Getter方法
std::string Transaction::getTransactionType() const {
    return transactionType;
}

std::string Transaction::getAccountFrom() const {
    return accountFrom;
}

std::string Transaction::getAccountTo() const {
    return accountTo;
}

double Transaction::getAmount() const {
    return amount;
}

std::string Transaction::getTimestamp() const {
    return timestamp;
}

std::string Transaction::getStatus() const {
    return status;
}

// Setter方法
void Transaction::setTransactionType(const std::string& type) {
    transactionType = type;
}

void Transaction::setAccountFrom(const std::string& from) {
    accountFrom = from;
}

void Transaction::setAccountTo(const std::string& to) {
    accountTo = to;
}

void Transaction::setAmount(double amount) {
    this->amount = amount;
}

void Transaction::setTimestamp(const std::string& time) {
    timestamp = time;
}

void Transaction::setStatus(const std::string& status) {
    this->status = status;
}

// 显示交易记录
void Transaction::displayTransaction() const {
    std::cout << "交易时间: " << timestamp << std::endl;
    std::cout << "交易类型: " << transactionType << std::endl;
    std::cout << "转出账户: " << accountFrom << std::endl;
    if (!accountTo.empty()) {
        std::cout << "转入账户: " << accountTo << std::endl;
    }
    std::cout << "交易金额: " << std::fixed << std::setprecision(2) << amount << std::endl;
    std::cout << "交易状态: " << status << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}