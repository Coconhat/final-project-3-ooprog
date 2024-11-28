#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

/*
For Users:
1. View a list of 4 movies, their showtimes, and ticket prices upon starting the program.
2. Make a booking by selecting a movie first, then choosing a seat from the displayed 2D seat layout for the selected movie room.
3. View, modify, or cancel existing bookings.

For Admins:
1. Manage movies and showtimes (add, edit, or delete movies).
2. Configure the seat layout of movie rooms.
*/

// Base class for all cinema-related functionality
class Cinema
{
public:
    virtual void viewMovies() {}
    virtual void makeBooking() {}
    virtual void cancelBooking() {}
    virtual void modifiedBooking() {}
    virtual void viewBooking() {}
    virtual void viewSeats() {}
    virtual void selectSeat() {}
    virtual void manageMovieAndShowtime() {}
    virtual void manageSeatLayout() {}
    virtual ~Cinema() = default;
};

// Handles movie-related logic
class Movies : public Cinema
{
public:
    void viewMovies() override
    {
        cout << "Viewing movies, showtime, and ticket price...\n";

        // 4 movies here
    }

    void manageMovieAndShowtime() override
    {
        cout << "Managing movies and showtimes...\n";
    }
};

// Handles booking-related functionality
class Booking : public Cinema
{
public:
    void makeBooking() override
    {
        cout << "User is making a booking...\n";
    }

    void cancelBooking() override
    {
        cout << "User is canceling a booking...\n";
    }

    void viewBooking() override
    {
        cout << "User is viewing their booking...\n";
    }

    void modifiedBooking() override
    {
        cout << "User is viewing their booking...\n";
    }
};

// Handles seat-related functionality
class Seat : public Cinema
{
public:
    void viewSeats() override
    {
            // 1 room palang to, dapat 4 depende sa movie room
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
    void manageSeatLayout() override
    {
        cout << "Managing seat layout...\n";
    }

    void selectSeat() override
    {
        cout << "User is selecting a seat...\n";
    }
};

class Payment
{
    // Singleton or Strategy Design Pattern from GoF heree
};

// Handles display logic
class Display : public Cinema
{
private:
    Movies movies;
    Booking booking;
    Seat seat;

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
             << "3 - Make a Booking\n"
             << "4 - Cancel a Booking\n"
             << "5 - View Booking\n"
             << "0 - Exit to Login\n";
    }

    void adminMenu()
    {
        cout << "\nAdmin Dashboard: Manage Showtimes, Tickets, and More \n"
             << "1 - Manage Movies and Showtimes\n"
             << "2 - Manage Seat Layout\n"
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

    // Handles user-related functionality
    void handleUserMenu()
    {
        string choice;
        while (choice != "0")
        {
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == "1")
            {
                movies.viewMovies();
            }
            else if (choice == "2")
            {
                seat.viewSeats();
            }
            else if (choice == "3")
            {
                booking.makeBooking();
            }
            else if (choice == "4")
            {
                booking.cancelBooking();
            }
            else if (choice == "5")
            {
                booking.viewBooking();
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

    // Handles admin-related functionality
    void handleAdminMenu()
    {
        string choice;
        while (choice != "0")
        {
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == "1")
            {
                movies.manageMovieAndShowtime();
            }
            else if (choice == "2")
            {
                seat.manageSeatLayout();
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
};

int main()
{
    Display display;
    display.handleLoginInput();
    return 0;
}
