#ifndef STATION_H
#define STATION_H

#include <iostream>
#include <map>
#include <string>
#include <queue>

using namespace std;

namespace custom {

    class Station {

        private:

            queue<int> normal;
            queue<int> special;
            queue<int> mechanic;

            pthread_mutex_t* mutex;

        public:

            Station(){
                if(pthread_mutex_init(mutex,NULL)) {
                    cout << "mutex initialization failed" << endl;
                }
            }


            map<string, int> total_votes = {
                {"Mary", 0},
                {"John", 0},
                {"Anna", 0},
            };

            int normal_waiting()    { return normal.size();   }

            int special_waiting()   { return special.size();  }

            int mechanic_waiting()  { return mechanic.size(); }

            int get_total_waiting() { return normal_waiting() + special_waiting() + mechanic_waiting(); }

            void add_normal ( int ticket_no )    { normal.push(ticket_no);   }

            void add_special ( int ticket_no )   { special.push(ticket_no);  }

            void add_mechanic ( int ticket_no )  { mechanic.push(ticket_no); }

            void increment_vote ( string candidate ) { total_votes[candidate]++; }

            queue<int> get_normal()      { return normal;   }

            queue<int> get_special()     { return special;  }

            queue<int> get_mechanic()    { return mechanic; }

            pthread_mutex_t* get_mutex() { return mutex;    }

            map<string, int> get_total_votes() { return total_votes; }

            int pop_normal() { 
                int val = normal.front();
                normal.pop();
                return val;   
            }
            
            int pop_special()    { 
                int val = special.front();
                special.pop();
                return val;
            }

            int pop_mechanic()   {
                int val = mechanic.front();
                mechanic.pop();
                return val;
            }
    };
}
#endif
