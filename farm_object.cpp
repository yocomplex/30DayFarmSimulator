#include "farm_object.h"
#include "debug.h"
#include <cstdlib>
#include <queue>
#include <vector>

using namespace std;

//------------------------ FarmObject Functions ------------------------
/* FarmObject constructor */
FarmObject::
FarmObject(char drawChar, ObjectType objectType, int x, int y)
{
	//Initialize all of the members to the given values
	this->objectType = objectType;
	this->drawChar = drawChar;
	position.x = x;
	position.y = y;
	count++;

	DebugLogger::writeToDebugLog("Created an object of type " 
	+ objectTypeToString(objectType) + " at (" + to_string(x) + ", " 
	+ to_string(y) + ")");
}

/* Converts an ObjectType to a nice string */
string FarmObject::objectTypeToString(ObjectType type)
{
	switch (type)
	{
		case ObjectType::CHICKEN:
			return "Chicken";
		case ObjectType::COW:
			return "Cow";
		case ObjectType::EGG:
			return "Egg";
		case ObjectType::GRASS:
			return "Grass";
		case ObjectType::EMPTY:
			return "Empty";
		case ObjectType::UNKNOWN:
			return "Unknown";
		default:
			return "Unknown";
	}
}

//Updates the object on every day
void FarmObject::update()
{
	doDailyActivity();
}

//------------------------ Grass Functions ------------------------

/* Grass constructor to set its initial (x, y) position. 
You can use this as a reference for the other constructors.
This sets the Grass's drawChar to 'G', its type to GRASS, 
and the x and y positions to the given x and y */
Grass::Grass(int x, int y) : FarmObject('G', ObjectType::GRASS, x, y)
{
	//Do nothing
}

/* Picks a random spot in the 9 tiles adjacent to this grass tile */
Position Grass::pickRandomSpot()
{
	int deltaX = 0;
	int deltaY = 0;

	//Reroll until we don't have 0 in both directions
	while(deltaX == 0 && deltaY == 0) 
	{ 
		deltaX = (rand() % 3) - 1; //Roll a number between 0 and -1
		deltaY = (rand() % 3) - 1;
	}
	return Position(position.x + deltaX, position.y + deltaY);
}

/* Does the daily activity for a piece of grass. 
This is provided for you as an example */
void Grass::doDailyActivity()
{

	//Decide whether or not to generate new grass
	//Get a random number and then there 
	// is a 1 in 6 chance to grow more grass
	if (rand() % 6 == 0)
	{
		//Pick a random adjacent spot to grow the grass in
		Position newPos = pickRandomSpot();
		//If the spot chosen is empty, add a new piece of Grass
		if (farm->isSpotEmpty(newPos.x, newPos.y))
			farm->addObject(new Grass(newPos.x, newPos.y));
	}
}

//------------------------ Cow Functions ------------------------
/* Description*/
Cow::Cow(int x, int y) : FarmObject('C', ObjectType::COW, x, y)
{
	//YOUR CODE HERE : Finish the Cow constructor by calling the base 
	//class constructor to set the drawChar to 'C' and set all members
	//Do nothing in body
}

/* Finds the closest blade of grass to this cow and returns the result. 
If there is no grass within its search radius, 
returns the Cow's position */
Position Cow::findClosestGrass()
{
	queue<Position> candidates; //Spots to check in order
	vector<Position> tried; //Positions that have already been tried
	Position temp, check;
	bool duplicate;
	unsigned int i;

	//Add all the candidates of distance 1
	candidates.push(Position(position.x + 1, position.y));
	candidates.push(Position(position.x - 1, position.y));
	candidates.push(Position(position.x, position.y + 1));
	candidates.push(Position(position.x, position.y - 1));

	//Try all of the candidates until we're out
	while (!candidates.empty())
	{
		temp = candidates.front();
		candidates.pop(); //Remove from the front of the queue
		//If the position is out of bounds, 
		// don't bother checking if grass is there
		if (temp.x < 0 || temp.y < 0 || temp.x >= farm->getWidth() 
		               || temp.y >= farm->getHeight())
			continue;

		//If the manhattan distance is not within the radius
		if (abs(temp.x - position.x) 
		+ abs(temp.y - position.y) > SEARCH_RADIUS)
			continue;

		//Check if the space contains grass
		if (farm->getObjectTypeAtSpot(temp.x, temp.y) 
		    == ObjectType::GRASS)
			return temp;

		// Else add all of the adjacent spaces 
		// to the array that haven't been checked yet
		check = Position(temp.x + 1, temp.y); //right
		duplicate = false;
		for (i = 0; i < tried.size(); i++)
		{
			if (check == tried[i])
			{
				duplicate = true;
				break;
			}
		}
		//Add if not alrady checked
		if (!duplicate) candidates.push(check);

		check = Position(temp.x - 1, temp.y); //left
		duplicate = false;
		for (i = 0; i < tried.size(); i++)
		{
			if (check == tried[i])
			{
				duplicate = true;
				break;
			}
		}
		//Add if not alrady checked
		if (!duplicate) candidates.push(check); 

		check = Position(temp.x, temp.y + 1); //down
		duplicate = false;
		for (i = 0; i < tried.size(); i++)
		{
			if (check == tried[i])
			{
				duplicate = true;
				break;
			}
		}
		//Add if not alrady checked

		if (!duplicate) candidates.push(check);

		check = Position(temp.x, temp.y - 1); //up
		duplicate = false;
		for (i = 0; i < tried.size(); i++)
		{
			if (check == tried[i])
			{
				duplicate = true;
				break;
			}
		}
		//Add if not alrady checked
		if (!duplicate) candidates.push(check);

		//Add the position just tested to the list of tried Positions
		tried.push_back(temp);
	}

	return position;
}

/* Does the Cow’s daily activity of searching and grazing on grass */
void Cow::doDailyActivity()
{
	// Find the closest grass
    Position closestGrass = findClosestGrass();

    // If the Cow is not going to move anywhere, don't move
    // if (closestGrass.x == -1 && closestGrass.y == -1 
	//     || closestGrass == position) {
	if (closestGrass == position) {
		return;
	}
 
    // Check if the destination has grass, remove it if it does
	// and move cow to that spot
    ObjectType destObject = 
	farm->getObjectTypeAtSpot(closestGrass.x, closestGrass.y);
    if (destObject == ObjectType::GRASS) {
        FarmObject::farm->deleteObject(closestGrass.x, closestGrass.y);
		FarmObject::farm->moveObject(this, closestGrass.x, closestGrass.y);
    }
    // // Move the cow to the closest grass
    // if (distance <= 1) {
    //     // Cow is already adjacent to the closest grass
    //     return;
    // }
	// if (dx >= 0 && dy >= 0) {
    //     // Move diagonally to the bottom right
    //     FarmObject::farm->moveObject(this, position.x + 1, position.y + 1);
    // } else if (dx >= 0 && dy < 0) {
    //     // Move diagonally to the top right
    //     FarmObject::farm->moveObject(this, position.x + 1, position.y - 1);
    // } else if (dx < 0 && dy >= 0) {
    //     // Move diagonally to the bottom left
    //     FarmObject::farm->moveObject(this, position.x - 1, position.y + 1);
    // } else {
    //     // Move diagonally to the top left
    //     FarmObject::farm->moveObject(this, position.x - 1, position.y - 1);
    // }
}

//------------------------ Chicken Functions ------------------------

/* Description */
Chicken::Chicken(int x, int y, int xDir, int yDir) 
: FarmObject('H', ObjectType::CHICKEN, xDir, yDir)
{
	this->xDir = xDir;
	this->yDir = yDir;
}


/* Chicken constructor to set the x and y position */
Chicken::Chicken(int x, int y) 
: FarmObject('H', ObjectType::CHICKEN, x, y)
{
	//If no directions were provided, pick random ones
	pickRandomDirections();
}


//Picks a random direction for the Chicken 
// to go between up, down, left, and right
void Chicken::pickRandomDirections()
{
	switch (rand() % 4) //Randomly generate a direction
	{
		case 0: //Up
			yDir = -1;
			xDir = 0;
			return;
		case 1: //Down
			yDir = 1;
			xDir = 0;
			return;
		case 2: //Left
			yDir = 0;
			xDir = -1;
			return;
		case 3: //Right
			yDir = 0;
			xDir = 1;
			return;
	}

}

/* Daily activities for the chicken */
void Chicken::doDailyActivity()
{
	// Handle the Chicken's collision, 
	// keeping the Chicken in bounds of the farm
	handleCollision();

	// Move the chicken in the direction it is moving
    int newX = position.x + xDir;
    int newY = position.y + yDir;
    farm->moveObject(this, newX, newY);

	// Randomly decide whether or not the Chicken 
	// should lay an Egg. If it does, use the layEgg function.
    if (rand() % 10 == 0) {
        layEgg();
    }
}

/* Lays an egg for the chicken in the 
   opposite direction that it is facing */
void Chicken::layEgg()
{
	//If the spot behind the chicken is empty, then lay the egg there
	if(farm->isSpotEmpty(position.x - xDir, position.y - yDir))
		farm->addObject(new Egg(position.x - xDir, position.y - yDir));
}

/* Handles any collisions the chicken may 
   have and flips its direction as needed */
void Chicken::handleCollision()
{
	if (position.x + xDir < 0 || position.x + xDir >= farm->getWidth())
		xDir *= -1;
	if (position.y + yDir < 0 || position.y + yDir >= farm->getHeight())
		yDir *= -1;
}

//------------------------ Egg Functions ------------------------

/* Egg constructor */
Egg::Egg(int x, int y) : FarmObject('E', ObjectType::EGG, x, y)
{
	daysSinceLaid = 0; //Set the egg to have been laid 0 days ago
}

/* 
Increment the daysSinceLaid and checks if the Egg should hatch. 
If the egg hatches, it should be replaced with a Chicken within 
the farm 
*/
void Egg::doDailyActivity()
{
	// Increment the amount of days since the Egg was laid
	daysSinceLaid++;

	// If it's time for the egg to hatch 
	if (daysSinceLaid >= incubationPeriod) {
		//Create a new Chicken at the position of the Egg
		Chicken* newChicken = new Chicken(position.x, position.y);

		// Replace the Egg with the new Chicken 
		// using the farm's replaceObject function
		farm->replaceObject(this, newChicken);
	}
}
