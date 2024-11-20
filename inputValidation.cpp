#include <iostream>

using namespace std;

bool isValidNum(const string &input)
{
    bool Decimal = false;

    for (char ch : input)
    {
        if (ch == '.')
        {
            if (Decimal)
            {
                return false;
            }
            Decimal = true;
        }
        else if (!isdigit(ch))
        {
            return false;
        }
    }
    return !input.empty();
}

// Function to get a valid price from the user
void getValidPrice(const string &label, double &amount)
{
    string input;
    while (true)
    {
        cout << "\nEnter " << label << ": ";
        cin >> input;

        if (isValidNum(input))
        {
            amount = stod(input);
            if (amount <= 0)
            {
                cout << "Error: input must be greater than zero." << endl;
            }
            else
            {
                break;
            }
        }
        else
        {
            cout << "Error: Please enter a valid positive numeric amount." << endl;
        }
    }
}

// Function to get a valid quantity from the user
void getValidQuantity(const string &label, int &amount)
{
    string input;
    while (true)
    {
        cout << "\nEnter " << label << ": ";
        cin >> input;

        // Check if the input contains a decimal point or is non-numeric
        bool isValid = true;
        for (char ch : input)
        {
            if (!isdigit(ch) && ch != '.')
            {
                isValid = false;
                break;
            }
        }

        // If the input is invalid, prompt again
        if (!isValid || (input.find('.') != string::npos))
        {
            cout << "Error: Please enter a valid positive whole number." << endl;
            continue;
        }

        // Convert input to integer
        amount = stoi(input);

        if (amount < 0)
        {
            cout << "Error: input must be greater than zero." << endl;
        }
        else
        {
            break;
        }
    }
}
void toUpperCase(string &str)
{
    for (char &c : str)
    {
        c = toupper(c);
    }
}