// main.cpp is the entry point for my Movie Ticket Booking System.
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include "Common.cpp"
#include "01_Movie.cpp"
#include "02_Seat.cpp"
#include "03_Screen.cpp"
#include "04_Cinema.cpp"
#include "05_Show.cpp"
#include "06_ShowSeat.cpp"
#include "07_Customer.cpp"
#include "08_Booking.cpp"
#include "09_Payment.cpp"
#include "11_PriceCalculator.cpp"
#include "12_TicketPrinter.cpp"
#include "13_BookingService.cpp"
#include "14_UpiPayment.cpp"
#include "15_CardPayment.cpp"
#include "16_CashPayment.cpp"
#include "17_CinemaMenu.cpp"


static void addSeatsToScreen(Screen& screen,const std::string& row,SeatType seatType){
    for (int i = 1; i <= SEATS_PER_SECTION; i++){
        std::string seatNumber = row + std::to_string(i);
        screen.addSeat(std::make_unique<Seat>(seatNumber, seatType));
    }
}
static void setupScreen(Screen& screen){
    addSeatsToScreen(screen, SILVER_ROW,   SeatType::SILVER);
    addSeatsToScreen(screen, GOLD_ROW,     SeatType::GOLD);
    addSeatsToScreen(screen, PLATINUM_ROW, SeatType::PLATINUM);
}

static std::unique_ptr<Cinema> createCinema(){
    auto cinema = std::make_unique<Cinema>(CINEMA_NAME);
    auto screen1 = std::make_unique<Screen>(SCREEN_ONE);
    setupScreen(*screen1);
    cinema->addScreen(std::move(screen1));

    auto screen2 = std::make_unique<Screen>(SCREEN_TWO);
    setupScreen(*screen2);
    cinema->addScreen(std::move(screen2));

    return cinema;
}

struct MovieData{
    std::string title;
    std::string language;
    int durationMinutes;
};

static std::vector<std::unique_ptr<Movie>> createMovies(){
    std::vector<MovieData> data = {
        {"Inception",    "English", 148},
        {"Interstellar", "English", 169},
        {"RRR",          "Telugu",  187}
    };
    std::vector<std::unique_ptr<Movie>> movies;
    for (const auto& entry : data){
        movies.push_back(std::make_unique<Movie>(
            entry.title, entry.language, entry.durationMinutes));
    }
    return movies;
}

struct ShowConfig{
    int movieIndex;
    int screenNumber;
    std::string startTime;
};

static std::vector<std::unique_ptr<Show>> createShows(const std::vector<std::unique_ptr<Movie>>& movies,Cinema& cinema){
    std::vector<ShowConfig> configs = {
        {FIRST_MOVIE_INDEX,  SCREEN_ONE, "10:00"},
        {FIRST_MOVIE_INDEX,  SCREEN_TWO, "13:30"},
        {SECOND_MOVIE_INDEX, SCREEN_ONE, "17:00"},
        {THIRD_MOVIE_INDEX,  SCREEN_TWO, "20:30"}
    };

    std::vector<std::unique_ptr<Show>> shows;
    for (const auto& config : configs){
        Show::Setup setup{
            movies[static_cast<std::size_t>(config.movieIndex)].get(),
            cinema.findScreen(config.screenNumber),
            config.startTime
        };
        shows.push_back(std::make_unique<Show>(setup));
    }
    return shows;
}

static void registerData(BookingService& service,const std::vector<std::unique_ptr<Movie>>& movies,const std::vector<std::unique_ptr<Show>>& shows){
    for (const auto& movie : movies){
        service.addMovie(movie.get());
    }
    for (const auto& show : shows){
        service.addShow(show.get());
    }
}


int main(){
    auto cinema = createCinema();
    auto movies = createMovies();
    auto shows  = createShows(movies, *cinema);

    BookingService bookingService;
    registerData(bookingService, movies, shows);

    CinemaMenu menu(bookingService);
    menu.run();

    return 0;
}
