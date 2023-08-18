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
    Point(float x, float y): posX(x), posY(y) {}
    Point(){ posX = 0; posY = 0;}
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
        std::cout << "Turtle Position: (" << posX << ", " << posY << ")\n" ;
    }
};

class Waterjet
{
public:
    float posX;
    float posY;
    float angleToGround;     // Initial angle with respect to x-axis, beta.
    float effectiveAngle;    // Wetted area angle, alpha
    float effectiveDistance; // Corresponds to circle radius, r

    Waterjet(float posX, float posY, float angleToGround, float effectiveAngle, float effectiveDistance)
    {
        this->posX = posX;
        this->posY = posY;
        this->angleToGround = angleToGround;
        this->effectiveAngle = effectiveAngle;
        this->effectiveDistance = effectiveDistance;
    }

    void updateAngleToGround()
    {
        angleToGround += effectiveAngle;
        if (angleToGround >= 360)
            angleToGround -= 360;
    }
};


class Logger 
{
private:
    std::ofstream logFile;

public:
    Logger(const std::string& filename);
    ~Logger();
    void logGameState(int timeStep, const std::vector<Turtle>& turtles, const std::vector<Waterjet>& waterjets, const std::vector<Turtle>& deadTurtles);
};

Logger::Logger(const std::string& filename)
{
    logFile.open(filename);
    if (!logFile.is_open()) {
        std::cerr << "Error opening the log file: " << filename << "\n";
    }
}

Logger::~Logger()
{
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::logGameState(int timeStep, const std::vector<Turtle>& turtles, const std::vector<Waterjet>& waterjets, const std::vector<Turtle>& deadTurtles)
{
    logFile << "Time Step: " << timeStep << "\n";
    
    // Log waterjet information
    for (const Waterjet& waterjet : waterjets) {
        logFile << "Waterjet Position: (" << waterjet.posX << ", " << waterjet.posY << ")\n";
        logFile << "Waterjet Angle: " << waterjet.angleToGround << "\n";
    }
    
    // Log turtle information
    for (const Turtle& turtle : turtles) {
        logFile << "Turtle Position: (" << turtle.posX << ", " << turtle.posY << ")\n";
        logFile << "Turtle Wet: " << (turtle.isWet ? "Yes" : "No") << "\n";
    }

    // Log dead turtle information
    for (const Turtle& turtle : deadTurtles) {
        logFile << "Turtle Position: (" << turtle.posX << ", " << turtle.posY << ")\n";
        logFile << "Turtle Wet: " << (turtle.isWet ? "Yes" : "No") << "\n";
    }
    logFile << "-------------------------------" << std::endl;
}

class ObjectInteraction
{
public:
    ObjectInteraction();
    float calculateTurtleToWaterjet(Waterjet &w1, Turtle &turtle);
    bool checkTurtleIsInWetArea(Waterjet &w1, Turtle &turtle, float angleToGround,float alpha);
    bool checkEnvCollision(Turtle &turtle,Environment *env);
};

ObjectInteraction::ObjectInteraction() {}

float ObjectInteraction::calculateTurtleToWaterjet(Waterjet &water, Turtle &turtle)
{
    float dx = water.posX - turtle.posX;
    float dy = water.posY - turtle.posY;
    return sqrtf(dx * dx + dy * dy);
}

bool ObjectInteraction::checkTurtleIsInWetArea(Waterjet &w1, Turtle &turtle, float angleToGround,float alpha)
{

    float distance = calculateTurtleToWaterjet(w1,turtle);
    // if not in r, return false

    if(distance > w1.effectiveDistance)
    {
        return false;
    }
    //
    float dx = w1.posX - turtle.posX;
    float dy = w1.posY - turtle.posY;

    if(dx == 0)
    {
        // invalid case for tangent
        // same axis, same x coordinate if distance > w1.effectiveDistance not valid then it is out of area
        return false;
    }

    float tan_value = dy/dx;
    float current_angle_with_x = atanf(tan_value) * 180 / M_PI; // [-90,90]

    float second_arch_line_angle = (static_cast<int>(angleToGround + alpha)) % 360;

    if(current_angle_with_x<0)
    {
        // area 2 or 4
        if(dy<=0 && dx>0)
        {
            // area 2
            float concern_angle = 180 - abs(current_angle_with_x);
            if(second_arch_line_angle>=concern_angle && concern_angle>=angleToGround)
            {
                return true;
            }
            return false;
        }
        else
        {
            // dy>=0 and dx<0 
            // area 4
            float concern_angle = 360 - abs(current_angle_with_x);
            if(second_arch_line_angle>=concern_angle && concern_angle>=angleToGround)
            {
                return true;
            }
            return false;
        }
    } else if(current_angle_with_x>0)
    {
        // 1 or 3 area
        if(dx<0 && dy<=0)
        {
            // area 1
            if (second_arch_line_angle>=current_angle_with_x && current_angle_with_x>=angleToGround)
            {
                return true;
            }
            return false;
        } else 
        {
            // dx > 0, dy > 0
            // area 3
            float concern_angle = 270 - abs(current_angle_with_x);
            if(second_arch_line_angle>=concern_angle && concern_angle>=angleToGround)
            {
                return true;
            }
            return false;
        }
    }
    if(distance<=w1.effectiveDistance)
    {
        return true;
    }
    // same axis, same y coordinate if distance > w1.effectiveDistance not valid then it is out of area
    return false;
}


bool ObjectInteraction::checkEnvCollision(Turtle &turtle,Environment *env)
{
    // // assumed env coords given according the variable names;
    // // assumed turtle is one pixel object at its positions
    bool isValidForX = turtle.posX>env->leftBottom.posX && turtle.posX>env->leftTop.posX && turtle.posX<env->rightBottom.posX && turtle.posX<env->rightTop.posX;
    bool isValidForY = turtle.posY<env->leftTop.posY && turtle.posY>env->leftBottom.posY && turtle.posY<env->rightTop.posY && turtle.posY>env->rightBottom.posY;
    if(isValidForX && isValidForY)
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
    for(int i=0;i<turtleList.size();i++)
    {
        if(!connector.checkEnvCollision(turtleList[i],this))
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
    for(int i=0;i<turtleList.size();i++)
    {
        for(int j=0;j<waterjetList.size();j++)
        {
            // TODO: check if turtle is already wet or not [disscuss]
            if(connector.checkTurtleIsInWetArea(waterjetList[j],turtleList[i],waterjetList[j].angleToGround,waterjetList[j].effectiveAngle))
            {
                turtleList[i].isWet = true;
                // print turtle name
                std::cout<<"Current wet Turtle: "<<turtleList[i].turtleName<<"\n";
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