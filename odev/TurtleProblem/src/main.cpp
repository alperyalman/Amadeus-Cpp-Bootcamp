#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <time.h>
#include <sstream> // for string streams
#include <algorithm>
#include <iterator>
#include <cassert>
#include <type_traits>
#include <utility>

class Point
{
public:
    float posX;
    float posY;
    Point(float x, float y) : posX(x), posY(y) {}
    Point()
    {
        posX = 0;
        posY = 0;
    }
};

class Turtle
{
public:
    std::string turtleName;
    bool isWet;
    float posX;
    float posY;
    float direction; // Direction of the turtle, degrees
    float velocity;  // Velocity of the turtle, meters/second

    Turtle(const std::string &turtleName, float initialX, float initialY, float initialDirection, float initialVelocity)
    {
        this->turtleName = turtleName;
        posX = initialX;
        posY = initialY;
        isWet = false;
        direction = initialDirection;
        velocity = initialVelocity;
        printTurtleInfo();
    }

    void updatePosition(float stepTime)
    {
        posX += stepTime * cosf(direction * M_PI / 180) * velocity;
        posY += stepTime * sinf(direction * M_PI / 180) * velocity;
    }

    std::stringstream printPosition()
    {
        std::stringstream posStr;
        posStr << "(" << posX << ", " << posY << ")\n";
        return posStr;
    }

    void printTurtleInfo()
    {
        std::cout << "Turtle Name: " << turtleName << " | ";
        std::cout << "Turtle Position: (" << posX << ", " << posY << ") | ";
        std::cout << "Turtle Veclocity: " << velocity << " m/s | ";
        std::cout << "Turtle Direction: " << direction << " deg" << std::endl;
    }
};

class Waterjet
{
public:
    std::string waterjetName;
    float posX;
    float posY;
    float angleToGround; // Angle with respect to x-axis, beta.
    float wetAreaAngle;  // Wet area angle, alpha
    float wetAreaRadius; // Wet area radius, r

    Waterjet(std::string waterjetName, float posX, float posY, float angleToGround, float wetAreaAngle, float wetAreaRadius)
    {
        this->waterjetName = waterjetName;
        this->posX = posX;
        this->posY = posY;
        this->angleToGround = angleToGround;
        this->wetAreaAngle = wetAreaAngle;
        this->wetAreaRadius = wetAreaRadius;
        printWaterjetInfo();
    }

    void updateAngleToGround()
    {
        angleToGround += wetAreaAngle;
        if (angleToGround >= 360)
            angleToGround -= 360;
    }
    void printWaterjetInfo()
    {
        std::cout << "Waterjet Name: " << waterjetName << " | ";
        std::cout << "Waterjet Position: (" << posX << ", " << posY << ") | ";
        std::cout << "Waterjet Initial Direction: " << angleToGround << " deg | ";
        std::cout << "Waterjet Wet Area Angle: " << wetAreaAngle << " deg | ";
        std::cout << "Waterjet Wet Area Radius: " << wetAreaRadius << " m" << std::endl;
    }
};

class Logger
{
private:
    std::ofstream logFile;

public:
    Logger(std::string filename);
    ~Logger();
    void logGameState(float timeStep, std::vector<Turtle> &turtles, std::vector<Waterjet> &waterjets, std::vector<Turtle> &deadTurtles);
};

Logger::Logger(std::string filename)
{
    logFile.open(filename);
    if (!logFile.is_open())
    {
        std::cerr << "Error opening the log file: " << filename << "\n";
    }
}

Logger::~Logger()
{
    if (logFile.is_open())
    {
        logFile.close();
    }
}

void Logger::logGameState(float timeStep, std::vector<Turtle> &turtles, std::vector<Waterjet> &waterjets, std::vector<Turtle> &deadTurtles)
{
    logFile << "Time Step: " << timeStep << "\n";

    // Log waterjet information
    logFile << "[WATERJETS]" << std::endl;
    for (const Waterjet &waterjet : waterjets)
    {
        logFile << "Waterjet Name: " << waterjet.waterjetName << " | ";
        logFile << "Waterjet Location: (" << waterjet.posX << ", " << waterjet.posY << ") | ";
        logFile << "Waterjet Angle: " << waterjet.angleToGround << "\n";
    }

    // Log turtle information
    logFile << "[ALIVE TURTLES]" << std::endl;
    for (const Turtle &turtle : turtles)
    {
        logFile << "Turtle Name: " << turtle.turtleName << " | ";
        logFile << "Turtle Location: (" << turtle.posX << ", " << turtle.posY << ") | "
                << "Turtle Speed: " << turtle.velocity << " | ";
        logFile << "Turtle Wet: " << (turtle.isWet ? "Yes" : "No") << "\n";
    }

    // Log dead turtle information
    logFile << "[DEAD TURTLES]" << std::endl;
    for (const Turtle &turtle : deadTurtles)
    {
        logFile << "Turtle Name: " << turtle.turtleName << " | ";
        logFile << "Turtle Death Location: (" << turtle.posX << ", " << turtle.posY << ") | ";
        logFile << "Turtle Wet: " << (turtle.isWet ? "Yes" : "No") << "\n";
    }
    logFile << "-------------------------------" << std::endl;
}

class Environment;
class ObjectInteraction
{
public:
    ObjectInteraction();
    float calculateTurtleToWaterjetDistance(Waterjet &waterjet, Turtle &turtle);
    float calculateTurtleToWaterjetAngle(Waterjet &waterjet, Turtle &turtle);
    bool checkTurtleIsInWetArea(Waterjet &waterjet, Turtle &turtle);
    bool checkEnvCollision(Turtle &turtle, Environment *env);
};

class Environment
{
private:
    ObjectInteraction connector;

public:
    float currentTime;
    Point leftTop;
    Point leftBottom;
    Point rightTop;
    Point rightBottom;
    std::vector<Turtle> turtleList;
    std::vector<Turtle> deadTurtleList;
    std::vector<Waterjet> waterjetList;

    Environment(Point &lTop, Point &lBottom, Point &rBottom, Point &rTop);
    Turtle createTurtle(const std::string &name, Environment &env);
    Waterjet createWaterjet(const std::string &name, Environment &env);
    // set current alive turtles
    void setTurtlesLives();
    // set current wet turtles
    void setWetInformationTurtles();
    void runSimulation(float stepTime, float simulationTime);
};

ObjectInteraction::ObjectInteraction() {}

float ObjectInteraction::calculateTurtleToWaterjetDistance(Waterjet &waterjet, Turtle &turtle)
{
    float dx = waterjet.posX - turtle.posX;
    float dy = waterjet.posY - turtle.posY;
    return sqrtf(dx * dx + dy * dy);
}

float ObjectInteraction::calculateTurtleToWaterjetAngle(Waterjet &waterjet, Turtle &turtle)
{
    float turtleAngle;
    float dx = turtle.posX - waterjet.posX;
    float dy = turtle.posY - waterjet.posY;

    turtleAngle = (atan2(dy, dx)) * 180 / M_PI;
    if (turtleAngle < 0)
        turtleAngle += 360;

    return turtleAngle;
}

bool ObjectInteraction::checkTurtleIsInWetArea(Waterjet &waterjet, Turtle &turtle)
{
    // Calculate the distance between waterjet and turtle
    float distance = calculateTurtleToWaterjetDistance(waterjet, turtle);

    // If not in r, return false
    if (distance > waterjet.wetAreaRadius)
    {
        return false;
    }

    // If the turtle is coincident with waterjet, assume that it is getting wet.
    if (distance == 0)
    {
        return true;
    }

    // Calculate the turtle angle with respect to waterjet position
    float turtleAngle = calculateTurtleToWaterjetAngle(waterjet, turtle);

    // Check if the turtle is in the wet area or not
    if ((turtleAngle >= waterjet.angleToGround) && (turtleAngle <= waterjet.angleToGround + waterjet.wetAreaAngle))
        return true;

    return false;
}

bool ObjectInteraction::checkEnvCollision(Turtle &turtle, Environment *env)
{
    // assumed env coords given according the variable names;
    // assumed turtle is one pixel object at its positions
    bool isValidForX = (turtle.posX > env->leftBottom.posX) && (turtle.posX > env->leftTop.posX) && (turtle.posX < env->rightBottom.posX) && (turtle.posX < env->rightTop.posX);
    bool isValidForY = turtle.posY < env->leftTop.posY && turtle.posY > env->leftBottom.posY && turtle.posY < env->rightTop.posY && turtle.posY > env->rightBottom.posY;
    if (isValidForX && isValidForY)
    {
        return true;
    }
    return false;
};

Environment::Environment(Point &lTop, Point &lBottom, Point &rBottom, Point &rTop)
{
    leftTop = lTop;
    leftBottom = lBottom;
    rightBottom = rBottom;
    rightTop = rTop;
}

// [SUGGESTION] random X and Y values can be generated from another function
Turtle Environment::createTurtle(const std::string &name, Environment &env)
{
    // srand(static_cast<unsigned int>(time(0)));

    float randomX = env.leftBottom.posX + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (env.rightBottom.posX - env.leftBottom.posX);
    float randomY = env.leftBottom.posY + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (env.leftTop.posY - env.leftBottom.posY);
    float randomDirection = static_cast<float>(rand() % 360);                                 // Random angle between 0 and 359 degrees
    float randomVelocity = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 10.0f; // Random float between 0 and 10
    Turtle turtle(name, randomX, randomY, randomDirection, randomVelocity);
    return turtle;
}

Waterjet Environment::createWaterjet(const std::string &name, Environment &env)
{
    // srand(static_cast<unsigned int>(time(0)));

    float randomX = env.leftBottom.posX + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (env.rightBottom.posX - env.leftBottom.posX);
    float randomY = env.leftBottom.posY + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (env.leftTop.posY - env.leftBottom.posY);
    float randomAngle = static_cast<float>(rand() % 360);            // Random angle between 0 and 359 degrees
    float randomWetAreaAngle = static_cast<float>(rand() % 21) + 10; // Random angle between 10 and 30 degrees
    float randomWetAreaRadius = static_cast<float>(rand() % 10) + 1; // Random distance between 1 and meters

    Waterjet waterjet(name, randomX, randomY, randomAngle, randomWetAreaAngle, randomWetAreaRadius);
    return waterjet;
}

// [SUGGESTION] dead or alive attribute can be set in checkEnvCollision
void Environment::setTurtlesLives()
{
    // std::vector<Turtle> newAliveTurtles;
    std::vector<Turtle> newDeadTurtles = deadTurtleList;
    std::vector<int> idxKilled;

    for (int i = 0; i < turtleList.size(); i++)
    {
        // if (connector.checkEnvCollision(turtleList[i], this))
        // {
        //     // newAliveTurtles.push_back(turtleList[i]);
        // }
        if (!connector.checkEnvCollision(turtleList[i], this))
        {
            deadTurtleList.push_back(turtleList[i]);
            std::cout << "Current time: " << currentTime << " seconds. " << turtleList[i].turtleName << " killed at the position (x, y) = (" << turtleList[i].posX << ", " << turtleList[i].posY << ")" << std::endl;
            idxKilled.push_back(i);
        }
    }

    for (int i = 0; i < idxKilled.size(); i++)
    {
        std::vector<Turtle>::const_iterator c_itr = std::next(turtleList.cbegin(), idxKilled[i]);
        turtleList.erase(c_itr);
    }

    // deadTurtleList = newDeadTurtles;
    // turtleList = newAliveTurtles;
}

// [SUGGESTION] dead or alive attribute can be set in checkTurtleIsInWetArea
void Environment::setWetInformationTurtles()
{
    // 1 -> wet
    // 0 -> not wet
    std::vector<int> wetInfo;
    for (int i = 0; i < turtleList.size(); i++)
    {
        for (int j = 0; j < waterjetList.size(); j++)
        {
            // TODO: check if turtle is already wet or not [disscuss]
            if (connector.checkTurtleIsInWetArea(waterjetList[j], turtleList[i]))
            {
                turtleList[i].isWet = true;
                std::cout << "Current time: " << currentTime << " seconds. " << turtleList[i].turtleName << " is wet from the position (x, y) = " << turtleList[i].printPosition().str();
            }
            // else koşulu yazılacak mı? (isWet = false)
            // print or log turtle current situation?
        }
    }
}

void Environment::runSimulation(float stepTime, float simulationTime)
{
    std::cout << "Simulation information" << std::endl;
    std::cout << "- Step time is " << stepTime << " seconds." << std::endl;
    std::cout << "- Maximum simulation time is " << simulationTime << " seconds.\n"
              << std::endl;

    Logger logger("LogFile.txt");
    currentTime = 0;
    // Log initial states
    logger.logGameState(currentTime, turtleList, waterjetList, deadTurtleList);

    while (!turtleList.empty() && currentTime <= simulationTime)
    {
        currentTime += stepTime;
        for (int i = 0; i < waterjetList.size(); i++)
        {
            waterjetList[i].updateAngleToGround();
        }
        for (int i = 0; i < turtleList.size(); i++)
        {
            turtleList[i].updatePosition(stepTime);
        }

        // Update turtleList and deadTurtleList.
        setTurtlesLives();

        // Wet area checks
        setWetInformationTurtles();

        logger.logGameState(currentTime, turtleList, waterjetList, deadTurtleList);
    }

    std::cout << "\nSimulation is finished after " << currentTime << " seconds.\n"
              << std::endl;
}

int main(int argc, const char *argv[])
{
    int numOfTurtles;
    int numOfWaterjets;
    Point lTop(0, 100);
    Point lBottom(0, 0);
    Point rBottom(100, 0);
    Point rTop(100, 100);

    // Create environment
    Environment env(lTop, lBottom, rBottom, rTop);
    srand(static_cast<unsigned int>(time(0)));

    // User specified numbers of Turtles and Waterjets will be generated randomly in the environment.
    std::cout << "Enter the number of turtles: ";
    std::cin >> numOfTurtles;

    std::cout << "Enter the number of waterjets: ";
    std::cin >> numOfWaterjets;
    std::cout << "\n"
              << std::endl;

    // Create random turtles and print their info.
    std::cout << "Following turtles are created." << std::endl;
    std::cout << "------------------------------" << std::endl;
    for (int i = 1; i <= numOfTurtles; i++)
    {
        std::ostringstream strT;
        strT << i;
        env.turtleList.push_back(env.createTurtle("Turtle " + strT.str(), env));
    }

    std::cout << "\n"
              << std::endl;

    // Create random waterjets and print their info.
    std::cout << "Following waterjets are created." << std::endl;
    std::cout << "--------------------------------" << std::endl;
    for (int j = 1; j <= numOfWaterjets; j++)
    {
        std::ostringstream strW;
        strW << j;
        env.waterjetList.push_back(env.createWaterjet("Waterjet " + strW.str(), env));
    }

    // Start simulation with step time and maximum simulation time.
    std::cout << "\n***************Simulation Start***************\n"
              << std::endl;

    env.runSimulation(0.1f, 100.0f);

    std::cout << "\n***************Simulation End***************\n"
              << std::endl;

    return 0;
}