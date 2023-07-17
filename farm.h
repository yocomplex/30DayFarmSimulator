#ifndef FARM_H
#define FARM_H
#include <fstream>

using namespace std;

class FarmObject; //Forward declaration of the FarmObject class
#define FarmObjRef FarmObject*

//Enum for types of objects
enum class ObjectType { UNKNOWN, GRASS, COW, CHICKEN, EGG, EMPTY };

class Farm
{
	FarmObject*** farmLand; //2D array of references to FarmObjects representing the land
	int width, height;

public:
	void clearBuffer(); //Clears out all of the spaces in the farmLand by setting them to nullptr
	void displayFarm(); //Displays the Farm to the terminal
	void addObject(FarmObject* object);
	void deleteObject(int x, int y);
	void replaceObject(FarmObject* originalObj, FarmObject* replaceObj);
	void moveObject(FarmObject* object, int newX, int newY);
	void allocateArray();
	void deallocateArray();
	void updateFarm();
	bool isSpotEmpty(int x, int y);
	ObjectType getObjectTypeAtSpot(int x, int y);
	int getWidth() const { return width; }
	int getHeight() { return height; }

	Farm(int width, int height);
	~Farm();
};

#endif
