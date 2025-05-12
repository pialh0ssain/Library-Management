#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <numeric>
#include <iomanip>
#include <ctime>

using namespace std;

struct Book {
    string id;
    string title;
    string author;
    vector<int> ratings;
    bool isBorrowed;

    Book(string i, string t, string a, bool b = false)
        : id(i), title(t), author(a), isBorrowed(b) {}
};

// === Function Declarations ===
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
void userPanel(vector<Book>& books);
void loadBooks(vector<Book>& books);
void saveBooks(const vector<Book>& books);
void searchBooks(const vector<Book>& books);
void borrowBook(vector<Book>& books);
void returnBook(vector<Book>& books);
void viewBorrowedBooks(const vector<Book>& books);
void calculateFineSummary(const vector<Book>& books);
void rateBook(const vector<Book>& books);
void viewBookRatings(const vector<Book>& books);
bool isValidEmail(string email);
bool isExistingAccount(string gmail);

// === Main Function ===
int main() {
    vector<Book> books;
    loadBooks(books);

    int userType, choice;
    string role;

    while (true) {
        cout << "\n===== Library Management System =====\n";
        cout << "1. Admin Access\n";
        cout << "2. User Access\n";
        cout << "3. Exit\n";
        cout << "Please choose an option: ";
        cin >> userType;

        if (userType == 1) role = "admin";
        else if (userType == 2) role = "user";
        else if (userType == 3) {
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

            if (choice == 1) signUp(role);
            else if (choice == 2) {
                if (login(role)) {
                    if (role == "admin") adminPanel();
                    else userPanel(books);
                }
            }
            else if (choice == 3) break;
            else cout << "\nInvalid input! Please try again.\n";
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


void loadBooks(vector<Book>& books) {
    ifstream inFile("books.txt");
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string id, title, author, statusStr;
        getline(ss, id, '|');
        getline(ss, title, '|');
        getline(ss, author, '|');
        getline(ss, statusStr);
        bool status = (statusStr == "1");
        books.emplace_back(id, title, author, status);
    }
    inFile.close();
}

void saveBooks(const vector<Book>& books) {
    ofstream outFile("books.txt");
    for (const auto& book : books) {
        outFile << book.id << "|" << book.title << "|" << book.author << "|"
                << (book.isBorrowed ? "1" : "0") << endl;
    }
    outFile.close();
}
void searchBooks(const vector<Book>& books) {
    string query;
    cout << "Enter book ID, title, or author to search: ";
    cin.ignore();
    getline(cin, query);
    bool found = false;

    for (const auto& book : books) {
        // ID, title, or author match
        if (book.id.find(query) != string::npos ||
            book.title.find(query) != string::npos ||
            book.author.find(query) != string::npos) {

            cout << "ID: " << book.id
                 << ", Title: " << book.title
                 << ", Author: " << book.author
                 << ", Status: " << (book.isBorrowed ? "Borrowed" : "Available") << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No matching books found.\n";
    }
}


void borrowBook(vector<Book>& books) {
    string query;
    cout << "Enter the book title or ID to borrow: ";
    cin.ignore();
    getline(cin, query);

    for (auto& book : books) {
        if (book.title == query || book.id == query) {
            if (!book.isBorrowed) {
                book.isBorrowed = true;
                saveBooks(books);
                cout << "You have borrowed \"" << book.title << "\" successfully.\n";
                return;
            } else {
                cout << "Sorry, this book is already borrowed.\n";
                return;
            }
        }
    }

    cout << "Book not found.\n";
}

void returnBook(vector<Book>& books) {
    string query;
    cout << "Enter the book title or ID to return: ";
    cin.ignore();
    getline(cin, query);

    for (auto& book : books) {
        if (book.title == query || book.id == query) {
            if (book.isBorrowed) {
                book.isBorrowed = false;
                saveBooks(books);
                cout << "You have returned \"" << book.title << "\" successfully.\n";
                return;
            } else {
                cout << "This book wasn't borrowed.\n";
                return;
            }
        }
    }

    cout << "Book not found.\n";
}

void viewBorrowedBooks(const vector<Book>& books) {
    cout << "Borrowed Books:\n";
    bool found = false;
    for (const auto& book : books) {
        if (book.isBorrowed) {
            cout << "Title: " << book.title << ", Author: " << book.author << endl;
            found = true;
        }
    }
    if (!found) cout << "No books currently borrowed.\n";
}

void calculateFineSummary(const vector<Book>& books) {
    // Demo version — implement real fine logic if needed
    cout << "Late Fine Summary:\n";
    cout << "You have 0 books with late return. Total fine: $0.00\n";
}

void rateBook(const vector<Book>& books) {
    string query;
    int rating;

    cout << "Enter title or ID of the book to rate: ";
    cin.ignore();
    getline(cin, query);

    auto it = find_if(books.begin(), books.end(), [&query](const Book& b) {
        return b.title == query || b.id == query;
    });

    if (it == books.end()) {
        cout << "Book not found.\n";
        return;
    }

    cout << "Enter your rating (1 to 5): ";
    cin >> rating;

    if (rating < 1 || rating > 5) {
        cout << "Invalid rating.\n";
        return;
    }

    ofstream outFile("ratings.txt", ios::app);
    outFile << it->id << "|" << it->title << "|" << rating << "\n";
    outFile.close();

    cout << "Thanks for rating \"" << it->title << "\"!\n";
}


void viewBookRatings(const vector<Book>& books) {
    ifstream inFile("ratings.txt");
    if (!inFile.is_open()) {
        cerr << "Error: Could not open ratings.txt file.\n";
        return;
    }

    map<string, vector<int>> ratings;
    string line;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string idStr, title, rateStr;

        if (!getline(ss, idStr, '|')) continue;
        if (!getline(ss, title, '|')) continue;
        if (!getline(ss, rateStr)) continue;

        try {
            int rate = stoi(rateStr);
            ratings[title].push_back(rate);
        } catch (...) {
            cerr << "Invalid rating line: " << line << endl;
        }
    }
    inFile.close();

    for (const auto& book : books) {
        cout << "-----------------------------\n";
        cout << "Title          : " << book.title << endl;

        auto it = ratings.find(book.title);
        if (it != ratings.end()) {
            const auto& r = it->second;
            double avg = accumulate(r.begin(), r.end(), 0.0) / r.size();
            cout << "Average Rating : " << fixed << setprecision(2) << avg << endl;
            cout << "Total Ratings  : " << r.size() << endl;
            cout << "All Ratings    : ";
            for (int rate : r) {
                cout << rate << " ";
            }
            cout << endl;
        } else {
            cout << "No ratings available.\n";
        }
    }
    cout << "-----------------------------\n";
}



void view_AllBooks(const vector<Book>& books) {
    if (books.empty()) {
        cout << "No books available in the library.\n";
        return;
    }

    cout << "\n===== All Books in Library =====\n\n";
    for (const auto& book : books) {
        cout << "ID     : " << book.id << endl;
        cout << "Title  : " << book.title << endl;
        cout << "Author : " << book.author << endl;
        cout << "Status : " << (book.isBorrowed ? "Borrowed" : "Available") << endl;
        cout << "----------------------------------\n";
    }
}




// User Panel
void userPanel(vector<Book>& books) {
    int choice;
    while (true) {
        cout << "\n===== User Panel =====\n";
        cout << "1. Search for Books\n";
        cout << "2. Borrow Books\n";
        cout << "3. Return Books\n";
        cout << "4. View Borrowed Books\n";
        cout << "5. View Late Fine Summary\n";
        cout << "6. Rate a Book\n";
        cout << "7. View Book Ratings\n";
        cout << "8. Logout & Return\n";
        cout << "9. View All Books\n";

        cout << "Please select an option: ";
        cin >> choice;

        if (choice == 1) {
            searchBooks(books);
        } else if (choice == 2) {
            borrowBook(books);
        } else if (choice == 3) {
            returnBook(books);
        } else if (choice == 4) {
            viewBorrowedBooks(books);
        } else if (choice == 5) {
            calculateFineSummary(books);
        } else if (choice == 6) {
            rateBook(books);
        } else if (choice == 7) {
            viewBookRatings(books);
        } else if (choice == 8) {
            cout << "\nLogging out... Returning to Main Menu.\n";
            break;
        } else if (choice == 9) {
            view_AllBooks(books);
        } else {
            cout << "\nInvalid option. Please try again.\n";
        }
    }
}
