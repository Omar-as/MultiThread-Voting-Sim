#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

#define SEP ' '
#define LOG_FILE "voters.log"
#define SEP_WIDTH 25

int n = 0;

namespace custom { 

    void log_voter_Data(int station_no, int ticket_no, string category, int req_time, int poll_stat_time, int turnaround_time) {

        if (req_time >= n) {

            ofstream logFile(LOG_FILE, ios::app);


            if (!logFile) {
                cerr << "File could not be opened" << endl;
                return;
            }

            logFile << left << setw(SEP_WIDTH) << setfill(SEP)
                    << station_no

                    << left << setw(SEP_WIDTH) << setfill(SEP)
                    << ticket_no

                    << left << setw(SEP_WIDTH) << setfill(SEP)
                    << category

                    << left << setw(SEP_WIDTH) << setfill(SEP)
                    << req_time

                    << left << setw(SEP_WIDTH) << setfill(SEP)
                    << poll_stat_time

                    << left << setw(SEP_WIDTH) << setfill(SEP)
                    << turnaround_time << endl;

            logFile.close();
        }
    }

    void resetLogFile(int nth_sec) {

        n = nth_sec;

        ofstream logFile(LOG_FILE, ios::trunc);

        if (!logFile) {
            cerr << "File could not be opened" << endl;
            return;
        }

        logFile << left << setw(SEP_WIDTH) << setfill(SEP)
                << "StationID"

                << left << setw(SEP_WIDTH) << setfill(SEP)
                << "VoterID"

                << left << setw(SEP_WIDTH) << setfill(SEP)
                << "Category"

                << left << setw(SEP_WIDTH) << setfill(SEP)
                << "Request Time"

                << left << setw(SEP_WIDTH) << setfill(SEP)
                << "Polling Station Time"

                << left << setw(SEP_WIDTH) << setfill(SEP)
                << "Turnaround Time" << endl;
        
        logFile << "-------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

        logFile.close();
    }


}
