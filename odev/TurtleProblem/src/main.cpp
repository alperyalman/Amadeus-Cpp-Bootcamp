/**
 * @file main.cpp
 * @author Utku Selim Koçoğu, Mert Ali Tombul, Mert Çevik, Alper Yalman
 * @brief Turtle problem
 * @version 0.1
 * @date 2023-08-19
 *
 * @copyright Copyright (c) 2023
 *
 */
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

/**
 * @brief Generate (x,y) points in cartesian coordinate system.
 *
 * Point class is used to represent a point in 2D cartesian coordinate system usin x-axis and y-axis coordinate values.
 */
class Point
{
public:
    float posX;
    float posY;
    /**
     * @brief Construct a new Point object
     *
     * @param x Value of x coordinate
     * @param y Value of y coordinate
     */
    Point(float x, float y) : posX(x), posY(y) {}
    Point()
    {
        posX = 0;
        posY = 0;
    }
};

/**
 * @brief Generete Turtle object with it's specific properties.
 *
 * Turtle class is used to represent a turtle with it's specific information such as name, position, direction, velocity,
 * and isWet.
 */
class Turtle
{
public:
    std::string turtleName;
    bool isWet;
    float posX;
    float posY;
    float direction; // Direction of the turtle, degrees
    float velocity;  // Velocity of the turtle, meters/second

    /**
     * @brief Construct a new Turtle object
     *
     * @param turtleName Turtle name
     * @param initialX Initial value of x coordinate, m
     * @param initialY Initial value of y coordinate, m
     * @param initialDirection Initial direction of the velocity with respect to x-axis, deg
     * @param initialVelocity Initial velocity value, m/s
     */
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

    /**
     * @brief Update turtle position in each time step.
     *
     * @param stepTime Simulation step time.
     */
    void updatePosition(float stepTime)
    {
        posX += stepTime * cosf(direction * M_PI / 180) * velocity;
        posY += stepTime * sinf(direction * M_PI / 180) * velocity;
    }

    /**
     * @brief Get turtle position to print.
     *
     * @return std::stringstream
     */
    std::stringstream printPosition()
    {
        std::stringstream posStr;
        posStr << "(" << posX << ", " << posY << ")\n";
        return posStr;
    }

    /**
     * @brief Print turtle object information when created
     *
     */
    void printTurtleInfo()
    {
        std::cout << "Turtle Name: " << turtleName << " | ";
        std::cout << "Turtle Position: (" << posX << ", " << posY << ") | ";
        std::cout << "Turtle Veclocity: " << velocity << " m/s | ";
        std::cout << "Turtle Direction: " << direction << " deg" << std::endl;
    }
};

/**
 * @brief Generete Waterjet object with it's specific properties.
 *
 * Waterjet class is used to represent a water with it's specific information such as name, position, angle with respect to x-axis,
 * wet area angle and wet area radius.
 *
 */
class Waterjet
{
public:
    std::string waterjetName;
    float posX;
    float posY;
    float angleToGround;
    float wetAreaAngle;
    float wetAreaRadius;

    /**
     * @brief Construct a new Waterjet object
     *
     * @param waterjetName Waterjet name
     * @param posX x coordinate value, m
     * @param posY y coordinate value, m
     * @param angleToGround Waterjet angle with respect to x-axis, beta
     * @param wetAreaAngle Wet area angle, alpha
     * @param wetAreaRadius Wet area radius, r
     */
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

    /**
     * @brief Update waterjet angle in each time step.
     *
     * Waterjet angle with respect to x-axis is updated. In each time step, waterjet angle is increased as wet area angle.
     * The output is range is [0,360).
     */
    void updateAngleToGround()
    {
        angleToGround += wetAreaAngle;
        if (angleToGround >= 360)
            angleToGround -= 360;
    }

    /**
     * @brief Print waterjet object information when created
     *
     */
    void printWaterjetInfo()
    {
        std::cout << "Waterjet Name: " << waterjetName << " | ";
        std::cout << "Waterjet Position: (" << posX << ", " << posY << ") | ";
        std::cout << "Waterjet Initial Direction: " << angleToGround << " deg | ";
        std::cout << "Waterjet Wet Area Angle: " << wetAreaAngle << " deg | ";
        std::cout << "Waterjet Wet Area Radius: " << wetAreaRadius << " m" << std::endl;
    }
};

/**
 * @brief Generete Logger object to create simulation log file.
 *
 * Logger class is used to create log file. All the updated information related to turtles and waterjets are logged into the
 * specified file during the simulation.
 *
 */
class Logger
{
private:
    std::ofstream logFile;

public:
    Logger(std::string filename);
    ~Logger();
    void logGameState(float time, std::vector<Turtle> &turtles, std::vector<Waterjet> &waterjets, std::vector<Turtle> &deadTurtles);
};

/**
 * @brief Construct a new Logger object
 *
 * @param filename Log file name
 */
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

/**
 * @brief Log all the information at the given time
 *
 * @param time Current time
 * @param turtles List of active turtles
 * @param waterjets List of waterjets
 * @param deadTurtles List of dead turtles
 */
void Logger::logGameState(float time, std::vector<Turtle> &turtles, std::vector<Waterjet> &waterjets, std::vector<Turtle> &deadTurtles)
{
    logFile << "Time Step: " << time << "\n";

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

/**
 * @brief Calculate and check turtle and waterjet interactions
 *
 * ObjectInteraction class is used to calculate turtle distance to waterjet, to calculate turtle angle with respect to x-axis,
 * to check if the turtle is in the environment, and to check if the turtle is in wet area.
 */
class ObjectInteraction
{
public:
    ObjectInteraction();
    float calculateTurtleToWaterjetDistance(Waterjet &waterjet, Turtle &turtle);
    float calculateTurtleToWaterjetAngle(Waterjet &waterjet, Turtle &turtle);
    bool checkTurtleIsInWetArea(Waterjet &waterjet, Turtle &turtle);
    bool checkEnvCollision(Turtle &turtle, Environment *env);
};

/**
 * @brief Generate the environment with corner
 *
 * Environment class is used to generate the main game environment and includes functions to create turtles and waterjets,
 * to run the simulation and to check any turtle is wet/dead.
 *
 */

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

/**
 * @brief Construct a new Object Interaction:: Object Interaction object
 *
 */
ObjectInteraction::ObjectInteraction() {}

/**
 * @brief Calulate the distance between the turtle and the waterjet
 *
 * @param waterjet Waterjet object
 * @param turtle Turtle object
 * @return float Distance, m
 */
float ObjectInteraction::calculateTurtleToWaterjetDistance(Waterjet &waterjet, Turtle &turtle)
{
    float dx = waterjet.posX - turtle.posX;
    float dy = waterjet.posY - turtle.posY;
    return sqrtf(dx * dx + dy * dy);
}

/**
 * @brief Calculate the turtle angle with respect to waterjet position
 *
 * @param waterjet Waterjet object
 * @param turtle Turtle object
 * @return float Turtle angle, [0, 360)
 */
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

/**
 * @brief Check if the turtle is in the wet area.
 *
 * @param waterjet Waterjet object
 * @param turtle Turtle object
 * @return true if the turtle is in wet are
 * @return false if the turtle is not in wet are
 */
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

/**
 * @brief Check if the turtle is exceed the environment limits
 *
 * @param turtle Turtle object
 * @param env Environment object
 * @return true if the turtle is in the environment
 * @return false if the turtle is not in the environment
 */
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

/**
 * @brief Construct a new Environment:: Environment object
 *
 * @param lTop Top left corner coordinates
 * @param lBottom Bottom left corner coordinates
 * @param rBottom Bottom right corner coordinates
 * @param rTop Top right corner coordinates
 */
Environment::Environment(Point &lTop, Point &lBottom, Point &rBottom, Point &rTop)
{
    leftTop = lTop;
    leftBottom = lBottom;
    rightBottom = rBottom;
    rightTop = rTop;
}

/**
 * @brief Create turtle randomly within the environment
 *
 * @param name Turtle name
 * @param env Environment object
 * @return Turtle
 */
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

/**
 * @brief Create waterjet randomly within the environment
 *
 * @param name Waterjet name
 * @param env Environment object
 * @return Waterjet
 */
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

/**
 * @brief Update alive and dead turtle lists
 *
 * setTurtlesLives() function checks if any turtle is out of the envrionment in each time step, and updates lists of active
 * turtles and dead turtles accordingly.
 */
void Environment::setTurtlesLives()
{
    // std::vector<Turtle> newAliveTurtles;
    std::vector<Turtle> newDeadTurtles = deadTurtleList;
    std::vector<int> idxKilled;

    for (int i = 0; i < turtleList.size(); i++)
    {
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

/**
 * @brief Update isWet information of turtles
 *
 * setWetInformationTurtles() function checks if any turtle is in the wet area, and updates the turtle's
 * isWet information accordingly.
 */
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

/**
 * @brief Start simulation
 *
 * runcSimulation() function starts simulation with specified step time and max simulation time. In each time step,
 * all the dynamic states of turtles and waterjets are updated and logged into the specified log file.
 *
 * @param stepTime Step time
 * @param simulationTime Maximum simulation time
 */
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

/**
 * @brief Program main function
 *
 * - Create the environment.
 *
 * - Number of turtles and waterjets are user inputs.
 *
 * - Create turtles and waterjets randomly.
 *
 * - Run the simulation.
 *
 */
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