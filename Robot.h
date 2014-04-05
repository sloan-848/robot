#ifndef ROBOT_H
#define ROBOT_H
#include <math.h>
#include <FEHIO.h>
#include <FEHServo.h>
#include <FEHMotor.h>
#include <FEHLCD.h>
#include <FEHUtility.h>
#include <FEHWONKA.h>
#include <string.h>
#include <time.h>
#define START 0
#define SCOOP 1

/*
 *Class for the robot. Includes sensor and motor pointers and methods to control the robot.
 */
class Robot{
public:
    Robot();

    //public methods for control and input
    bool cdsStart();
    int cdsColor();

    void moveForward(float distance, int power);
    void moveForward(float distX, float distY, int power);
    void forwardToXPoint(float pointX, int power);
    void moveBackward(float distance, int power);
    void moveBackward(float distX, float distY, int power);
    void moveBackwardToLight(float timeOut, int power, int lightLevel);
    void timeForward(float time, int power);
    void timeBack(float time, int power);

    void turnLeft(int degrees);
    void turnLeftCheck(float degrees, char directionC, int direction);
    void turnRightCheck(float degrees, char directionC, int direction);
    void turnRight(int degrees);
    void turnRightTime(float time);
    void turnRightONE(float destHeading);
    void turnLeftToHeading(float degree);
    void turnRightToHeading(float degree);

    void findAngle();
    void setArmAngle(int LRangle, int UDangle);
    int getOvenCount();
    void wait(float time);

    float getX();
    float getY();
    float getHeading();

    bool getSwitchPress();


    int checkCDS();


    //Light Constants
    int const static BLUELIGHT = 1;
    int const static REDLIGHT = 2;

    //Speed Constants
    int const static SLOW = 40;
    int const static MEDIUM = 65;
    int const static FAST = 70;
    int const static SUPERFAST = 80;
    int const static MAX = 95;

    //Time Constants
    float const static SHORT = .20;
    float const static LONG = 2.0;

    //Diagnostic Methods
    bool validRPS();
    bool validShafts();



private:
    //initialize pointers to all objects
    FEHMotor* leftMotor;
    FEHMotor* rightMotor;

    AnalogInputPin* cdsCell;

    FEHServo* LRservo;
    FEHServo* UDservo;

    FEHWONKA* rps;

    FEHEncoder* leftEncoder;
    FEHEncoder* rightEncoder;

    DigitalInputPin* switch1;
    DigitalInputPin* switch2;
    DigitalInputPin* switch3;

    //initialize constants and private values
    float PI;
    int TOTALCOUNTS;
    float WHEELRADIUS;
    int ovenCount;
    int startLightValue;
    int scoopLightValue;

    float initX;
    float initY;

    //Private methods; used internally

    void printStartScreen();

};
#endif // ROBOT_H
