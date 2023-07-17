//Project : Farm Simulation
//Author: Sam Black

#include <iostream>
#include <vector>
#include <cstdlib>
#include "farm_object.h"
#include "debug.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <time.h>
#include <unistd.h>
#endif

using namespace std;

//Constants for the size of the farm
const int FARM_WIDTH = 80;
const int FARM_HEIGHT = 14;

//Constants for drawing/ refresh rate
const unsigned int DAY_LENGTH = 120; //Number of ms to render a day
const unsigned long TOTAL_DAYS = 20;
const unsigned long SLEEP_TIME = DAY_LENGTH * TOTAL_DAYS;

int FarmObject::count = 0; //Initialize Farm Object count to 0
Farm* FarmObject::farm = nullptr;
ofstream DebugLogger::debugLog;
#define DEFAULT_SEED 12202 //Default seed to use if none is specified

void randomlyAddObjects(Farm* farm, const unsigned int& seed);
int findTotalWeight(int weights[]);
ObjectType randomlyPickObject(int totalWeight, int weights[]);

int main(int argc, char** argv)
{
    //Error check the command line arguments

    //Try to convert the seed to 
    int seed = DEFAULT_SEED; //Default seed
    if (argc == 2) //If the user specified a seed
    {
        try { seed = atoi(argv[1]); }
        catch (exception&)
        {
            cout << "Error, could not convert " << argv[1] << " to an int" << endl;
            return 1;
        }
    }
    else if (argc > 2)
    {
        cout << "ERROR: Usage is: ./virt_farm [optional_seed]" << endl;
        return 1;
    }

    DebugLogger::openDebugFile("debug_log.txt"); //Open the debug file

    Farm* farm = new Farm(FARM_WIDTH, FARM_HEIGHT); //Create the Farm
    FarmObject::setFarm(farm); //Set the static reference

    //***** Put all of the objects into the Farm *****
#ifndef _WIN32
    //int retVal;
    timespec sleepTime, remainingTime;
    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = 1000000 * SLEEP_TIME;
#endif
    randomlyAddObjects(farm, seed); //Randomly add objects to the farm using our seed (modify the objectCount for more objects) 

    //DEBUG: You can add things here to test if you'd like, just make sure to delete it when you're done

    unsigned long dayCounter = 0; //How many frames have passed
    while (dayCounter < TOTAL_DAYS)
    {
        farm->updateFarm(); //Update all of the object in the Farm
        farm->displayFarm(); //Draw the updated Farm
        cout << "Day: " << dayCounter + 1 << "/" << TOTAL_DAYS << endl;

#ifdef _WIN32
        Sleep(SLEEP_TIME);
#else
//sleep(1);
        nanosleep(&sleepTime, &remainingTime);
#endif
        dayCounter++; //Count this day
    }

    //Deallocate the farm and all of the objects in the Farm
    delete farm;
    DebugLogger::closeDebugFile(); //Close the debug log

    return 0;
}

//Object weights array
const int OBJ_COUNT = 5;
//Weights for GRASS, COW, CHICKEN, EGG, EMPTY respectively
int weights[OBJ_COUNT] = {12, 3, 2, 1, 179};

int findTotalWeight(int weights[])
{
    int totalWeight = 0;
    for (int i = 0; i < OBJ_COUNT; i++)
        totalWeight += weights[i];

    return totalWeight;
}

ObjectType randomlyPickObject(int totalWeight, int weights[])
{
    int randVal = rand() % totalWeight; //Pick a random weight
    int cumulativeWeight = weights[0];
    for (int i = 1; i < OBJ_COUNT; i++)
    {
        if (randVal < cumulativeWeight)
            return static_cast<ObjectType>(i);
        cumulativeWeight += weights[i];
    }

    return static_cast<ObjectType>(OBJ_COUNT);
}

//Randomly fills the given farm with the amount of objects given and does not place things on top of each other
void randomlyAddObjects(Farm* farm, const unsigned int& seed)
{
    int i, j;
    srand(seed); //Seed the random number generation

    //Go through and generate all of the objects
    /*
    int max = FARM_WIDTH * FARM_HEIGHT / 2; //Max by default is half full
    if(max > maxObjectCount) //If the objectCount is too big, use the upper bound from the last line
        max = maxObjectCount;
        */

    int totalWeight = findTotalWeight(weights); //Find the total weights for objects for RNG
    for (i = 0; i < FARM_HEIGHT; i++)
    {
        for (j = 0; j < FARM_WIDTH; j++)
        {
            //Figure out what kind of object to generate
            switch (randomlyPickObject(totalWeight, weights))
            {
                case ObjectType::EMPTY: //If empty, don't add anything
                    break;
                case ObjectType::CHICKEN: //Add chicken
                    farm->addObject(new Chicken(j, i));
                    break;
                case ObjectType::COW: //Add cow
                    farm->addObject(new Cow(j, i));
                    break;
                case ObjectType::EGG: //Add egg
                    farm->addObject(new Egg(j, i));
                    break;
                case ObjectType::GRASS: //Add grass
                    farm->addObject(new Grass(j, i));
                    break;
                default:
                    break;
            }
        }
    }
}
