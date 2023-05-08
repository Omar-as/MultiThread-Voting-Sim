#ifndef STATION_H
#define STATION_H

#include <iostream>
#include <queue>
#include <pthread.h>
/* #include "voter.cpp" */

using namespace std;

namespace custom {

    template <typename T>
    class MyQueue : public queue<T>{

        private:
            pthread_mutex_t* mutex;

        public:

            MyQueue(){
                mutex = new pthread_mutex_t;
                if(pthread_mutex_init(mutex, NULL)) {
                    cout << "mutex initialization failed" << endl;
                }
            }

            MyQueue(const MyQueue &other) = delete;

            MyQueue &operator=(const MyQueue &other) = delete;

            ~MyQueue() {
                pthread_mutex_destroy(mutex);
                delete mutex;
            }

            void push(T val){

                pthread_mutex_lock (mutex);

                queue<T>::push(val);

                pthread_mutex_unlock (mutex);
                
            }

            T pop(){

                pthread_mutex_lock (mutex);

                auto val = queue<T>::front();
                queue<T>::pop();

                pthread_mutex_unlock (mutex);

                return val;
                
            }

            T front(){

                pthread_mutex_lock (mutex);

                auto val = queue<T>::front();

                pthread_mutex_unlock (mutex);
                
                return val;

            }

            T back(){

                pthread_mutex_lock (mutex);

                auto val = queue<T>::back();

                pthread_mutex_unlock (mutex);

                return val;
                
            }

            bool Isempty(){

                pthread_mutex_lock (mutex);

                auto val = queue<T>::empty();

                pthread_mutex_unlock (mutex);
                
                return val;
            }

            int size(){

                pthread_mutex_lock (mutex);

                auto val = queue<T>::size();

                pthread_mutex_unlock (mutex);

                return val;
                
            }

    };
}
#endif
