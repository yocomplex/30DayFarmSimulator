//This header defines the abstract base class for all objects in the Farm as well as simple objects that
//inherit from it like grass and animals
#ifndef FARM_OBJECT_H
#define FARM_OBJECT_H

#include <iostream>
#include <fstream>
#include <string>
#include "farm.h"

using namespace std;

//Simple struct to represent position in 2D space
struct Position {
	int x, y;

	bool operator == (Position other) { return x == other.x && y == other.y; }
	bool operator != (Position other) { return x != other.x || y != other.y; }

	Position() { x = 0; y = 0; }
	Position(int x, int y) { this->x = x; this->y = y; }
};

//Abstract base class that acts as the parent to all types of things that can go in the Farm.
//Defines some basic virtual functions that all other classes will have and can choose to implement
class FarmObject
{
	char drawChar; //Character used for drawing this object to the terminal
	static int count; //Count of how many objects have been made
	ObjectType objectType;
protected:
	Position position; //Position of the object in the 2D farm
	static Farm* farm;

	//Constructor for all FarmObjects to initialize data
	FarmObject(char drawChar = ' ', ObjectType objectType = ObjectType::UNKNOWN, int x = 0, int y = 0);

public:
	virtual void doDailyActivity() = 0; //Abstract function for the object's day, whatever they do
	//virtual void onCollideWithObject(const FarmObject* other) {} //Called when two objects collide. Provides a reference to the object collided with
	void update(); //Called every day for each object

	static string objectTypeToString(ObjectType type); //Returns a nice string for the ObjectType

	char getDrawChar() const { return drawChar; }
	static int getCount() { return count; }
	ObjectType getObjectType() const { return objectType; }
	Position getPosition() const { return position; }
	void setPosition(int x, int y) { position.x = x; position.y = y; }
	static void setFarm(Farm* farmRef) { farm = farmRef; }

	virtual ~FarmObject() {} //Virtual destructor to be safe
};

//Grass class that inherits from the FarmObject
//Grass sits in place and is eaten by animals. Each day, the grass will spawn more in adjacent tiles
class Grass : public FarmObject
{
public:
	Grass(int x, int y);

	void doDailyActivity();
	Position pickRandomSpot();
};

class Cow : public FarmObject
{
	const int SEARCH_RADIUS = 5; //Max manhattan distance to search for grass

public:
	Cow(int x, int y);

	Position findClosestGrass();
	void doDailyActivity();
};

class Chicken : public FarmObject
{
	int xDir, yDir;
public:
	Chicken(int x, int y, int xDir, int yDir);
	Chicken(int x, int y);

	void layEgg();
	void handleCollision();
	void doDailyActivity();
	void pickRandomDirections();
};

class Egg : public FarmObject
{
	int daysSinceLaid;
	const int incubationPeriod = 3;
public:
	Egg(int x, int y);
	void doDailyActivity();
};

#endif
