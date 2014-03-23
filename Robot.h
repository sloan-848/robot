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
#define START 0
#define SCOOP 1

/*
 *Class for the robot. Includes sensor and motor pointers and methods to control the robot.
 */
class Robot{
public:
    Robot();

    //public methods for control and input
    void findAngle();
    bool cdsStart();
    int cdsColor();
    void moveForward(float distance, int power);
    void moveBackward(float distance, int power);
    void timeBack(int time, int power);
    void turnLeft(int time);
    void turnRight(int degrees);
    void setArmAngle(int LRangle, int UDangle);
    int getOvenCount();

    //Constants
    int const static BLUELIGHT = 1;
    int const static REDLIGHT = 2;

    //Diagnostic Methods
    bool validRPS();



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

    //Private methods; used internally
    int checkCDS();
    void printStartScreen();

};
#endif // ROBOT_H
