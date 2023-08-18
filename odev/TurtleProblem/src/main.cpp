#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

class Environment;

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

    Turtle(std::string &turtleName, float initialX, float initialY, float initialDirection, float initialVelocity)
    {
        this->turtleName = turtleName;
        posX = initialX;
        posY = initialY;
        isWet = false;
        direction = initialDirection;
        velocity = initialVelocity;
    }

    void updatePosition(float stepTime)
    {
        posX += stepTime * cosf(direction * M_PI / 180) * velocity;
        posY += stepTime * sinf(direction * M_PI / 180) * velocity;
    }

    void printPosition()
    {
        std::cout << "Turtle Position: (" << posX << ", " << posY << ")\n";
    }
};

class Waterjet
{
public:
    float posX;
    float posY;
    float angleToGround; // Angle with respect to x-axis, beta.
    float wetAreaAngle;  // Wet area angle, alpha
    float wetAreaRadius; // Wet area radius, r

    Waterjet(float posX, float posY, float angleToGround, float wetAreaAngle, float wetAreaRadius)
    {
        this->posX = posX;
        this->posY = posY;
        this->angleToGround = angleToGround;
        this->wetAreaAngle = wetAreaAngle;
        this->wetAreaRadius = wetAreaRadius;
    }

    void updateAngleToGround()
    {
        angleToGround += wetAreaAngle;
        if (angleToGround >= 360)
            angleToGround -= 360;
    }
};

class Logger
{
private:
    std::ofstream logFile;

public:
    Logger(const std::string &filename);
    ~Logger();
    void logGameState(int timeStep, const std::vector<Turtle> &turtles, const std::vector<Waterjet> &waterjets, const std::vector<Turtle> &deadTurtles);
};

Logger::Logger(const std::string &filename)
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

void Logger::logGameState(int timeStep, const std::vector<Turtle> &turtles, const std::vector<Waterjet> &waterjets, const std::vector<Turtle> &deadTurtles)
{
    logFile << "Time Step: " << timeStep << "\n";

    // Log waterjet information
    for (const Waterjet &waterjet : waterjets)
    {
        logFile << "Waterjet Position: (" << waterjet.posX << ", " << waterjet.posY << ")\n";
        logFile << "Waterjet Angle: " << waterjet.angleToGround << "\n";
    }

    // Log turtle information
    for (const Turtle &turtle : turtles)
    {
        logFile << "Turtle Position: (" << turtle.posX << ", " << turtle.posY << ")\n";
        logFile << "Turtle Wet: " << (turtle.isWet ? "Yes" : "No") << "\n";
    }

    // Log dead turtle information
    for (const Turtle &turtle : deadTurtles)
    {
        logFile << "Turtle Position: (" << turtle.posX << ", " << turtle.posY << ")\n";
        logFile << "Turtle Wet: " << (turtle.isWet ? "Yes" : "No") << "\n";
    }
    logFile << "-------------------------------" << std::endl;
}

class ObjectInteraction
{
public:
    ObjectInteraction();
    float calculateTurtleToWaterjetDistance(Waterjet &waterjet, Turtle &turtle);
    float calculateTurtleToWaterjetAngle(Waterjet &waterjet, Turtle &turtle);
    bool checkTurtleIsInWetArea(Waterjet &waterjet, Turtle &turtle);
    bool checkEnvCollision(Turtle &turtle, Environment *env);
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
    // // assumed env coords given according the variable names;
    // // assumed turtle is one pixel object at its positions
    bool isValidForX = turtle.posX > env->leftBottom.posX && turtle.posX > env->leftTop.posX && turtle.posX < env->rightBottom.posX && turtle.posX < env->rightTop.posX;
    bool isValidForY = turtle.posY < env->leftTop.posY && turtle.posY > env->leftBottom.posY && turtle.posY < env->rightTop.posY && turtle.posY > env->rightBottom.posY;
    if (isValidForX && isValidForY)
    {
        return true;
    }
    return false;
}

class Environment
{
private:
    ObjectInteraction connector;
    // Logger logger;
public:
    Point leftTop;
    Point leftBottom;
    Point rightTop;
    Point rightBottom;
    std::vector<Turtle> turtleList;
    std::vector<Waterjet> waterjetList;

    Environment(Point &lTop, Point &lBottom, Point &rBottom, Point &rTop)
    {
        leftTop = lTop;
        leftBottom = lBottom;
        rightBottom = rBottom;
        rightTop = rTop;
    }

    void addTurtle(Turtle turtle)
    {
        turtleList.push_back(turtle);
    }

    void addWaterjet(Waterjet waterjet)
    {
        waterjetList.push_back(waterjet);
    }

    // set current alive turtles
    void setTurtlesLives();

    // set current wet turtles
    void setWetInformationTurtles();

    void runSimulation(float stepTime, float simulationTime);
};

void Environment::setTurtlesLives()
{
    std::vector<Turtle> newAliveTurtles;
    for (int i = 0; i < turtleList.size(); i++)
    {
        if (!connector.checkEnvCollision(turtleList[i], this))
        {
            newAliveTurtles.push_back(turtleList[i]);
        }
    }
    turtleList = newAliveTurtles;
}

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
            if (connector.checkTurtleIsInWetArea(waterjetList[j], turtleList[i], waterjetList[j].angleToGround, waterjetList[j].wetAreaAngle))
            {
                turtleList[i].isWet = true;
                // print turtle name
                std::cout << "Current wet Turtle: " << turtleList[i].turtleName << "\n";
                turtleList[i].printPosition();
            }
        }
    }
}

void Environment::runSimulation(float stepTime, float simulationTime)
{
    // // currentTime += stepTime;

    // // Update waterjets angle
    // // Update turtles positions
    // // Math calculations
    // // Dead check
    // // Wet check
    // // Print or log
    // // List updates

    // // preparing environment

    // float currentTime = stepTime;
    // while (currentTime <= simulationTime)
    // {
    // }
}

int main(int argc, const char *argv[])
{

    return 0;
}