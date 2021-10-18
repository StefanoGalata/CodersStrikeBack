#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

struct CheckpointPosition
{
    int checkpointX;
    int checkpointY;

    int distanceFromLastCheckpoint;

    CheckpointPosition() : checkpointX(0), checkpointY(0), distanceFromLastCheckpoint(0){};
    CheckpointPosition(int x, int y, int distance) : checkpointX(x), checkpointY(y), distanceFromLastCheckpoint(distance){};
};

// register a new checkpoint, returns true if the checkpoint was already registered
bool RegisterCheckPoint(vector<CheckpointPosition>& checkpointList, int& posX, int& posY)
{
    bool checkpointAlreadyRegistered = false;
    for(CheckpointPosition iterator : checkpointList)
    {
        // if we found that this checkpoint is already registered we can break the loop because we registered all checkpoints
        if(iterator.checkpointX == posX && iterator.checkpointY == posY)
        {
            checkpointAlreadyRegistered = true;
            break;
        }
    }

    // if this is a new checkpoint then we can add it to the list
    if(!checkpointAlreadyRegistered)
    {
        CheckpointPosition checkpointToAdd = CheckpointPosition(posX, posY, 0);
        // if it is the first checkpoint that we add
        if(checkpointList.size()==0)
        {
            checkpointList.push_back(checkpointToAdd);
            //cerr << "added checkpoint: " << to_string(posX) + ", " + to_string(posY) << endl;
        }
        else
        {
            CheckpointPosition lastCheckpoint = checkpointList.back();
            int distance = sqrt(pow(lastCheckpoint.checkpointX - posX,2) + pow(lastCheckpoint.checkpointY-posY,2));
            checkpointToAdd.distanceFromLastCheckpoint = distance;
            checkpointList.push_back(checkpointToAdd);
        }
    }

    return checkpointAlreadyRegistered;
}

void PrintDebugCheckpointList(vector<CheckpointPosition>& checkpointList)
{
    for(int i = 0; i<checkpointList.size(); ++i)
    {
        cerr << "checkpoint " + to_string(i+1) << " | x = " << checkpointList[i].checkpointX <<  " y = " << checkpointList[i].checkpointY << " d = " << checkpointList[i].distanceFromLastCheckpoint << endl;
    }
}

CheckpointPosition GetFurthestCheckpoint(vector<CheckpointPosition>& checkpointList)
{
    if(checkpointList.size() <= 0)
    {
        return CheckpointPosition();
    }
    CheckpointPosition furthestCheckpoint = checkpointList.at(0);
    //cerr << "checkpoint is " << to_string(1) << " With distance: " << to_string(furthestCheckpoint.distanceFromLastCheckpoint) << endl;
    int j = 0;
    for(int i = 1; i<checkpointList.size(); i++)
    {
        //cerr << "checkpoint is " << to_string(i+1) << " With distance: " << to_string(checkpointList[i].distanceFromLastCheckpoint) << endl;
        if(checkpointList[i].distanceFromLastCheckpoint > furthestCheckpoint.distanceFromLastCheckpoint)
        {
            furthestCheckpoint = checkpointList[i];
            j = i;
           
        }
    }

    //cerr << "furthestCheckpoint is " << to_string(j+1) << " With distance: " << to_string(furthestCheckpoint.distanceFromLastCheckpoint) << endl;
   
    return furthestCheckpoint;
}

int main()
{
    int availableBoosts = 1;
    bool allCheckpointRegistered = false;

    vector<CheckpointPosition> registeredCheckpoints = {};
    registeredCheckpoints.empty();

    int firstCheckpointX;
    int firstCheckpointY;

    bool firstCheckpointAssigned = false;
    bool firstFrameSkipped = true;

    int lastPodPositionX = 0; 
    int lastPodPositionY = 0;


    // game loop
    while (1) {
        int x;
        int y;

        //vector<int> myPodPosition = {x,y};
        
        int nextCheckpointX;
        int nextCheckpointY;

        //vector<int> nextCheckpointPosition = {nextCheckpointX, nextCheckpointY};

        int nextCheckpointDist;
        int nextCheckpointAngle;

        bool shouldUseBoost = false;

        // we need to skip the first frame because the checkpoint positions are wrong in that frame
        if(!firstCheckpointAssigned && !firstFrameSkipped)
        {
            firstCheckpointAssigned = true;
            firstCheckpointX = nextCheckpointX;
            firstCheckpointY = nextCheckpointY;
        }

        if(!firstFrameSkipped)
        {
            // register every checkpoint to see which one is the furtherst and use the boost later
            RegisterCheckPoint(registeredCheckpoints, nextCheckpointX, nextCheckpointY);
        }
        firstFrameSkipped = false;

        // this means that we registered all checkpoints
        if(!allCheckpointRegistered && firstCheckpointX == nextCheckpointX && firstCheckpointY == nextCheckpointY && registeredCheckpoints.size() > 2)
        {
            // fill the distanceFromLastCheckpoint of the first checkpoint
            CheckpointPosition lastCheckpoint = registeredCheckpoints.back();
            int distance = sqrt(pow(lastCheckpoint.checkpointX - nextCheckpointX, 2) + pow(lastCheckpoint.checkpointY-nextCheckpointY,2));
            registeredCheckpoints[0].distanceFromLastCheckpoint = distance;
            allCheckpointRegistered = true;
        }

        if(allCheckpointRegistered)
        {
            CheckpointPosition furthestCheckpoint = GetFurthestCheckpoint(registeredCheckpoints);
            if(furthestCheckpoint.checkpointX == nextCheckpointX && furthestCheckpoint.checkpointY == nextCheckpointY)
            {
                shouldUseBoost = true;
            }
        }


        
        cin >> x >> y >> nextCheckpointX >> nextCheckpointY >> nextCheckpointDist >> nextCheckpointAngle; cin.ignore();

        int opponentX;
        int opponentY;

        //vector<int> opponentPosition = {opponentX, opponentY};

        cin >> opponentX >> opponentY; cin.ignore();

        //int distanceFromOpponent = sqrt(pow(opponentX-x,2) + pow(opponentY-y,2));
        int opponentDistanceFromLastCheckPoint = sqrt(pow(opponentX-nextCheckpointX,2) + pow(opponentY-nextCheckpointY,2));

        int velocity = 100;

        //if(nextCheckpointDist < 1500 &&)
        
        if(nextCheckpointAngle > 20 || nextCheckpointAngle < -20 || nextCheckpointDist<=3000)
        {
            velocity = 100.f * (1.f-abs(((float)nextCheckpointAngle/90.f))) * ((float)nextCheckpointDist/(2.f*600.f)));
            cerr << "Velocity calc = " + to_string(velocity) << endl;
            velocity = clamp(velocity, 0, 100);
        }
        
        
        string thrust = to_string(velocity);
        
        cerr << "Velocity = " + to_string(velocity) << endl;
        cerr << "Next dist = " + to_string(nextCheckpointDist) << endl;
        cerr << "Opponent Distance from checkPoint = " << opponentDistanceFromLastCheckPoint << endl;
        cerr << "Next checkpoint = " + to_string(nextCheckpointX) << ", " << to_string(nextCheckpointY) << endl;
        cerr << "Next checkpoint angle = " + to_string(nextCheckpointAngle) << endl;
        cerr << "Cosine(nextCheckpointAngle) = " + to_string(cos(nextCheckpointAngle)) << endl;
        cerr << "Available boosts = " + to_string(availableBoosts) << endl;
        cerr << "All CheckpointRegistered = " + to_string(allCheckpointRegistered) << endl;
        cerr << "Thrust = " + thrust << endl;
        cerr << "Registered checkpoints = " + to_string(registeredCheckpoints.size()) << endl;
        cerr << "Delta position = " << x-lastPodPositionX << ", " << y-lastPodPositionY << endl;
        //cerr << "Angle between my and opp pods = " << angleBetweenMyPodAndOpponentPod << endl; 
        //cerr << "Distance from opponent = " << distanceFromOpponent << endl;

        lastPodPositionX = x;
        lastPodPositionY = y;
        

        PrintDebugCheckpointList(registeredCheckpoints);
        
        if(shouldUseBoost && availableBoosts > 0 && (nextCheckpointAngle<=20 && nextCheckpointAngle>=-20))
        {
            --availableBoosts;
            thrust = "BOOST";
        }

        cout << nextCheckpointX << " " << nextCheckpointY << " " + thrust << endl;
    }
}