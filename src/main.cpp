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
#include "custom/voter.cpp"

/* Other Imports */

#include <argparse/argparse.hpp>
#include <random>
#include <chrono>

/******************************************************************************/

/****************/
/* Global State */
/****************/

/******************************************************************************/

/*************************/
/* Function Declarations */
/*************************/

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
    custom::args_struct args = { probability, time };

    // Send values to create voters
    pthread_create( &creation_thread, NULL, custom::create_voters, (void*) &args );

    // Join Threads
    pthread_join(creation_thread, NULL);

    return EXIT_SUCCESS;
}

/******************************************************************************/

/********************/
/* Helper Functions */
/********************/

/******************************************************************************/
