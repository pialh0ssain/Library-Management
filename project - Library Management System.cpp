#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


using namespace std;

struct Book {
    string id;
    string title;
    string author;
};

void signUp(string role);
bool login(string role);
void adminPanel();
void userPanel();
void addBook();
void removeBook();
void viewAllBooks();
void updateBook();
void approveUsers();
void viewApprovedUsers();



int main() {
    int userType, choice;
    string role;

    while (true) {
        cout << "\n===== Library Management System =====\n";
        cout << "1. Admin Access\n";
        cout << "2. User Access\n";
        cout << "3. Exit\n";
        cout << "Please choose an option: ";
        cin >> userType;

        if (userType == 1) {
            role = "admin";
        } else if (userType == 2) {
            role = "user";
        } else if (userType == 3) {
            cout << "\nExiting the system...\n";
            break;
        } else {
            cout << "\nInvalid choice! Please try again.\n";
            continue;
        }

        while (true) {
            cout << "\n===== " << (role == "admin" ? "Admin" : "User") << " Panel =====\n";
            cout << "1. Create New Account\n";
            cout << "2. Login to Account\n";
            cout << "3. Return to Main Menu\n";
            cout << "Please select an option: ";
            cin >> choice;

            if (choice == 1) {
                signUp(role);
            } else if (choice == 2) {
                if (login(role)) {
                    if (role == "admin") {
                        adminPanel();
                    } else {
                        userPanel();
                    }
                }
            } else if (choice == 3) {
                break;
            } else {
                cout << "\nInvalid input! Please try again.\n";
            }
        }
    }
    return 0;
}

bool isValidEmail(string email) {
    int atCount = 0;
    int atPos = -1;
    int len = email.length();

    for (int i = 0; i < len; i++) {
        char c = email[i];
        if (!(islower(c) || isdigit(c) || c == '@' || c == '.' || c == '-' || c == '_')) {
            return false;
        }
        if (c == '@') {
            atCount++;
            atPos = i;
        }
    }

    if (atCount != 1 || atPos == 0 || atPos == len - 1) return false;

    int dotPos = email.find('.', atPos);
    if (dotPos == string::npos || dotPos == atPos + 1 || dotPos == len - 1) return false;

    if (email.find("..") != string::npos) return false;

    string domain = email.substr(email.rfind('.') + 1);
    if (domain != "com" && domain != "net" && domain != "org") return false;

    return true;
}

bool isExistingAccount(string gmail) {
    ifstream file("users.txt");
    string fileGmail, filePassword, fileRole;
    while (file >> fileGmail >> filePassword >> fileRole) {
        if (fileGmail == gmail) {
            return true;
        }
    }
    return false;
}

void signUp(string role) {
    string gmail, password;

    cout << "\nEnter Your Email: ";
    cin >> gmail;

    if (!isValidEmail(gmail)) {
        cout << "Invalid email format! Try again.\n";
        return;
    }

    if (isExistingAccount(gmail)) {
        cout << "Account already exists. Please log in.\n";
        return;
    }

    if (role == "admin") {
        string secret;
        cout << "Enter Admin Secret Code: ";
        cin >> secret;
        if (secret != "nub@123456") {
            cout << "Invalid admin secret code! Signup failed.\n";
            return;
        }
    }

    cout << "Set a Password: ";
    cin >> password;

    ofstream file("users.txt", ios::app);
    if (role == "user") {
        file << gmail << " " << password << " " << role << " pending" << endl;
    } else {
        file << gmail << " " << password << " " << role << " approved" << endl;
    }
    file.close();

    cout << "\n" << (role == "admin" ? "Admin" : "User") << " account created successfully!\n";
}

bool login(string role) {
    string gmail, password, fileGmail, filePassword, fileRole, status;

    cout << "\nEnter Your Email: ";
    cin >> gmail;

    cout << "Enter Your Password: ";
    cin >> password;

    ifstream file("users.txt");
    bool found = false;

    while (file >> fileGmail >> filePassword >> fileRole >> status) {
        if (fileGmail == gmail && filePassword == password && fileRole == role) {
            if (role == "user" && status != "approved") {
                cout << "\nYour account is pending approval by admin.\n";
                return false;
            }
            found = true;
            break;
        }
    }

    file.close();

    if (found) {
        cout << "\nLogin Successful! Welcome, " << gmail << " (" << role << ")\n";
        return true;
    } else {
        cout << "\nInvalid Credentials! Please try again.\n";
        return false;
    }
}

void adminPanel() {
    int choice;
    while (true) {
        cout << "\n===== Admin Panel =====\n";
        cout << "1. Add Book\n";
        cout << "2. Remove Book\n";
        cout << "3. View All Books\n";
        cout << "4. Update Book\n";
        cout << "5. Logout & Return\n";
        cout << "6. Approve Users\n";
        cout << "7. View Approved Users\n";
        cout << "Please select an option: ";
        cin >> choice;

        if (choice == 1) {
            addBook();
        } else if (choice == 2) {
            removeBook();
        } else if (choice == 3) {
            viewAllBooks();
        } else if (choice == 4) {
            updateBook();
        } else if (choice == 5) {
            cout << "\nLogging out... Returning to Main Menu.\n";
            break;
        } else if (choice == 6) {
            approveUsers();
        } else if (choice == 7) {
            viewApprovedUsers();
        } else {
            cout << "\nInvalid choice! Please try again.\n";
        }
    }
}


void addBook() {
    string id, title, author;
    cout << "\nEnter Book ID: ";
    cin >> id;
    cin.ignore();
    cout << "Enter Book Title: ";
    getline(cin, title);
    cout << "Enter Author Name: ";
    getline(cin, author);

    ofstream file("books.txt", ios::app);
    file << id << "|" << title << "|" << author << endl;
    file.close();

    cout << "\nBook added successfully.\n";
}

void removeBook() {
    string id;
    cout << "\nEnter Book ID to remove: ";
    cin >> id;

    ifstream inFile("books.txt");
    ofstream tempFile("temp.txt");

    bool found = false;
    string line;

    while (getline(inFile, line)) {
        if (line.substr(0, line.find('|')) != id) {
            tempFile << line << endl;
        } else {
            found = true;
        }
    }

    inFile.close();
    tempFile.close();

    remove("books.txt");
    rename("temp.txt", "books.txt");

    if (found) {
        cout << "\nBook removed successfully.\n";
    } else {
        cout << "\nBook not found.\n";
    }
}

void viewAllBooks() {
    ifstream file("books.txt");
    string line;

    cout << "\n===== Book List =====\n";

    while (getline(file, line)) {
        size_t pos1 = line.find('|');
        size_t pos2 = line.rfind('|');

        string id = line.substr(0, pos1);
        string title = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string author = line.substr(pos2 + 1);

        cout << "ID: " << id << "\nTitle: " << title << "\nAuthor: " << author << "\n---\n";
    }

    file.close();
}

void updateBook() {
    string id;
    cout << "\nEnter Book ID to update: ";
    cin >> id;

    ifstream inFile("books.txt");
    ofstream tempFile("temp.txt");

    bool found = false;
    string line;

    while (getline(inFile, line)) {
        size_t pos1 = line.find('|');
        string currentId = line.substr(0, pos1);

        if (currentId == id) {
            found = true;
            string newTitle, newAuthor;
            cin.ignore();
            cout << "Enter new title: ";
            getline(cin, newTitle);
            cout << "Enter new author: ";
            getline(cin, newAuthor);

            tempFile << id << "|" << newTitle << "|" << newAuthor << endl;
        } else {
            tempFile << line << endl;
        }
    }

    inFile.close();
    tempFile.close();

    remove("books.txt");
    rename("temp.txt", "books.txt");

    if (found) {
        cout << "\nBook updated successfully.\n";
    } else {
        cout << "\nBook not found.\n";
    }
}

void approveUsers() {
    vector<string> updatedLines;
    string line;

    ifstream inFile("users.txt");
    while (getline(inFile, line)) {
        istringstream iss(line);
        string gmail, password, role, status;
        iss >> gmail >> password >> role >> status;

        if (role == "user" && status == "pending") {
            cout << "Approve user " << gmail << "? (y/n): ";
            char ch;
            cin >> ch;
            if (ch == 'y' || ch == 'Y') {
                line = gmail + " " + password + " " + role + " approved";
            }
        }
        updatedLines.push_back(line);
    }
    inFile.close();

    ofstream outFile("users.txt");
    for (string& l : updatedLines) {
        outFile << l << endl;
    }
    outFile.close();

    cout << "\nUser approval process completed.\n";
}

void viewApprovedUsers() {
    ifstream inFile("users.txt");
    string gmail, password, role, status;
    bool found = false;

    cout << "\nApproved Users:\n";
    cout << "-----------------------------\n";

    while (inFile >> gmail >> password >> role >> status) {
        if (role == "user" && status == "approved") {
            cout << "Email: " << gmail << endl;
            found = true;
        }
    }

    inFile.close();

    if (!found) {
        cout << "No approved users found.\n";
    }

    cout << "-----------------------------\n";
}


void userPanel() {
    int choice;
    while (true) {
        cout << "\n===== User Panel =====\n";
        cout << "1. Search for Books (Coming Soon)\n";
        cout << "2. Borrow Books (Coming Soon)\n";
        cout << "3. Logout & Return\n";
        cout << "Please select an option: ";
        cin >> choice;

        if (choice == 3) {
            cout << "\nLogging out... Returning to Main Menu.\n";
            break;
        } else {
            cout << "\nFeature under development. Please check back later.\n";
        }
    }
}
