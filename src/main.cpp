/******************************************************************************/

/***********/
/* Imports */
/***********/

/* STD */
#include <iostream>

using namespace std;

/* Custom Imports */

#include "custom/sleep.cpp"
#include "custom/struct.cpp"
/* #include "custom/voter.cpp" */
#include "custom/station.cpp"

/* Other Imports */

#include <argparse/argparse.hpp>
#include <random>
#include <chrono>
#include <map>

/******************************************************************************/

/*************/
/* Constants */
/*************/

/* #define NORMAL   3 */
/* #define SPECIAL  2 */
/* #define MECHANIC 1 */

/******************************************************************************/

/****************/
/* Global State */
/****************/

map<int,custom::Station> stations;

/******************************************************************************/

/*************************/
/* Function Declarations */
/*************************/

int get_least_crowded_station(int number_of_stations);
void* create_voters( void* args_ptr );

/******************************************************************************/

/*********/
/* Logic */
/*********/

int main(int argc, char **argv) { 

    // Canditates
    const map<string, float> Canditates = {
        {"Mary", 0.40},
        {"John", 0.15},
        {"Anna", 0.45}
    };

    // Pareser initializer
    argparse::ArgumentParser program("voting simulator");

    // Adding flags to parse
    program
        .add_argument("-t")
        .required()
        .help("specify the total time for the simulation")
        .scan<'i', int>();
        
    program
        .add_argument("-p")
        .required()
        .help("specify the probability of a voter arriving")
        .scan<'f', float>();

    program
        .add_argument("-c")
        .help("specify the number of polling stations")
        .default_value(1)
        .scan<'i', int>();

    // Parse arguments 
    try {
      program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
      cerr << err.what() << endl;
      cerr << program;
      return 1;
    }

    // Parser Constants
    auto time               = program.get<int>("-t");
    auto probability        = program.get<float>("-p");
    auto number_of_stations = program.get<int>("-c");

    // Constant Verifiers
    if (time <= 0){
        cout << "The total time for the simulation (-t) should be more than 0" << endl;
        return EXIT_FAILURE;
    }
    if (probability < 0 || probability > 1){
        cout << "The probability (-p) should be more than or equal to 0 and less than or equal to 1" << endl;
        return EXIT_FAILURE;
    }
    if (number_of_stations <= 0){
        cout << "The number of stations (-c) should be more than 0" << endl;
        return EXIT_FAILURE;
    }


    for (int i = 0; i < number_of_stations; i++) {
        pair<int,custom::Station> st =  {i+1, custom::Station()};
        stations.insert(st); 
    }


    // Thread variable
    pthread_t creation_thread;

    // Set values of the struct
    custom::args_struct args = { probability, time, number_of_stations };

    // Send values to create voters
    pthread_create( &creation_thread, NULL, create_voters, (void*) &args );

    // Join Threads
    pthread_join(creation_thread, NULL);

    return EXIT_SUCCESS;
}

/******************************************************************************/

/********************/
/* Helper Functions */
/********************/

int get_least_crowded_station( int number_of_stations ) {

    int stat_number = 1;
    int total_waiting = stations[1].get_total_waiting();

    for (int i = 0; i < number_of_stations; i++ ){

        int current = stations[i+1].get_total_waiting();

        if ( current < total_waiting ){
            total_waiting = current;
            stat_number = i+1;
        }
    }

    return stat_number;
}

void* create_voters( void* args_ptr )
{
    // variables from struct
    float   probability;
    int     sim_time;
    int     number_of_stations;

    auto args = *(custom::args_struct *) args_ptr;
    probability         = args.probability;
    sim_time            = args.sim_time;
    number_of_stations  = args.station_number; 

    // Time per voter 
    const auto t = 1;

    // Ticket Counter
    int ticket_no = 1;

    // Simulation timer
    auto current_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
    auto end_sim_time =  static_cast<int>(current_time) + sim_time;

    while(current_time < end_sim_time){

        // Random value gen for probability
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0, 1);

        double random_number = dis(gen);

        /* auto min = std::min_element(next_ticket.begin(), next_ticket.end()); */
        /* auto least_crowded_station =  std::distance(next_ticket.begin(), min); */

        int least_crowded_station = get_least_crowded_station(number_of_stations);
        auto& station = stations[least_crowded_station];

        if ( random_number <= probability ) {
            station.add_normal(ticket_no);
        }
        else { 
            station.add_special(ticket_no);
        }

        ticket_no++;

        // Thread sleeps for t secs
        custom::pthread_sleep(t);
        current_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
    }

    /* cout << "station [" << station_number << "] finished creating voters" << endl; */

    for (auto it = stations.begin(); it != stations.end(); it++) {

        cout << "Station Number:" << it->first << endl;

        auto station = it->second;
        auto normal  = station.get_normal();
        auto special = station.get_special();

        cout << "Ordinary Voters:" << endl;
        while (!normal.empty()) {
            cout << " " << normal.front();
            normal.pop();
        }

        cout << endl << "Special Voters:" << endl;
        while (!special.empty()) {
            cout << " " << special.front();
            special.pop();
        }
        cout << endl;

    }

    return EXIT_SUCCESS;
}

/******************************************************************************/
