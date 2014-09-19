//
//  main.cpp
//  MarchMadness
//
//  Created by Kyle Wilson on 4/18/14.
//  Copyright (c) 2014 Kyle Wilson. All rights reserved.
//

#include "MM_Simulation.h"
#include "Utility.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, const char * argv[])
{
    // Default filenames
    const char* team_file_name_default = "teams.csv";
    const char* win_pct_file_name_default = "win_pct.csv";
    
    // Final filenames
    string team_file_name;
    string win_pct_file_name;
    
    // Test if default files are present. Otherwise, ask for file names.
    ifstream team_file_test(team_file_name_default);
    ifstream win_pct_file_test(win_pct_file_name_default);
    if (!team_file_test.is_open() || !win_pct_file_test.is_open()) {
        cout << "Default input files not detected. Default team file should be \"teams.csv\" and default\n"
            "matrix file should be \"win_pct.csv\".  Files should be in same folder as executable.\n" << endl;
        cout << "Team file name?" << endl;
        cin >> team_file_name;
        cout << "Win percentage matrix file name?" << endl;
        cin >> win_pct_file_name;
    } else {
        team_file_name = team_file_name_default;
        win_pct_file_name = win_pct_file_name_default;
    }
    team_file_test.close();
    win_pct_file_test.close();
    
    // Conduct the simulation
    try {
        // Create the March Madness simulation
        MM_Simulation tournament_sim(team_file_name, win_pct_file_name);
        
        // Run it 10,000 times
        tournament_sim.run(10000);
        
        // Print simulation statistics
        tournament_sim.print_statistics();
    
    } catch (Error& e) {
        // Terminate
        cout << e.what() << endl;
        return 0;
    }
    
    return 0;
}
