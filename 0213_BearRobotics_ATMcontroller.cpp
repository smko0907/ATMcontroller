#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

// ATM Controller 
class ATMController {
private:
    unordered_map<string, int> accountBalance; // Account ID, balance
    string currentAccount;
    string currency = "$";

public:
    ATMController() {
        // Sample accounts for simulation
        accountBalance["123456789"] = 0; // Initial balance of account ID "123456789" : $0
        accountBalance["123123123"] = 100000; // Initial balance of account ID "123123123" : $100000
        accountBalance["123000321"] = 300; // Initial balance of account ID "123000321" : $300
    }

    bool insertCard(string accountID){
        // Check if the account ID starts with "123"
        if (accountID.substr(0, 3) != "123") {
            cout << "We do not accept cards from other banks.\n";
            ejectCard();
            return false;
        }
        // Check if the account exists in the bank system
        if (accountBalance.find(accountID) != accountBalance.end()) {
            currentAccount = accountID;
            return true;
        }
        else {
            cout << "Invalid account ID. Please try again. \n";
            ejectCard();
            return false;
        }
    }

    void displayBalance() {
        if (currentAccount.empty()) return;
        cout << "Current Balance: " << currency << accountBalance[currentAccount] << endl;
    }

    void cashDeposit(int amount){
        if (currentAccount.empty()) return;
        accountBalance[currentAccount] += amount;
        cout << "Deposit Complete: " << currency << amount << endl;
    }

    void transfer(int amount, string accountNo){
        if (currentAccount.empty()) return;
        if (accountBalance.find(accountNo) != accountBalance.end()) {
            cout << "Transfering to :" << accountNo << endl;
            if (accountBalance[currentAccount] >= amount) {
                accountBalance[currentAccount] -= amount;
                cout << "Transaction Completed :" << currency << amount << endl;
            }
            else cout << "Insufficient Balance. Transaction Incomplete. \n";
        }
        else cout << "Invalid account. Please Try again. \n";
    }

    void cashWithdrawal (int amount){
        if (currentAccount.empty()) return;
        if (accountBalance[currentAccount] >= amount) {
            accountBalance[currentAccount] -= amount;
            cout << " Withdrawal Complete: " << currency << amount << endl;
        }
        else cout << "Insufficient Balance. Withdrawal Incomplete. \n";
    }

    void ejectCard() {
        currentAccount = "";
        cout << "Please tkae your card and receipt. \n";
    }
};


// Input Test
int main() {
    ATMController atm;

    if (atm.insertCard("123123123")) {
        atm.displayBalance();
        atm.cashDeposit(2000);
        atm.transfer(300,"123456789");
        atm.cashWithdrawal(50);
        atm.ejectCard();
    }
    return 0;
}