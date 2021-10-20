#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

struct FVector
{
    float X;
    float Y;

    void Normalize()
    {
        Y = X/Length();
        Y = Y/Length();
    }

    float Length()
    {
       return sqrt((X*X)+(Y*Y));
    }

    FVector() : X(0), Y(0) {}
    FVector(float x, float y) : X(x), Y(y) {}

    bool operator==(const FVector otherVector)
    {
        return (this->X == otherVector.X && this->Y == otherVector.Y);
    }

    FVector& operator*(const float multiplier)
    {
       multiplier * this->X; 
       multiplier * this->Y;
       return *this;
    }

    FVector& operator-(FVector otherVector)
    {
        this->X-otherVector.X;
        this->Y - otherVector.Y;
        return *this;
    }
};

struct CheckpointEntry
{
    FVector checkpointPosition;

    int distanceFromLastCheckpoint;

    CheckpointEntry() : checkpointPosition(FVector()), distanceFromLastCheckpoint(0){};
    CheckpointEntry(FVector position, int distance) : checkpointPosition(position), distanceFromLastCheckpoint(distance){} 
    CheckpointEntry(float x, float y, int distance) : checkpointPosition(FVector(x,y)), distanceFromLastCheckpoint(distance) {}
    
};

// register a new checkpoint, returns true if the checkpoint was already registered
bool RegisterCheckPoint(vector<CheckpointEntry>& checkpointList, FVector& checkpointPosition)
{
    bool checkpointAlreadyRegistered = false;
    for(CheckpointEntry iterator : checkpointList)
    {
        // if we found that this checkpoint is already registered we can break the loop because we registered all checkpoints
        if(iterator.checkpointPosition == checkpointPosition)
        {
            checkpointAlreadyRegistered = true;
            break;
        }
    }

    // if this is a new checkpoint then we can add it to the list
    if(!checkpointAlreadyRegistered)
    {
        CheckpointEntry checkpointToAdd = CheckpointEntry(checkpointPosition, 0);
        // if it is the first checkpoint that we add
        if(checkpointList.size()==0)
        {
            checkpointList.push_back(checkpointToAdd);
            //cerr << "added checkpoint: " << to_string(posX) + ", " + to_string(posY) << endl;
        }
        else
        {
            CheckpointEntry lastCheckpoint = checkpointList.back();
            int distance = sqrt(pow(lastCheckpoint.checkpointPosition.X - checkpointPosition.X,2) + pow(lastCheckpoint.checkpointPosition.Y-checkpointPosition.Y,2));
            checkpointToAdd.distanceFromLastCheckpoint = distance;
            checkpointList.push_back(checkpointToAdd);
        }
    }

    return checkpointAlreadyRegistered;
}

void PrintDebugCheckpointList(vector<CheckpointEntry>& checkpointList)
{
    for(int i = 0; i<checkpointList.size(); ++i)
    {
        cerr << "checkpoint " + to_string(i+1) << " | x = " << checkpointList[i].checkpointPosition.X <<  " y = " << checkpointList[i].checkpointPosition.Y << " d = " << checkpointList[i].distanceFromLastCheckpoint << endl;
    }
}

CheckpointEntry GetFurthestCheckpoint(vector<CheckpointEntry>& checkpointList)
{
    if(checkpointList.size() <= 0)
    {
        return CheckpointEntry();
    }
    CheckpointEntry furthestCheckpoint = checkpointList.at(0);
    int j = 0;
    for(int i = 1; i<checkpointList.size(); i++)
    {
        if(checkpointList[i].distanceFromLastCheckpoint > furthestCheckpoint.distanceFromLastCheckpoint)
        {
            furthestCheckpoint = checkpointList[i];
            j = i;
           
        }
    }

    return furthestCheckpoint;
}


int main()
{
    int availableBoosts = 1;
    bool allCheckpointRegistered = false;

    bool doOnce = true;

    // List of checkpoints
    vector<CheckpointEntry> registeredCheckpoints = {};

    FVector firstCheckpoint;
    FVector nextCheckpoint;

    FVector lastPodPosition;
    int angleToStopFeedVelocity = 90;
    int checkpointRadius = 600;

    int brakeFactor = 2;


    // game loop
    while (1) {
        FVector podPosition;

        int nextCheckpointDist;
        int nextCheckpointAngle;

        bool shouldUseBoost = false;

        cin >> podPosition.X >> podPosition.Y >> nextCheckpoint.X >> nextCheckpoint.Y >> nextCheckpointDist >> nextCheckpointAngle; cin.ignore();

        if(doOnce)
        {
            firstCheckpoint = nextCheckpoint;
            doOnce = false;
        }


        // register every checkpoint to see which one is the furtherst and use the boost later
        RegisterCheckPoint(registeredCheckpoints, nextCheckpoint);

        // this means that we registered all checkpoints
        if(!allCheckpointRegistered && firstCheckpoint == nextCheckpoint && registeredCheckpoints.size() > 2)
        {
            // fill the distanceFromLastCheckpoint of the first checkpoint
            CheckpointEntry lastCheckpoint = registeredCheckpoints.back();
            int distance = sqrt(pow(lastCheckpoint.checkpointPosition.X - nextCheckpoint.X, 2) + pow(lastCheckpoint.checkpointPosition.Y-nextCheckpoint.Y,2));
            registeredCheckpoints[0].distanceFromLastCheckpoint = distance;
            allCheckpointRegistered = true;
        }

        if(allCheckpointRegistered)
        {
            CheckpointEntry furthestCheckpoint = GetFurthestCheckpoint(registeredCheckpoints);
            if(furthestCheckpoint.checkpointPosition == nextCheckpoint)
            {
                shouldUseBoost = true;
            }
        }


        FVector opponentPosition;

        cin >> opponentPosition.X >> opponentPosition.Y; cin.ignore();

        int velocity;

        if(nextCheckpointAngle > -angleToStopFeedVelocity && nextCheckpointAngle < angleToStopFeedVelocity)
        {
            velocity = 100;
        }
        else
        {
            velocity = 100.f * (1.f-(float)nextCheckpointAngle/90.f) * (float)(nextCheckpointDist/2.f*checkpointRadius);
            cerr << "Velocity calc = " << to_string(velocity) << endl;
            velocity = clamp(velocity, 0, 100);
        }

        
        string thrust = to_string(velocity);

        FVector targetDestination;

        targetDestination = nextCheckpoint-((podPosition-lastPodPosition) * brakeFactor);

        cerr << "Velocity = " << velocity << endl;
        cerr << "Next dist = " << nextCheckpointDist << endl;
        cerr << "target pos = " << targetDestination.X << ", " << targetDestination.Y << endl;
        cerr << "Next checkpoint = " << nextCheckpoint.X << ", " << nextCheckpoint.Y << endl;
        cerr << "Next checkpoint Vec = " << nextCheckpoint.X << ", " << nextCheckpoint.Y << endl;
        cerr << "Next checkpoint angle = " << nextCheckpointAngle << endl;
        cerr << "Available boosts = " << availableBoosts << endl;
        cerr << "Should use boost = " << shouldUseBoost << endl;
        cerr << "All CheckpointRegistered = " << allCheckpointRegistered << endl;
        cerr << "Thrust = " + thrust << endl;
        cerr << "Registered checkpoints = " << registeredCheckpoints.size() << endl;
        

        lastPodPosition = podPosition;
        

        PrintDebugCheckpointList(registeredCheckpoints);
        
        if(shouldUseBoost && availableBoosts > 0 && (nextCheckpointAngle<=20 && nextCheckpointAngle>=-20))
        {
            --availableBoosts;
            thrust = "BOOST";
        }

        cout << targetDestination.X << " " << targetDestination.Y << " " + thrust << endl;
    }
}