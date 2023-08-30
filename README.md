# Electronic Voting Simulation

The Electronic Voting Simulation project aims to develop a simulator that replicates an election day scenario. The simulation models independent voters waiting in queues to cast their votes, polling stations for voting, and regular maintenance in case of failures. The project emphasizes real-time simulation, thread synchronization, and deadlock prevention using POSIX threads (pthreads) API.

## Table of Contents
- [Dependencies](#dependencies)
- [Compile and Run](#compile-and-run)
- [Part I](#part-i)
- [Part II](#part-ii)
- [Part III](#part-iii)
- [Part IV](#part-iv)
- [Implementation](#implementation)

## Dependencies:
- gcc 12.2.0
- argparse 2.9

## Compile and Run

To compile the code: 
``` cd src/ make ``` 

To Run the code:
``` ./main -p NORMAL_VOTER_PROB -t SIMUL_TIME -c STATION_COUNT \ -n LOG_INIT_WAIT -f FAILURE_PROB ```

## Part I

In this part, the simulation focuses on basic voter behavior. Voters arrive at the polling station, acquire a queue number, and cast their votes for one of the presidential candidates. Key rules include:

- Simulation uses real-time until the specified time.
- A single polling station is used.
- New voters arrive with probability 'p' every 't' seconds.
- Each voter takes '2t' seconds to cast their vote.
- The vote distribution among candidates is predefined.
- Voters must wait for their queue number before voting.

Command Line Arguments:
- `-t`: Total simulation time (e.g., `-t 200`)
- `-p`: Probability of a new voter's arrival (e.g., `-p 0.5`)

## Part II

This section introduces priority for elderly and pregnant voters. A queuing machine favors them over regular voters. The simulation conditions are as follows:

- Additional special voters (elderly/pregnant) arrive with probability '1 - p'.
- The queuing machine prioritizes elderly/pregnant voters until certain conditions are met.

## Part III

This section addresses starvation issues for special voters caused by the priority system introduced in Part II. Students are required to suggest and implement a solution to avoid this starvation while briefly explaining it in the report.

## Part IV

This part involves multiple polling stations, each with its own queuing machine. Voters are assigned to the least busy station. Stations can also face failures that require maintenance.

Command Line Arguments:
- `-c`: Number of polling stations (e.g., `-c 3`)


## Implementation

- Utilize data structures like C++ STL queues.
- Incorporate a random number generator with a seed option.
- Represent voters as threads, each with appropriate synchronization.
- Build complexity incrementally, testing each part before proceeding.
- Use provided pthread sleep code instead of system calls.
