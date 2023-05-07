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

map<int,custom::Station*> stations;

/******************************************************************************/

/*************************/
/* Function Declarations */
/*************************/

int get_least_crowded_station( int number_of_stations );
void* create_voters( void* args_ptr );
void* start_station( void* args_ptr );

/******************************************************************************/

/*********/
/* Logic */
/*********/

int main(int argc, char **argv) { 

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

    program
        .add_argument("-n")
        .help("The nth second to start")
        .default_value(0)
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
    auto nth_second         = program.get<int>("-n");
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
        custom::Station* station = new custom::Station();
        pair<int,custom::Station*> st =  { i+1, station };
        stations.insert(st); 
    }


    // Thread variables
    pthread_t creation_thread;
    vector<pthread_t> station_threads(number_of_stations);

    // Set values of the struct
    custom::voter_args_struct args = { probability, time, number_of_stations };

    // create the thread that creates voters
    pthread_create( &creation_thread, NULL, create_voters, (void*) &args );

    vector<custom::station_args_struct*> station_args_ptrs(number_of_stations);
    for (int i = 0; i < number_of_stations; i++) {
        custom::station_args_struct* station_args = new custom::station_args_struct{ time, i+1, nth_second };
        station_args_ptrs[i] = station_args;

        pthread_create( &station_threads[i], NULL, start_station, (void*) station_args);
    }

    // Join Threads
    pthread_join(creation_thread, NULL);
    for (int i = 0; i < number_of_stations; i++) {
        pthread_join(station_threads[i], NULL);
    }

    return EXIT_SUCCESS;
}

/******************************************************************************/

/********************/
/* Helper Functions */
/********************/

int get_least_crowded_station( int number_of_stations ) {

    int stat_number = 1;
    int total_waiting = stations[1]->get_total_waiting();

    for (int i = 0; i < number_of_stations; i++ ){

        int current = stations[i+1]->get_total_waiting();

        if ( current < total_waiting ){
            total_waiting = current;
            stat_number = i+1;
        }
    }

    return stat_number;
}

void* vote_thread_func(void* args_ptr) {

    auto args = (pair<custom::Station*, custom::Voter *>*) args_ptr;
    auto voter = args->second;
    auto station = args->first;


    auto lock = voter->get_mutex();
    auto cond = voter->get_cond();
    pthread_mutex_lock(lock);

    while( !voter->get_ready() ){
        pthread_cond_wait(cond, lock);
    }

    // Candidates with CDF interval
    const map<string, pair<float,float>> Candidates = {
        {"Mary", {0.00, 0.40}},
        {"John", {0.40, 0.55}},
        {"Anna", {0.55, 1.00}}
    };
    // random generator to generate a number between 0 and 1
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);

    double random_number = dis(gen);

    custom::pthread_sleep(2);
    // for each candidate check if the random number is within the candidates interval
    // if so return the name of the candidate
    for (auto it = Candidates.begin(); it != Candidates.end(); it++) {
        if(random_number >= it->second.first && random_number <= it->second.second){
             station->increment_vote( it->first );
             break;
        }
    }

    pthread_mutex_unlock(lock);

    return 0;
}

void* create_voters( void* args_ptr )
{
    // variables from struct
    float   probability;
    int     sim_time;
    int     number_of_stations;

    auto args = *(custom::voter_args_struct *) args_ptr;
    probability         = args.probability;
    sim_time            = args.sim_time;
    number_of_stations  = args.number_of_stations;

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

        int least_crowded_station = get_least_crowded_station(number_of_stations);
        auto& station = stations[least_crowded_station];

        string queue = (random_number <= probability) ? "normal" : "special";
        station->add_voter(ticket_no, queue);

        custom::Voter* voter;
        if(queue == "special"){
            voter = station->get_special().back();
        }
        else if(queue == "normal"){
            voter = station->get_normal().back();
        }
        pair<custom::Station*, custom::Voter *> args= {station, voter};

        pthread_t voter_thread;
        pthread_create( &voter_thread, NULL, vote_thread_func, &args );

        voter->set_thread(&voter_thread);

        ticket_no++;

        // Thread sleeps for t secs
        custom::pthread_sleep(t);
        current_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
    }

    return EXIT_SUCCESS;
}

string vote(){
    // Candidates with CDF interval
    const map<string, pair<float,float>> Candidates = {
        {"Mary", {0.00, 0.40}},
        {"John", {0.40, 0.55}},
        {"Anna", {0.55, 1.00}}
    };
    // random generator to generate a number between 0 and 1
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);

    double random_number = dis(gen);
    // for each candidate check if the random number is within the candidates interval
    // if so return the name of the candidate
    for (auto it = Candidates.begin(); it != Candidates.end(); it++) {
        if(random_number >= it->second.first && random_number <= it->second.second){
            return it->first;
        }
    }
    // the return statement should not be reached
    return NULL;
}
void log(custom::Station* station, int station_number, int current_time, int starting_time){
    auto total_votes = station->get_total_votes();
    auto ordinary_queue = station->get_normal();
    auto special_queue = station->get_special();
    auto curr_time = (current_time - starting_time);
    /* cout << "Special is empty: " << special_queue.empty() << endl; */
    cout << "At " << curr_time << " sec, polling station " << station_number << ", elderly/pregnant:";
    while(!special_queue.empty()) {
        cout << special_queue.front()->get_ticket_number();
        special_queue.pop();
        if (!special_queue.empty()) cout << ", ";
    }
    /* cout << endl << "Ordinary is empty: " << ordinary_queue.empty() << endl; */
    cout << endl << "At " << curr_time << " sec, polling station " << station_number << ", ordinary:";
    while(!ordinary_queue.empty()) {
        cout << ordinary_queue.front()->get_ticket_number();
        ordinary_queue.pop();
        if (!ordinary_queue.empty()) cout << ", ";
    }
    cout << endl;

    cout << "Station : " << station_number << endl;
    cout << "Mary: " << total_votes["Mary"] << " John: "  <<  total_votes["John"] << " Anna: " << total_votes["Anna"] <<  endl;

}

void* start_station( void* args_ptr ) {
    // variables from struct
    int sim_time;
    int station_number;
    int n;

    auto args      = *(custom::station_args_struct *) args_ptr;
    sim_time       = args.sim_time;
    station_number = args.station_number;
    n              = args.nth_second;
    /* cout << "starting station" << station_number << endl; */


    // Simulation timer
    auto t = 1;
    auto station = stations[ station_number ];
    auto current_time =  chrono::system_clock::to_time_t(chrono::system_clock::now());
    auto starting_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
    auto end_sim_time =  static_cast<int>(starting_time) + sim_time;
    custom::Voter* voter;

    while(current_time < end_sim_time) {

        // logging starting from the nth second
        if (current_time - starting_time >= n) {
            log(station, station_number, current_time, starting_time);
        }

        // checks if there are any voters waiting to vote
        if(station->normal_waiting() <= 0 && station->special_waiting() <= 0) {
            current_time = chrono::system_clock::to_time_t(chrono::system_clock::now());

            custom::pthread_sleep(t);
            continue;
        }

        // checks if we have have less than 5 ordinary voters waiting and at least 1 special voter
        // if so it gives the special voter priority to vote
        if(station->normal_waiting() < 5 && station->special_waiting() > 0) {
            voter = station->pop_special();
        }

        // checks if there are no special voters waiting
        // if so we let an ordinary voter vote
        else if(station->special_waiting() <= 0) {
            voter = station->pop_normal();
        }

        // if none of the above conditions hold we check the tickets of the first ordinary voter and the first special voter
        // and let the one with the lowest ticket number vote
        else {
            voter = station->get_normal().front()->get_ticket_number() > station->get_special().front()->get_ticket_number() ? station->pop_special() : station->pop_normal();
        }


        // makes the voter vote and we get their voting result
        string vote_res = vote();

        // increment the total number of votes for the given result
        station->increment_vote( vote_res );

        // sleep the thread for 2t
        custom::pthread_sleep(2 * t);
        current_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
    }

    return 0;
}

/******************************************************************************/
