# 30DayFarmSimulator
***Shout out to Professor Samuel Black for creating this project! 
I thought creating this farm simulator was awesome and it works just right with Virtuals, Object Oriented Programming Philosophies, and Pseudo Randomness***

Overview:
Farmin ain’t an easy job. We’ll be simulating a farm so we can get the same experience with none of the difficult labor
You will have a 2D plot of land representing your farm and all of the creatures on it. Every day, all of the flora and fauna on the farm will go about their day, 
growing, eating, and living their best lives. The Farm will be kept as a 2D array of pointers to objects on the farm, which will be done using a base FarmObject class. 
Every day, all objects on the Farm will be updated to do a daily activity. Grass will grow, cows will graze, chickens will run about, and eggs will incubate. 
Since all of these objects need to do something every day, but differently, they will utilize a single virtual function.
The purpose of this project is to gain practice on virtuals, object oriented programming principles, and learn how to generate and use random numbers.

DebugLogger:
This is a static class that contains helper functions. This class will open a debug log that messages can be logged to along with a 
timestamp of when they were logged. The skeleton utilizes this class to open and close a log and write meaningful messages.

Sample Output:
Use the provided executables *virt_farm.exe* to test. The program can be used with a default seed by simply running it: ./virt_farm. 
