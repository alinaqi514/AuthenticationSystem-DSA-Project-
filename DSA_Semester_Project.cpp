#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <stack>
#include <queue>
#include <limits>  // Include for numeric_limits
using namespace std;

class User {
public:
    string username;
    string password;
    string CNIC;
    string Gmail;
    int Age;  // Change data type to int for Age
    string Gender;

    User(const string& u, const string& p, const string& c, const string& g, int a, const string& G)
        : username(u), password(p), CNIC(c), Gmail(g), Age(a), Gender(G) {}
};

class AuthenticationSystem {
private:
    list<User> users;
    stack<string> loginHistory;
    queue<string> passwordChangeRequests;

public:
    AuthenticationSystem() {
        loadUserData();
    }

    void loadUserData() {
        ifstream inFile("t.txt");
        if (inFile.is_open()) {
            string username, password, CNIC, Gmail, Gender;
            int Age;
            while (inFile >> username >> password >> CNIC >> Gmail >> Gender >> Age) {
                users.push_back(User(username, password, CNIC, Gmail, Age, Gender));
            }
            inFile.close();
        } else {
            cout << "No existing user data found.\n";
        }
    }

    void saveUserData() {
        ofstream outFile("t.txt");
        if (outFile.is_open()) {
            for (const auto& user : users) {
                outFile << user.username << "\n " << user.password << "\n" << user.CNIC << "\n " << user.Gmail << "\n " << user.Gender << " \n" << user.Age << endl;
            }
            outFile.close();
        } else {
            cout << "Error: Unable to open 'user.txt' for writing.\n";
        }
    }

    void registerUser(const string& username, const string& password, const string& CNIC, const string& Gmail, const string& Gender, int Age) {
        users.push_back(User(username, password, CNIC, Gmail, Age, Gender));
        cout << "User registered successfully.\n";
        saveUserData();
    }

    void loginUser(const string& CNIC, const string& password) {
        for (const auto& user : users) {
            cout << "Comparing: Entered CNIC: " << CNIC << ", Stored CNIC: " << user.CNIC << endl;
            cout << "Comparing: Entered Password: " << password << ", Stored Password: " << user.password << endl;

            if (user.CNIC == CNIC && user.password == password) {
                cout << "Login successful. Welcome, user with CNIC " << CNIC << "!\n";
                loginHistory.push(CNIC);
                return;
            }
        }
        cout << "Invalid CNIC or password.\n";
    }

    void LoginHistory() {
        ofstream outFile("LoginHistory.txt", ios::app);
        if (outFile.is_open()) {
            while (!loginHistory.empty()) {
                outFile << loginHistory.top() << "\n";
                loginHistory.pop();
            }
            outFile.close();
        } else {
            cout << "Error: Unable to open 'LoginHistory.txt' for writing.\n";
        }
    }

    void changePassword(const string& CNIC, const string& newPassword) {
        passwordChangeRequests.push(CNIC);
        cout << "Password change request queued for user with CNIC: " << CNIC << "\n";
    }

    void endProgram() {
        processPasswordChangeRequests();
        saveUserData();
        LoginHistory();
        cout << "Program ending. Thank you!\n";
    }

    void displayAllUsers() {
        cout << "All Users:\n";
        for (const auto& user : users) {
            cout << "Username: " << user.username << ", Password: " << user.password
                 << ", CNIC: " << user.CNIC << ", Gmail: " << user.Gmail
                 << ", Gender: " << user.Gender << ", Age: " << user.Age << "\n";
        }
    }

    void processPasswordChangeRequests() {
        while (!passwordChangeRequests.empty()) {
            string CNIC = passwordChangeRequests.front();
            passwordChangeRequests.pop();

            for (auto& user : users) {
                if (user.CNIC == CNIC) {
                    cout << "Enter new password for user with CNIC " << CNIC << ": ";
                    cin >> user.password;
                    cout << "Password changed successfully.\n";
                    saveUserData();
                    return;
                }
            }
        }
    }
};

int main() {
    AuthenticationSystem authSystem;

    while (true) {
        cout << "               <------------------------------****************************************-------------------------------->" << endl;
        cout << "               <---------------------------------WELCOME TO AUTHENTICATION SYSTEM------------------------------------>" << endl;
        cout << "                                                      1. Register" << endl;
        cout << "                                                      2. Login" << endl;
        cout << "                                                      3. Change Password" << endl;
        cout << "                                                      4. Display All Users" << endl;
        cout << "                                                      5. End Program" << endl;
        int choice;
        cin >> choice;
        switch (choice) {
            case 1: {
                string username, password, CNIC, Gmail, Gender;
                int age;

                cout << "Enter full name: ";
                cin.ignore();
                getline(cin, username);

                cout << "Enter Your CNIC: " << endl;
                cin >> CNIC;
                cout << "Enter Your Gmail: " << endl;
                cin >> Gmail;
                cout << "Enter Your Gender: " << endl;
                cin >> Gender;
                cout << "Enter Your Age (19 etc): " << endl;
                cin >> age;
                if(cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Age must be a number.\n";
                    continue;
                }
                cout << "Enter password: " << endl;
                cin >> password;
                authSystem.registerUser(username, password, CNIC, Gmail, Gender, age);
                break;
            }

            case 2: {
                string CNIC, password;
                cout << "Enter CNIC: " << endl;
                cin.ignore();
                getline(cin, CNIC);
                cout << "Enter password: ";
                cin >> password;
                authSystem.loginUser(CNIC, password);
                break;
            }

            case 3: {
                string CNIC, newPassword;
                cout << "Enter CNIC: " << endl;
                cin >> CNIC;
                cout << "Enter new password: ";
                cin >> newPassword;
                authSystem.changePassword(CNIC, newPassword);
                break;
            }
            case 4:
                authSystem.displayAllUsers();
                break;
            case 5:
                authSystem.processPasswordChangeRequests();
                authSystem.LoginHistory();
                authSystem.endProgram();
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}
