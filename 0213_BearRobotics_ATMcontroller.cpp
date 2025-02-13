#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

// -------------------------
// Bank system
// (to securely separate account information from the atm)
// -------------------------

class BankSystem {
private:
    unordered_map<string, int> accountBalance; // Account ID, balance
    unordered_map<string, string> accountPin;  // Account ID, PIN
public:
    BankSystem(){
        // Sample accounts for simulation
        accountBalance["123456789"] = 0; // Initial balance of account ID "123456789" : $0
        accountPin["123456789"] = "1234"; // Pin number of account ID "123456789"

        accountBalance["123123123"] = 100000; // Initial balance of account ID "123123123" : $100000
        accountPin["123123123"] = "2323"; // Pin number of account ID "123123123"

        accountBalance["123000321"] = 300; // Initial balance of account ID "123000321" : $300
        accountPin["123000321"] = "0000"; // Pin number of account ID "123000321"
    }

    // Verify if an account exists
    bool validAccount(const string& accountID){
        return accountBalance.find(accountID) != accountBalance.end();
    }

    // Verify if the pins are correct
    bool verifyPin (const string& accountID, const string& inputPin){
        return accountPin[accountID] == inputPin;
    }

    // Get the balance by matching the account ID
    int findBalance(const string& accountID){
        return accountBalance[accountID];
    }
};

// -------------------------
// ATM controller
// (i/o system for users)
// -------------------------
class ATMController {
private:
    BankSystem& bank; // Reference to the bank system
    string currentAccount; // To store the current Account ID
    string currency = "$"; // Currency of the bank account
    int currentBalance = 0;
    bool authentication = false; // Flag to check the authentication

public:
    ATMController(BankSystem& bankSystem) : bank(bankSystem) {}

    bool insertCard(string accountID){
        // Check if the account ID starts with "123"
        if (accountID.substr(0, 3) != "123") {
            cout << "We do not accept cards from other banks.\n";
            ejectCard();
            return false;
        }
        // Check if the account exists in the bank system
        if (bank.validAccount(accountID)) {
            cout << "Card inserted successfully. \n";
            currentAccount = accountID;
            return true;
        }
        else {
            cout << "Invalid account ID. Please try again. \n";
            ejectCard();
            return false;
        }
    }

    bool enterPIN(string inputPin) {
        // Double-check whether if the card is readable
        if (currentAccount.empty()) {
            cout << "Please insert your card. \n";
            return false;
        }
        // Check if the PIN is correct
        if (bank.verifyPin(currentAccount, inputPin)) {
            cout << "PIN verified. \n";
            authentication = true;
            return true;
        }
        else {
            cout << "Invalid PIN. Please try again. \n";
            return false;
        }
    }

    bool selectAccount(){
        // Double-check for authentication
        if (!authentication){
            cout << "Please enter your PIN first. \n";
            return false;
        }
        // Select account for the next procedure
        cout << "Proceeding with account: " << currentAccount << endl;
        return true;
    }

    void displayBalance() {
        // Double-check for authentication
        if (!authentication){
            cout << "Please enter your PIN first. \n";
            return;
        }
        // Display the current balance of the selected account
        currentBalance = bank.findBalance(currentAccount);
        cout << "Current Balance: " << currency << currentBalance << endl;
    }

    void cashDeposit(int amount){
        // Double-check for authentication
        if (!authentication){
            cout << "Please enter your PIN first. \n";
            return;
        }
        // Deposit cash to the current account
        currentBalance += amount;
        cout << "Deposit Complete: " << currency << amount << endl;
    }

    void transfer(int amount, string recipient){
        // Double-check for authentication
        if (!authentication){
            cout << "Please enter your PIN first. \n";
            return;
        }
        // Check if the recipient's bank account exists in the bank system
        if (bank.validAccount(recipient)) {
            cout << "Transfering to: " << recipient << endl;
            if (currentBalance >= amount) {
                currentBalance -= amount;
                cout << "Transaction Completed :" << currency << amount << endl;
            }
            else cout << "Insufficient Balance. Transaction Incomplete. \n";
        }
        else cout << "Invalid account. Please Try again. \n";
    }

    void cashWithdrawal (int amount){
        // Double-check for authentication
        if (!authentication){
            cout << "Please enter your PIN first. \n";
            return;
        }
        // Check if there is sufficient amount of money and then proceed to cash withdrawal
        if (currentBalance >= amount) {
            currentBalance -= amount;
            cout << "Withdrawal Complete: " << currency << amount << endl;
        }
        else cout << "Insufficient Balance. Withdrawal Incomplete. \n";
    }

    void ejectCard() {
        // Eject the card and receipt once all the processes are done
        cout << "Please take your card and receipt. \n";
        currentAccount = "";
        authentication = false;
    }
};


// -------------------------
// Input test for ATM simulation
// -------------------------
int main() {
    BankSystem bank;
    ATMController atm(bank);

    // Simulation for account ID "123123123"
    if (atm.insertCard("123123123")) {
        if (atm.enterPIN("2323")){
            if (atm.selectAccount()){
                atm.displayBalance();
                atm.cashDeposit(2000);
                atm.transfer(300,"123456789");
                atm.cashWithdrawal(50);
            }
        }
        atm.ejectCard();
    }
    return 0;
}