#ifndef STATION_H
#define STATION_H

#include <iostream>
#include <map>
#include <string>
#include <queue>
/* #include "myqueue.cpp" */
#include "voter.cpp"

using namespace std;

namespace custom {

    class Station {

        private:

            map<string ,queue<Voter*>> voters = {
                {"normal", {}},
                {"special", {}},
                {"mechanic", {}}
            };
            /* queue<Voter*> special; */
            /* queue<Voter*> mechanic; */

            pthread_mutex_t* mutex_queues;
            pthread_mutex_t* mutex_candidates;
            pthread_mutex_t* mutex_vote;



        public:

            Station() {
                mutex_queues = new pthread_mutex_t;
                mutex_candidates = new pthread_mutex_t;
                mutex_vote = new pthread_mutex_t;
                if(pthread_mutex_init(mutex_queues, NULL)) {
                    cout << "mutex initialization failed" << endl;
                }
                if(pthread_mutex_init(mutex_candidates, NULL)) {
                    cout << "mutex initialization failed" << endl;
                }
                if(pthread_mutex_init(mutex_vote, NULL)) {
                    cout << "mutex initialization failed" << endl;
                }

            }

            ~Station() {
                pthread_mutex_destroy(mutex_queues);
                pthread_mutex_destroy(mutex_candidates);
                pthread_mutex_destroy(mutex_vote);
                delete mutex_queues;
                delete mutex_candidates;
                delete mutex_vote;
            }


            map<string, int> total_votes = {
                {"Mary", 0},
                {"John", 0},
                {"Anna", 0},
            };

            int queue_size(string queue_name)    {

                auto lock = get_mutex();

                pthread_mutex_lock (lock);

                auto val = voters[queue_name].size();

                pthread_mutex_unlock (lock);
                return val;   
            }

            int get_total_waiting() {

                auto val = queue_size("normal") + queue_size("special") + queue_size("mechanic"); 

                return val;   
            }

            Voter* add_voter ( int ticket_no, string queue_name ) {

                auto lock = get_mutex();

                // aquiring lock for pushing a voter into the queue
                pthread_mutex_lock (lock);

                auto voter = new Voter(ticket_no);

                voters[queue_name].push(voter);

                // release lock
                pthread_mutex_unlock (lock);
                return voter;
            }

            void increment_vote ( string candidate ) {
                auto lock = get_candidates_mutex();

                // aquiring lock for incrementing the vote count
                pthread_mutex_lock (lock);

                total_votes[candidate]++; 

                // release lock
                pthread_mutex_unlock (lock);
            }

            queue<Voter*> get_queue(string queue_name)      { 
                auto lock = get_mutex();

                /* cout << "before the get normal lock" << endl; */

                pthread_mutex_lock (lock);
                /* cout << "after the get normal lock" << endl; */

                auto val = voters[queue_name];

                pthread_mutex_unlock (lock);
                /* cout << "after the get normal unlock" << endl; */
                return val;   
            }

            pthread_mutex_t* get_mutex() { return mutex_queues; }

            pthread_mutex_t* get_candidates_mutex() { return mutex_candidates; }

            pthread_mutex_t* get_vote_mutex() { return mutex_vote; }

            map<string, int> get_total_votes() { 
                auto lock = get_candidates_mutex();

                pthread_mutex_lock (lock);

                auto val = total_votes; 

                pthread_mutex_unlock (lock);
                return val;   
            }

            Voter* pop_queue(string queue_name) {

                auto lock = get_mutex();

                // aquiring lock for poping from the queue
                pthread_mutex_lock (lock);
                auto queue = &voters[queue_name];

                auto val = queue->front();
                queue->pop();

                // release lock
                pthread_mutex_unlock (lock);

                return val;
            }

            Voter* queue_front(string queue_name) {

                auto lock = get_mutex();

                // aquiring lock for poping from the queue
                pthread_mutex_lock (lock);

                auto val = voters[queue_name].front();

                // release lock
                pthread_mutex_unlock (lock);

                return val;
            }

    };
}
#endif
