#ifndef SIM_STATS_H
#define SIM_STATS_H

#include "Team.h"
#include <string>
#include <memory>

class Sim_Stats {
public:
    // Create a Sim_Stats object with the specified team
    Sim_Stats(std::shared_ptr<Team> team_ptr_);
    
    // Update simulation statistics for one simulation run
    void update_stats(bool final_four, bool champion);
    
    // Print simulation statistics
    void print_stats();
    
    // Reset simulation statistics
    void reset_stats();
    
    // Accessors
    int get_team_ID() {return team_ptr->get_ID();}
    const std::string& get_team_name() {return team_ptr->get_name();}
    int get_team_seed() {return team_ptr->get_seed();}
    
private:
    std::shared_ptr<Team> team_ptr;
    int num_sim;
    int num_final_four;
    int num_champion;
};

#endif
