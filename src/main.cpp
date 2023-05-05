/******************************************************************************/

/***********/
/* Imports */
/***********/

/* STD */
#include <iostream>

using namespace std;

/* Custom Imports */

#include "custom/sleep.cpp"

/* Other Imports */

#include <argparse/argparse.hpp>
#include <vector>
#include <random>
#include <chrono>

/******************************************************************************/

/****************/
/* Global State */
/****************/

// Voter creation arguments struct
struct args_struct {
    float probability;
    int sim_time;
    /* int station_number; */
};

/******************************************************************************/

/*************************/
/* Function Declarations */
/*************************/

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
        return 1;
    }
    if (probability < 0 || probability > 1){
        cout << "The probability (-p) should be more than or equal to 0 and less than or equal to 1" << endl;
        return 1;
    }
    if (number_of_stations <= 0){
        cout << "The number of stations (-c) should be more than 0" << endl;
        return 1;
    }

    // Thread variable
    pthread_t creation_thread;

    // Set values of the struct
    args_struct args = { probability, time };

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

void* create_voters( void* args_ptr )
{
    // variables from struct
    float probability;
    int sim_time;
    int station_number;

    auto args = *(args_struct *) args_ptr;
    probability = args.probability;
    sim_time = args.sim_time;
    /* station_number = args.station_number; */

    // Time per voter 
    const auto t = 1;

    // Ticket Counter
    auto next_ticket = 1;

    // Voters "Queues"
    vector<int> ordinary_voters = {};
    vector<int> special_voters = {};

    // Simulation timer
    auto current_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
    auto end_sim_time =  static_cast<int>(current_time) + sim_time;

    while(current_time < end_sim_time){

        // Random value gen for probability
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0, 1);

        double random_number = dis(gen);

        if (random_number <= probability) { ordinary_voters.push_back(next_ticket); }
        else { special_voters.push_back(next_ticket); }

        next_ticket++;

        // Thread sleeps for t secs
        custom::pthread_sleep(t);
        current_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
    }

    /* cout << "station [" << station_number << "] finished creating voters" << endl; */

    cout << "Ordinary Voters:" << endl;
    for(int i : ordinary_voters){
        cout << " " << i;
    }
    cout << endl << "Special Voters:" << endl;
    for(int i : special_voters){
        cout << " " << i;
    }
    cout << endl;

    return EXIT_SUCCESS;
}

/******************************************************************************/
