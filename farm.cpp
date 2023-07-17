/*
* Name: Joshua Zamora, NSHE: 5006270225, EC ASSIGNMENT 1
* Description: Simulation of a farm using Virtuals, OOP
  and Pseudo-randomness
* Input: implementation files
* Output: 20 days of farm simulation
*/
#include "farm.h"
#include "farm_object.h"
#include "debug.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <ctime>
#include <iomanip>

using namespace std;


//------------------------ Farm functions ------------------------
/* Allocates a 2D array that the farmLand member will point at */
void Farm::allocateArray()
{
    // Allocate array
    farmLand = new FarmObject**[height];
    for (int i = 0; i < height; i++) {
        // Allocate each row
        farmLand[i] = new FarmObject*[width];
        // Initialize each element to nullptr
        for (int j = 0; j < width; j++) {
            farmLand[i][j] = nullptr;
        }
    }
}

/*  deallocate the farmLand array allocated in allocateArray() */
void Farm::deallocateArray()
{
    // Deallocate each object in the array
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (farmLand[i][j] != nullptr) {
                delete farmLand[i][j];
            }
        }
    }
    // Deallocate each row
    for (int i = 0; i < height; i++) {
        delete[] farmLand[i];
    }

    // Deallocate array of rows
    delete[] farmLand;
}

/* Sets the entire farmLand to nullptr. Optional to use */
void Farm::clearBuffer()
{
    int i, j;
    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
            farmLand[i][j] = nullptr;
}

/* Displays the Farm to the terminal */
void Farm::displayFarm()
{
    //Clear the screen
    #ifdef _WIN32
    system("CLS");
    #else
    cout << "\033[2J\033[1;1H";
    #endif


    int i, j;
    //Draw the top of the tank
    cout << setfill('-') << setw(width + 2) << "" 
         << setfill(' ') << endl;
    for (i = 0; i < height; i++)
    {
        cout << "|"; //Draw the left wall of the farm
        for (j = 0; j < width; j++)
        {
            //If the space is not empty, draw the object there
            if (farmLand[i][j] != nullptr)
                cout << farmLand[i][j]->getDrawChar();
            else //Else if the space is empty, draw a blank space
                cout << " ";
        }
        cout << "|" << endl; //Draw the right wall of the farm
    }
    //Draw the top of the farm
    cout << setfill('-') << setw(width + 2) << "" << setfill(' ') << endl; 
}

/* Adds an object to the farm at the given position. 
If the position is not empty, does not add the
object. Logs information to the debug log. */
void Farm::addObject(FarmObject* object)
{
    Position pos = object->getPosition();
    //If the space is not empty, write a message
    if (farmLand[pos.y][pos.x] != nullptr)
    {
        DebugLogger::writeToDebugLog("Could not add " +
            FarmObject::objectTypeToString(object->getObjectType()) +
            " at position (" 
            + to_string(pos.x) + ", " + to_string(pos.y) + ")" 
            + " because a " + 
            FarmObject::objectTypeToString(
                farmLand[pos.y][pos.x]->getObjectType()) +
            " is already there"
        );
        return;
    }

    //If it is empty, add the object
    farmLand[pos.y][pos.x] = object;
    DebugLogger::writeToDebugLog("Added a " +
        FarmObject::objectTypeToString(object->getObjectType()) +
        " at position (" + to_string(pos.x) + ", " 
        + to_string(pos.y) + ")");
}

/* Deletes an object at the given (x, y) position and 
sets the spot to nullptr. If there is no object there, 
does nothing. Logs the result to the debug log. */
void Farm::deleteObject(int x, int y)
{
    //If the space is empty, then it shouldn't be deleted
    if (farmLand[y][x] == nullptr)
    {
        DebugLogger::writeToDebugLog(
            "Tried to delete nothing at position " +
        to_string(x) + ", " + to_string(y) + ")");
        return;
    }

    //Print the object before it gets deleted
    DebugLogger::writeToDebugLog("Deleted a " +
        FarmObject::objectTypeToString(
            farmLand[y][x]->getObjectType()) +
        " at position (" + to_string(x) + ", " + to_string(y) + ")");
    delete farmLand[y][x];
    farmLand[y][x] = nullptr;
}

/* Replaces the originalObj with the replacement object 
in the Farm. This will put the replacement object in
the same position as the original object was. This 
deletes the old object. Logs the result to the debug log.*/
void Farm::replaceObject(FarmObject* originalObj, FarmObject* replaceObj)
{
    Position originalPos = originalObj->getPosition();

    DebugLogger::writeToDebugLog("Replaced a " +
        FarmObject::objectTypeToString(originalObj->getObjectType()) +
        " with a " +
        FarmObject::objectTypeToString(replaceObj->getObjectType()) +
        " at position (" + to_string(originalPos.x) + ", " 
        + to_string(originalPos.y) + ")");

    delete originalObj;
    farmLand[originalPos.y][originalPos.x] = replaceObj;
}

/* Moves the object from its given position to the 
given (x, y) position. Updates the object's position if successful
If the position is filled or out of bounds, does not move the object.
Writes the result to the debug log. */
void Farm::moveObject(FarmObject* object, int newX, int newY)
{
    //If the space is not empty, write a message
    if (farmLand[newY][newX] != nullptr)
    {
        DebugLogger::writeToDebugLog("Could not move " +
            FarmObject::objectTypeToString(object->getObjectType()) +
            " to position (" 
            + to_string(newX) + ", " + to_string(newY) + ")" +
            " because a " +
            FarmObject::objectTypeToString(
                farmLand[newY][newX]->getObjectType()) +
            " is already there"
        );
        return;
    }

    //If it is empty, move the object
    Position objPos = object->getPosition();
    farmLand[newY][newX] = object;
    farmLand[objPos.y][objPos.x] = nullptr; //Clear the old space out
    //Update the position for the moved object
    object->setPosition(newX, newY);
    DebugLogger::writeToDebugLog("Moved a " +
        FarmObject::objectTypeToString(object->getObjectType()) +
        " to position (" + to_string(newX) + ", " 
        + to_string(newY) + ")");
}

/*Updates all of the objects on the Farm*/
void Farm::updateFarm()
{
    int i, j;
    for(i = 0; i < height; i++)
        for (j = 0; j < width; j++)
        {
            //Update any object that exists at the current spot
            if (farmLand[i][j] != nullptr)
                farmLand[i][j]->update();
        }
}

/* Tells whether the spot at the given (x, y) 
position is currently empty */
bool Farm::isSpotEmpty(int x, int y)
{
    //If out of bounds, spot is invalid
    if (x < 0 || x >= width || y < 0 || y >= height)
        return false;

    return farmLand[y][x] == nullptr; //If nullptr, space is empty
}

/* Tells what object is at the given (x, y) position */
ObjectType Farm::getObjectTypeAtSpot(int x, int y)
{
    //If the spot is empty, return empty
    if (farmLand[y][x] == nullptr)
        return ObjectType::EMPTY;

    //Else return the object type
    return farmLand[y][x]->getObjectType();
}

/* Initializes the Farm to be of the given dimensions. 
Sets the width and height and then allocates
the 2D farmLand array */
Farm::Farm(int width, int height)
{
    this->width = width;
    this->height = height;
    allocateArray();
}

/* Destructor to deallocate the 2D farmLand array */
Farm::~Farm()
{
    deallocateArray();
}