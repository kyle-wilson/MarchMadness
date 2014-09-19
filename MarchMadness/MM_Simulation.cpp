#include "MM_Simulation.h"
#include "Team.h"
#include "Sim_Stats.h"
#include "Utility.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <queue>
#include <algorithm>
#include <cassert>

using namespace std;

// Helper function(s)
void get_team_name(ifstream& ifs, string& name);

// Represents a game
struct Game {
    Game(shared_ptr<Team> A, shared_ptr<Team> B)
    : teamA(A), teamB(B) {}
    shared_ptr<Team> teamA;
    shared_ptr<Team> teamB;
};

// Results of a single run of a simulation
struct Sim_Result {
    int champ_ID;
    vector<int> final_four_IDs;
};

// Initialize simulation with teams and win_pct from files
MM_Simulation::MM_Simulation(const string& team_file_name, const string& win_pct_file_name)
{
    const int num_teams = 64;
    const int num_buffer_size = 4;
    const int dbl_buffer_size = 15;
    const char delim = ',';
    
    // Ensure team and win pct files exist
    ifstream team_file(team_file_name);
    if (!team_file.is_open())
        throw Error("Cannot open team input file!");
    ifstream win_pct_file(win_pct_file_name);
    if (!win_pct_file.is_open())
        throw Error("Cannot open win pct input file!");
    
    // Initialize set of teams
    teams.resize(num_teams);
    for (int i = 0; i < num_teams; ++i) {
        // Get ID from file
        char num_buffer[num_buffer_size];
        team_file.getline(num_buffer, num_buffer_size, delim);
        int cur_id = atoi(num_buffer);
        
        // Verify ID is correct
        if (cur_id != i+1)
            throw Error("Can't read input file. Check IDs and newlines!");
        
        // Get team name from file
        string cur_name;
        getline(team_file, cur_name);
        
        // Add team to containers
        shared_ptr<Team> temp_team(new Team(cur_id, cur_name));
        teams[i] = temp_team;
        team_stats.push_back(Sim_Stats(temp_team));
    }
    team_file.close();
    
    // Initialize win percentage matrix
    // Set number of rows to number of teams
    win_pcts.resize(num_teams);
    for (int y = 0; y < num_teams; ++y) {
        // Resize this row to number of teams
        win_pcts[y].resize(num_teams);
        for (int x = 0; x < num_teams - 1; ++x) {
            // Read and insert double from file
            char dbl_buffer[dbl_buffer_size];
            win_pct_file.getline(dbl_buffer, dbl_buffer_size, delim);
            win_pcts[y][x] = atof(dbl_buffer);
        }
        double last;
        win_pct_file >> last;
        win_pcts[y][63] = last;
    }
}

// Run the simulation "num" number of times. Default value 10 times.
void MM_Simulation::run(int num)
{
    const int num_teams = 64;
    
    for (int x = 0; x < num; ++x) {
        Sim_Result result = single_run();
        
        // Update stats for each team
        for (int i = 0; i < num_teams; ++i) {
            // Check if team was champion or in final four
            bool champion = (i+1 == result.champ_ID);
            auto it = find(result.final_four_IDs.begin(), result.final_four_IDs.end(), i+1);
            bool final_four = (it != result.final_four_IDs.end());
            team_stats[i].update_stats(final_four, champion);
        }
    }
}

// Print simulation statistics
void MM_Simulation::print_statistics()
{
    for_each(team_stats.begin(), team_stats.end(), mem_fn(&Sim_Stats::print_stats));
}

// Reset simulation statistics
void MM_Simulation::reset_statistics()
{
    for_each(team_stats.begin(), team_stats.end(), mem_fn(&Sim_Stats::reset_stats));
}

// Run the simulation a single time
Sim_Result MM_Simulation::single_run()
{
    
    // Create a tournament
    queue<Game> tournament;
    Sim_Result result;
    
    vector<int> game_order {1,61,29,33,17,45,13,49,21,41,9,53,25,37,5,57,2,62,30,34,18,46,14,50,22,42,10,54,26,38,6,58,3,63,31,35,19,47,15,51,23,43,11,55,27,39,7,59,4,64,32,36,20,48,16,52,24,44,12,56,28,40,8,60};
    
    reverse(game_order.begin(), game_order.end());
    
    while (game_order.size()) {
        int A_ID = game_order.back();
        game_order.pop_back();
        int B_ID = game_order.back();
        game_order.pop_back();
        Game next_game(teams[A_ID-1], teams[B_ID-1]);
        tournament.push(next_game);
    }
    
    // Run the simulation
    int game_counter = 0;
    while (tournament.size() > 1) {
        // Get next two games and their winners to make next round game
        
        // Game A
        Game gameA = tournament.front();
        shared_ptr<Team> gameA_winner = get_winner(gameA);
        tournament.pop();
        
        // Game B
        Game gameB = tournament.front();
        shared_ptr<Team> gameB_winner = get_winner(gameB);
        tournament.pop();
        
        // Check for final four teams
        game_counter += 2;
        if (game_counter == 58 || game_counter == 60) {
            result.final_four_IDs.push_back(gameA_winner->get_ID());
            result.final_four_IDs.push_back(gameB_winner->get_ID());
        }
        
        // Create and add next round game
        tournament.push(Game(gameA_winner, gameB_winner));
    }
    
    // Play championship game
    Game championship = tournament.front();
    //cout << championship.teamA->get_name() << " vs. " << championship.teamB->get_name() << endl;
    shared_ptr<Team> champion = get_winner(championship);
    //cout << champion->get_name() << " wins!" << endl;
    
    // Set champion in result
    result.champ_ID = champion->get_ID();
    
    return result;
}

// Return winner of the Game
shared_ptr<Team> MM_Simulation::get_winner(const Game& g)
{
    int teamA_ID = g.teamA->get_ID();
    int teamB_ID = g.teamB->get_ID();
    
    // Get random number and win pct
    double rand_num = static_cast<double>(rand())/
                        static_cast<double>(RAND_MAX);
    double win_pct = win_pcts[teamB_ID-1][teamA_ID-1];
    
    if (teamA_ID < teamB_ID) {
        if (rand_num <= win_pct) {
            return g.teamA;
        } else {
            return g.teamB;
        }
    } else {
        if (rand_num <= win_pct) {
            return g.teamB;
        } else {
            return g.teamA;
        }
    }
}

// Trim team name from input file to include minimal whitespace.
void get_team_name(ifstream& ifs, string& name) {
    string temp;
    getline(ifs, temp);
    name.resize(temp.size());
    
    // Remove duplicate whitespace
    auto it = unique(temp.begin(), temp.end(), [](char c1, char c2){return isspace(c1)&&isspace(c2);});
    name.resize(distance(temp.begin(), it));
    
    // Replace all whitespace with spaces and copy temp string into title
    replace_copy_if(temp.begin(), it, name.begin(), [](char c){return isspace(c);}, ' ');
    
    // Remove first and last spaces if necessary
    if (isspace(name.front()))
        name.erase(name.begin());
    if (isspace(name.back()))
        name.erase(name.size()-1, 1);
    
    // If title size < 1, throw error
    if (name.size() < 1)
        throw Error("Unable to parse title from team input file!");
}

