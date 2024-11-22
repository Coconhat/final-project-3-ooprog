#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// base class of all user related functionality
class User
{
public:
    virtual void viewMovies() = 0;
    //  virtual void book = 0;
    //  virtual void selectSeat = 0;
    //  virtual void makeChanges = 0;
    virtual ~User() {}
};

// base class of all admin related functionality
class Admin : public User
{
public:
    // add more virtual void if necessary :>
    virtual void manageMovieAndShowtime() = 0;
    virtual void manageSeatLayout() = 0;
    // virtual void saleReport = 0;
    virtual ~Admin() {}
};

// all movies related logic goes here
class Movies : public Admin
{
public:
    // for user
    void viewMovies() override
    {
        cout << "viewing movies, showtime, and ticket price...\n";
        cout << "ask user if they want to book...\n"; // make booking function
        cout << "select seats...\n";                  // make select seat function
        cout << "payment option...\n";
    }

    // for admin
    void manageMovieAndShowtime() override
    {
        cout << "Give option on what to do (edit title, edit show time, edit ticket price)\n";
    }

    // for admin
    void manageSeatLayout() override
    {
        cout << "Option to view or edit seat availability\n";
    }
};

// all display logic goes here
class Display : public Movies
{
public:
    void loginMenu()
    {
        cout << "\nWelcome to Group 3 Cinema: \n"
             << "1 - User\n"
             << "2 - Admin\n"
             << "3 - Exit\n";
    }

    void userMenu()
    {
        cout << "\nWelcome to Group 3 Cinema: Your Ultimate Movie Experience! \n"
             << "1 - View Movies\n"
             << "2 - View Seats\n"
             << "3 - Choice 3\n"
             << "4 - Choice 4\n"
             << "5 - Choice 5\n"
             << "6 - Choice 6\n"
             << "7 - Choice 7\n"
             << "8 - Choice 8\n"
             << "9 - Choice 9\n"
             << "0 - Exit to Login\n";
    }

    void adminMenu()
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

    // all function related to user goes here
    void handleUserMenu()
    {
        string choice;
        while (choice != "0")
        {
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == "1")
            {
                viewMovies();
                cout << "User Choice 1\n";
            }
            else if (choice == "2")
            {
                viewSeats();
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

    // all function related to admin goes here
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
    return 0;
}
