#ifndef STATION_H
#define STATION_H

#include <iostream>
#include <map>
#include <string>
#include <queue>
#include "voter.cpp"

using namespace std;

namespace custom {

    class Station {

        private:

            queue<Voter*> normal;
            queue<Voter*> special;
            queue<Voter*> mechanic;

            pthread_mutex_t* mutex_queues;
            pthread_mutex_t* mutex_candidates;



        public:

            Station(){
                mutex_queues = new pthread_mutex_t;
                if(pthread_mutex_init(mutex_queues,NULL)) {
                    cout << "mutex initialization failed" << endl;
                }
            }

            ~Station() {
                pthread_mutex_destroy(mutex_queues);
                delete mutex_queues;
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

            void add_voter ( int ticket_no, string queue_name ) {

                auto lock = get_mutex();

                // aquiring lock for pushing a mechanic into the queue
                pthread_mutex_lock (lock);


                auto voter = new Voter(ticket_no);

                if(queue_name == "normal") {
                    normal.push(voter);
                }
                else if(queue_name == "special") {
                    special.push(voter);
                } 
                else if (queue_name == "mechanic"){
                    mechanic.push(voter); 
                }

                // release lock
                pthread_mutex_unlock (lock);
            }

            void increment_vote ( string candidate ) {
                auto lock = get_mutex();

                // aquiring lock for incrementing the vote count
                pthread_mutex_lock (lock);

                total_votes[candidate]++; 

                // release lock
                pthread_mutex_unlock (lock);
            }

            queue<Voter*> get_normal()      { return normal;   }

            queue<Voter*> get_special()     { return special;  }

            queue<Voter*> get_mechanic()    { return mechanic; }

            pthread_mutex_t* get_mutex() { return mutex_queues; }

            map<string, int> get_total_votes() { return total_votes; }

            Voter* pop_normal() {

                auto lock = get_mutex();

                // aquiring lock for poping from the queue
                pthread_mutex_lock (lock);

                auto val = normal.front();
                normal.pop();

                // release lock
                pthread_mutex_unlock (lock);

                return val;
            }
            
            Voter* pop_special() {

                auto lock = get_mutex();

                // aquiring lock for poping from the queue
                pthread_mutex_lock (lock);

                auto val = special.front();
                special.pop();

                // release lock
                pthread_mutex_unlock (lock);

                return val;
            }

            Voter* pop_mechanic() {

                auto lock = get_mutex();

                // aquiring lock for poping from the queue
                pthread_mutex_lock (lock);

                auto val = mechanic.front();
                mechanic.pop();

                // release lock
                pthread_mutex_unlock (lock);

                return val;
            }
    };
}
#endif
