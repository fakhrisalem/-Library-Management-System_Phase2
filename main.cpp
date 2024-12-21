#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

struct User {
    string username;
    string password;
    string role;
    int age;
    string gender;
    bool banned = false;
};

struct Book {
    string name;
    string genre;
    int copies;
};

void signUp();
bool login(User &loggedInUser);
void editProfile(User &user);
void manageAccounts();
void addBook();
void removeBook();
void searchBook();
vector<User> loadUsers();
vector<Book> loadBooks();
void saveUsers(const vector<User> &users);
void saveBooks(const vector<Book> &books);

int main() {
    User loggedInUser;
    bool running = true;

    while (running) {
        cout << "\nLibrary Management System\n";
        cout << "1. Sign Up\n";
        cout << "2. Log In\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                signUp();
                break;
            case 2:
                if (login(loggedInUser)) {
                    cout << "\nWelcome, " << loggedInUser.username << "!\n";
                    if (loggedInUser.role == "admin") {
                        int adminChoice;
                        do {
                            cout << "\nAdmin Menu:\n";
                            cout << "1. Manage User Accounts\n";
                            cout << "2. Add Book\n";
                            cout << "3. Remove Book\n";
                            cout << "4. Log Out\n";
                            cout << "Enter your choice: ";
                            cin >> adminChoice;
                            cin.ignore();

                            switch (adminChoice) {
                                case 1:
                                    manageAccounts();
                                    break;
                                case 2:
                                    addBook();
                                    break;
                                case 3:
                                    removeBook();
                                    break;
                                case 4:
                                    cout << "Logging out...\n";
                                    break;
                                default:
                                    cout << "Invalid choice. Try again.\n";
                            }
                        } while (adminChoice != 4);
                    } else {
                        int userChoice;
                        do {
                            cout << "\nUser Menu:\n";
                            cout << "1. Edit Profile\n";
                            cout << "2. Search Book\n";
                            cout << "3. Log Out\n";
                            cout << "Enter your choice: ";
                            cin >> userChoice;
                            cin.ignore();

                            switch (userChoice) {
                                case 1:
                                    editProfile(loggedInUser);
                                    break;
                                case 2:
                                    searchBook();
                                    break;
                                case 3:
                                    cout << "Logging out...\n";
                                    break;
                                default:
                                    cout << "Invalid choice. Try again.\n";
                            }
                        } while (userChoice != 3);
                    }
                }
                break;
            case 3:
                running = false;
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }

    cout << "Goodbye!\n";
    return 0;
}

void signUp() {
    User newUser;
    cout << "\nSign Up:\n";
    cout << "Enter username: ";
    cin >> newUser.username;
    cout << "Enter password: ";
    cin >> newUser.password;
    cout << "Enter age: ";
    cin >> newUser.age;
    cout << "Enter gender: ";
    cin >> newUser.gender;

    do {
        cout << "Select role (admin/student/librarian): ";
        cin >> newUser.role;
    } while (newUser.role != "admin" && newUser.role != "student" && newUser.role != "librarian");

    vector<User> users = loadUsers();
    users.push_back(newUser);
    saveUsers(users);
    cout << "Sign up successful!\n";
}

bool login(User &loggedInUser) {
    string username, password;
    cout << "\nLog In:\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    vector<User> users = loadUsers();
    for (User &user : users) {
        if (user.username == username && user.password == password) {
            if (user.banned) {
                cout << "Account is banned. Contact admin.\n";
                return false;
            }
            loggedInUser = user;
            return true;
        }
    }

    cout << "Invalid username or password.\n";
    return false;
}

void editProfile(User &user) {
    cout << "\nEdit Profile:\n";
    cout << "Enter new username: ";
    cin >> user.username;
    cout << "Enter new password: ";
    cin >> user.password;
    cout << "Enter new age: ";
    cin >> user.age;
    cout << "Enter new gender: ";
    cin >> user.gender;

    vector<User> users = loadUsers();
    for (User &u : users) {
        if (u.username == user.username) {
            u = user;
            break;
        }
    }

    saveUsers(users);
    cout << "Profile updated successfully!\n";
}

void manageAccounts() {
    vector<User> users = loadUsers();
    cout << "\nManage Accounts:\n";
    for (size_t i = 0; i < users.size(); ++i) {
        cout << i + 1 << ". Username: " << users[i].username << ", Role: " << users[i].role << ", Banned: " << (users[i].banned ? "Yes" : "No") << "\n";
    }

    cout << "Enter the number of the user to manage: ";
    int choice;
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > users.size()) {
        cout << "Invalid choice.\n";
        return;
    }

    User &selectedUser = users[choice - 1];

    cout << "\n1. Delete Account\n";
    cout << "2. Ban/Unban Account\n";
    cout << "3. Change Password\n";
    cout << "Enter your choice: ";
    int action;
    cin >> action;
    cin.ignore();

    switch (action) {
        case 1:
            users.erase(users.begin() + (choice - 1));
            cout << "Account deleted.\n";
            break;
        case 2:
            selectedUser.banned = !selectedUser.banned;
            cout << (selectedUser.banned ? "Account banned.\n" : "Account unbanned.\n");
            break;
        case 3:
            cout << "Enter new password: ";
            cin >> selectedUser.password;
            cout << "Password changed.\n";
            break;
        default:
            cout << "Invalid action.\n";
    }

    saveUsers(users);
}

void addBook() {
    Book newBook;
    cout << "\nAdd Book:\n";
    cout << "Enter book name: ";
    getline(cin, newBook.name);
    cout << "Enter book genre: ";
    getline(cin, newBook.genre);
    cout << "Enter number of copies: ";
    cin >> newBook.copies;
    cin.ignore();

    vector<Book> books = loadBooks();
    books.push_back(newBook);
    saveBooks(books);
    cout << "Book added successfully!\n";
}

void removeBook() {
    vector<Book> books = loadBooks();
    cout << "\nRemove Book:\n";
    for (size_t i = 0; i < books.size(); ++i) {
        cout << i + 1 << ". Name: " << books[i].name << ", Genre: " << books[i].genre << ", Copies: " << books[i].copies << "\n";
    }

    cout << "Enter the number of the book to remove: ";
    int choice;
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > books.size()) {
        cout << "Invalid choice.\n";
        return;
    }

    books.erase(books.begin() + (choice - 1));
    saveBooks(books);
    cout << "Book removed successfully!\n";
}

void searchBook() {
    vector<Book> books = loadBooks();
    cout << "\nSearch Book:\n";
    cout << "Enter book name or genre: ";
    string query;
    getline(cin, query);

    transform(query.begin(), query.end(), query.begin(), ::tolower);
    for (const Book &book : books) {
        string name = book.name;
        string genre = book.genre;
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        transform(genre.begin(), genre.end(), genre.begin(), ::tolower);
        if (name.find(query) != string::npos || genre.find(query) != string::npos) {
            cout << "Name: " << book.name << ", Genre: " << book.genre << ", Copies: " << book.copies << "\n";
        }
    }
}

vector<User> loadUsers() {
    ifstream file("users.txt");
    vector<User> users;
    User user;
    while (file >> user.username >> user.password >> user.role >> user.age >> user.gender >> user.banned) {
        users.push_back(user);
    }
    return users;
}

vector<Book> loadBooks() {
    ifstream file("books.txt");
    vector<Book> books;
    Book book;
    while (file >> ws, getline(file, book.name)) {
        getline(file, book.genre);
        file >> book.copies;
        file.ignore();
        books.push_back(book);
    }
    return books;
}

void saveUsers(const vector<User> &users) {
    ofstream file("users.txt");
    for (const User &user : users) {
        file << user.username << ' ' << user.password << ' ' << user.role << ' ' << user.age << ' ' << user.gender << ' ' << user.banned << '\n';
    }
}

void saveBooks(const vector<Book> &books) {
    ofstream file("books.txt");
    for (const Book &book : books) {
        file << book.name << '\n' << book.genre << '\n' << book.copies << '\n';
    }
}
