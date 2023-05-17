#ifndef STRUCT_H
#define STRUCT_H

namespace custom {

    // Voter creation arguments struct
    struct voter_args_struct {
       float probability;
       int sim_time;
       int number_of_stations;
    };

    // station creation arguments struct
    struct station_args_struct {
       int sim_time;
       int station_number;
       float failure_probability;
    };
}
#endif
