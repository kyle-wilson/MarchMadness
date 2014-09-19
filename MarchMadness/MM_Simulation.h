#ifndef MM_SIMULATION_H
#define MM_SIMULATION_H

#include "Sim_Stats.h" // fix
#include <memory>
#include <string>
#include <vector>
#include <set>

// Forward declarations
class Team;
struct Game;
struct Sim_Result;

class MM_Simulation {
public:
    // Initialize simulation with teams and win_pct from files
    // Warning: Can throw Error from constructor!
    MM_Simulation(const std::string& team_file_name, const std::string& win_pct_file_name);
    
    // Run the simulation "num" number of times. Default value 100 times.
    void run(int num =100);
    
    // Print simulation statistics
    void print_statistics();
    
    // Reset simulation statistics
    void reset_statistics();
    
private:
    // Teams
    std::vector<std::shared_ptr<Team>> teams;
    // Team stats
    std::vector<Sim_Stats> team_stats;
    
    // Win percentage matrix of lower ID'd team
    std::vector<std::vector<double>> win_pcts;
    
    // Run the simulation a single time
    Sim_Result single_run();
    
    // Return winner of the game
    std::shared_ptr<Team> get_winner(const Game& g);
};

#endif
