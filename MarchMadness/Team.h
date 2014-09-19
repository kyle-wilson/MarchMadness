#ifndef TEAM_H
#define TEAM_H

/*
 Team is a simple class that represents a team in the NCAA March
 Madness tournament.
 */
#include <string>
#include <cmath>

class Team {
public:
    // Create new team w/ specified ID & name
    Team(int ID_, std::string name_) : ID(ID_), name(name_) {}
    
    // Return actual tournament seed
    int get_seed() {return ceil(static_cast<double>(ID)/4.);}
    
    // Accessors
    int get_ID() {return ID;}
    const std::string& get_name() {return name;}
    
private:
    int ID;
    std::string name;
    
    // Disallow copy/move ctors/assignment
    // Teams are unique, we don't want copies
    Team(const Team& orig) =delete;
    Team(Team&& rhs) =delete;
    Team& operator= (const Team& orig) =delete;
    Team& operator= (Team&& rhs) = delete;
};

#endif
