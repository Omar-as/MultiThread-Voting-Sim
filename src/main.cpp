#include "sleep.cpp"
#include <argparse/argparse.hpp>
#include <random>

using namespace std;
void* create_voters( void* args_ptr)
{
    float probability;
    float sim_time;
    auto args = *(pair<float, int> *) args_ptr;
    probability = args.first;
    sim_time = args.second;

    while(1){
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0, 1);

        double random_number = dis(gen);

        if (random_number <= probability) {
            
            cout << "create" << endl;

        } else {

            cout << "failed" << endl;
        }
        slp::pthread_sleep(1);
    }
    return 0;
}
int main(int argc, char **argv) { 
    const map<string, float> Canditates = {
        {"Mary", 0.40},
        {"John", 0.15},
        {"Anna", 0.45}
    };
    argparse::ArgumentParser program("voting simulator");
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

    try {
      program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
      cerr << err.what() << endl;
      cerr << program;
      return 1;
    }

    auto time               = program.get<int>("-t");
    auto probability        = program.get<float>("-p");
    auto number_of_stations = program.get<int>("-c");
    pair<float, int> args = {probability, time};
    pthread_t creation_thread;
    pthread_create( &creation_thread, NULL, create_voters, (void*) &args);

    pthread_join(creation_thread, NULL);
    return 0;
}

