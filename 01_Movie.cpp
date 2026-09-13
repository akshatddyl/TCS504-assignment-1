// this class represents a movie playing in the cinema.
#include <bits/stdc++.h>
class Movie{
private:
    std::string title;
    std::string language;
    int durationMinutes;

public:
    Movie(std::string title, std::string language, int durationMinutes)
        : title(std::move(title)),
          language(std::move(language)),
          durationMinutes(durationMinutes){}
    std::string getTitle() const{
        return this->title;
    }
    std::string getLanguage() const{
        return this->language;
    }

    int getDurationMinutes() const{
        return this->durationMinutes;
    }
};
