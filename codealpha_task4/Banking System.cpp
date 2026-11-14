#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <ctime>

using namespace std;

class Customer {
public:
    int id;
    string name;
    string phone;
};

class Account {
public:
    int number;
    int customerId;
    double balance;
};

class Transaction {
public:
    int accountNumber;
    string type;
    double amount;
    string date;
};

map<int, Customer> customers;
map<int, Account> accounts;
vector<Transaction> transactions;

int nextCustomerId = 1;
int nextAccountNumber = 1001;

string currentDate() {

    time_t now = time(0);
    tm *local = localtime(&now);

    char buffer[30];

    strftime(buffer, sizeof(buffer),
             "%Y-%m-%d %H:%M:%S", local);

    return buffer;
}

void saveData() {

    ofstream customerFile("customers.txt");
    ofstream accountFile("accounts.txt");
    ofstream transactionFile("transactions.txt");

    for (auto item : customers) {

        customerFile << item.second.id << "|"
                     << item.second.name << "|"
                     << item.second.phone << "\n";
    }

    accountFile << fixed << setprecision(2);

    for (auto item : accounts) {

        accountFile << item.second.number << "|"
                    << item.second.customerId << "|"
                    << item.second.balance << "\n";
    }

    transactionFile << fixed << setprecision(2);

    for (const Transaction &t : transactions) {

        transactionFile << t.accountNumber << "|"
                        << t.type << "|"
                        << t.amount << "|"
                        << t.date << "\n";
    }
}

void loadData() {

    ifstream customerFile("customers.txt");

    Customer c;
    char separator;

    while (customerFile >> c.id >> separator &&
           getline(customerFile, c.name, '|') &&
           getline(customerFile, c.phone)) {

        if (!c.name.empty() && c.name[0] == ' ')
            c.name.erase(0, 1);

        customers[c.id] = c;

        if (c.id >= nextCustomerId)
            nextCustomerId = c.id + 1;
    }

    ifstream accountFile("accounts.txt");

    Account a;

    while (accountFile >> a.number >> separator
           >> a.customerId >> separator
           >> a.balance) {

        accounts[a.number] = a;

        if (a.number >= nextAccountNumber)
            nextAccountNumber = a.number + 1;
    }

    ifstream transactionFile("transactions.txt");

    Transaction t;

    while (transactionFile >> t.accountNumber >> separator &&
           getline(transactionFile, t.type, '|') &&
           transactionFile >> t.amount >> separator &&
           getline(transactionFile, t.date)) {

        transactions.push_back(t);
    }
}

void addTransaction(int accountNumber,
                    string type, double amount) {

    Transaction t;

    t.accountNumber = accountNumber;
    t.type = type;
    t.amount = amount;
    t.date = currentDate();

    transactions.push_back(t);
}

void createCustomer() {

    Customer c;

    c.id = nextCustomerId++;

    cout << "Enter customer name: ";
    cin >> ws;
    getline(cin, c.name);

    cout << "Enter phone number: ";
    getline(cin, c.phone);

    customers[c.id] = c;

    saveData();

    cout << "Customer created successfully!\n";
    cout << "Customer ID: " << c.id << endl;
}

void openAccount() {

    int customerId;
    double deposit;

    cout << "Enter customer ID: ";
    cin >> customerId;

    if (customers.find(customerId) == customers.end()) {
        cout << "Customer not found.\n";
        return;
    }

    cout << "Enter initial deposit: ";
    cin >> deposit;

    if (deposit < 0) {
        cout << "Invalid deposit.\n";
        return;
    }

    Account a;

    a.number = nextAccountNumber++;
    a.customerId = customerId;
    a.balance = deposit;

    accounts[a.number] = a;

    addTransaction(a.number, "ACCOUNT OPENED", deposit);

    saveData();

    cout << "Account created successfully!\n";
    cout << "Account Number: " << a.number << endl;
}

void depositMoney() {

    int number;
    double amount;

    cout << "Enter account number: ";
    cin >> number;

    if (accounts.find(number) == accounts.end()) {
        cout << "Account not found.\n";
        return;
    }

    cout << "Enter deposit amount: ";
    cin >> amount;

    if (amount <= 0) {
        cout << "Invalid amount.\n";
        return;
    }

    accounts[number].balance += amount;

    addTransaction(number, "DEPOSIT", amount);

    saveData();

    cout << "Deposit successful!\n";
    cout << "New Balance: "
         << accounts[number].balance << endl;
}

void withdrawMoney() {

    int number;
    double amount;

    cout << "Enter account number: ";
    cin >> number;

    if (accounts.find(number) == accounts.end()) {
        cout << "Account not found.\n";
        return;
    }

    cout << "Enter withdrawal amount: ";
    cin >> amount;

    if (amount <= 0 ||
        amount > accounts[number].balance) {

        cout << "Invalid amount or insufficient balance.\n";
        return;
    }

    accounts[number].balance -= amount;

    addTransaction(number, "WITHDRAWAL", amount);

    saveData();

    cout << "Withdrawal successful!\n";
    cout << "Remaining Balance: "
         << accounts[number].balance << endl;
}

void transferMoney() {

    int from, to;
    double amount;

    cout << "From account number: ";
    cin >> from;

    cout << "To account number: ";
    cin >> to;

    if (accounts.find(from) == accounts.end() ||
        accounts.find(to) == accounts.end() ||
        from == to) {

        cout << "Invalid account numbers.\n";
        return;
    }

    cout << "Enter transfer amount: ";
    cin >> amount;

    if (amount <= 0 ||
        amount > accounts[from].balance) {

        cout << "Invalid amount or insufficient balance.\n";
        return;
    }

    accounts[from].balance -= amount;
    accounts[to].balance += amount;

    addTransaction(from, "TRANSFER OUT", amount);
    addTransaction(to, "TRANSFER IN", amount);

    saveData();

    cout << "Transfer successful!\n";
}

void viewAccount() {

    int number;

    cout << "Enter account number: ";
    cin >> number;

    if (accounts.find(number) == accounts.end()) {
        cout << "Account not found.\n";
        return;
    }

    Account a = accounts[number];
    Customer c = customers[a.customerId];

    cout << "\n===== ACCOUNT DETAILS =====\n";

    cout << "Account Number: " << a.number << endl;
    cout << "Customer Name: " << c.name << endl;
    cout << "Phone: " << c.phone << endl;

    cout << fixed << setprecision(2);

    cout << "Balance: " << a.balance << endl;

    cout << "\nRecent Transactions:\n";

    int count = 0;

    for (int i = (int)transactions.size() - 1;
         i >= 0 && count < 5; i--) {

        if (transactions[i].accountNumber == number) {

            cout << transactions[i].date << " | "
                 << transactions[i].type << " | "
                 << transactions[i].amount << endl;

            count++;
        }
    }
}

int main() {

    loadData();

    int choice;

    do {

        cout << "\n===== BANKING SYSTEM =====\n";

        cout << "1. Create Customer\n";
        cout << "2. Open Account\n";
        cout << "3. Deposit Money\n";
        cout << "4. Withdraw Money\n";
        cout << "5. Transfer Funds\n";
        cout << "6. View Account\n";
        cout << "0. Exit\n";

        cout << "Choose option: ";
        cin >> choice;

        switch (choice) {

            case 1:
                createCustomer();
                break;

            case 2:
                openAccount();
                break;

            case 3:
                depositMoney();
                break;

            case 4:
                withdrawMoney();
                break;

            case 5:
                transferMoney();
                break;

            case 6:
                viewAccount();
                break;

            case 0:
                saveData();
                cout << "Data saved. Goodbye!\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 0);

    return 0;
}