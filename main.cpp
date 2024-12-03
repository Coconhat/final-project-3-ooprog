#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <sstream>
#include "inputValidation.cpp" //validation function included here

using namespace std;
using namespace chrono;

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

struct Movie
{
    string title;
    string showtime;
    string until;
    double price;
};

// base class for movie
class Cinema
{
public:
    virtual void viewMovies() = 0;
    virtual void manageMovieAndShowtime() = 0;
    virtual ~Cinema() = default;
};

// base class for booking
class BookingManager
{
    virtual void makeBooking() = 0;
    virtual void cancelBooking() = 0;
    virtual void modifiedBooking() = 0;
    virtual void viewBooking() = 0;
};
// base class for seat
class SeatManager
{
    virtual void viewSeats() = 0;
    virtual void selectSeat() = 0;
    virtual void manageSeatLayout() = 0;
};

// base class for show time
class ShowtimeManager
{
    virtual void getSchedule(int room, int &hour, int &minute, int &untilHour, int &untilMinute, map<int, vector<Movie>> &rooms, string &showtime, string &until) = 0;
};

// base class for ticket
class TicketManager
{
    virtual void printTicket() = 0;
};

class Showtime : public ShowtimeManager
{
public:
    void getSchedule(int room, int &hour, int &minute, int &untilHour, int &untilMinute, map<int, vector<Movie>> &rooms, string &showtime, string &until) override
    {
        // Ask for showtime
        cout << "Enter Showtime (hour and minute):\n";
        getValidInput("Hour", hour, 0, 23);
        getValidInput("Minute", minute, 0, 59);

        // Convert to total minutes since midnight
        int startTime = hour * 60 + minute;

        // Ask for until time
        cout << "\nEnter Until time (hour and minute):\n";
        getValidInput("Hour", untilHour, 0, 23);
        getValidInput("Minute", untilMinute, 0, 59);

        // Convert until time to total minutes
        int endTime = untilHour * 60 + untilMinute;

        // Validate time until time for next movie
        if (endTime <= startTime)
        {
            cout << "Invalid schedule: 'Until' time must be after 'Showtime'.\n";
            return; // Exit the function early if the "until time" is not valid
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
                return; // Exit the function early if there's a conflict
            }
        }

        // Format times as strings
        stringstream ss, ff;
        ss << setw(2) << setfill('0') << hour << ":" << setw(2) << setfill('0') << minute;
        showtime = ss.str();

        ff << setw(2) << setfill('0') << untilHour << ":" << setw(2) << setfill('0') << untilMinute;
        until = ff.str();
    }
};

class Ticket : public TicketManager
{
private:
    string movieTitle;
    int room;
    vector<string> seats;
    double totalPrice;

public:
    Ticket(const string &movie, int roomNumber, const vector<string> &seatList, double price)
        : movieTitle(movie), room(roomNumber), seats(seatList), totalPrice(price) {}

    // Method to print ticket details
    void printTicket() override
    {
        cout << "\nBooking confirmed!\n";
        cout << "Movie: " << movieTitle << endl;
        cout << "Room: " << room << endl;
        cout << "Seats: ";
        for (const auto &seat : seats)
            cout << seat << " ";
        cout << endl;
        cout << "Total Price: PHP" << totalPrice << endl;
    }
};

class Movies : public Cinema
{
public:
    static map<int, vector<Movie>> rooms;
    Movies()
    {
        static bool isInitialized = false;
        if (!isInitialized)
        {
            rooms[1].push_back({"Avengers: Endgame", "03:00", "05:15", 300});
            rooms[2].push_back({"Inception", "05:30", "07:30", 150});
            rooms[3].push_back({"The Lion King", "15:00", "17:15", 300});
            rooms[4].push_back({"Joker", "09:30", "11:45", 250});
            isInitialized = true;
        }
    }

    void viewMovies() override
    {
        // print header
        cout << "----------------------------------------------------------\n";
        cout << setw(10) << left << "Room"
             << setw(25) << "Movie Title"
             << setw(20) << "Showtime"
             << setw(15) << "Until"
             << setw(15) << "Ticket Price\n";
        cout << "----------------------------------------------------------\n";

        int previousRoom = -1;

        for (const auto &room : rooms)
        {
            // Check if the room number has changed
            if (room.first != previousRoom && previousRoom != -1)
            {
                cout << "\n";
            }

            for (const auto &movie : room.second)
            {

                if (room.first != previousRoom)
                {
                    cout << setw(10) << left << to_string(room.first);
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

            previousRoom = room.first;
        }

        cout << "----------------------------------------------------------\n";
    }

    void manageMovieAndShowtime() override
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

        if (rooms[room].size() >= maxRoom)
        {
            cout << "Room " << room << " already has 2 movies in it.\n";
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
        Showtime show;
        show.getSchedule(room, hour, minute, untilHour, untilMinute, rooms, showtime, until);

        if (showtime.empty() || until.empty())
        {
            cout << "Movie not added due to invalid schedule.\n";
            return;
        }

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
map<int, vector<Movie>> Movies::rooms;

// Handles booking-related functionality
class Seat : public SeatManager
{
private:
    // Static to record other's method booked seats
    static map<int, map<string, bool>> roomBookedSeats;

public:
    static void initializeSeats()
    {
        for (int room = 1; room <= 4; ++room)
        {
            for (char row = 'A'; row <= 'J'; ++row)
            {
                for (int col = 1; col <= 10; ++col)
                {
                    string seatId = string(1, row) + to_string(col);
                    if (roomBookedSeats[room].find(seatId) == roomBookedSeats[room].end())
                    {
                        roomBookedSeats[room][seatId] = false;
                    }
                }
            }
        }
    }

    Seat()
    {
        initializeSeats();
    }

    bool isValidSeat(const string &seatId)
    {
        // Check if seat ID is valid (A1-J10)
        if (seatId.length() < 2 || seatId.length() > 3)
            return false;

        char row = seatId[0];
        int col;
        try
        {
            col = stoi(seatId.substr(1));
        }
        catch (...)
        {
            return false;
        }

        return (row >= 'A' && row <= 'J' && col >= 1 && col <= 10);
    }

    void viewRoomSeats(int roomIndex)
    {
        initializeSeats();

        cout << "\nSeat Layout for Room " << roomIndex << "\n\n";

        // Print column numbers
        cout << "   ";
        for (int j = 1; j <= 10; ++j)
        {
            cout << setw(6) << j;
        }
        cout << "\n\n";

        // Print seat layout
        for (int i = 0; i < 10; ++i)
        {

            char rowLetter = 'A' + i;
            cout << rowLetter << "   ";

            for (int j = 1; j <= 10; ++j)
            {
                // Create seat identifier
                string seatId = string(1, rowLetter) + to_string(j);

                bool isBooked = roomBookedSeats[roomIndex][seatId];

                if (isBooked)
                {
                    cout << setw(6) << "X";
                }
                else
                {
                    cout << setw(6) << seatId;
                }
            }
            cout << endl;
        }
    }

    void viewSeats() override
    {
        int roomIndex;
        cout << "Enter room number (1-4) to view seat layout: ";
        getValidRoom("room number", roomIndex);

        viewRoomSeats(roomIndex);
    }

    void selectSeat() override
    {
        int roomIndex;
        cout << "Enter room number (1-4) to view seat layout: ";
        getValidRoom("room number", roomIndex);

        vector<string> selectedSeats = selectSeats(roomIndex, 1);

        if (!selectedSeats.empty())
        {
            cout << "You selected seat: " << selectedSeats[0] << endl;
        }
    }

    vector<string> selectSeats(int roomIndex, int ticketCount)
    {
        vector<string> selectedSeats;

        while (selectedSeats.size() < ticketCount)
        {
            viewRoomSeats(roomIndex);

            string selectedSeat;
            bool validSelection = false;

            while (!validSelection)
            {
                cout << "Please select seat " << (selectedSeats.size() + 1)
                     << " of " << ticketCount << " (e.g., A1, B2): ";
                cin >> selectedSeat;

                if (!isValidSeat(selectedSeat))
                {
                    cout << "Invalid seat format. Please use format like A1, B2.\n";
                    continue;
                }

                if (roomBookedSeats[roomIndex][selectedSeat])
                {
                    cout << "Seat " << selectedSeat << " is already booked. Please choose another seat.\n";
                    continue;
                }

                if (find(selectedSeats.begin(), selectedSeats.end(), selectedSeat) != selectedSeats.end())
                {
                    cout << "You have already selected seat " << selectedSeat << ". Choose a different seat.\n";
                    continue;
                }

                selectedSeats.push_back(selectedSeat);
                validSelection = true;
            }

            if (selectedSeats.size() < ticketCount)
            {
                string confirm;
                cout << "Seat " << selectedSeat << " selected. Continue selecting? (y/n): ";
                cin >> confirm;
                toUpperCase(confirm);

                if (confirm == "N")
                {
                    cout << "Cancelling seat selection.\n";
                    return {}; // Return an empty vector to indicate cancellation.
                }
                else if (confirm != "Y")
                {
                    cout << "Invalid input. Please enter 'y' or 'n'. Continuing selection by default.\n";
                }
            }
        }

        return selectedSeats;
    }

    void bookSeats(int roomIndex, const vector<string> &seats)
    {
        for (const auto &seat : seats)
        {
            roomBookedSeats[roomIndex][seat] = true;
        }
    }

    void manageSeatLayout() override
    {
        cout << "Managing seat layout...\n";
    }
};

map<int, map<string, bool>> Seat::roomBookedSeats;

// Handles booking-related functionality
class Booking : public BookingManager
{
private:
    Seat seatManager;
    Movies movies;

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
        int roomChoice, ticketCount;

        cout << "Available Movies:\n";
        movies.viewMovies();

        cout << "\nPlease enter the room number of your movie choice: ";
        getValidRoom("room number", roomChoice);

        if (movies.rooms.find(roomChoice) == movies.rooms.end() || movies.rooms[roomChoice].empty())
        {
            cout << "Invalid room selection or no movies available in this room.\n";
            return;
        }

        Movie selectedMovie;
        if (movies.rooms[roomChoice].size() > 1)
        {
            cout << "Enter the number of your movie: \n";
            for (int i = 0; i < movies.rooms[roomChoice].size(); i++)
            {
                cout << i + 1 << " - " << movies.rooms[roomChoice][i].title << " (" << movies.rooms[roomChoice][i].showtime << ")\n";
            }
            int index;

            do
            {
                cout << "\nEnter movie index (1 or 2): ";
                getValidQuantity("movie index", index);
            } while (index < 1 || index > movies.rooms[roomChoice].size());

            selectedMovie = movies.rooms[roomChoice][index - 1];
        }
        else
        {
            selectedMovie = movies.rooms[roomChoice][0];
        }

        // Get ticket count
        cout << "\nEnter number of tickets: ";
        getValidQuantity("ticket count", ticketCount);

        // Calculate total price
        double totalPrice = selectedMovie.price * ticketCount;

        // Select seats
        vector<string> selectedSeats = seatManager.selectSeats(roomChoice, ticketCount);

        if (selectedSeats.size() != ticketCount)
        {
            cout << "Seat selection cancelled.\n";
            return;
        }

        // Get booking date
        string bookingDate;
        cout << "Enter booking date (YYYY-MM-DD): ";
        cin >> bookingDate;

        // Confirm booking
        string confirm;
        cout << "Confirm booking? (y/n): ";
        cin >> confirm;
        toUpperCase(confirm);

        if (confirm == "Y")
        {
            // Create a Ticket object
            Ticket ticket(selectedMovie.title, roomChoice, selectedSeats, totalPrice);

            // Print the ticket details
            ticket.printTicket();

            // Save the booking
            BookingInfo newBooking = {selectedMovie.title, roomChoice, selectedSeats, ticketCount, totalPrice, bookingDate};
            userBookings.push_back(newBooking);

            // Mark seats as booked
            seatManager.bookSeats(roomChoice, selectedSeats);

            cout << "Booking successfully saved!\n";
        }
        else
        {
            cout << "Booking canceled.\n";
        }
    }

    void cancelBooking() override
    {
        cout << "User is canceling a booking...\n";
        // Implementation needed
    }

    void viewBooking() override
    {
        cout << "User is viewing their booking...\n";
        // Implementation needed
    }

    void modifiedBooking() override
    {
        cout << "User is modifying their booking...\n";
        // Implementation needed
    }
};

class Payment
{ // SINGLETON PATTERN
private:
    // Singleton instance
    static Payment *instance;

    // Mutex for thread safety
    static mutex instanceMutex;

    // Payment properties
    int paymentID;
    float amount;
    string method;
    string status;

    // Private constructor
    Payment() : paymentID(0), amount(0.0), method("Unknown"), status("Pending") {}

public:
    // Public static method
    static Payment *getInstance()
    {
        // Mutex Thread Lock Safety
        if (instance == nullptr)
        {
            lock_guard<mutex> lock(instanceMutex); // Thread-safe
            if (instance == nullptr)
            {
                instance = new Payment();
            }
        }
        return instance;
    }

    // Delete copy constructor and assignment operator
    Payment(const Payment &) = delete;
    Payment &operator=(const Payment &) = delete;

    // Setters and Getters
    void setPaymentID(int id) { paymentID = id; }
    int getPaymentID() const { return paymentID; }

    void setAmount(float amt) { amount = amt; }
    float getAmount() const { return amount; }

    void setMethod(const string &mtd) { method = mtd; }
    string getMethod() const { return method; }

    void setStatus(const string &sts) { status = sts; }
    string getStatus() const { return status; }

    // Business logic: Payment processing
    bool processPayment()
    {
        if (method == "Credit Card" || method == "Gcash" || method == "Cash")
        {
            status = "Processed";
            return true;
        }
        else
        {
            status = "Failed";
            return false;
        }
    }

    // Business logic: Display payment details
    void displayPaymentDetails() const
    {
        cout << "Payment ID: " << paymentID << endl;
        cout << "Amount: $" << amount << endl;
        cout << "Method: " << method << endl;
        cout << "Status: " << status << endl;
    }
};

// Initialize static members
Payment *Payment::instance = nullptr;
mutex Payment::instanceMutex;

// Handles display logic
class Display : public Movies, public Seat, public Booking
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
                viewMovies();
            }
            else if (choice == "2")
            {
                viewSeats();
            }
            else if (choice == "3")
            {
                makeBooking();
            }
            else if (choice == "4")
            {
                cancelBooking();
            }
            else if (choice == "5")
            {
                viewBooking();
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
                manageMovieAndShowtime();
            }
            else if (choice == "2")
            {
                manageSeatLayout();
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
