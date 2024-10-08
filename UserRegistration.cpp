#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <string>

using namespace std;

// Enum for roles
enum Role {
    Passenger,  // Passenger
    Driver      // Driver
};

// Profile class to manage roles
class Profile {
private:
    Role userRole;

public:
    Profile(Role role) : userRole(role) {}

    string getRole() const {
        return (userRole == Passenger) ? "Passenger" : "Driver";
    }

    void updateRole(Role newRole) {
        userRole = newRole;
    }

    Role getRoleEnum() const {
        return userRole;
    }
};

// User class to manage user information
class User {
private:
    string username;
    string password;
    string fullName;
    string phoneNumber;
    string email;
    string idType;
    string idNumber;
    double creditPoints;
    Profile profile;

    // Validate password format
    bool isPasswordValid(const string& pass) {
        const int minLength = 8;
        regex passwordRegex("(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[@$!%*?&])[A-Za-z\\d@$!%*?&]{8,}");
        return pass.length() >= minLength && regex_match(pass, passwordRegex);
    }

public:
    // Constructor for manual input
    User(string uName, string pWord, string fName, string pNumber, string mail, string iType, string iNumber, Role role)
        : username(uName), fullName(fName), phoneNumber(pNumber), email(mail), idType(iType), idNumber(iNumber), 
          creditPoints(10), profile(role) {

        if (!isPasswordValid(pWord)) {
            throw invalid_argument("Invalid password! Must be at least 8 characters, including uppercase, lowercase, digits, and special characters.");
        }
        password = pWord;
    }

    // Constructor for loading from CSV
    User(string uName, string pWord, string fName, string pNumber, string mail, string iType, string iNumber, double points, Role role)
        : username(uName), password(pWord), fullName(fName), phoneNumber(pNumber), email(mail), idType(iType), 
          idNumber(iNumber), creditPoints(points), profile(role) {}

    // Save user information to CSV
    void saveToCSV(const string& filename, bool append = true) const {
        ofstream file;
        file.open(filename, append ? ios::app : ios::trunc); // Open file in append or truncate mode
        file << username << "," << password << "," << fullName << "," << phoneNumber << "," << email << ","
             << idType << "," << idNumber << "," << creditPoints << "," << profile.getRole() << "\n";
        file.close();
    }

    // Static method to load users from CSV
    static vector<User> loadFromCSV(const string& filename) {
        vector<User> users;
        ifstream file(filename);
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string uName, pWord, fName, pNumber, mail, iType, iNumber, roleStr;
            double points;
            Role role;
            string pointsStr;

            getline(ss, uName, ',');
            getline(ss, pWord, ',');
            getline(ss, fName, ',');
            getline(ss, pNumber, ',');
            getline(ss, mail, ',');
            getline(ss, iType, ',');
            getline(ss, iNumber, ',');
            getline(ss, pointsStr, ',');
            getline(ss, roleStr, ',');

            points = stod(pointsStr);
            role = (roleStr == "Passenger") ? Passenger : Driver;

            users.emplace_back(uName, pWord, fName, pNumber, mail, iType, iNumber, points, role);
        }
        file.close();
        return users;
    }

    // Save all users back to CSV (for update purposes)
    static void saveAllToCSV(const string& filename, const vector<User>& users) {
        ofstream file;
        file.open(filename, ios::trunc); // Truncate to overwrite the entire file
        for (const auto& user : users) {
            file << user.username << "," << user.password << "," << user.fullName << "," << user.phoneNumber << ","
                 << user.email << "," << user.idType << "," << user.idNumber << "," << user.creditPoints << ","
                 << user.profile.getRole() << "\n";
        }
        file.close();
    }

    // Profile Management ( Update user profile )
    void updateProfile() {
        cout << "Update Profile for: " << username << endl;
        cout << "Enter new full name (leave blank to keep unchanged): ";
        string newName;
        cin.ignore();
        getline(cin, newName);
        if (!newName.empty()) fullName = newName;

        cout << "Enter new phone number (leave blank to keep unchanged): ";
        string newPhone;
        getline(cin, newPhone);
        if (!newPhone.empty()) phoneNumber = newPhone;

        cout << "Enter new email (leave blank to keep unchanged): ";
        string newEmail;
        getline(cin, newEmail);
        if (!newEmail.empty()) email = newEmail;

        cout << "Enter new ID type (leave blank to keep unchanged): ";
        string newIDType;
        getline(cin, newIDType);
        if (!newIDType.empty()) idType = newIDType;

        cout << "Enter new ID/Passport number (leave blank to keep unchanged): ";
        string newIDNumber;
        getline(cin, newIDNumber);
        if (!newIDNumber.empty()) idNumber = newIDNumber;
    }

    // Update password
    void updatePassword() {
        cout << "Enter current password: ";
        string currentPassword;
        cin >> currentPassword;

        if (currentPassword == password) {
            cout << "Enter new password (must be at least 8 characters, including uppercase, lowercase, digits, and special characters): ";
            string newPassword;
            cin >> newPassword;

            if (isPasswordValid(newPassword)) {
                password = newPassword;
                cout << "Password updated successfully.\n";
            } else {
                cout << "New password is invalid. Please try again.\n";
            }
        } else {
            cout << "Incorrect current password.\n";
        }
    }

    // Display user profile
    void viewProfile() const {
        cout << "Username: " << username << endl;
        cout << "Full Name: " << fullName << endl;
        cout << "Phone Number: " << phoneNumber << endl;
        cout << "Email: " << email << endl;
        cout << "ID Type: " << idType << endl;
        cout << "ID Number: " << idNumber << endl;
        cout << "Credit Points: " << creditPoints << endl;
        cout << "Role: " << profile.getRole() << endl;
    }

    // Get username for finding users
    string getUsername() const {
        return username;
    }
};

// Function to update a user's information
void updateUserProfile(vector<User>& users, const string& csvFile) {
    string username;
    cout << "Enter the username of the profile you want to update: ";
    cin >> username;

    bool userFound = false;
    for (auto& user : users) {
        if (user.getUsername() == username) {
            userFound = true;
            int choice;
            cout << "1. Update personal information\n2. Change password\nEnter your choice: ";
            cin >> choice;

            if (choice == 1) {
                user.updateProfile();
            } else if (choice == 2) {
                user.updatePassword();
            }

            // Save updates back to CSV
            User::saveAllToCSV(csvFile, users);
            cout << "User information updated successfully.\n";
            break;
        }
    }

    if (!userFound) {
        cout << "User not found.\n";
    }
}

// Function for User Registration
void registerUser(const string& csvFile) {
    string username, password, fullName, phoneNumber, email, idType, idNumber;
    Role role;

    cout << "Enter Username: ";
    cin >> username;

    cout << "Enter Password (must be at least 8 characters, including uppercase, lowercase, digits, and special characters): ";
    cin >> password;

    cout << "Enter Full Name: ";
    cin.ignore();  // Ignore newline from the previous input
    getline(cin, fullName);

    cout << "Enter Phone Number: ";
    cin >> phoneNumber;

    cout << "Enter Email: ";
    cin >> email;

    cout << "Enter ID Type (e.g., Citizen ID, Passport): ";
    cin.ignore();
    getline(cin, idType);

    cout << "Enter ID/Passport Number: ";
    cin >> idNumber;

    // Select role (Passenger or Driver)
    int roleChoice;
    cout << "Select role: \n1. Passenger\n2. Driver\nEnter your choice: ";
    cin >> roleChoice;
    role = (roleChoice == 1) ? Passenger : Driver;

    try {
        // Create new user and save to CSV
        User newUser(username, password, fullName, phoneNumber, email, idType, idNumber, role);
        // Load existing users
        vector<User> users = User::loadFromCSV(csvFile);
        // Remove old user if exists
        users.erase(remove_if(users.begin(), users.end(), [&](const User& u) {
            return u.getUsername() == username;
        }), users.end());
        // Add new user
        users.push_back(newUser);
        // Save all users back to CSV
        User::saveAllToCSV(csvFile, users);
        cout << "User registration successful and saved to CSV.\n";
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

int main() {
    string csvFile = "users.csv";

    int action;
    cout << "1. Register new user\n2. Update existing user\nEnter your choice: ";
    cin >> action;

    if (action == 1) {
        // Register a new user
        registerUser(csvFile);
    } else if (action == 2) {
        // Load all registered users from the CSV file
        vector<User> users = User::loadFromCSV(csvFile);
        // Update existing user profile or password
        updateUserProfile(users, csvFile);
    }

    // Display all registered users
    cout << "\nUsers registered in CSV:\n";
    vector<User> users = User::loadFromCSV(csvFile);
    for (const auto& user : users) {
        user.viewProfile();
        cout << "-----------------\n";
    }

    return 0;
}
