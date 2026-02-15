#include "ATMSystem.h"
#include <windows.h>  // 为了使用Sleep函数
//因为快要交作业了，本来打算再写一个管理员的，但是时间紧迫，暂时先完成主要功能
//至于管理员对应的操作函数先展示留下，等闲时继续完成
void ATMSystem::clearScreen(){
    system("cls");
}

void ATMSystem::showAdminLogin(){
    std::string account, password;
    
    std::cout << "================管理员登录================" << std::endl;
    std::cout << std::endl;
    std::cout << "请输入管理员账户： ";
    std::cin >> account;
    std::cout << "请输入管理员密码： ";
    // 使用AccountManager的密码输入功能隐藏密码
    password = manager.getPassword("请输入密码： ");
    
    if (adminManager.authenticateAdmin(account, password)) {
        std::cout << std::endl;
        std::cout << "管理员登录成功！" << std::endl;
        std::cout << std::endl;
        Sleep(2000);
        clearScreen();
        isAdminLoggedIn = true;
        showAdminMainMenu();
    } else {
        std::cout << std::endl;
        std::cout << "管理员账户或密码错误！" << std::endl;
        std::cout << std::endl;
        Sleep(2000);
        clearScreen();
        showlogMenu();
    }
}

void ATMSystem::showAdminMainMenu(){
    adminManager.showAdminMenu();
    
    // 退出管理员菜单后返回普通登录菜单
    isAdminLoggedIn = false;
    clearScreen();
    showlogMenu();
}

void ATMSystem::showlogMenu(){
    int choice;
    std::string input;
    std::cout << "================ATM 系统================" << std::endl;
    std::cout << std::endl;
    std::cout << "           1. 用户登录" << std::endl;
    std::cout << "           2. 用户注册" << std::endl;
    std::cout << "           3. 管理员登录" << std::endl;
    std::cout << "           4. 退出" << std::endl;
    std::cout << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "请选择（输入对应数字即可）： ";
    std::cin >> choice;
    
    switch(choice){
        case 1:{
            std::cout << std::endl;
            std::cout << "请输入账户： ";
            std::cin >> input;
            if(!Utils::validateAccountNumber(input)) {  // 验证账户格式
                std::cout << std::endl;
                std::cout << "账户格式不对，请重新输入，谢谢！" << std::endl;
                Sleep(2000); // 等待2秒后返回主菜单
                clearScreen();
                showlogMenu();
                return;
            }
            std::string tempAccountNumber = input;  // 正确保存输入的账户号
            
            std::cout << "请输入密码： " << std::endl;
            std::string tempPassword = manager.getPassword("请输入六位数密码： ");  // 获取密码输入
            
            // 使用正确的账户号和密码进行验证
            User user = manager.allusers[tempAccountNumber];
            bool loginResult = manager.verifyLogin(tempAccountNumber, tempPassword);
            if (loginResult) {
                isLoggedIn = true;
                // 更新user对象为登录的用户信息
                user = manager.allusers[tempAccountNumber];
                std::cout << std::endl;
                std::cout << "登录成功！欢迎使用ATM！" << std::endl;
                std::cout << std::endl;
                // 延迟一点时间让用户看到欢迎信息
                Sleep(5000);
                clearScreen();
                // showMainMenu会持续显示直到用户选择退出登录
                showMainMenu(user);
                // 当showMainMenu返回时，用户已退出登录
                // 控制权交还给run()函数中的循环，重新显示登录菜单

            } else {
                std::cout << std::endl;
                std::cout << "账户或密码错误！" << std::endl;
                std::cout << std::endl;
                Sleep(2000); // 等待2秒后返回主菜单
                clearScreen();
                showlogMenu();
                return;

            }
            break;  // 修复：添加break，避免执行case2逻辑
        }
        case 2:{
            std::cout << std::endl;
            std::cout << "请输入姓名： ";
            std::cin >> input;
            std::string tempName = input;
            std::cout << "请输入身份证号： ";
            std::cin >> input;
            if(Utils::validateIDCard(input)){
                std::string tempIdcard = input;
                std::cout << std::endl;
                std::string firstPassword = manager.getPassword("请设置六位数密码： ");
                std::string secondPassword = manager.getPassword("请确认密码： ");
                std::string tempPassword;
                
                if(firstPassword == secondPassword){
                    std::cout << std::endl;
                    std::cout << "密码设置成功！" << std::endl;  // 补全std::
                    tempPassword = firstPassword; // 保存明文密码，User构造函数会自动哈希
                }
                else{
                    std::cout << std::endl;
                    std::cout << "密码不一致，请重新输入，谢谢！" << std::endl;  // 补全std::
                    Sleep(1000);
                    clearScreen();
                    showlogMenu();  // 重新显示菜单
                    return;  // 修复：终止当前注册流程，避免创建无效用户
                }
                
                std::string tempAccountNumber = Utils::accountRandom();
                // 使用第一个构造函数（明文密码，isHashed默认为false），明确表示传入的是明文密码
                User tempcurrentUser(tempAccountNumber, tempPassword, tempName, tempIdcard, 10000.0, false);
                manager.allusers[tempAccountNumber] = tempcurrentUser;     
                std::cout << std::endl;
                std::cout << "注册成功，欢迎使用，谢谢！" << std::endl;  // 补全std::
                std::cout << "请在十秒内牢记您的账户： " << tempAccountNumber << std::endl;
                manager.saveAccounts(); // 保存到文件
                Sleep(10000);

            } 
            else {
                std::cout << std::endl;
                std::cout << "身份证号格式错误，请重新输入，谢谢！" << std::endl;  // 补全std::
                clearScreen();
                showlogMenu();  // 显示菜单
                return;  // 修复：终止当前注册流程，避免创建无效用户
            }

            std::cout << std::endl;
            Sleep(2000);
            clearScreen();
            showlogMenu();

            break;  // 修复：添加break
        }
        case 3:{
            // 管理员登录
            clearScreen();
            showAdminLogin();
            break;
        }
        case 4:{  // 修复：补充退出分支
            std::cout << std::endl;
            std::cout << "感谢使用，再见！" << std::endl; 
            std::cout << std::endl;
            Sleep(1000);
            isRunning = false;
            return;
        }
        default:{  // 修复：补充非法输入处理
            std::cout << std::endl;
            std::cout << "输入错误，请选择1-4的数字！" << std::endl;
            std::cout << std::endl;
            Sleep(2000);
            clearScreen();
            showlogMenu();
            return;
        }
    }
}
//展示登录菜单
void ATMSystem::showMainMenu( User& currentUser){
    std::cout << "============= 用户主菜单 =============" << std::endl;
    std::cout << std::endl;
    std::cout << "           1. 查询余额" << std::endl;
    std::cout << "           2. 存款" << std::endl;
    std::cout << "           3. 取款" << std::endl;
    std::cout << "           4. 转账" << std::endl;
    std::cout << "           5. 修改密码" << std::endl;
    std::cout << "           6. 挂失账户或申请解锁账户" << std::endl;
    std::cout << "           7. 限额设置" << std::endl;
    std::cout << "           8. 查看交易记录" << std::endl;
    std::cout << "           9. 退出登录" << std::endl;
    std::cout << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << "请选择： ";
    int choice;
    std::string input;
    //初始化一下（一定要对每个变量写注释，不然第二天就忘记了，这个很重要！！！！！！！！！！！）
    std::cin >> choice;
    switch (choice){
        case 1:{
            double tempBanlance;
            tempBanlance = currentUser.getBalance();
            std::cout << std::endl;
            std::cout << "您的余额为： " << tempBanlance << std::endl;
            std::cout << std::endl;
            Sleep(2000);
            clearScreen();
            showMainMenu(currentUser);
            
            break;
        }//查看余额
        case 2:{
            double tempamounts1, tempamounts2;
            bool isok = true;
            while(isok){
                std::cout << std::endl;
                std::cout << "请输入存款金额： ";
                std::cin >> tempamounts1;
                std::cout << "请确认存款金额： ";
                std::cin >> tempamounts2;
                if(tempamounts1 == tempamounts2){
                    currentUser.deposit(tempamounts2);
                    std::cout << std::endl;
                    std::cout << "已成功存款 " << tempamounts2 << std::endl;
                    std::cout << std::endl;
                    manager.saveAccounts(); // 保存账户数据到文件
                    Sleep(2000);
                    clearScreen();
                    showMainMenu(currentUser);
                    isok = false;
                }
                else if(tempamounts1 != tempamounts2){
                    std::cout << std::endl;
                    std::cout << "存款金额不一致，请重新输入，谢谢！" << std::endl;
                    Sleep(2000);
                }
            }
            break;
        }//存款

        case 3:{
            double tempamounts1, tempamounts2;
            bool isok = true;
            while(isok){
                std::cout << std::endl;
                std::cout << "请输入取款金额(每次只能取100倍数的金额)： ";
                std::cin >> tempamounts1;
                std::cout << "请确认取款金额： ";
                std::cin >> tempamounts2;
                if(tempamounts1 == tempamounts2){
                    if((int)tempamounts1 % 100 == 0){ // 检查是否为100的倍数
                        // 检查余额是否充足
                        if(tempamounts1 > currentUser.getBalance()) {
                            std::cout << std::endl;
                            std::cout << "余额不足，请重新输入，谢谢！" << std::endl;
                            std::cout << std::endl;
                            Sleep(2000);
                            clearScreen();
                            showMainMenu(currentUser);
                            return; // 退出当前操作
                        }
                        
                        // 检查单笔限额
                        if(tempamounts1 > currentUser.getOnceLimit()) {
                            std::cout << std::endl;
                            std::cout << "取款金额超过单笔限额，请重新输入，谢谢！" << std::endl;
                            std::cout << std::endl;
                            Sleep(2000);
                            clearScreen();
                            showMainMenu(currentUser);
                            return; // 退出当前操作
                        }
                        
                        // 检查每日限额
                        if(!currentUser.is_or_not_dayLimit(tempamounts1)) {
                            std::cout << std::endl;
                            std::cout << "取款金额超过当日限额，请重新输入，谢谢！" << std::endl;
                            std::cout << std::endl;
                            Sleep(2000);
                            clearScreen();
                            showMainMenu(currentUser);
                            return; // 退出当前操作
                        }
                        
                        // 执行取款操作
                        currentUser.withdraw(tempamounts2);
                        std::cout << std::endl;
                        std::cout << "已成功取款 " << tempamounts2 << std::endl;
                        std::cout << std::endl;
                        manager.saveAccounts(); // 保存账户数据到文件
                        Sleep(2000);
                        clearScreen();
                        showMainMenu(currentUser);
                        isok = false;
                    }else{
                        std::cout << std::endl;
                        std::cout << "取款金额必须是100的倍数，请重新输入，谢谢！" << std::endl;
                        std::cout << std::endl;
                        Sleep(2000);
                    }
                }
                else if(tempamounts1 != tempamounts2){
                    std::cout << std::endl;
                    std::cout << "取款金额不一致，请重新输入，谢谢！" << std::endl;
                    Sleep(2000);
                }
            }
            break;
        }//取款
        case 4:{
            std::string tempAccountnumber1, tempAccountnumber2;
            double tempamounts1, tempamounts2;
            bool isok = true;
            while(isok){
                std::cout << std::endl;
                std::cout << "请输入转账对象账户：";
                std::cin >> tempAccountnumber1;
                std::cout << "请确认转账对象账户： " << tempAccountnumber1 << std::endl;
                std::cout << "请输入转账金额： ";
                std::cin >> tempamounts1;
                std::cout << "请确认转账金额： " << tempamounts1 << std::endl;
                if(tempAccountnumber1 == tempAccountnumber2 && tempamounts1 == tempamounts2){
                    if(manager.allusers.find(tempAccountnumber1) != manager.allusers.end()){
                        // 检查余额是否充足
                        if(tempamounts1 > currentUser.getBalance()) {
                            std::cout << std::endl;
                            std::cout << "余额不足，无法转账，请重新输入，谢谢！" << std::endl;
                            std::cout << std::endl;
                            Sleep(2000);
                            clearScreen();
                            showMainMenu(currentUser);
                            return; // 退出当前操作
                        }
                        
                        // 检查单笔限额
                        if(tempamounts1 > currentUser.getOnceLimit()) {
                            std::cout << std::endl;
                            std::cout << "转账金额超过单笔限额，请重新输入，谢谢！" << std::endl;
                            std::cout << std::endl;
                            Sleep(2000);
                            clearScreen();
                            showMainMenu(currentUser);
                            return; // 退出当前操作
                        }
                        
                        // 检查每日限额
                        if(!currentUser.is_or_not_dayLimit(tempamounts1)) {
                            std::cout << std::endl;
                            std::cout << "转账金额超过当日限额，请重新输入，谢谢！" << std::endl;
                            std::cout << std::endl;
                            Sleep(2000);
                            clearScreen();
                            showMainMenu(currentUser);
                            return; // 退出当前操作
                        }
                        
                        if(currentUser.transferTo(manager.allusers[tempAccountnumber1], tempamounts1)){
                            std::cout << std::endl;
                            std::cout << "转账成功" << std::endl;
                            std::cout << std::endl;
                            manager.saveAccounts(); // 保存账户数据到文件
                            Sleep(2000);
                            clearScreen();
                            showMainMenu(currentUser);
                            isok = false;
                        }
                        else{
                            std::cout << std::endl;
                            std::cout << "对方账户有问题，或者您的转账金额达到限额，请重试，谢谢！" << std::endl;
                            std::cout << std::endl;
                            Sleep(2000);
                        }
                    }
                    else{
                        std::cout << std::endl;
                        std::cout << "对方账户不存在，请重试，谢谢！" << std::endl;
                        std::cout << std::endl;
                        Sleep(2000);
                    }
                }
                else if(tempAccountnumber1 != tempAccountnumber2 || tempamounts1 != tempamounts2){
                    tempAccountnumber2 = tempAccountnumber1;
                    tempamounts2 = tempamounts1;
                }
            }
            break;
        }//转账
        case 5:{
            std::string tempPassword1, tempPassword2, tempPassword3;
            bool isok = true;
            while(isok){
                std::cout << std::endl;
                std::cout << "请输入原密码： " << std::endl;
                tempPassword1 = manager.getPassword("请输入原密码： ");
                if(currentUser.verifyPassword(tempPassword1)){
                    std::cout << std::endl;
                    tempPassword2 = manager.getPassword("请输入新密码： ");
                    tempPassword3 = manager.getPassword("请确认新密码： ");
                    if(tempPassword2 == tempPassword3){
                        if(currentUser.changePassword(tempPassword1, tempPassword2)){
                            std::cout << std::endl;
                            std::cout << "密码修改成功" << std::endl;
                            std::cout << std::endl;
                            manager.saveAccounts(); // 保存账户数据到文件
                            Sleep(2000);
                            clearScreen();
                            showMainMenu(currentUser);
                            isok = false;
                        }
                        else{
                            std::cout << std::endl;
                            std::cout << "密码格式错误，请重试，谢谢！" << std::endl;
                            std::cout << std::endl;
                            Sleep(2000);
                        }
                    }
                    else{
                        std::cout << std::endl;
                        std::cout << "两次输入密码不一致，请重试，谢谢！" << std::endl;
                        std::cout << std::endl;
                        Sleep(2000);
                    }
                }
                else{
                    std::cout << std::endl;
                    std::cout << "原密码错误，请重试，谢谢！" << std::endl;
                    std::cout << std::endl;
                    Sleep(2000);
                }
            }
            break;
        }//修改密码
        case 6:{
            int tempChoice;
            std::cout << std::endl;
            std::cout << "请选择：" << std::endl;
            std::cout << "1. 挂失账户" << std::endl;
            std::cout << "2. 申请解锁账户" << std::endl;
            std::cin >> tempChoice;
            switch(tempChoice){
                case 1:{
                    currentUser.reportLostAccount();
                    std::cout << std::endl;
                    std::cout << "挂失成功" << std::endl;
                    std::cout << std::endl;
                    manager.saveAccounts(); // 保存账户数据到文件
                    Sleep(2000);
                    clearScreen();
                    showMainMenu(currentUser);
                    break;
                }
                case 2:{
                    currentUser.applyUnlockAccount(manager);
                    std::cout << std::endl;
                    std::cout << "申请成功" << std::endl;
                    std::cout << std::endl;
                    manager.saveAccounts(); // 保存账户数据到文件
                    Sleep(2000);
                    clearScreen();
                    showMainMenu(currentUser);
                    break;
                }
                default:{
                    std::cout << std::endl;
                    std::cout << "输入错误，请重试，谢谢！" << std::endl;
                    std::cout << std::endl;
                    Sleep(2000);
                    clearScreen();
                    showMainMenu(currentUser);
                    break;
                }
            }
            break;
        }//挂失账户或申请解锁账户
        case 7:{
            int tempChoice;
            double templimit1, templimit2;
            std::cout << std::endl;
            std::cout << "请选择：" << std::endl;
            std::cout << "1. 设置每日限额" << std::endl;
            std::cout << "2. 设置单笔限额" << std::endl;
            std::cin >> tempChoice;
            switch(tempChoice){
                case 1:{
                    std::cout << std::endl;
                    std::cout << "当前每日限额为：" << currentUser.getDayLimit() << std::endl;
                    std::cout << "请输入新的每日限额：" << std::endl;
                    std::cin >> templimit1;
                    if(currentUser.setDayLimit(templimit1)){
                        std::cout << std::endl;
                        std::cout << "设置成功" << std::endl;
                        std::cout << std::endl;
                        manager.saveAccounts(); // 保存账户数据到文件
                        Sleep(2000);
                        clearScreen();
                        showMainMenu(currentUser);
                    }
                    else{
                        std::cout << std::endl;
                        std::cout << "设置失败" << std::endl;
                        std::cout << std::endl;
                        Sleep(2000);
                        clearScreen();
                        showMainMenu(currentUser);
                    }
                    break;
                }
                case 2:{
                    std::cout << std::endl;
                    std::cout << "当前单笔限额为：" << currentUser.getOnceLimit() << std::endl;
                    std::cout << "请输入新的单笔限额：" << std::endl;
                    std::cin >> templimit1;
                    if(currentUser.setOnceLimit(templimit1)){
                        std::cout << std::endl;
                        std::cout << "设置成功" << std::endl;
                        std::cout << std::endl;
                        manager.saveAccounts(); // 保存账户数据到文件
                        Sleep(2000);
                        clearScreen();
                        showMainMenu(currentUser);
                    }
                    else{
                        std::cout << std::endl;
                        std::cout << "设置失败" << std::endl;
                        std::cout << std::endl;
                        Sleep(2000);
                        clearScreen();
                        showMainMenu(currentUser);
                    }
                    break;
                }
                default:{
                    std::cout << std::endl;
                    std::cout << "输入错误，请重试，谢谢！" << std::endl;
                    std::cout << std::endl;
                    Sleep(2000);
                    clearScreen();
                    showMainMenu(currentUser);
                    break;
                }
            }
            break;
        }//限额设置
        case 8:{
            std::cout << std::endl;
            currentUser.viewTransactionHistory(); // 调用查看交易记录功能
            std::cout << std::endl;
            Sleep(5000);
            clearScreen();
            showMainMenu(currentUser);
            break;
        }//查看交易记录
        case 9:{
            char confirm;
            std::cout << std::endl;
            std::cout << "确认退出登录？(y/n):";
            std::cin >> confirm;
            if(confirm == 'y' || confirm == 'Y'){
                std::cout << std::endl;
                std::cout << "已退出登录" << std::endl;
                std::cout << std::endl;
                isLoggedIn = false;
                
                // 重置当前用户
                user = User();  // 重置为默认用户
                
                manager.saveAccounts(); // 在退出登录时保存账户数据到文件
                Sleep(2000);
                clearScreen();
                showlogMenu(); // 返回登录菜单
                return;
            }
            else{
                clearScreen();
                showMainMenu(user);
                return;
            }
            break;
        }//退出登录
        default:{
            std::cout << std::endl;
            std::cout << "输入错误，请重试，谢谢！" << std::endl;
            std::cout << std::endl;
            Sleep(2000);
            clearScreen();
            showMainMenu(currentUser);
            break;
        }
    }
}

void ATMSystem::run()
{   
    // 初始化系统状态
    isLoggedIn = false;
    isAdminLoggedIn = false;
    isRunning = true;
    // currentUser = new User();  // 移除：不再需要通过new创建User对象

    // 从文件加载账户数据
    manager.loadAccounts();
    std::cout << std::endl;
    std::cout << "ATM系统正在启动..." << std::endl;
    Sleep(1000);
    std::cout << "系统启动完成！" << std::endl;
    std::cout << std::endl;
    Sleep(1000); // 等待1秒后进入菜单
    clearScreen();
    
    while (isRunning)
    { 
        if(!isLoggedIn && !isAdminLoggedIn){
            showlogMenu();            
        }
        // 登录状态由showMainMenu中的选项9来控制
        // 当用户选择退出登录时，isLoggedIn会被设置为false
        // 此处不需要再次检查和调用showMainMenu
    }
    
    // 移除：不再需要手动清理内存，因为没有使用new动态分配
}