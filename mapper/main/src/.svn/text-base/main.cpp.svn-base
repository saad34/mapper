#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include <iostream>

using namespace std;

int main() {
    
    string cityName;
    
    do {
    
    cout << "Please input the \".bin\" name of the city you want drawn"
            " (type 'q' to quit): ";
    cin >> cityName; 
    cout << endl;
    
    bool loaded = true;
    if(cityName != "q") loaded = draw_map("/cad2/ece297s/public/maps/" + cityName + ".bin");
    if(!loaded) cout << "Invalid name!" << endl;
    } while(cityName != "q");
   
    return 0;
}