#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/


int main()
{
    int availableBoosts = 1;
    int velocity = 100;
    // game loop
    while (1) {
        int x;
        int y;
        
        int nextCheckpointX; // x position of the next check point
        int nextCheckpointY; // y position of the next check point
        int nextCheckpointDist; // distance to the next checkpoint
        int nextCheckpointAngle; // angle between your pod orientation and the direction of the next checkpoint
        cin >> x >> y >> nextCheckpointX >> nextCheckpointY >> nextCheckpointDist >> nextCheckpointAngle; cin.ignore();
        int opponentX;
        int opponentY;
        cin >> opponentX >> opponentY; cin.ignore();

        int deceleration = 10;

        int deaccDist = 2000;


        // You have to output the target position
        // followed by the power (0 <= thrust <= 100)
        // i.e.: "x y thrust"
        string thrust = "100";

        if(availableBoosts>0 && velocity == 100)
        {   
            --availableBoosts;
            thrust = "BOOST";
        }
        
        else if(nextCheckpointAngle >= 90 || nextCheckpointAngle <= -90)
        {
            thrust = "0";
        } 
        else if(nextCheckpointDist > deaccDist){
            velocity = 100;
            thrust = to_string(velocity);
        }
        else
        {
            velocity = max(velocity-deceleration, 40);
            thrust = to_string(velocity);
        }

        cerr << "Velocity = " + to_string(velocity) << endl;
        cerr << "Next dist = " + to_string(nextCheckpointDist) << endl;
        cerr << "Next checkpoint angle = " + to_string(nextCheckpointAngle) << endl;
        
        cout << nextCheckpointX << " " << nextCheckpointY << " " + thrust << endl;
    }
}