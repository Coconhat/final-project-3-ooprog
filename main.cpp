#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

class User
{
public:
    virtual void userMenu()
    {
        cout << "\nWelcome to Group 3 Cinema: Your Ultimate Movie Experience! \n"
             << "1 - View Seats\n"
             << "2 - Choice 2\n"
             << "3 - Choice 3\n"
             << "4 - Choice 4\n"
             << "5 - Choice 5\n"
             << "6 - Choice 6\n"
             << "7 - Choice 7\n"
             << "8 - Choice 8\n"
             << "9 - Choice 9\n"
             << "0 - Exit to Login\n";
    }
    // more virtual void for user centered
};

class Admin : public User
{
public:
    virtual void adminMenu()
    {
        cout << "\nAdmin Dashboard: Manage Showtimes, Tickets, and More \n"
             << "1 - Choice 1\n"
             << "2 - Choice 2\n"
             << "3 - Choice 3\n"
             << "4 - Choice 4\n"
             << "5 - Choice 5\n"
             << "6 - Choice 6\n"
             << "7 - Choice 7\n"
             << "8 - Choice 8\n"
             << "9 - Choice 9\n"
             << "0 - Exit to Login\n";
    }
    // more virtual void for admin centered
};

class Display : public Admin
{
public:
    void loginMenu()
    {
        cout << "\nWelcome to Group 3 Cinema: \n"
             << "1 - User\n"
             << "2 - Admin\n"
             << "3 - Exit\n";
    }

    void handleLoginInput()
    {
        string choice;

        while (choice != "3")
        {
            loginMenu();
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == "1")
            {
                userMenu();
                handleUserMenu();
            }
            else if (choice == "2")
            {
                adminMenu();
                handleAdminMenu();
            }
            else if (choice == "3")
            {
                cout << "Exiting... Goodbye!\n";
                break;
            }
            else
            {
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    void handleUserMenu()
    {
        string choice;
        while (choice != "0")
        {
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == "1")
            {
                viewSeats();
                cout << "User Choice 1\n";
            }
            else if (choice == "2")
            {
                cout << "User Choice 2\n";
            }
            else if (choice == "3")
            {
                cout << "User Choice 3\n";
            }
            else if (choice == "4")
            {
                cout << "User Choice 4\n";
            }
            else if (choice == "5")
            {
                cout << "User Choice 5\n";
            }
            else if (choice == "6")
            {
                cout << "User Choice 6\n";
            }
            else if (choice == "7")
            {
                cout << "User Choice 7\n";
            }
            else if (choice == "8")
            {
                cout << "User Choice 8\n";
            }
            else if (choice == "9")
            {
                cout << "User Choice 9\n";
            }
            else if (choice == "0")
            {
                cout << "Exiting User Menu...\n";
                break;
            }
            else
            {
                cout << "Invalid choice. Please try again.\n";
            }
            userMenu();
        }
    }

    void handleAdminMenu()
    {
        string choice;
        while (choice != "0")
        {
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == "1")
            {
                cout << "Admin Choice 1\n";
            }
            else if (choice == "2")
            {
                cout << "Admin Choice 2\n";
            }
            else if (choice == "3")
            {
                cout << "Admin Choice 3\n";
            }
            else if (choice == "4")
            {
                cout << "Admin Choice 4\n";
            }
            else if (choice == "5")
            {
                cout << "Admin Choice 5\n";
            }
            else if (choice == "6")
            {
                cout << "Admin Choice 6\n";
            }
            else if (choice == "7")
            {
                cout << "Admin Choice 7\n";
            }
            else if (choice == "8")
            {
                cout << "Admin Choice 8\n";
            }
            else if (choice == "9")
            {
                cout << "Admin Choice 9\n";
            }
            else if (choice == "0")
            {
                cout << "Exiting Admin Menu...\n";
                break;
            }
            else
            {
                cout << "Invalid choice. Please try again.\n";
            }
            adminMenu();
        }
    }

    void viewSeats()
    {
        vector<vector<char>> seats(10, vector<char>(10, 'A'));

        for (int i = 0; i < 10; ++i)
        {
            for (int j = 0; j < 10; ++j)
            {
                cout << "[" << static_cast<char>('A' + i) << j + 1 << "]  ";
            }
            cout << endl;
        }
        cout.flush();
    }
};

int main()
{
    Display display;
    display.handleLoginInput();
}