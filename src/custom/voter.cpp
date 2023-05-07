#ifndef VOTER_H
#define VOTER_H

#include <iostream>
#include <pthread.h>
/* #include "station.cpp" */
using namespace std;
namespace custom {
    void* vote_thread_func(void* args){

        return 0;
    }
    class Voter {
        private:
            pthread_mutex_t* mutex_vote;
            pthread_cond_t* cond_vote;
            pthread_t* voter_thread;

            int ticket_number;
        public:
            Voter(int ticket_no) {
                mutex_vote = new pthread_mutex_t;
                if(pthread_mutex_init(mutex_vote,NULL)) {
                    cout << "mutex_vote initialization failed" << endl;
                }

                cond_vote = new pthread_cond_t;
                if(pthread_cond_init(cond_vote,NULL)) {
                    cout << "cond_vote initialization failed" << endl;
                }

                pthread_t voter_thread;
                pthread_create( &voter_thread, NULL, vote_thread_func, NULL );
                ticket_number = ticket_no;
            }
            ~Voter() {
                pthread_mutex_destroy(mutex_vote);
                delete mutex_vote;
                pthread_cond_destroy(cond_vote);
                delete cond_vote;
            }
            void vote(){}
            int get_ticket_number(){
                return ticket_number;
            }
    };
}

#endif
