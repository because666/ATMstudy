#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>

class Transaction {
private:
    std::string transactionType;    // 交易类型：存款、取款、转账
    std::string accountFrom;        // 转出账户
    std::string accountTo;          // 转入账户（如果是转账）
    double amount;                  // 交易金额
    std::string timestamp;          // 交易时间
    std::string status;             // 交易状态

public:
    Transaction() = default;
    Transaction(const std::string& type, 
                const std::string& from, 
                double amt, 
                const std::string& to = "");
    
    // 获取函数，用来获取的
    std::string getTransactionType() const;
    std::string getAccountFrom() const;
    std::string getAccountTo() const;
    double getAmount() const;
    std::string getTimestamp() const;
    std::string getStatus() const;
    
    // 设置函数，用来设置
    void setTransactionType(const std::string& type);
    void setAccountFrom(const std::string& from);
    void setAccountTo(const std::string& to);
    void setAmount(double amount);
    void setTimestamp(const std::string& time);
    void setStatus(const std::string& status);
    
    // 显示交易记录
    void displayTransaction() const;
};

#endif