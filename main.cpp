#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
#include "inputValidation.cpp" //validation function included here

using namespace std;

/*
For Users:
1. View a list of 4 movies, their showtimes, and ticket prices upon starting the program.
2. Make a booking by selecting a movie first, then choosing a seat from the displayed 2D seat layout for the selected movie room.
3. View, modify, or cancel existing bookings.

For Admins:
1. Manage movies and showtimes (add, edit, or delete movies).
2. EACH ROOM CAN SHOW MAXIMUM OF 2 MOVIES FOR A DAY. EX(ROOM 1 CAN SHOW movie a from 8-10 and movie b from 2-5)
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

struct Movie
{
    string title;
    string showtime;
    string until;
    double price;
};

class Movies
{
private:
public:
    map<int, vector<Movie>> rooms;

    Movies()
    {
        // Initialize rooms with default movies
        rooms[1].push_back({"Avengers: Endgame", "03:00", "05:15", 300});
        rooms[2].push_back({"Inception", "05:30", "07:30", 150});
        rooms[3].push_back({"The Lion King", "15:00", "17:15", 300});
        rooms[4].push_back({"Joker", "09:30", "11:45", 250});
    }

    void viewMovies()
    {
        // print header
        cout << "----------------------------------------------------------\n";
        cout << setw(10) << left << "Room"
             << setw(25) << "Movie Title"
             << setw(20) << "Showtime"
             << setw(15) << "Until"
             << setw(15) << "Ticket Price\n";
        cout << "----------------------------------------------------------\n";

        int previousRoom = -1; // To track the previous room number

        for (const auto &room : rooms)
        {
            // Check if the room number has changed
            if (room.first != previousRoom && previousRoom != -1)
            {
                cout << "\n"; // Add extra newline when the room changes
            }

            for (const auto &movie : room.second) // loop over the movies struct
            {
                // Print room number only once per room
                if (room.first != previousRoom)
                {
                    cout << setw(10) << left << to_string(room.first); // Print room number
                }
                else
                {
                    cout << setw(10) << ""; // Empty space if the room number is the same
                }

                cout << setw(25) << movie.title
                     << setw(20) << movie.showtime
                     << setw(15) << movie.until
                     << setw(15) << fixed << setprecision(2) << movie.price << "\n";
            }

            previousRoom = room.first; // Update previousRoom to current room
        }

        cout << "----------------------------------------------------------\n";
    }

    void manageMovieAndShowtime()
    {
        string choice;
        while (choice != "0")
        {
            cout << "\nMovie Management:\n"
                 << "1 - Add Movie\n"
                 << "2 - Edit Movie\n"
                 << "3 - Delete Movie\n"
                 << "0 - Exit Movie Management\n";
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == "1")
            {
                addMovie();
            }
            else if (choice == "2")
            {
                editMovie();
            }
            else if (choice == "3")
            {
                deleteMovie();
            }
            else if (choice == "0")
            {
                cout << "Exiting Movie Management...\n";
                break;
            }
            else
            {
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }

private:
    void addMovie()
    {
        int room;
        const int maxRoom = 2;
        cout << "Enter room number (1-4): ";
        getValidRoom("room", room);

        // Check if room already has movies
        if (rooms[room].size() >= maxRoom)
        {
            cout << "Room " << room << " already has a movie in it.\n";
            return;
        }

        string title;
        string showtime;
        string until;
        double price;

        cout << "\nEnter movie title: ";
        cin.ignore();
        getline(cin, title);

        int hour, minute, untilHour, untilMinute;

        // Ask for showtime
        cout << "Enter Showtime (hour and minute):\n";
        getValidInput("Hour", hour, 0, 23);
        getValidInput("Minute", minute, 0, 59);

        //validate here too

        // Ask for until time
        cout << "\nEnter Until time (hour and minute):\n";
        getValidInput("Hour", untilHour, 0, 23);
        getValidInput("Minute", untilMinute, 0, 59);

        // Convert to total minutes since midnight
        int startTime = hour * 60 + minute;
        int endTime = untilHour * 60 + untilMinute;

        // Validate time until time for next moviezxs
        if (endTime <= startTime)
        {
            cout << "Invalid schedule: 'Until' time must be after 'Showtime'.\n";
            return;
        }

        // Check for conflicts with existing movies in the room
        for (const auto &movie : rooms[room])
        {
            int existingStartTime = stoi(movie.showtime.substr(0, 2)) * 60 + stoi(movie.showtime.substr(3, 2));
            int existingEndTime = stoi(movie.until.substr(0, 2)) * 60 + stoi(movie.until.substr(3, 2));

            if ((startTime >= existingStartTime && startTime < existingEndTime) ||
                (endTime > existingStartTime && endTime <= existingEndTime) ||
                (startTime <= existingStartTime && endTime >= existingEndTime))
            {
                cout << "Schedule conflict detected with movie \"" << movie.title << "\".\n";
                return;
            }
        }

        // Format times as strings
        stringstream ss, ff;
        ss << setw(2) << setfill('0') << hour << ":" << setw(2) << setfill('0') << minute;
        showtime = ss.str();

        ff << setw(2) << setfill('0') << untilHour << ":" << setw(2) << setfill('0') << untilMinute;
        until = ff.str();

        getValidPrice("price", price);

        // Add movie to the selected room
        rooms[room].push_back({title, showtime, until, price});
        cout << "Movie added successfully to Room " << room << "!\n";
    }

    void editMovie()
    {
        int room;
        cout << "Enter room number (1-4): ";
        getValidRoom("room", room);

        if (rooms.find(room) == rooms.end() || rooms[room].empty())
        {
            cout << "No movies found in Room " << room << "!\n";
            return;
        }

        viewMoviesInRoom(room);

        int index;
        cout << "Enter movie index to edit: ";
        getValidQuantity("index", index);

        if (index < 1 || index > rooms[room].size())
        {
            cout << "Invalid movie index!\n";
            return;
        }

        string title;
        string showtime;
        string until;
        double price;

        int hour, minute;
        int untilHour, untilMinute;

        cout << "Enter new title (leave blank to keep current): ";
        cin.ignore();
        getline(cin, title);

        cout << "\nEnter new showtime:\n";
        getValidInput("hour", hour, 0, 24, true);
        getValidInput("minute", minute, 0, 60, true);

        cout << "\nEnter new until time:\n";
        getValidInput("hour", untilHour, 0, 24, true);
        getValidInput("minute", untilMinute, 0, 60, true);

        getValidPrice("price", price);

        // Update the movie fields if they're not empty
        if (!title.empty())
            rooms[room][index - 1].title = title;

        if (!showtime.empty())
            rooms[room][index - 1].showtime = showtime;

        if (!until.empty())
            rooms[room][index - 1].until = until;

        stringstream ss, ff;
        ss << setw(2) << setfill('0') << hour << ":" << setw(2) << setfill('0') << minute;
        rooms[room][index - 1].showtime = ss.str();

        ff << setw(2) << setfill('0') << untilHour << ":" << setw(2) << setfill('0') << untilMinute;
        rooms[room][index - 1].until = ff.str();

        if (price > 0)
            rooms[room][index - 1].price = price;

        cout << "Movie updated successfully in Room " << room << "!\n";
    }

    void deleteMovie()
    {
        int room;
        cout << "Enter room number (1-4): ";
        getValidQuantity("room", room);

        if (rooms.find(room) == rooms.end() || rooms[room].empty())
        {
            cout << "No movies found in Room " << room << "!\n";
            return;
        }

        viewMoviesInRoom(room);

        int index;
        cout << "Enter movie index to delete: ";
        getValidQuantity("index", index);

        if (index < 1 || index > rooms[room].size())
        {
            cout << "Invalid movie index!\n";
            return;
        }

        cout << "Movie \"" << rooms[room][index - 1].title << "\" deleted successfully from Room " << room << "!\n";
        rooms[room].erase(rooms[room].begin() + index - 1);
    }

    void viewMoviesInRoom(int room)
    {
        cout << "\nMovies in Room " << room << ":\n";
        cout << "----------------------------------------------------------\n";
        cout << setw(10) << left << "Index"
             << setw(25) << "Movie Title"
             << setw(20) << "Showtime"
             << setw(15) << "Ticket Price\n";
        cout << "----------------------------------------------------------\n";

        for (int i = 0; i < rooms[room].size(); ++i)
        {
            cout << setw(10) << left << to_string(i + 1)
                 << setw(25) << rooms[room][i].title
                 << setw(20) << rooms[room][i].showtime
                 << setw(15) << fixed << setprecision(2) << rooms[room][i].price << "\n";
        }

        cout << "----------------------------------------------------------\n";
    }
};

// Handles booking-related functionality
class Booking : public Cinema
{
private:
    Movies movies;
    map<int, vector<string>> bookedSeats;

    struct BookingInfo
    {
        string movieTitle;
        int room;
        vector<string> seats;
        int ticketCount;
        double totalPrice;
        string date;
    };
    vector<BookingInfo> userBookings;

public:
    void makeBooking() override
    {
        int roomChoice;

        cout << "Available Movies:\n";
        movies.viewMovies();

        cout << "\nPlease enter the room number of your movie choice: ";
        getValidRoom("room number", roomChoice);

        if (movies.rooms.find(roomChoice) == movies.rooms.end() || movies.rooms[roomChoice].empty())
        {
            cout << "Invalid room selection or no movies available in this room.\n";
            return;
        }

        if (movies.rooms[roomChoice].size() > 1)
        {
            cout << "Enter the number of your movie: ";
            for (int i = 0; i < movies.rooms[roomChoice].size(); i++)
            {
                cout << i + 1 << " - " << movies.rooms[roomChoice][i].title << " (" << movies.rooms[roomChoice][i].showtime << ")\n";
            }
            int index;
            getValidInput("movie index", index, 1, movies.rooms[roomChoice].size());

            cout << "yey";
        }

        // if 2 movies prompt the user to enter 1 for movie a and 2 for movie b

        // show ticket price and ask for how many ticket

        // ask for date

        // get seat layout function

        // payment function

        // confirmation
    }

    void cancelBooking() override
    {
        cout << "User is canceling a booking...\n";
        // view booking function

        // ask the user which booking they want to cancel

        // confirm cancel and update the seat layout
    }

    void viewBooking() override
    {
        cout << "User is viewing their booking...\n";

        // show booking
    }

    void modifiedBooking() override
    {
        cout << "User is viewing their booking...\n";

        // show view booking function

        // ask what they booking they want to modified

        // modify what? ticket or seat

        //..
    }
};

// Handles seat-related functionality
class Seat : public Cinema
{
public:
    void viewSeats(int roomIndex, const map<string, bool> &bookedSeats)
    {
        // Print room and seat layout information
        cout << "\nSeat Layout for Room " << roomIndex << "\n\n";

        // Print column numbers
        cout << "     "; // Adjusted initial spacing for alignment with row labels
        for (int j = 1; j <= 10; ++j)
        {
            if (j == 1)
            {
                cout << j; // Print the first number without extra margin
            }
            else
            {
                cout << setw(5) << j; // For subsequent numbers, use setw for spacing
            }
        }
        cout << "\n\n";

        // Print seat layout
        for (int i = 0; i < 10; ++i)
        {
            // Print row letter
            char rowLetter = 'A' + i;
            cout << rowLetter << "   ";

            for (int j = 0; j < 10; ++j)
            {
                // Create seat identifier
                string seatId = string(1, rowLetter) + to_string(j + 1);

                // Display the seat identifier instead of [O] or [X]
                // Check if seat is booked
                if (bookedSeats.count(seatId) && bookedSeats.at(seatId))
                {
                    cout << "X"; // Booked seat, show the seat identifier followed by '@'
                }
                else
                {
                    cout << seatId << "   "; // Available seat, just show the seat identifier
                }
            }
            cout << endl;
        }
    }

    // Original viewSeats method can be removed or modified as needed
    void viewSeats() override
    {
        int roomIndex;
        cout << "Enter room number (1-4) to view seat layout: ";
        getValidRoom("room number", roomIndex);

        map<string, bool> bookedSeats;
        viewSeats(roomIndex, bookedSeats);
    }
    void manageSeatLayout() override
    {
        cout << "Managing seat layout...\n";
    }

    void selectSeat() override
    {
        cout << "Please select your seats (e.g A1, B2): ";
    };
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
        // add user functionality here too :>
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
