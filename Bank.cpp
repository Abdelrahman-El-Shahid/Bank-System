#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Account {
    int accountNumber;
    string accountHolder;
    double balance;
};

struct TreeNode {
    Account data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const Account& acc) : data(acc), left(nullptr), right(nullptr) {}
};

class BinaryTree {
private:
    TreeNode* root;

public:
    BinaryTree() : root(nullptr) {}

    ~BinaryTree() {
        clear(root);
    }

    TreeNode* getRoot() const {
        return root;
    }

    void insert(const Account& account) {
        TreeNode* newNode = new TreeNode(account);
        if (root == nullptr) {
            root = newNode;
        }
        else {
            TreeNode* curr = root;
            while (true) {
                if (account.accountNumber < curr->data.accountNumber) {
                    if (curr->left == nullptr) {
                        curr->left = newNode;
                        break;
                    }
                    curr = curr->left;
                }
                else {
                    if (curr->right == nullptr) {
                        curr->right = newNode;
                        break;
                    }
                    curr = curr->right;
                }
            }
        }
    }

    void deleteAccount(int accountNumber) {
        root = removeAccount(root, accountNumber);
    }

    void clear(TreeNode* root) {
        if (root != nullptr) {
            clear(root->left);
            clear(root->right);
            delete root;
        }
    }

    void display() const {
        displayInOrder(root);
    }

private:
    TreeNode* removeAccount(TreeNode* root, int accountNumber) {
        if (root == nullptr) {
            return root;
        }

        if (accountNumber < root->data.accountNumber) {
            root->left = removeAccount(root->left, accountNumber);
        }
        else if (accountNumber > root->data.accountNumber) {
            root->right = removeAccount(root->right, accountNumber);
        }
        else {
            if (root->left == nullptr) {
                TreeNode* temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr) {
                TreeNode* temp = root->left;
                delete root;
                return temp;
            }

            TreeNode* successor = findMinNode(root->right);
            root->data = successor->data;
            root->right = removeAccount(root->right, successor->data.accountNumber);
        }
        return root;
    }

    TreeNode* findMinNode(TreeNode* node) {
        TreeNode* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    void displayInOrder(TreeNode* root) const {
        if (root != nullptr) {
            displayInOrder(root->left);
            cout << "\n\nAccount Number: " << root->data.accountNumber << endl;
            cout << "\n\nAccount Holder: " << root->data.accountHolder << endl;
            cout << "\n\nBalance: $" << root->data.balance << endl;
            cout << endl;
            displayInOrder(root->right);
        }
    }
};

struct Node {
    Account data;
    Node* next;

    Node(const Account& acc) : data(acc), next(nullptr) {}
};

class Stack {
public:
    Node* top;

    Stack() : top(nullptr) {}

    ~Stack() {
        clear();
    }

    void push(const Account& account) {
        Node* newNode = new Node(account);
        newNode->next = top;
        top = newNode;
    }

    void pop() {
        if (isEmpty()) {
            cout << "Stack is empty." << endl;
        }
        else {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
    }


    bool isEmpty() const {
        return top == nullptr;
    }

    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }
};

void saveToFile(Stack& stack) {
    ofstream file("accounts.dat");  // Open the file for writing
    if (file.is_open()) {
        Node* current = stack.top;
        while (current != nullptr) {
            file << current->data.accountNumber << endl;
            file << current->data.accountHolder << endl;
            file << current->data.balance << endl;
            current = current->next;
        }
        file.close();
        cout << " \n\nData saved to file." << endl;
    }
    else {
        cout << "Unable to open the file." << endl;
    }
}

void readFromFile(BinaryTree& tree) {
    ifstream file("accounts.dat");
    if (file.is_open()) {
        Account account;
        while (file >> account.accountNumber) {
            file.ignore();
            getline(file, account.accountHolder);
            file >> account.balance;
            file.ignore();
            tree.insert(account);
        }
        file.close();

    }


}


void deposit(Stack& stack, TreeNode* accountNode) {
    double amount;
    cout << "\n\nEnter the amount to deposit: $";
    cin >> amount;
    cin.ignore();

    accountNode->data.balance += amount;
    stack.push(accountNode->data);
    cout << "\n\nDeposit successful." << endl;
}

void withdraw(Stack& stack, TreeNode* accountNode) {
    double amount;
    cout << "\n\nEnter the amount to withdraw: $";
    cin >> amount;
    cin.ignore();

    if (amount > accountNode->data.balance) {
        cout << "\n\nInsufficient funds." << endl;
    }
    else {
        accountNode->data.balance -= amount;
        stack.push(accountNode->data);
        cout << "\n\nWithdrawal successful." << endl;
    }
}

void viewAccountInfo(TreeNode* accountNode) {
    cout << "Account Number: " << accountNode->data.accountNumber << endl;
    cout << "Account Holder: " << accountNode->data.accountHolder << endl;
    cout << "\n\nBalance: $" << accountNode->data.balance << endl;
    cout << endl;
}

int main() {
    BinaryTree tree;
    Stack stack;
    cout << "\n";
    cout << "----------------------------------------------- Welcome With our service -----------------------------------------------  " << endl;

    readFromFile(tree);

    int choice;
    while (true) {
        cout << "\n\nAre You have an account?\n\n";
        cout << "\n\nIf you dont have  press one  to add account\n\n";
        cout << "<------------------------------------------Here you can find our services  ------------------------------------------> \n\n";

        cout << "<--------------    press #1 to  Add account      -------------->\n\n";
        cout << "<--------------    press #2. Deposit funds       --------------> \n\n";
        cout << "<--------------    press #3. Withdraw funds       --------------> \n\n";
        cout << "<--------------    press #4. Delete account      -------------->\n\n";
        cout << "<--------------    press #5. Display accounts    -------------->\n\n";
        cout << "<--------------    press #6. View account info   -------------->\n\n";
        cout << "<--------------    press #7. Exit                -------------->" << endl << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: {
            Account account;
            cout << "\n\nEnter account number: ";
            cin >> account.accountNumber;
            cin.ignore();

            cout << "\n\nEnter account holder name: ";
            getline(cin, account.accountHolder);

            cout << "\n\nEnter initial balance: $";
            cin >> account.balance;
            cin.ignore();

            tree.insert(account);
            stack.push(account);
            saveToFile(stack);  // Save new account information to file
            break;
        }
        case 2: {
            int accountNumber;
            cout << "\n\nEnter the account number: ";
            cin >> accountNumber;
            cin.ignore();

            TreeNode* accountNode = tree.getRoot();
            while (accountNode != nullptr) {
                if (accountNumber == accountNode->data.accountNumber) {
                    deposit(stack, accountNode);
                    break;
                }
                else if (accountNumber < accountNode->data.accountNumber) {
                    accountNode = accountNode->left;
                }
                else {
                    accountNode = accountNode->right;
                }
            }
            if (accountNode == nullptr) {
                cout << "\n\nAccount not found." << endl;
            }
            saveToFile(stack);
            break;
        }
        case 3: {
            int accountNumber;
            cout << "\n\nEnter the account number: ";
            cin >> accountNumber;
            cin.ignore();

            TreeNode* accountNode = tree.getRoot();
            while (accountNode != nullptr) {
                if (accountNumber == accountNode->data.accountNumber) {
                    withdraw(stack, accountNode);
                    break;
                }
                else if (accountNumber < accountNode->data.accountNumber) {
                    accountNode = accountNode->left;
                }
                else {
                    accountNode = accountNode->right;
                }
            }
            if (accountNode == nullptr) {
                cout << "\n\nAccount not found." << endl;
            }
            saveToFile(stack);
            break;
        }
        case 4: {
            int accountNumber;
            cout << "\n\nEnter the account number: ";
            cin >> accountNumber;
            cin.ignore();

            tree.deleteAccount(accountNumber);
            saveToFile(stack);  // Save updated account information to file
            cout << "\n\nAccount deleted." << endl << endl;
            break;
        }
        case 5: {
            tree.display();
            break;
        }
        case 6: {
            int accountNumber;
            cout << "\n\nEnter the account number: ";
            cin >> accountNumber;
            cin.ignore();

            TreeNode* accountNode = tree.getRoot();
            while (accountNode != nullptr) {
                if (accountNumber == accountNode->data.accountNumber) {
                    viewAccountInfo(accountNode);
                    break;
                }
                else if (accountNumber < accountNode->data.accountNumber) {
                    accountNode = accountNode->left;
                }
                else {
                    accountNode = accountNode->right;
                }
            }
            if (accountNode == nullptr) {
                cout << "Account not found." << endl;
            }
            break;
        }
        case 7: {
            saveToFile(stack);  // Save stack information to file before exiting
            cout << "\n\nExiting the program." << endl;
            return 0;
        }
        default:
            cout << "\n\nInvalid choice. Please try again." << endl;
        }
    }

    return 0;
}
