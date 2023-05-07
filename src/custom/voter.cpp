#ifndef VOTER_H
#define VOTER_H

#include <iostream>
#include <pthread.h>
using namespace std;
namespace custom {

    class Voter {
        private:
            pthread_mutex_t* mutex_vote;

            pthread_mutex_t* mutex_ready;
            pthread_cond_t* cond_vote;
            pthread_t voter_thread;

            bool ready;
            int ticket_number;
        public:
            Voter(int ticket_no) {
                mutex_vote = new pthread_mutex_t;
                if(pthread_mutex_init(mutex_vote, NULL)) {
                    cout << "mutex_vote initialization failed" << endl;
                }

                mutex_ready = new pthread_mutex_t;
                if(pthread_mutex_init(mutex_ready, NULL)) {
                    cout << "mutex ready initialization failed" << endl;
                }

                cond_vote = new pthread_cond_t;
                if(pthread_cond_init(cond_vote, NULL)) {
                    cout << "cond_vote initialization failed" << endl;
                }

                /* pthread_t voter_thread; */
                /* pthread_create( &voter_thread, NULL, vote_thread_func, NULL ); */
                ticket_number = ticket_no;
                ready = false;
            }
            ~Voter() {
                pthread_mutex_destroy(mutex_vote);
                delete mutex_vote;
                pthread_mutex_destroy(mutex_ready);
                delete mutex_ready;
                pthread_cond_destroy(cond_vote);
                delete cond_vote;
            }
            /* void vote(){} */
            int get_ticket_number() {
                return ticket_number;
            }
            pthread_mutex_t* get_mutex() {
                return mutex_vote;
            }
            pthread_cond_t* get_cond() {
                return cond_vote;
            }
            pthread_t get_thread() {
                return voter_thread;
            }

            bool get_ready() {
                return ready;
            }
            void set_thread(pthread_t thread) {
                voter_thread = thread;
            }
            void set_ready( bool set){
                auto lock = mutex_ready;
                pthread_mutex_lock(lock);

                ready = set;

                pthread_mutex_unlock(lock);
            }

    };
}

#endif
