//13_BookingService.cpp: Facade pattern that orchestrates booking operations.
//Facade Pattern: CinemaMenu uses BookingService as the single interface for all booking-related operations (listing, booking, cancelling).

// SOLID Dependency Inversion Principle (DIP):
// BookingService receives Payment& and does not create concrete payment objects (UpiPayment, CardPayment, CashPayment).
//
// Association: Customer interacts with BookingService through the booking request (BookingService::Request).

#include <iostream>
#include <vector>
#include <string>
#include <memory>

class BookingService{
public:
    //nested parameter struct for booking requests.
    struct Request{
        int showId;
        std::vector<std::string> seatNumbers;
        Customer customer;
    };
private:
    std::vector<Movie*> movies;
    std::vector<Show*> shows;
    std::vector<std::unique_ptr<Booking>> bookings;
    TicketPrinter ticketPrinter;

    Movie* findMovieByIndex(int movieChoice) const{
        int index = movieChoice - FIRST_DISPLAY_INDEX;
        int movieCount = static_cast<int>(movies.size());
        bool isOutOfRange = (index < 0 || index >= movieCount);
        if (isOutOfRange){
            return nullptr;
        }
        return movies[static_cast<std::size_t>(index)];
    }

    Show* findShow(int showId) const{
        for (auto* show : shows){
            if (show->getShowId() == showId){
                return show;
            }
        }
        return nullptr;
    }

    Booking* findBooking(int bookingId){
        for (const auto& booking : bookings){
            if (booking->getBookingId() == bookingId){
                return booking.get();
            }
        }
        return nullptr;
    }

    bool isDuplicateSeat(const std::vector<ShowSeat*>& selectedSeats,
                         const ShowSeat& seat) const{
        for (const auto* selected : selectedSeats){
            if (selected->getSeatNumber() == seat.getSeatNumber()){
                return true;
            }
        }
        return false;
    }
    bool tryAddSeat(Show& show,
                    const std::string& seatNumber,
                    std::vector<ShowSeat*>& selectedSeats){
        ShowSeat* showSeat = show.findShowSeat(seatNumber);
        if (showSeat == nullptr){
            printSeatNotFound(seatNumber);
            return false;
        }
        if (showSeat->isBooked()){
            printSeatAlreadyBooked(seatNumber);
            return false;
        }
        if (isDuplicateSeat(selectedSeats, *showSeat)){
            printDuplicateSeat(seatNumber);
            return false;
        }
        selectedSeats.push_back(showSeat);
        return true;
    }

    bool collectAvailableSeats(Show& show,
                               const std::vector<std::string>& seatNumbers,
                               std::vector<ShowSeat*>& selectedSeats){
        for (const auto& seatNumber : seatNumbers){
            bool added = tryAddSeat(show, seatNumber, selectedSeats);
            if (!added){
                return false;
            }
        }
        return true;
    }
    //seat state management
    void holdSeats(std::vector<ShowSeat*>& seats){
        for (auto* seat : seats){
            seat->book();
        }
    }

    void releaseSeats(const std::vector<ShowSeat*>& seats){
        for (auto* seat : seats){
            seat->release();
        }
    }
    //payment processing...
    bool processPayment(std::vector<ShowSeat*>& selectedSeats,
                        double totalAmount,
                        Payment& payment)
    {
        //runtime polymorphism: payment.pay() dispatches to the
        //concrete strategy (UpiPayment, CardPayment, or CashPayment).
        //SOLID LSP: Any Payment subclass works here via Payment&.
        bool paymentSucceeded = payment.pay(totalAmount);
        if (!paymentSucceeded){
            releaseSeats(selectedSeats);
            printPaymentFailed();
            return false;
        }
        return true;
    }
    //booking creation
    void addConfirmedBooking(const Booking::Details& details){
        auto booking = std::make_unique<Booking>(details);
        ticketPrinter.print(*booking);
        bookings.push_back(std::move(booking));
    }
    void printNoMovies() const{
        std::cout << colorText("No movies are currently playing.\n", TerminalColor::YELLOW);
    }
    void printNoShowsForMovie(const std::string& movieTitle) const{
        std::cout << colorText("No shows found for \"" + movieTitle + "\".\n", TerminalColor::YELLOW);
    }
    void printMovieLine(int displayIndex, const Movie& movie) const{
        std::cout << displayIndex << ". "
                  << colorText(movie.getTitle(), TerminalColor::BOLD)
                  << " (" << movie.getLanguage()
                  << ", " << movie.getDurationMinutes() << " min)\n";
    }

    void printShowLine(const Show& show) const{
        std::cout << "  Show ID: " << colorText(std::to_string(show.getShowId()), TerminalColor::CYAN)
                  << " | Screen: " << show.getScreen()->getScreenNumber()
                  << " | Time: " << show.getStartTime() << "\n";
    }
    void printShowNotFound(int showId) const{
        std::cout << colorText("Show with ID " + std::to_string(showId) + " not found.\n", TerminalColor::RED);
    }

    void printSeatNotFound(const std::string& seatNumber) const{
        std::cout << colorText("Seat \"" + seatNumber + "\" does not exist in this show.\n", TerminalColor::RED);
    }

    void printSeatAlreadyBooked(const std::string& seatNumber) const{
        std::cout << colorText("Sorry, seat \"" + seatNumber + "\" is already booked.\n", TerminalColor::RED);
    }

    void printDuplicateSeat(const std::string& seatNumber) const{
        std::cout << colorText("Seat \"" + seatNumber + "\" is entered more than once. "
                  "Please avoid duplicates.\n", TerminalColor::YELLOW);
    }

    void printPaymentFailed() const{
        std::cout << colorText("Booking not confirmed because payment failed. "
                  "Seats have been released.\n", TerminalColor::RED);
    }

    void printBookingNotFound(int bookingId) const{
        std::cout << colorText("Booking with ID " + std::to_string(bookingId) + " not found.\n", TerminalColor::RED);
    }

    void printBookingAlreadyCancelled(int bookingId) const{
        std::cout << colorText("Booking " + std::to_string(bookingId) + " is already cancelled.\n", TerminalColor::YELLOW);
    }

    void printCancellationSuccessful(int bookingId) const{
        std::cout << colorText("Booking " + std::to_string(bookingId) +
                  " has been cancelled successfully. Seats are now available.\n", TerminalColor::GREEN);
    }

    void printNoSeatsSelected() const{
        std::cout << colorText("No seat numbers provided. Booking aborted.\n", TerminalColor::RED);
    }

public:
    void addMovie(Movie* movie){
        movies.push_back(movie);
    }

    void addShow(Show* show){
        shows.push_back(show);
    }

    int getMovieCount() const{
        return static_cast<int>(movies.size());
    }

    bool listMovies() const{
        if (movies.empty()){
            printNoMovies();
            return false;
        }
        std::cout << colorText("\n--- Now Playing ---\n", TerminalColor::BOLD_CYAN);
        int displayIndex = FIRST_DISPLAY_INDEX;
        for (const auto* movie : movies)
        {
            printMovieLine(displayIndex, *movie);
            ++displayIndex;
        }
        std::cout << "\n";
        return true;
    }

    bool listShowsForMovie(int movieChoice) const{
        const Movie* movie = findMovieByIndex(movieChoice);
        if (movie == nullptr){
            std::cout << colorText("Invalid movie choice.\n", TerminalColor::RED);
            return false;
        }
        bool foundAny = false;
        std::cout << colorText("\nShows for \"" + movie->getTitle() + "\":\n", TerminalColor::BOLD);
        for (const auto* show : shows){
            if (show->getMovie() == movie)
            {
                printShowLine(*show);
                foundAny = true;
            }
        }
        if (!foundAny){
            printNoShowsForMovie(movie->getTitle());
            return false;
        }
        std::cout << "\n";
        return true;
    }

    bool displaySeatLayout(int showId) const{
        const Show* show = findShow(showId);
        if (show == nullptr){
            printShowNotFound(showId);
            return false;
        }
        show->displaySeatLayout();
        return true;
    }
    //SOLID DIP: BookingService uses Payment& (abstraction), not concrete types.
    bool bookSeats(const Request& request, Payment& payment){
        Show* show = findShow(request.showId);
        if (show == nullptr){
            printShowNotFound(request.showId);
            return false;
        }
        if (request.seatNumbers.empty()){
            printNoSeatsSelected();
            return false;
        }
        std::vector<ShowSeat*> selectedSeats;
        bool allSeatsValid = collectAvailableSeats(
            *show, request.seatNumbers, selectedSeats);
        if (!allSeatsValid){
            return false;
        }

        double totalAmount = PriceCalculator::calculate(selectedSeats);
        holdSeats(selectedSeats);

        bool paid = processPayment(selectedSeats, totalAmount, payment);
        if (!paid)return false;

        Booking::Details details{
            show,
            request.customer,
            selectedSeats,
            totalAmount,
            payment.getMethodName()
        };
        addConfirmedBooking(details);
        return true;
    }

    bool cancelBooking(int bookingId){
        Booking* booking = findBooking(bookingId);
        if (booking == nullptr){
            printBookingNotFound(bookingId);
            return false;
        }
        if (!booking->isConfirmed()){
            printBookingAlreadyCancelled(bookingId);
            return false;
        }
        releaseSeats(booking->getSeats());
        booking->markCancelled();
        printCancellationSuccessful(bookingId);
        return true;
    }
};