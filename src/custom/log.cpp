#include <string>
#include <iostream>
#include <fstream>

using namespace std;

namespace custom { 

    void log_voter_Data(int station_no, int ticket_no, string category, int req_time, int poll_stat_time, int turnaround_time){


        ofstream logFile("voters.log", std::ios::app);


        if (!logFile) {
            std::cerr << "File could not be opened" << std::endl;
            return;
        }

        logFile << station_no << "\t" 
                << ticket_no << "\t" 
                << category << "\t" 
                << req_time << "\t" 
                << poll_stat_time << "\t" 
                << turnaround_time << "\n";

        logFile.close();

    }

    void resetLogFile() {

        std::ofstream logFile("voters.log", std::ios::trunc);

        if (!logFile) {
            std::cerr << "File could not be opened" << std::endl;
            return;
        }

        logFile << "StationID   VoterID    Category    Request Time     Polling Station Time      Turnaround Time" << "\n";

        logFile.close();
    }


}
