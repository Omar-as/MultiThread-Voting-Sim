#ifndef VOTER_H
#define VOTER_H

#include <iostream>
#include <pthread.h>
#include <string>

using namespace std;

namespace custom {

    class Voter {

        private:

            // Mutexes
            pthread_mutex_t* mutex_vote;
            pthread_mutex_t* mutex_ready;
            pthread_cond_t* cond_vote;

            // Thread
            pthread_t voter_thread;

            bool ready;
            int ticket_number;
            int request_time;
            string category;

        public:

            /********************/
            /*** Constructor ***/
            /********************/

            Voter(int ticket_no, int req_time, string cat) {
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

                ticket_number = ticket_no;
                request_time = req_time; 
                category = cat;
                ready = false;
            }

            /*****************************************************************/

            /******************/
            /*** Destructor ***/
            /******************/

            ~Voter() {
                pthread_mutex_destroy(mutex_vote);
                delete mutex_vote;
                pthread_mutex_destroy(mutex_ready);
                delete mutex_ready;
                pthread_cond_destroy(cond_vote);
                delete cond_vote;
            }

            /*****************************************************************/

            /***************/
            /*** Getters ***/
            /***************/

            int     get_ticket_number() { return ticket_number; }
            int     get_request_time()  { return request_time; }
            string  get_category()      { return category; }

            pthread_mutex_t* get_mutex()        { return mutex_vote; }
            pthread_cond_t*  get_cond()         { return cond_vote; }
            pthread_t        get_thread()       { return voter_thread; }

            bool get_ready() {

                auto lock = mutex_ready;

                pthread_mutex_lock(lock);

                auto val = ready;

                pthread_mutex_unlock(lock);

                return val;
            }

            /*****************************************************************/

            /***************/
            /*** Setters ***/
            /***************/

            void set_thread(pthread_t thread) {

                auto lock = mutex_ready;

                pthread_mutex_lock(lock);

                voter_thread = thread;

                pthread_mutex_unlock(lock);

            }
            void set_ready( bool set){

                auto lock = mutex_ready;

                pthread_mutex_lock(lock);

                ready = set;

                pthread_mutex_unlock(lock);

            }

            /*****************************************************************/

    };
}

#endif
