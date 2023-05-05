#ifndef VOTER_H
#define VOTER_H

#include "struct.cpp"
#include "sleep.cpp"

#include <random>
#include <chrono>
#include <map>
#include <algorithm>

using namespace std;

/******************************************************************************/

/****************/
/* Global State */
/****************/
map<int,vector<int>> ordinary_voters = {};
map<int,vector<int>> special_voters = {};

/******************************************************************************/

namespace custom {
   void* create_voters( void* args_ptr )
   {
        // variables from struct
        float probability;
        int sim_time;
        int station_number;

        auto args = *(custom::args_struct *) args_ptr;
        probability = args.probability;
        sim_time = args.sim_time;
        /* station_number = args.station_number; */
        station_number = 1;
        for (int station = 1; station <= station_number; station++) {
            ordinary_voters.insert({station, {}});
            special_voters.insert({station, {}});
        }

        // Time per voter 
        const auto t = 1;

        // Ticket Counter
        vector<int> next_ticket(station_number, 1);

        // Voters "Queues"
        /* vector<int> ordinary_voters = {}; */
        /* vector<int> special_voters = {}; */

        // Simulation timer
        auto current_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
        auto end_sim_time =  static_cast<int>(current_time) + sim_time;

        while(current_time < end_sim_time){

            // Random value gen for probability
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<> dis(0, 1);

            double random_number = dis(gen);

            auto min = std::min_element(next_ticket.begin(), next_ticket.end());
            auto least_crowded_station =  std::distance(next_ticket.begin(), min);
            if ( random_number <= probability ) { ordinary_voters[least_crowded_station].push_back(next_ticket[least_crowded_station]); }
            else { special_voters[least_crowded_station].push_back(next_ticket[least_crowded_station]); }
            next_ticket[least_crowded_station]++;


            // Thread sleeps for t secs
            custom::pthread_sleep(t);
            current_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
        }

        /* cout << "station [" << station_number << "] finished creating voters" << endl; */

        /* cout << "Ordinary Voters:" << endl; */
        /* for(int i : ordinary_voters){ */
        /*     cout << " " << i; */
        /* } */
        /* cout << endl << "Special Voters:" << endl; */
        /* for(int i : special_voters){ */
        /*     cout << " " << i; */
        /* } */
        /* cout << endl; */

        return EXIT_SUCCESS;
    }
}

#endif
