//05_Show.cpp represents a screening of a movie on a screen at a given time.
//Composition: Show owns ShowSeat objects via unique_ptr
//Aggregation: Uses Movie and Screen via raw pointers but also doesn't own them.
#include <bits/stdc++.h>

class Show{
public:
    //nested parameter struct to keep the constructor clean
    struct Setup{
        Movie* movie;
        Screen* screen;
        std::string startTime;
    };

private:
    //static member for auto-generating show id.
    static int nextShowId;
    int showId;
    //aggregation: show references Movie and Screen but does not own them.
    Movie* movie;
    Screen* screen;
    std::string startTime;
    //composition: show owns its ShowSeat objects.
    std::vector<std::unique_ptr<ShowSeat>> showSeats;

    void createShowSeats(){
        const auto& seats = screen->getSeats();
        for (const auto& seat : seats){
            showSeats.push_back(std::make_unique<ShowSeat>(seat.get()));
        }
    }
    void printShowHeader() const{
        std::cout << "\n--- Seat Layout for Show " << showId
                  << " | " << movie->getTitle()
                  << " | Screen " << screen->getScreenNumber()
                  << " | " << startTime << " ---\n";
        std::cout << std::left
                  << std::setw(10) << "Seat"
                  << std::setw(12) << "Type"
                  << std::setw(12) << "Status"
                  << "\n";
        std::cout << std::string(34, '-') << "\n";
    }
    void printSeatLine(const ShowSeat& showSeat) const{
        std::cout << std::left
                  << std::setw(10) << showSeat.getSeatNumber()
                  << std::setw(12) << toString(showSeat.getSeatType())
                  << std::setw(12) << (showSeat.isAvailable()
                       ? toString(SeatStatus::AVAILABLE)
                       : toString(SeatStatus::BOOKED))
                  << "\n";
    }
public:
    explicit Show(const Setup& setup)
        : showId(nextShowId++),
          movie(setup.movie),
          screen(setup.screen),
          startTime(setup.startTime)
    {
        createShowSeats();
    }

    int getShowId() const{
        return showId;
    }

    //aggregation: returns non-owning pointer to the Movie.
    Movie* getMovie() const{
        return movie;
    }

    //aggregation: returns non-owning pointer to the Screen.
    Screen* getScreen() const{
        return screen;
    }

    std::string getStartTime() const{
        return startTime;
    }

    ShowSeat* findShowSeat(const std::string& seatNumber) const{
        for (const auto& showSeat : showSeats){
            if (showSeat->getSeatNumber() == seatNumber){
                return showSeat.get();
            }
        }
        return nullptr;
    }

    
    void displaySeatLayout() const
    {
        printShowHeader();
        for (const auto& showSeat : showSeats)
        {
            printSeatLine(*showSeat); //displaySeatLayout only displays; it does not modify any seat.
        }
        std::cout << std::endl;
    }
};

//static member usage
int Show::nextShowId = FIRST_SHOW_ID;
