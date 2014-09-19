#include "Sim_Stats.h"
#include <memory>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

// Saves old cout formatting and sets formatting to Sim_Stat's formatting when
// constructed. Guarantees old formatting will be reinstated when destructed.
class SetSimFormatAndRestore {
public:
    SetSimFormatAndRestore()
    : old_flags(std::cout.flags()), old_precision(static_cast<int>(std::cout.precision()))
    {
        cout.precision(2);
        cout.setf(ios::fixed, ios::floatfield);
    }
    
    ~SetSimFormatAndRestore()
    {
        std::cout.flags(old_flags);
        std::cout.precision(old_precision);
    }
private:
    std::ios::fmtflags old_flags;
    int old_precision;
};

// Create a Sim_Stats object with the specified team
Sim_Stats::Sim_Stats(shared_ptr<Team> team_ptr_)
: team_ptr(team_ptr_), num_sim(0), num_final_four(0), num_champion(0)
{}

// Update simulation statistics for one simulation run
void Sim_Stats::update_stats(bool final_four, bool champion)
{
    if (final_four)
        num_final_four++;
    if (champion)
        num_champion++;
    num_sim++;
}

// Print simulation statistics
void Sim_Stats::print_stats()
{
    cout << team_ptr->get_seed() << " " << team_ptr->get_name() << " Statistics:" << endl;
    if (!num_sim) {
        cout << "\tNo tournament simulations have been run." << endl;
    }
    else {
        double champion_pct = (static_cast<double>(num_champion)/static_cast<double>(num_sim))*100.;
        double final_four_pct = (static_cast<double>(num_final_four)/static_cast<double>(num_sim))*100.;
        
        SetSimFormatAndRestore set_restore_settings;
        
        cout << "\t" << setw(15) << "Champion:" << setw(7) << champion_pct << "% (" << num_champion << "/" << num_sim << ")" << endl;
        cout << "\t" << setw(15) << "Final Four:" << setw(7) << final_four_pct << "% (" << num_final_four << "/" << num_sim << ")" << endl;
    }
}

// Reset simulation statistics
void Sim_Stats::reset_stats()
{
    num_champion = 0;
    num_final_four = 0;
    num_sim = 0;
}
