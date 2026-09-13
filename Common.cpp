//Common.cpp includes Enums, constants, and helping func().
#include <bits/stdc++.h>

enum class SeatType { SILVER, GOLD, PLATINUM };
enum class SeatStatus { AVAILABLE, BOOKED };
enum class BookingStatus { CONFIRMED, CANCELLED };

// seat price consts
static constexpr double SILVER_PRICE   = 150.0;
static constexpr double GOLD_PRICE     = 250.0;
static constexpr double PLATINUM_PRICE = 400.0;
static constexpr double ZERO_AMOUNT    = 0.0;

// menu consts
static constexpr int MENU_LIST_MOVIES     = 1;
static constexpr int MENU_LIST_SHOWS      = 2;
static constexpr int MENU_VIEW_SEAT_LAYOUT = 3;
static constexpr int MENU_BOOK_SEATS      = 4;
static constexpr int MENU_CANCEL_BOOKING  = 5;
static constexpr int MENU_EXIT            = 6;
static constexpr int MIN_MENU_CHOICE      = 1;
static constexpr int MAX_MENU_CHOICE      = 6;
static constexpr int FIRST_DISPLAY_INDEX  = 1;

// payment related consts
static constexpr int PAYMENT_CHOICE_UPI  = 1;
static constexpr int PAYMENT_CHOICE_CARD = 2;
static constexpr int PAYMENT_CHOICE_CASH = 3;
static constexpr int MIN_PAYMENT_CHOICE  = 1;
static constexpr int MAX_PAYMENT_CHOICE  = 3;

// id realted consts
static constexpr int FIRST_SHOW_ID      = 101;
static constexpr int INITIAL_BOOKING_ID = 1001;

// cinema menu consts
static const std::string CINEMA_NAME     = "PVR Doon City Centre";
static constexpr int SCREEN_ONE          = 1;
static constexpr int SCREEN_TWO          = 2;
static constexpr int SEATS_PER_SECTION   = 4;
static const std::string SILVER_ROW      = "A";
static const std::string GOLD_ROW        = "B";
static const std::string PLATINUM_ROW    = "C";

//currency
static const std::string CURRENCY_SYMBOL = "Rs. ";

//pay fail consts
static const std::string FAILED_UPI_ID      = "fail@upi";
static const std::string FAILED_CARD_PREFIX  = "0000";

static constexpr int FIRST_MOVIE_INDEX  = 0;
static constexpr int SECOND_MOVIE_INDEX = 1;
static constexpr int THIRD_MOVIE_INDEX  = 2;

std::string toString(SeatType seatType)
{
    switch (seatType)
    {
        case SeatType::SILVER:   return "SILVER";
        case SeatType::GOLD:     return "GOLD";
        case SeatType::PLATINUM: return "PLATINUM";
    }
    return "UNKNOWN";
}

std::string toString(SeatStatus seatStatus)
{
    switch (seatStatus)
    {
        case SeatStatus::AVAILABLE: return "AVAILABLE";
        case SeatStatus::BOOKED:    return "BOOKED";
    }
    return "UNKNOWN";
}

std::string toString(BookingStatus bookingStatus)
{
    switch (bookingStatus)
    {
        case BookingStatus::CONFIRMED: return "CONFIRMED";
        case BookingStatus::CANCELLED: return "CANCELLED";
    }
    return "UNKNOWN";
}

//splits a line by whitespace into tokens
std::vector<std::string> splitLine(const std::string& line)
{
    std::vector<std::string> tokens;
    std::istringstream stream(line);
    std::string token;
    while (stream >> token)tokens.push_back(token);
    return tokens;
}

bool tryParseInt(const std::string& text, int& value)
{
    std::istringstream stream(text);
    stream >> value;
    if (stream.fail())
    {
        return false;
    }
    std::string remainder;
    stream >> remainder;
    return remainder.empty();
}

bool tryParseDouble(const std::string& text, double& value)
{
    std::istringstream stream(text);
    stream >> value;
    if (stream.fail())
    {
        return false;
    }
    //rejects trailing non-whitespace characters
    std::string remainder;
    stream >> remainder;
    return remainder.empty();
}

bool startsWith(const std::string& text, const std::string& prefix)
{
    if (prefix.size() > text.size())
    {
        return false;
    }
    return text.compare(0, prefix.size(), prefix) == 0;
}

bool isBlank(const std::string& text)
{
    for (char ch : text)
    {
        if (!std::isspace(static_cast<unsigned char>(ch)))
        {
            return false;
        }
    }
    return true;
}