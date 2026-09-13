//17_CinemaMenu.cpp: Menu-driven UI for the Movie Ticket Booking System.
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstdlib>

class CinemaMenu{
private:
    BookingService& bookingService;
    void displayMenu() const{
        std::cout << colorText("\n===== MOVIE TICKET BOOKING =====\n", TerminalColor::BOLD_CYAN);
        std::cout << colorText(std::string(CINEMA_NAME), TerminalColor::BOLD) << "\n";
        std::cout << MENU_LIST_MOVIES     << ". List Movies\n";
        std::cout << MENU_LIST_SHOWS      << ". List Shows for a Movie\n";
        std::cout << MENU_VIEW_SEAT_LAYOUT << ". View Seat Layout\n";
        std::cout << MENU_BOOK_SEATS      << ". Book Seats\n";
        std::cout << MENU_CANCEL_BOOKING  << ". Cancel Booking\n";
        std::cout << MENU_EXIT            << ". Exit\n";
    }

    void printInvalidNumber() const{
        std::cout << colorText("Please enter a valid number.\n", TerminalColor::RED);
    }

    void printOutOfRange(int minValue, int maxValue) const{
        std::cout << colorText("Please enter a value between " +
                  std::to_string(minValue) + " and " + std::to_string(maxValue) + ".\n", TerminalColor::YELLOW);
    }

    void printBlankInput() const{
        std::cout << colorText("Input cannot be blank. Please try again.\n", TerminalColor::RED);
    }

    void printNoSeatsEntered() const{
        std::cout << colorText("No seats entered. Booking aborted.\n", TerminalColor::YELLOW);
    }

    void printFarewell() const{
        std::cout << colorText("\nThank you for visiting " + std::string(CINEMA_NAME) + ". Goodbye!\n", TerminalColor::GREEN);
    }
    std::string readLine(const std::string& prompt) const{
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)){
            std::cout << "\nInput stream ended. Exiting gracefully.\n";
            std::exit(EXIT_SUCCESS);
        }
        return line;
    }
    int readInteger(const std::string& prompt) const{
        while (true){
            std::string line = readLine(prompt);
            int value = 0;
            if (tryParseInt(line, value)){
                return value;
            }
            printInvalidNumber();
        }
    }

    int readIntegerInRange(const std::string& prompt,
                           int minValue, int maxValue) const{
        while (true){
            int value = readInteger(prompt);
            bool inRange = (value >= minValue && value <= maxValue);
            if (inRange){
                return value;
            }
            printOutOfRange(minValue, maxValue);
        }
    }

    double readDouble(const std::string& prompt) const{
        while (true){
            std::string line = readLine(prompt);
            double value = 0.0;
            if (tryParseDouble(line, value))
            {
                return value;
            }
            printInvalidNumber();
        }
    }

    double readNonNegativeDouble(const std::string& prompt) const{
        while (true){
            double value = readDouble(prompt);
            if (value >= ZERO_AMOUNT){
                return value;
            }
            std::cout << colorText("Value cannot be negative. Please try again.\n", TerminalColor::RED);
        }
    }

    std::string readNonEmptyLine(const std::string& prompt) const{
        while (true){
            std::string line = readLine(prompt);
            if (!isBlank(line)){
                return line;
            }
            printBlankInput();
        }
    }

    int readMenuChoice() const{
        return readIntegerInRange("Enter choice: ", MIN_MENU_CHOICE, MAX_MENU_CHOICE);
    }

    int readMovieChoice() const{
        int maxChoice = bookingService.getMovieCount();
        return readIntegerInRange("Enter movie number: ",FIRST_DISPLAY_INDEX, maxChoice);
    }

    int readShowId() const{
        return readInteger("Enter Show ID: ");
    }

    int readBookingId() const{
        return readInteger("Enter Booking ID: ");
    }

    std::vector<std::string> readSeatNumbers() const{
        std::string line = readLine(
            "Enter seat numbers (space-separated, e.g. A1 B2): ");
        std::vector<std::string> seats = splitLine(line);
        return seats;
    }

    Customer readCustomer() const{
        std::string name = readNonEmptyLine("Enter your name: ");
        std::string phone = readNonEmptyLine("Enter your phone: ");
        return Customer(name, phone);
    }

    int readPaymentChoice() const{
        std::cout << colorText("\n--- Payment Method ---\n", TerminalColor::BOLD_CYAN);
        std::cout << PAYMENT_CHOICE_UPI  << ". UPI\n";
        std::cout << PAYMENT_CHOICE_CARD << ". Card\n";
        std::cout << PAYMENT_CHOICE_CASH << ". Cash\n";
        return readIntegerInRange(
            "Choose payment method: ",
            MIN_PAYMENT_CHOICE, MAX_PAYMENT_CHOICE);
    }

    std::string readUpiId() const{
        return readNonEmptyLine("Enter UPI ID: ");
    }

    std::string readCardNumber() const{
        return readNonEmptyLine("Enter Card Number: ");
    }

    double readCashReceived() const{
        return readNonNegativeDouble("Enter cash amount: ");
    }

    //Factory  pattern used for seperate object creation
    //CinemaMenu creates the concrete payment as BookingService.cpp does not.
    std::unique_ptr<Payment> createPayment() const{
        int paymentChoice = readPaymentChoice();
        switch (paymentChoice){
            case PAYMENT_CHOICE_UPI:{
                std::string upiId = readUpiId();
                return std::make_unique<UpiPayment>(upiId);
            }
            case PAYMENT_CHOICE_CARD:
            {
                std::string cardNumber = readCardNumber();
                return std::make_unique<CardPayment>(cardNumber);
            }
            case PAYMENT_CHOICE_CASH:
            {
                double cash = readCashReceived();
                return std::make_unique<CashPayment>(cash);
            }
            default:
                return nullptr;
        }
    }

    void handleListMovies(){
        bookingService.listMovies();
    }

    bool chooseMovieAndListShows(){
        bool hasMovies = bookingService.listMovies();
        if (!hasMovies){
            return false;
        }
        int movieChoice = readMovieChoice();
        return bookingService.listShowsForMovie(movieChoice);
    }

    void handleListShows(){
        chooseMovieAndListShows();
    }

    void handleSeatLayout(){
        int showId = readShowId();
        bookingService.displaySeatLayout(showId);
    }

    void handleBookSeats(){
        bool showsAvailable = chooseMovieAndListShows();
        if (!showsAvailable){
            return;
        }

        int showId = readShowId();
        bool layoutDisplayed = bookingService.displaySeatLayout(showId);
        if (!layoutDisplayed){
            return;
        }

        std::vector<std::string> seatNumbers = readSeatNumbers();
        if (seatNumbers.empty()){
            printNoSeatsEntered();
            return;
        }

        Customer customer = readCustomer();

        std::unique_ptr<Payment> payment = createPayment();
        if (payment == nullptr){
            return;
        }

        BookingService::Request request{
            showId,
            seatNumbers,
            customer
        };
        bookingService.bookSeats(request, *payment);
    }

    void handleCancelBooking(){
        int bookingId = readBookingId();
        bookingService.cancelBooking(bookingId);
    }
    bool handleMenuChoice(int menuChoice){
        switch (menuChoice){
            case MENU_LIST_MOVIES:     handleListMovies();     break;
            case MENU_LIST_SHOWS:      handleListShows();      break;
            case MENU_VIEW_SEAT_LAYOUT: handleSeatLayout();    break;
            case MENU_BOOK_SEATS:      handleBookSeats();      break;
            case MENU_CANCEL_BOOKING:  handleCancelBooking();  break;
            case MENU_EXIT:            return false;
            default:                   break;
        }
        return true;
    }

public:
    explicit CinemaMenu(BookingService& bookingService)
        : bookingService(bookingService)
    {}
    void run(){
        bool running = true;
        while (running){
            displayMenu();
            int menuChoice = readMenuChoice();
            running = handleMenuChoice(menuChoice);
        }
        printFarewell();
    }
};