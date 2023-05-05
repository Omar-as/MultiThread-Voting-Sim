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

      public:

         map<string, int> total_votes = {
            {"Mary", 0},
            {"John", 0},
            {"Anna", 0},
         };

         int normal_waiting()   { return normal.size();   }

         int speacial_waiting() { return special.size();  }

         int mechanic_waiting() { return mechanic.size(); }

         int get_total_waiting() { return normal_waiting() + speacial_waiting() + mechanic_waiting(); }

         void add_normal( int ticket_no )    { normal.push(ticket_no);   }

         void add_special( int ticket_no )   { special.push(ticket_no);  }

         void add_mechanic( int ticket_no )  { mechanic.push(ticket_no); }

         queue<int> get_normal()    { return normal;   }
         
         queue<int> get_special()   { return special;  }

         queue<int> get_mechanic()  { return mechanic; }
   };
}
#endif
