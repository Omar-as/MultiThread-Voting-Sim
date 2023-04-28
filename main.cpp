#include "sleep.hh"
#include <argparse/argparse.hpp>

int main(int argc, char **argv) { 
    argparse::ArgumentParser program("voting simulator");
    program
        .add_argument("-t", "--time")
        .required()
        .help("specify the time")
        .scan<'i', int>();
        
    program
        .add_argument("-p", "--probability")
        .required()
        .help("specify the probability")
        .scan<'f', float>();

    try {
      program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
      std::cerr << err.what() << std::endl;
      std::cerr << program;
      return 1;
    }
    auto time        = program.get<int>("-t");
    auto probability = program.get<float>("-p");

    std::cout << "time: " << time << "\n";

    std::cout << "prob: " << probability << "\n";


    return 0;
}
