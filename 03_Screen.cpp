//03_Screen.cpp represents a screen in the cinema.
// Composition is used here as screen owns seat objects via cpp's unique_ptr.
#include <string>
#include <vector>
#include <memory>

class Screen{
private:
    int screenNumber;
    //composition: screen owns its seat objects and when a screen is destroyed its seats are destroyed too.
    std::vector<std::unique_ptr<Seat>> seats;

public:
    explicit Screen(int screenNumber): screenNumber(screenNumber){}
    int getScreenNumber() const{
        return this->screenNumber;
    }
    void addSeat(std::unique_ptr<Seat> seat){
        seats.push_back(std::move(seat));
    }
    const std::vector<std::unique_ptr<Seat>>& getSeats() const{
        return seats;
    }
    Seat* findSeat(const std::string& seatNumber) const{
        for (const auto& seat : seats){
            if (seat->getSeatNumber() == seatNumber){
                return seat.get();
            }
        }
        return nullptr;
    }
};
