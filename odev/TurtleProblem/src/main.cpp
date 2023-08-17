#include <iostream>
#include <list>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
// ahaha
// öldüğü konumu da tutacaksın loglayacaksın

/*

    Initial inputs:
        1) Turtle velocity, initial coordinate, x ekseni ile aci
        2) Fiskiye initial coordinate, r (yaricapi degeri) , angleToGround (x eksenine gore aci), alfa (taradigi alan) acisi (birim zamandaki taramasi gereken aci)

    Simulasyon ne zaman bitecek?
        a) butun turtler oldugunde (dis areaya vurunca)
        b) Simulasyon end time'i baslangicta verilebilir.

    Simulasyon envr height ve width

    - Fiskiye islatinca herhangi bir turtleyi, turtle name'ini printle


    1- Fiskiye - Turtle arasindaki uzaklik bul. (r ile karsilastir)
    2- Teta acisi hesaplicaz (bunu line kurma denklemindeki arctanh'dan bulmak lazim)

Environmentta turtle öldü mü ölmedi mi kontrol et

    Game Loop:
        1-Init objects
        2-Increment time, update coordinate
        3-

*/
class Turtle
{
public:
    std::string turtleName;
    bool isWet = false;
    float posX;
    float posY;
    float direction; // Direction of the turtle, degrees
    float velocity;  // Velocity of the turtle, meters/second

    Turtle(std::string turtleName, float initialX, float initialY, float initialDirection, float initialVelocity)
    {
        this->turtleName = turtleName;
        posX = initialX;
        posY = initialY;
        direction = initialDirection;
        velocity = initialVelocity;
    }

    void updatePosition(float stepTime)
    {
        posX += stepTime * cos(direction * M_PI / 180) * velocity;
        posY += stepTime * sin(direction * M_PI / 180) * velocity;
    }

    void printPosition()
    {
        std::cout << "Turtle Position: (" << posX << ", " << posY << ")" << std::endl;
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

    void UpdateAngleToGround()
    {
        angleToGround += effectiveAngle;
        if (angleToGround >= 360)
            angleToGround -= 360;
    }
};

class Environment
{
private:
    MathCalculator obj;

public:
    int height;
    int width;
    std::list<Turtle> turtleList;
    std::vector<Waterjet> waterjetList;

    Environment(int height, int width)
    {
        this->width = width;
        this->height = height;
    }

    void addTurtle(Turtle turtle)
    {
        turtleList.push_back(turtle);
    }
    void addWaterjet(Waterjet waterjet)
    {
        waterjetList.push_back(waterjet);
    }
    // check turtle is dead or not (method) - mertcevk

    // check turtle is wet or not (method) - mertcevik

    // utku
    void runSimulation(float stepTime, float simulationTime)
    {
        currentTime += stepTime;

        // Update waterjets angle
        // Update turtles positions
        // Math calculations
        // Dead check
        // Wet check
        // Print or log
        // List updates

        // preparing environment

        float currentTime = stepTime;
        while (currentTime <= simulationTime)
        {
        }
    }
};

class MathCalculator
{
private:
public:
    MathCalculator();
    ~MathCalculator();
    float calculateTurtleToWaterjet(Waterjet &w1, Turtle &turtle);
    bool checkTurtleIsInWetArea(Waterjet &w1, Turtle &turtle, float angleToGround);
};

float MathCalculator::calculateTurtleToWaterjet(Waterjet &water, Turtle &turtle)
{
    float dx = water.posX - turtle.posX;
    float dy = water.posY - turtle.posY;
    return sqrt(dx * dx + dy * dy);
}

bool MathCalculator::checkTurtleIsInWetArea(Waterjet &w1, Turtle &turtle, float angleToGround)
{

    float distance = calculateTurtleToWaterjet(w1, turtle);
    if (distance > w1.effectiveDistance)
    {
        return false;
    }
    float dx = abs(w1.posX - turtle.posX);
    float dy = abs(w1.posY - turtle.posY);

    // dx, dy check 0 cases write
}

int main(int argc, const char *argv[])
{

    return 0;
}
