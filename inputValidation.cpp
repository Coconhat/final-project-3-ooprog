#include <iostream>
#include <ctime>
#include <chrono>
#include <regex>
using namespace std;

// Function to get a valid date from the user in YYYY-MM-DD format
string getCurrentDate()
{
    time_t now = time(0);
    tm *currentTime = localtime(&now);

    stringstream ss;
    ss << (currentTime->tm_year + 1900) << "-"
       << (currentTime->tm_mon + 1) << "-"
       << currentTime->tm_mday;

    return ss.str();
}

// Function to check if the date is valid
bool isValidDate(const string &date)
{
    regex datePattern(R"(\d{4}-\d{2}-\d{2})");
    if (!regex_match(date, datePattern))
    {
        cout << "Invalid date format. Please use YYYY-MM-DD.\n";
        return false;
    }
    return true;
}

// Function to check if the date is within the 15-day range
bool isWithin15Days(const string &bookingDate)
{
    time_t now = time(0);
    tm *currentTime = localtime(&now);

    int bookingYear, bookingMonth, bookingDay;
    sscanf(bookingDate.c_str(), "%d-%d-%d", &bookingYear, &bookingMonth, &bookingDay);

    tm bookingTime = {};
    bookingTime.tm_year = bookingYear - 1900;
    bookingTime.tm_mon = bookingMonth - 1;
    bookingTime.tm_mday = bookingDay;

    time_t bookingTime_t = mktime(&bookingTime);
    double diffInSeconds = difftime(bookingTime_t, now);
    double diffInDays = diffInSeconds / (60 * 60 * 24);

    if (diffInDays < 0)
    {
        cout << "The date cannot be in the past.\n";
        return false;
    }
    else if (diffInDays > 15)
    {
        cout << "You can only book a movie within 15 days in advance.\n";
        return false;
    }
    return true;
}

// Function to check if a string represents a valid number
// This function ensures the input is a valid numeric value with at most one decimal point.
bool isValidNum(const string &input)
{
    bool Decimal = false;

    // Loop through each character in the input string
    for (char ch : input)
    {
        if (ch == '.')
        {
            // If a decimal point is already encountered, return false to prevent multiple decimals
            if (Decimal)
            {
                return false;
            }
            Decimal = true;
        }
        else if (!isdigit(ch)) // If a character is not a digit and not a decimal point, return false
        {
            return false;
        }
    }
    // Return true if input is not empty and passes all checks
    return !input.empty();
}

// Function to get a valid price from the user
// Prompts the user until a valid positive price is entered.
void getValidPrice(const string &label, double &amount, bool allowBlank = false)
{
    string input;
    while (true)
    {
        cout << "\nEnter " << label << ": "; // Display the prompt
        cin >> input;

        if (allowBlank && input.empty())
        {

            return;
        }

        // Check if the input is a valid number
        if (isValidNum(input))
        {
            amount = stod(input); // Convert the valid input to a double
            if (amount <= 0)      // Check if the amount is greater than zero
            {
                cout << "Error: input must be greater than zero." << endl;
            }
            else
            {
                break; // Exit the loop if valid input is given
            }
        }
        else
        {
            // Display error if the input is not a valid numeric value
            cout << "Error: Please enter a valid positive numeric amount." << endl;
        }
    }
}

// Function to get a valid quantity from the user
// Prompts the user until a valid positive whole number (integer) is entered.
void getValidQuantity(const string &label, int &amount)
{
    string input;
    while (true)
    {
        cout << "\nEnter " << label << ": "; // Display the prompt
        cin >> input;

        // Check if the input contains a decimal point or non-numeric characters
        bool isValid = true;
        for (char ch : input)
        {
            if (!isdigit(ch) && ch != '.')
            {
                isValid = false; // Set isValid to false if any character is non-numeric or non-digit
                break;
            }
        }

        // If the input is invalid or contains a decimal point, prompt the user again
        if (!isValid || (input.find('.') != string::npos))
        {
            cout << "Error: Please enter a valid positive whole number." << endl;
            continue;
        }

        // Convert the input to an integer
        amount = stoi(input);

        // Ensure the amount is greater than zero
        if (amount < 0)
        {
            cout << "Error: input must be greater than zero." << endl;
        }
        else
        {
            break; // Exit the loop if valid input is given
        }
    }
}

void getValidRoom(const string &label, int &given)
{
    string input;
    while (true)
    {
        cout << "\nEnter " << label << ": ";
        cin >> input;

        // Check if the input contains non-numeric characters
        bool isValid = true;
        for (char ch : input)
        {
            if (!isdigit(ch)) // Check if the input contains any non-digit characters
            {
                isValid = false;
                break;
            }
        }

        // If the input is invalid or out of range (0-24), prompt the user again
        if (!isValid || input.empty())
        {
            cout << "Error: Please enter a valid positive integer." << endl;
            continue;
        }

        // Convert the input to an integer
        given = stoi(input);

        // Ensure the hour is between 0 and 24
        if (given < 1 || given > 4)
        {
            cout << "Error: " << label << " must be between " << 1 << " and " << 4 << endl;
        }
        else
        {
            break; // Exit the loop if valid input is given
        }
    }
}

void getValidInput(const string &label, int &given, int min, int max, bool allowBlank = false)
{
    string input;
    while (true)
    {
        cout << "Enter " << label << (allowBlank ? " (leave blank to keep current): " : ": ");
        getline(cin, input);

        if (allowBlank && input.empty())
        {

            return;
        }

        // Check if the input contains non-numeric characters
        bool isValid = true;
        for (char ch : input)
        {
            if (!isdigit(ch)) // Check if the input contains any non-digit characters
            {
                isValid = false;
                break;
            }
        }

        // If the input is invalid, prompt the user again
        if (!isValid || input.empty())
        {
            cout << "Error: Please enter a valid positive integer for " << label << "." << endl;
            continue;
        }

        // Convert the input to an integer
        given = stoi(input);

        // Ensure the input is within range
        if (given < min || given > max)
        {
            cout << "Error: " << label << " must be between " << min << " and " << max << "." << endl;
        }
        else
        {
            break; // Exit the loop if valid input is given
        }
    }
}

// Function to convert a string to uppercase
// Loops through each character in the string and converts it to uppercase.
void toUpperCase(string &str)
{
    for (char &c : str)
    {
        c = toupper(c); // Convert each character to uppercase
    }
}
