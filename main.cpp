#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <thread>
#include "inputValidation.cpp"

using namespace std;
using namespace chrono;

// Font Formatting
const string RESET = "\033[0m";         // Reset (code 0)
const string BOLD = "\033[1m";          // Bold (code 1)
const string ITALICS = "\033[3m";       // Italics (code 3)
const string RED = "\033[1;31m";        // Red (code 31)
const string GREEN = "\033[1;32m";      // Green (code 32)
const string YELLOW = "\033[1;33m";     // Yellow (code 33)
const string BLUE = "\033[1;34m";       // Blue (code 34)
const string MAGENTA = "\033[1;35m";    // Magenta (code 35)
const string CYAN = "\033[1;36m";       // Cyan (code 36)
const string ORANGE = "\033[38;5;130m"; // Orange (256-color palette, extended code)

// Wait for user input before returning to the menu
void returnMenu()
{
    string userInput;
    cout << "0 - Back: ";
    while (true)
    {
        cin >> userInput;
        if (userInput == "0")
        {
            break; // Exit the function and return to the menu
        }
        else
        {
            cout << "Invalid input. Enter 0 to return: ";
        }
    }
};

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
    virtual void paymentStatus() = 0;
    virtual void confirmPayment() = 0;
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

        int endTime = untilHour * 60 + untilMinute;

        if (endTime <= startTime)
        {
            cout << "Invalid schedule: 'Until' time must be after 'Showtime'.\n";
            return; // Exit the function early if the "until time" is not valid
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
    string date;

public:
    Ticket(const string &movie, int roomNumber, const vector<string> &seatList, double price, const string &bookingDate)
        : movieTitle(movie), room(roomNumber), seats(seatList), totalPrice(price), date(bookingDate) {}

    void printTicket() override
    {
        cout << "\nBooking confirmed!\n";
        cout << "Movie: " << movieTitle << endl;
        cout << "Room: " << room << endl;
        cout << "Seats: ";
        for (const auto &seat : seats)
            cout << seat << " ";
        cout << endl;
        cout << "Date: " << date << endl;
        cout << "Total Price: Php" << totalPrice << endl;
    }
};

class Movies : public Cinema
{
public:
    static map<int, vector<Movie>> rooms;
    Movies() {}
    void viewMovies() override
    {
        cout << "----------------------------------------------------------\n";
        cout << setw(10) << left << "Room"
             << setw(25) << "Movie Title"
             << setw(20) << "Showtime"
             << setw(15) << "Until"
             << setw(15) << "Ticket Price\n";
        cout << "----------------------------------------------------------\n";

        for (int room = 1; room <= 4; ++room)
        {
            // Check if the room exists in the map and has movies
            if (rooms.find(room) == rooms.end() || rooms[room].empty())
            {
                cout << setw(10) << left << room
                     << setw(25) << "No movies available"
                     << setw(20) << ""
                     << setw(15) << ""
                     << setw(15) << "" << "\n";
            }
            else
            {
                for (const auto &movie : rooms[room])
                {
                    cout << setw(10) << left << room
                         << setw(25) << movie.title
                         << setw(20) << movie.showtime
                         << setw(15) << movie.until
                         << setw(15) << fixed << setprecision(2) << movie.price << "\n\n";
                }
            }
        }

        cout << "----------------------------------------------------------\n";
    }

    void manageMovieAndShowtime() override
    {
        string choice;
        while (choice != "0")
        {
            cout << "Movie Management:\n"
                 << "1 - Add Movie\n"
                 << "2 - Edit Movie\n"
                 << "3 - Delete Movie\n"
                 << "0 - Exit Movie Management\n";
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == "1")
            {
                system("cls");
                addMovie();
            }
            else if (choice == "2")
            {
                system("cls");
                editMovie();
            }
            else if (choice == "3")
            {
                system("cls");
                deleteMovie();
            }
            else if (choice == "0")
            {
                cout << "Exiting Movie Management...\n";
                this_thread::sleep_for(chrono::milliseconds(1000)); // Pause for 1 second
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
        const int maxRoom = 1;
        cout << "Enter room number (1-4): ";
        getValidRoom("room", room);

        if (rooms[room].size() >= maxRoom)
        {
            cout << RED << "Room " << room << " already has a movie in it.\n"
                 << RESET;
            this_thread::sleep_for(chrono::milliseconds(3000)); // Pause for 3 seconds
            system("cls");
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

        Showtime show;
        show.getSchedule(room, hour, minute, untilHour, untilMinute, rooms, showtime, until);

        if (showtime.empty() || until.empty())
        {
            cout << RED << "Movie not added due to invalid schedule.\n"
                 << RESET;
            this_thread::sleep_for(chrono::milliseconds(3000)); // Pause for 3 seconds
            system("cls");
            return;
        }

        getValidPrice("price", price);

        // Add movie to the selected room
        rooms[room].push_back({title, showtime, until, price});
        cout << GREEN << "Movie added successfully to Room " << room << "!\n"
             << RESET;
        this_thread::sleep_for(chrono::milliseconds(3000)); // Pause for 3 seconds
        system("cls");
    }

    void editMovie()
    {
        int room;
        cout << "Enter room number (1-4): ";
        getValidRoom("room", room);

        // Check if the room has any movies
        if (rooms.find(room) == rooms.end() || rooms[room].empty())
        {
            cout << ORANGE << "No movies found in Room " << room << "!\n"
                 << RESET;
            this_thread::sleep_for(chrono::milliseconds(3000)); // Pause for 3 seconds
            system("cls");
            return;
        }

        cout << "Editing the movie in Room " << room << ":\n";
        viewMoviesInRoom(room);

        string title;
        string showtime;
        string until;
        double price;
        int hour = -1, minute = -1;
        int untilHour = -1, untilMinute = -1;

        cout << "Enter new title (leave blank to keep current): ";
        cin.ignore();
        getline(cin, title);

        cout << "\nEnter new showtime:\n";
        getValidTime("hour", hour, 0, 24, true);
        getValidTime("minute", minute, 0, 60, true);

        cout << "\nEnter new until time:\n";
        getValidTime("hour", untilHour, 0, 24, true);
        getValidTime("minute", untilMinute, 0, 60, true);

        getValidPrice("price", price);

        Movie &movie = rooms[room][0];

        // Update title if not blank
        if (!title.empty())
        {
            movie.title = title;
        }

        // Update showtime only if both hour and minute are valid
        if (hour != -1 && minute != -1)
        {
            stringstream ss;
            ss << setw(2) << setfill('0') << hour << ":" << setw(2) << setfill('0') << minute;
            movie.showtime = ss.str();
        }

        // Update until time only if both hour and minute are valid
        if (untilHour != -1 && untilMinute != -1)
        {
            stringstream ff;
            ff << setw(2) << setfill('0') << untilHour << ":" << setw(2) << setfill('0') << untilMinute;
            movie.until = ff.str();
        }

        // Update price if positive
        if (price > 0)
        {
            movie.price = price;
        }

        cout << GREEN << "Movie Updated!" << RESET;
        this_thread::sleep_for(chrono::milliseconds(3000)); // Pause for 3 seconds
        system("cls");
    }

    void deleteMovie()
    {
        int room;

        cout << "Enter room number (1-4): ";
        getValidQuantity("room", room);

        if (rooms.find(room) == rooms.end() || rooms[room].empty())
        {
            cout << ORANGE << "No movies found in Room " << room << "!\n"
                 << RESET;
            this_thread::sleep_for(chrono::milliseconds(3000)); // Pause for 3 seconds
            system("cls");
            return;
        }

        cout << "\nMovie in Room " << room << ":\n";
        viewMoviesInRoom(room);

        string userChoice;
        cout << "Are you sure you want to delete this movie? (Y for Yes, N for No): ";
        cin >> userChoice;

        while (userChoice != "Y" && userChoice != "y" && userChoice != "N" && userChoice != "n")
        {
            cout << RED << "Invalid input. Please enter 'Y' for Yes or 'N' for No: " << RED;
            cin >> userChoice;
        }

        if (userChoice == "Y" || userChoice == "y")
        {
            cout << GREEN << "Movie \"" << rooms[room][0].title << "\" deleted successfully from Room " << room << "!\n"
                 << RESET;
            this_thread::sleep_for(chrono::milliseconds(3000)); // Pause for 3 seconds
            system("cls");
            rooms[room].clear();
        }
        else
        {
            cout << RED << "Deletion cancelled.\n"
                 << RESET;
            this_thread::sleep_for(chrono::milliseconds(3000)); // Pause for 3 seconds
            system("cls");
        }
    }

    void viewMoviesInRoom(int room)
    {
        cout << "-----------------------------------------------------------------------------------\n";
        cout << setw(25) << left << "Movie Title"
             << setw(20) << "Showtime"
             << setw(15) << "Until"
             << setw(15) << "Ticket Price\n";
        cout << "-----------------------------------------------------------------------------------\n";

        if (!rooms[room].empty())
        {
            for (const auto &movie : rooms[room])
            {
                cout << setw(25) << left << movie.title
                     << setw(20) << movie.showtime
                     << setw(15) << movie.until
                     << setw(15) << fixed << setprecision(2) << movie.price << "\n";
            }
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
    static map<int, map<string, map<string, bool>>> roomBookedSeats;
    static map<int, map<string, map<string, bool>>> roomReservedSeats;

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
                    // Initialize all rooms and dates
                    for (int dayOffset = 0; dayOffset < 5; ++dayOffset)
                    {
                        string date = getDateAfterOffset(dayOffset);
                        if (roomBookedSeats[room][date].find(seatId) == roomBookedSeats[room][date].end())
                        {
                            roomBookedSeats[room][date][seatId] = false;
                        }
                        if (roomReservedSeats[room][date].find(seatId) == roomReservedSeats[room][date].end())
                        {
                            roomReservedSeats[room][date][seatId] = false;
                        }
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

    void viewRoomSeats(int roomIndex, const string &bookingDate)
    {
        initializeSeats();

        cout << "\nSeat Layout for Room " << roomIndex << " on " << bookingDate << "\n\n";

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

                bool isBooked = roomBookedSeats[roomIndex][bookingDate][seatId];
                bool isReserved = roomReservedSeats[roomIndex][bookingDate][seatId];

                if (isBooked || isReserved)
                {
                    cout << RED << setw(6) << "X" << RESET;
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

        // Get booking date
        string bookingDate = getCurrentDate();

        cout << "Do you want to view for the current date (" << bookingDate << ")? (Y for Yes, N for No): ";
        string userChoice;
        cin >> userChoice;

        while (userChoice != "Y" && userChoice != "y" && userChoice != "N" && userChoice != "n")
        {
            cout << "Invalid input. Please enter 'Y' for Yes or 'N' for No: ";
            cin >> userChoice;
        }

        if (userChoice == "N" || userChoice == "n")
        {
            int dayChoice = 0;
            cout << "Please select a number between 1 and 5 to book a date:\n";
            cout << "1. 1 day after today\n";
            cout << "2. 2 days after today\n";
            cout << "3. 3 days after today\n";
            cout << "4. 4 days after today\n";
            cout << "5. 5 days after today\n";
            cout << "Enter your choice (1-5) or press 'n' to exit: ";

            string userInput;
            cin >> userInput;

            if (userInput == "n" || userInput == "N")
            {
                cout << "Exiting date selection.\n";
                return;
            }

            while (dayChoice < 1 || dayChoice > 5)
            {
                if (userInput == "1" || userInput == "2" || userInput == "3" || userInput == "4" || userInput == "5")
                {
                    dayChoice = stoi(userInput);
                    break;
                }
                else
                {
                    cout << "Invalid input. Please enter a number between 1 and 5 or press 'n' to exit: ";
                    cin >> userInput;

                    if (userInput == "n" || userInput == "N")
                    {
                        cout << "Exiting date selection.\n";
                        return;
                    }
                }
            }

            time_t now = time(0);
            tm *currentTime = localtime(&now);
            currentTime->tm_mday += dayChoice; // Add selected days to current date
            mktime(currentTime);

            char buffer[11]; // Buffer for date string (YYYY-MM-DD + null terminator)
            strftime(buffer, sizeof(buffer), "%Y-%m-%d", currentTime);

            bookingDate = string(buffer);
        }

        cout << "Viewing seat layout for room " << roomIndex << " on " << bookingDate << endl;
        viewRoomSeats(roomIndex, bookingDate);
    }

    void selectSeat() override
    {
        int roomIndex;
        cout << "Enter room number (1-4) to view seat layout: ";
        getValidRoom("room number", roomIndex);

        string bookingDate = getCurrentDate();

        cout << "Do you want to use the current date (" << bookingDate << ")? (Y for Yes, N for No): ";
        string userChoice;
        cin >> userChoice;

        vector<string> selectedSeats = selectSeats(roomIndex, 1, bookingDate);

        if (!selectedSeats.empty())
        {
            cout << "You selected seat: " << selectedSeats[0] << endl;
        }
    }

    vector<string> selectSeats(int roomIndex, int ticketCount, const string &bookingDate)
    {
        vector<string> selectedSeats;

        while (selectedSeats.size() < ticketCount)
        {
            viewRoomSeats(roomIndex, bookingDate);

            string selectedSeat;
            bool validSelection = false;

            while (!validSelection)
            {
                cout << "Please select seat " << (selectedSeats.size() + 1)
                     << " of " << ticketCount << " (e.g., A1, B2): ";
                cin >> selectedSeat;
                toUpperCase(selectedSeat);

                if (!isValidSeat(selectedSeat))
                {
                    cout << "Invalid seat format. Please use format like A1, B2.\n";
                    continue;
                }

                if (roomBookedSeats[roomIndex][bookingDate][selectedSeat])
                {
                    cout << RED << "Seat " << selectedSeat << " is already booked for " << bookingDate << ". Please choose another seat.\n"
                         << RESET;
                    this_thread::sleep_for(chrono::milliseconds(3000)); // Pause for 3 seconds
                    system("cls");
                    continue;
                }

                if (find(selectedSeats.begin(), selectedSeats.end(), selectedSeat) != selectedSeats.end())
                {
                    cout << RED << "You have already selected seat " << selectedSeat << ". Choose a different seat.\n"
                         << RESET;
                    this_thread::sleep_for(chrono::milliseconds(3000)); // Pause for 3 seconds
                    system("cls");
                    continue;
                }

                selectedSeats.push_back(selectedSeat);
                validSelection = true;

                
                if (selectedSeats.size() < ticketCount)
                {
                    string confirm;
                    cout << "Seat " << selectedSeats.back() << " selected. Continue selecting? (y/n): ";
                    cin >> confirm;
                    toUpperCase(confirm);

                    
                    while (confirm != "Y" && confirm != "N")
                    {
                        cout << "Invalid input. Please enter 'Y' or 'N': ";
                        cin >> confirm;
                        toUpperCase(confirm);
                    }

                    if (confirm == "N")
                    {
                        cout << "Cancelling seat selection.\n";
                        return {}; 
                    }
                }
            }
        }
        return selectedSeats;
    }

    void bookSeats(int roomIndex, const vector<string> &seats, const string &bookingDate)
    {
        for (const auto &seat : seats)
        {
            roomBookedSeats[roomIndex][bookingDate][seat] = true;
        }
    }
    void manageSeatLayout() override
    {
        int roomIndex;
        cout << "Enter room number (1-4) to manage seat layout: ";
        getValidRoom("room number", roomIndex);

        // Get booking date
        string bookingDate = getCurrentDate();

        cout << "Do you want to check for the current date (" << bookingDate << ")? (Y for Yes, N for No): ";
        string userChoice;
        cin >> userChoice;

        toUpperCase(userChoice);

        while (userChoice != "Y" && userChoice != "y" && userChoice != "N" && userChoice != "n")
        {
            cout << "Invalid input. Please enter 'Y' for Yes or 'N' for No: ";
            cin >> userChoice;
        }

        if (userChoice == "N" || userChoice == "n")
        {
            int dayChoice = 0;
            cout << "Please select a number between 1 and 5 to book a date:\n";
            cout << "1. 1 day after today\n";
            cout << "2. 2 days after today\n";
            cout << "3. 3 days after today\n";
            cout << "4. 4 days after today\n";
            cout << "5. 5 days after today\n";
            cout << "Enter your choice (1-5) or press 'n' to exit: ";

            string userInput;
            cin >> userInput;

            if (userInput == "n" || userInput == "N")
            {
                cout << "Exiting date selection.\n";
                return;
            }

            while (dayChoice < 1 || dayChoice > 5)
            {
                if (userInput == "1" || userInput == "2" || userInput == "3" || userInput == "4" || userInput == "5")
                {
                    dayChoice = stoi(userInput);
                    break;
                }
                else
                {
                    cout << "Invalid input. Please enter a number between 1 and 5 or press 'n' to exit: ";
                    cin >> userInput;

                    if (userInput == "n" || userInput == "N")
                    {
                        cout << "Exiting date selection.\n";
                        this_thread::sleep_for(chrono::milliseconds(1000)); // Pause for 1 second
                        return;
                    }
                }
            }

            time_t now = time(0);
            tm *currentTime = localtime(&now);
            currentTime->tm_mday += dayChoice; // Add selected days to current date
            mktime(currentTime);

            char buffer[11]; // Buffer for date string (YYYY-MM-DD + null terminator)
            strftime(buffer, sizeof(buffer), "%Y-%m-%d", currentTime);

            bookingDate = string(buffer);
        }

        bool isManaging = true;

        while (isManaging)
        {
            viewRoomSeats(roomIndex, bookingDate);

            cout << "\n\nOptions:\n";
            cout << "1. Reserve a seat\n";
            cout << "2. Delete a reserved seat\n";
            cout << "3. Exit seat management\n";
            cout << "Enter your choice: ";
            string choice;
            cin >> choice;

            if (choice == "1")
            {
                string selectedSeat;
                cout << "Enter seat to reserve (e.g., A1, B2): ";
                cin >> selectedSeat;
                toUpperCase(selectedSeat);

                if (!isValidSeat(selectedSeat))
                {
                    cout << "Invalid seat format. Please use format like A1, B2.\n";
                    continue;
                }

                if (roomBookedSeats[roomIndex][bookingDate][selectedSeat])
                {
                    cout << RED << "Seat " << selectedSeat << " is already booked by a customer and cannot be reserved.\n"
                         << RESET;
                    this_thread::sleep_for(chrono::milliseconds(3000)); // Pause for 3 seconds
                    system("cls");
                    continue;
                }

                if (roomReservedSeats[roomIndex][bookingDate][selectedSeat])
                {
                    cout << RED << "Seat " << selectedSeat << " is already reserved by an admin.\n"
                         << RESET;
                    this_thread::sleep_for(chrono::milliseconds(3000)); // Pause for 3 seconds
                    system("cls");
                    continue;
                }

                roomReservedSeats[roomIndex][bookingDate][selectedSeat] = true;
                cout << GREEN << "Seat " << selectedSeat << " reserved successfully.\n"
                     << RESET;
                this_thread::sleep_for(chrono::milliseconds(3000)); // Pause for 3 seconds
                system("cls");
            }
            else if (choice == "2")
            {
                string selectedSeat;
                cout << "Enter seat to delete reservation (e.g., A1, B2): ";
                cin >> selectedSeat;
                toUpperCase(selectedSeat);

                if (!isValidSeat(selectedSeat))
                {
                    cout << RED << "Invalid seat format. Please use format like A1, B2.\n"
                         << RESET;
                    this_thread::sleep_for(chrono::milliseconds(3000)); // Pause for 3 seconds
                    system("cls");
                    continue;
                }

                if (roomBookedSeats[roomIndex][bookingDate][selectedSeat])
                {
                    cout << "Seat " << selectedSeat << " is booked by a customer and cannot be deleted.\n";
                    continue;
                }

                if (!roomReservedSeats[roomIndex][bookingDate][selectedSeat])
                {
                    cout << RED << "Seat " << selectedSeat << " is not reserved.\n"
                         << RESET;
                    this_thread::sleep_for(chrono::milliseconds(3000)); // Pause for 3 seconds
                    system("cls");
                    continue;
                }

                roomReservedSeats[roomIndex][bookingDate][selectedSeat] = false;
                cout << GREEN << "Reservation for seat " << selectedSeat << " has been deleted.\n"
                     << RESET;
                this_thread::sleep_for(chrono::milliseconds(3000)); // Pause for 3 seconds
                system("cls");
            }
            else if (choice == "3")
            {
                cout << "Exiting seat management.\n";
                isManaging = false;
            }
            else
            {
                cout << RED << "Invalid choice. Please select a valid option (1, 2, or 3).\n"
                     << RESET;
                this_thread::sleep_for(chrono::milliseconds(3000)); // Pause for 3 seconds
                system("cls");
            }
        }
    }
    void releaseSeats(int roomIndex, const vector<string> &seats, const string &bookingDate)
    {
        for (const auto &seat : seats)
        {
            // Check if the seat is currently booked for the specific date
            if (roomBookedSeats[roomIndex][bookingDate].find(seat) != roomBookedSeats[roomIndex][bookingDate].end() &&
                roomBookedSeats[roomIndex][bookingDate][seat])
            {
                // Mark the seat as available (released) for the specific date
                roomBookedSeats[roomIndex][bookingDate][seat] = false;
                cout << "Seat " << seat << " has been released for " << bookingDate << ".\n";
            }
            else
            {
                cout << "Seat " << seat << " was not booked or does not exist for " << bookingDate << ".\n";
            }
        }
    }
    bool isSeatAvailable(int roomIndex, const string &seat, const string &bookingDate)
    {
        return !roomBookedSeats[roomIndex][bookingDate][seat] &&
               !roomReservedSeats[roomIndex][bookingDate][seat];
    }
    bool checkAvailableSeat(int roomIndex, const vector<string> &seats, const string &bookingDate)
    {
        for (const string &seat : seats)
        {
            if (!isSeatAvailable(roomIndex, seat, bookingDate))
            {
                return false;
            }
        }
        return true;
    }

    // Fixed getDateAfterOffset function with proper date handling
    static string getDateAfterOffset(int dayOffset)
    {
        time_t now = time(0);
        tm futureTime = *localtime(&now);

        // Use mktime to handle month/year transitions properly
        futureTime.tm_mday += dayOffset;
        mktime(&futureTime);

        stringstream ss;
        ss << (futureTime.tm_year + 1900) << "-"
           << setfill('0') << setw(2) << (futureTime.tm_mon + 1) << "-"
           << setfill('0') << setw(2) << futureTime.tm_mday;

        return ss.str();
    }
};

map<int, map<string, map<string, bool>>> Seat::roomBookedSeats;
map<int, map<string, map<string, bool>>> Seat::roomReservedSeats;
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
        cout << "Amount: Php: " << amount << endl;
        cout << "Method: " << method << endl;
        cout << "Status: " << status << endl;
    }
};

// Initialize static members
Payment *Payment::instance = nullptr;
mutex Payment::instanceMutex;

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
        string paymentStatus;
    };
    vector<BookingInfo> userBookings;

public:
    void makeBooking() override
    {
        int roomChoice, ticketCount;

        cout << "Available Movies:\n";
        movies.viewMovies();

        if (movies.rooms.empty())
        {
            cout << "No movies available for booking.\n";
            return;
        }

        cout << "\nPlease enter the room number of your movie choice: ";
        getValidRoom("room number", roomChoice);

        if (movies.rooms[roomChoice].empty())
        {
            cout << "\nNo movies available in this room.\n";
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

        cout << "\nEnter number of tickets: ";
        getValidQuantity("ticket count", ticketCount);

        double totalPrice = selectedMovie.price * ticketCount;

        // Get booking date
        string bookingDate = getCurrentDate();

        cout << "Do you want to book for the current date (" << bookingDate << ")? (Y for Yes, N for No): ";
        string userChoice;
        cin >> userChoice;

        while (userChoice != "Y" && userChoice != "y" && userChoice != "N" && userChoice != "n")
        {
            cout << "Invalid input. Please enter 'Y' for Yes or 'N' for No: ";
            cin >> userChoice;
        }

        if (userChoice == "N" || userChoice == "n")
        {
            int dayChoice = 0;
            cout << "Please select a number between 1 and 5 to book a date:\n";
            cout << "1. 1 day after today\n";
            cout << "2. 2 days after today\n";
            cout << "3. 3 days after today\n";
            cout << "4. 4 days after today\n";
            cout << "5. 5 days after today\n";
            cout << "Enter your choice (1-5) or press 'n' to exit: ";

            string userInput;
            cin >> userInput;

            if (userInput == "n" || userInput == "N")
            {
                cout << "Exiting date selection.\n";
                return;
            }

            while (dayChoice < 1 || dayChoice > 5)
            {
                if (userInput == "1" || userInput == "2" || userInput == "3" || userInput == "4" || userInput == "5")
                {
                    dayChoice = stoi(userInput);
                    break;
                }
                else
                {
                    cout << "Invalid input. Please enter a number between 1 and 5 or press 'n' to exit: ";
                    cin >> userInput;

                    if (userInput == "n" || userInput == "N")
                    {
                        cout << "Exiting date selection.\n";
                        return;
                    }
                }
            }

            time_t now = time(0);
            tm *currentTime = localtime(&now);
            currentTime->tm_mday += dayChoice; // Add selected days to current date
            mktime(currentTime);

            char buffer[11]; // Buffer for date stringss
            strftime(buffer, sizeof(buffer), "%Y-%m-%d", currentTime);

            bookingDate = string(buffer);
        }

        // Select seats
        vector<string> selectedSeats = seatManager.selectSeats(roomChoice, ticketCount, bookingDate);

        if (selectedSeats.size() != ticketCount)
        {
            cout << "Seat selection canceled.\n";
            return;
        }

        cout << "Booking date is set to: " << bookingDate << endl;

        // Confirm booking
        string confirm;
        cout << "Confirm booking? (y/n): ";
        cin >> confirm;
        toUpperCase(confirm);

        if (confirm == "Y")
        {
            // Create a Ticket object
            Ticket ticket(selectedMovie.title, roomChoice, selectedSeats, totalPrice, bookingDate);

            // Print the ticket details
            ticket.printTicket();

            // Save the booking
            BookingInfo newBooking = {selectedMovie.title, roomChoice, selectedSeats, ticketCount, totalPrice, bookingDate, "Pending"};
            userBookings.push_back(newBooking);

            // Mark seats as booked
            seatManager.bookSeats(roomChoice, selectedSeats, bookingDate);

            cout << "Booking successfully saved!\n";
            // Set payment status to "Pending"
            Payment *payment = Payment::getInstance();
            payment->setStatus("Pending");

            payment->setAmount(totalPrice); // Set the amount once

            // Display payment status
            paymentStatus();
            confirmPayment();
        }
        else
        {
            cout << "Booking canceled.\n";
        }
    }

    void cancelBooking() override
    {
        if (userBookings.empty())
        {
            cout << "No bookings available to cancel.\n";
            return;
        }

        viewBooking();

        int bookingIndex;
        getValidQuantity("index", bookingIndex);

        if (bookingIndex < 1 || bookingIndex > userBookings.size())
        {
            cout << "Invalid booking selection.\n";
            return;
        }

        bookingIndex--;

        // Updated to pass the date to releaseSeats
        seatManager.releaseSeats(userBookings[bookingIndex].room,
                                 userBookings[bookingIndex].seats,
                                 userBookings[bookingIndex].date);
        cout << "Canceling booking for movie: " << userBookings[bookingIndex].movieTitle << "\n";
        double refundAmount = userBookings[bookingIndex].totalPrice;
        userBookings.erase(userBookings.begin() + bookingIndex);

        cout << "Booking canceled successfully.\n";
        cout << "PHP " << refundAmount << " has been refunded.\n";
    }

    void viewBooking() override
    {
        if (userBookings.empty())
        {
            cout << "You currently have no bookings.\n";
            return;
        }
        cout << "\n\nYour Bookings:\n";

        for (size_t i = 0; i < userBookings.size(); ++i)
        {
            const auto &booking = userBookings[i];

            cout << i + 1 << ". Movie: " << booking.movieTitle << "\n"
                 << "   Room: " << booking.room << "\n"
                 << "   Date: " << booking.date << "\n"
                 << "   Seats: ";

            for (const auto &seat : booking.seats)
            {
                cout << seat << " ";
            }

            cout << "\n   Total Price: Php" << booking.totalPrice << "\n"
                 << "   Payment Status: " << (booking.paymentStatus.empty() ? "Pending" : booking.paymentStatus) << "\n\n"; // Display "Pending" if empty
        }
    }

    void modifiedBooking() override
    {
        if (userBookings.empty())
        {
            cout << "You have no bookings to modify.\n";
            return;
        }

        viewBooking();
        int bookingIndex;
        getValidQuantity("index", bookingIndex);

        if (bookingIndex < 1 || bookingIndex > userBookings.size())
        {
            cout << "Invalid booking selection.\n";
            return;
        }

        bookingIndex--;
        BookingInfo &bookingToModify = userBookings[bookingIndex];

        bool isModified = true;
        while (isModified)
        {
            cout << "Select what you want to modify:\n";
            cout << "1. Seats\n";
            cout << "2. Ticket count\n";
            cout << "3. Date\n";
            cout << "4. Exit\n";
            string modificationChoice;
            cout << "Enter choice for modification: ";
            cin >> modificationChoice;

            if (modificationChoice == "1")
            {
                int newTicketCount = bookingToModify.ticketCount;

                seatManager.releaseSeats(bookingToModify.room,
                                         bookingToModify.seats,
                                         bookingToModify.date);

                vector<string> newSeats = seatManager.selectSeats(bookingToModify.room,
                                                                  newTicketCount,
                                                                  bookingToModify.date);
                if (newSeats.size() != newTicketCount)
                {
                    cout << "Seat selection canceled.\n";
                    // Rebook original seats since modification was canceled
                    seatManager.bookSeats(bookingToModify.room,
                                          bookingToModify.seats,
                                          bookingToModify.date);
                    return;
                }

                seatManager.bookSeats(bookingToModify.room, newSeats, bookingToModify.date);
                bookingToModify.seats = newSeats;

                cout << "Seats modified successfully.\n";
            }
            else if (modificationChoice == "2")
            {
                int newTicketCount;
                cout << "Enter new number of tickets: ";
                getValidQuantity("new ticket count", newTicketCount);

                int currentTotalPrice = bookingToModify.totalPrice;
                int newTotalPrice = newTicketCount * movies.rooms[bookingToModify.room][0].price;
                int priceDifference = newTotalPrice - currentTotalPrice;

                if (priceDifference > 0)
                {
                    cout << "The new ticket count will cost an additional $" << priceDifference << ".\n";
                    cout << "Are you willing to pay the additional amount? (Y for Yes, N for No): ";
                    string paymentChoice;
                    cin >> paymentChoice;

                    if (paymentChoice == "Y" || paymentChoice == "y")
                    {

                        seatManager.releaseSeats(bookingToModify.room, bookingToModify.seats, bookingToModify.date);

                        vector<string> newSeats = seatManager.selectSeats(bookingToModify.room, newTicketCount, bookingToModify.date);
                        if (newSeats.size() != newTicketCount)
                        {
                            cout << "Seat selection canceled.\n";
                            return;
                        }

                        seatManager.bookSeats(bookingToModify.room, newSeats, bookingToModify.date);
                        bookingToModify.ticketCount = newTicketCount;
                        bookingToModify.seats = newSeats;
                        bookingToModify.totalPrice = newTotalPrice;

                        cout << "Ticket count modified successfully. Payment of $" << priceDifference << " completed.\n";
                    }
                    else
                    {
                        cout << "Modification canceled due to payment rejection.\n";
                        continue;
                    }
                }
                else if (priceDifference < 0)
                {
                    int refundAmount = -priceDifference;
                    cout << "The new ticket count is cheaper by $" << refundAmount << ".\n";
                    cout << "You will be refunded this amount.\n";

                    seatManager.releaseSeats(bookingToModify.room, bookingToModify.seats, bookingToModify.date);

                    vector<string> newSeats = seatManager.selectSeats(bookingToModify.room, newTicketCount, bookingToModify.date);
                    if (newSeats.size() != newTicketCount)
                    {
                        cout << "Seat selection canceled.\n";
                        return;
                    }

                    seatManager.bookSeats(bookingToModify.room, newSeats, bookingToModify.date);
                    bookingToModify.ticketCount = newTicketCount;
                    bookingToModify.seats = newSeats;
                    bookingToModify.totalPrice = newTotalPrice;

                    cout << "Ticket count modified successfully. Refund of $" << refundAmount << " processed.\n";
                }
                else
                {
                    cout << "The new ticket count has no price difference. Proceeding with the modification.\n";

                    seatManager.releaseSeats(bookingToModify.room, bookingToModify.seats, bookingToModify.date);

                    vector<string> newSeats = seatManager.selectSeats(bookingToModify.room, newTicketCount, bookingToModify.date);
                    if (newSeats.size() != newTicketCount)
                    {
                        cout << "Seat selection canceled.\n";
                        return;
                    }

                    seatManager.bookSeats(bookingToModify.room, newSeats, bookingToModify.date);
                    bookingToModify.ticketCount = newTicketCount;
                    bookingToModify.seats = newSeats;
                }
            }
            else if (modificationChoice == "3")
            {
                string bookingDate;
                int dayChoice = 0;
                cout << "Please select a number between 1 and 5 to book a date:\n";
                cout << "1. 1 day after today\n";
                cout << "2. 2 days after today\n";
                cout << "3. 3 days after today\n";
                cout << "4. 4 days after today\n";
                cout << "5. 5 days after today\n";
                cout << "Enter your choice (1-5): ";

                string userInput;
                cin >> userInput;

                while (dayChoice < 1 || dayChoice > 5)
                {
                    if (userInput == "1" || userInput == "2" || userInput == "3" || userInput == "4" || userInput == "5")
                    {
                        dayChoice = stoi(userInput);
                        break;
                    }
                    else
                    {
                        cout << "Invalid input. Please enter a number between 1 and 5: ";
                        cin >> userInput;
                    }
                }

                // Calculate the selected date
                time_t now = time(0);
                tm *currentTime = localtime(&now);
                currentTime->tm_mday += dayChoice;
                mktime(currentTime);

                char buffer[11];
                strftime(buffer, sizeof(buffer), "%Y-%m-%d", currentTime);
                bookingDate = string(buffer);

                // Prompt user to enter new seats for the selected date
                vector<string> newSeats;
                cout << "Enter new seat selections for " << bookingDate << ": ";
                string seatInput;
                cin.ignore(); // Clear buffer
                getline(cin, seatInput);

                stringstream ss(seatInput);
                string seat;
                while (ss >> seat)
                {
                    newSeats.push_back(seat);
                }

                if (newSeats.empty())
                {
                    cout << "No seats selected. Canceling modification.\n";
                    return;
                }

                // Check seat availability for the new date
                int roomIndex = bookingToModify.room;
                Seat seatObj;
                bool seatsAvailable = seatObj.checkAvailableSeat(roomIndex, newSeats, bookingDate); // Use the correct function name here

                if (!seatsAvailable)
                {
                    cout << "Some or all of the selected seats are unavailable on " << bookingDate << ".\n";
                    cout << "Please try another date or other seats.\n";
                    return;
                }

                // Update booking with new date and seats
                bookingToModify.date = bookingDate;
                bookingToModify.seats = newSeats;
                cout << "Booking date and seats modified successfully.\n";
            }

            else if (modificationChoice == "4")
            {
                isModified = false;
                cout << "Modification canceled.\n";
            }
            else
            {
                cout << "Invalid choice. Try again.\n";
            }
        }
    }

    void paymentStatus() override
    {
        if (userBookings.empty())
        {
            cout << "No bookings found.\n";
            return;
        }
    }
    void confirmPayment() override
    {
        Payment *payment = Payment::getInstance();
        float totalPrice = userBookings.back().totalPrice; // Assuming we check for the last booking's total price
        bool paymentSuccess = false;

        // Input payment details
        string method;
        float amount;

        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        int paymentID = rand() % 90000 + 10000; // Generates a number between 10000 and 99999
        payment->setPaymentID(paymentID);

        cout << "Payment ID: " << paymentID << endl;

        string methodChoice;
        bool ispaying = true;
        while (ispaying) // Loop until a valid choice is made
        {
            cout << "Select Payment Method:\n";
            cout << "1 - Credit Card\n";
            cout << "2 - Gcash\n";
            cout << "3 - Cash\n";
            cout << "Enter choice (1-3): ";
            cin >> methodChoice;

            if (methodChoice == "1")
            {
                payment->setMethod("Credit Card");
                break; // Exit the loop
            }
            else if (methodChoice == "2")
            {
                payment->setMethod("Gcash");
                break; // Exit the loop
            }
            else if (methodChoice == "3")
            {
                payment->setMethod("Cash");
                break; // Exit the loop
            }
            else
            {
                cout << "Invalid choice. Please enter 1, 2, or 3.\n"; // Prompt user to try again
            }
        }

        while (!paymentSuccess) // Loop until payment is successfully processed
        {

            getValidFloat("amount", amount);

            // Check if the entered amount matches the total price
            if (amount == totalPrice)
            {
                // Process the payment
                if (payment->processPayment())
                {
                    cout << "Payment processed successfully.\n";
                    cout << "Booking confirmed.\n";
                    userBookings.back().paymentStatus = "Completed"; // Update the payment status
                    break;
                }
                else
                {
                    cout << "Payment failed due to an invalid method. Please try again.\n";
                }
            }
            else
            {
                cout << "Payment amount mismatch! Expected: Php " << totalPrice << ". Please try again.\n";
            }
        }

        // Display payment details
        payment->displayPaymentDetails();
    }
};

// Handles display logic
class Display : public Movies, public Seat, public Booking
{
public:
    void loginMenu()
    {
        system("cls");
        cout << BOLD << ORANGE << " -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - \n|" << RESET
             << BOLD << YELLOW << " WELCOME TO CIN3MA," << ITALICS << " Your Ultimate Movie Experience! " << RESET << BOLD << ORANGE << "|\n"
             << "| " << RESET << BOLD << ORANGE << "                                                   |\n"
             << "| " << RESET << CYAN << "1 - User" << BOLD << ORANGE << "                                           |\n"
             << "| " << RESET << MAGENTA << "2 - Admin" << BOLD << ORANGE << "                                          |\n"
             << "| " << RESET << RED << "3 - Exit" << BOLD << ORANGE << "                                           |\n"
             << " -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - \n\n"
             << RESET;
    }

    void userMenu()
    {
        system("cls");
        cout << BOLD << YELLOW << "WELCOME TO CIN3MA," << ITALICS << " Your Ultimate Movie Experience! \n"
             << RESET
             << CYAN << "1 - View Movies\n"
             << "2 - View Seats\n"
             << "3 - Make a Booking\n"
             << "4 - Cancel a Booking\n"
             << "5 - Change Booking\n"
             << "6 - View Booking\n"
             << RED << "0 - Exit to Login\n"
             << RESET;
    }

    void adminMenu()
    {
        system("cls");
        cout << YELLOW << "ADMIN DASHBOARD: Manage Showtimes, Tickets, & More \n"
             << MAGENTA << "1 - Manage Movies and Showtimes\n"
             << "2 - Manage Seat Layout\n"
             << RED << "0 - Exit to Login\n"
             << RESET;
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
                system("cls");
                userMenu();
                handleUserMenu();
            }
            else if (choice == "2")
            {
                system("cls");
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
                system("cls");
                viewMovies();
                returnMenu();
            }
            else if (choice == "2")
            {
                system("cls");
                viewSeats();
                returnMenu();
            }
            else if (choice == "3")
            {
                system("cls");
                makeBooking();
                returnMenu();
            }
            else if (choice == "4")
            {
                system("cls");
                cancelBooking();
                returnMenu();
            }
            else if (choice == "5")
            {
                system("cls");
                modifiedBooking();
                returnMenu();
            }
            else if (choice == "6")
            {
                system("cls");
                viewBooking();
                returnMenu();
            }
            else if (choice == "0")
            {
                cout << "Exiting User Menu...\n";
                this_thread::sleep_for(chrono::milliseconds(1000)); // Pause for 1 seconds
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
                system("cls");
                manageMovieAndShowtime();
            }
            else if (choice == "2")
            {
                system("cls");
                manageSeatLayout();
            }
            else if (choice == "0")
            {
                cout << "Exiting Admin Menu...\n";
                this_thread::sleep_for(chrono::milliseconds(1000)); // Pause for 1 second
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