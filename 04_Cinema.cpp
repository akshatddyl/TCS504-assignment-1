//04_Cinema.cpp represents the cinema complex.
// Composition is used here as Cinema owns Screen objects via unique_ptr
#include <string>
#include <vector>
#include <memory>
class Cinema{
private:
    std::string name;
    //composition: cinema owns its screen objects and when the cinema is destroyed all screens are also destroyed.
    std::vector<std::unique_ptr<Screen>> screens;

public:
    explicit Cinema(std::string name): name(std::move(name)){}
    std::string getName() const{
        return this->name;
    }
    void addScreen(std::unique_ptr<Screen> screen){
        screens.push_back(std::move(screen));
    }
    Screen* findScreen(int screenNumber) const{
        for (const auto& screen : screens){
            if (screen->getScreenNumber() == screenNumber){
                return screen.get();
            }
        }
        return nullptr;
    }
};
