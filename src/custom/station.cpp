#ifndef STATION_H
#define STATION_H

#include <iostream>
#include <map>
#include <string>
#include <queue>

using namespace std;

namespace custom {
    void* vote_thread_func(void* args){

        return 0;
    }
    class Station {

        private:

            queue<pair<int, pthread_t*>> normal;
            queue<pair<int, pthread_t*>> special;
            queue<pair<int, pthread_t*>> mechanic;

            pthread_mutex_t* mutex;
            pthread_mutex_t* mutex_vote;
            pthread_cond_t* cond_vote;



        public:

            Station(){
                mutex = new pthread_mutex_t;
                if(pthread_mutex_init(mutex,NULL)) {
                    cout << "mutex initialization failed" << endl;
                }
                mutex_vote = new pthread_mutex_t;
                if(pthread_mutex_init(mutex_vote,NULL)) {
                    cout << "mutex_vote initialization failed" << endl;
                }
                cond_vote = new pthread_cond_t;
                if(pthread_cond_init(cond_vote,NULL)) {
                    cout << "cond_vote initialization failed" << endl;
                }
            }

            ~Station() {
                pthread_mutex_destroy(mutex);
                delete mutex;
                pthread_mutex_destroy(mutex_vote);
                delete mutex_vote;
                pthread_cond_destroy(cond_vote);
                delete cond_vote;
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

                pthread_t voter_thread;

                pthread_create( &voter_thread, NULL, vote_thread_func, NULL );
                pair<int, pthread_t*> p = {ticket_no, &voter_thread};

                if(queue_name == "normal") {
                    normal.push(p);
                }
                else if(queue_name == "special") {
                    normal.push(p);
                } 
                else if (queue_name == "mechanic"){
                    mechanic.push(p); 
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

            queue<pair<int,pthread_t*>> get_normal()      { return normal;   }

            queue<pair<int,pthread_t*>> get_special()     { return special;  }

            queue<pair<int,pthread_t*>> get_mechanic()    { return mechanic; }

            pthread_mutex_t* get_mutex() { return mutex; }

            map<string, int> get_total_votes() { return total_votes; }

            pair<int,pthread_t*> pop_normal() {

                auto lock = get_mutex();

                // aquiring lock for poping from the queue
                pthread_mutex_lock (lock);

                auto val = normal.front();
                normal.pop();

                // release lock
                pthread_mutex_unlock (lock);

                return val;
            }
            
            pair<int,pthread_t*> pop_special() {

                auto lock = get_mutex();

                // aquiring lock for poping from the queue
                pthread_mutex_lock (lock);

                auto val = special.front();
                special.pop();

                // release lock
                pthread_mutex_unlock (lock);

                return val;
            }

            pair<int,pthread_t*> pop_mechanic() {

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
