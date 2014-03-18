#ifndef ROBOT_H
#define ROBOT_H
#include <math.h>
//#include <FEHIO.h>
#include <FEHIO.h>
#include <FEHServo.h>
#include <FEHMotor.h>
#include <FEHLCD.h>
#include <FEHUtility.h>
#include <FEHWONKA.h>
#include <string.h>
#define START 0
#define SCOOP 1

class Robot{
public:
    Robot();

    void findAngle();
    bool cdsReady(int op);
    int checkCDS();
    void moveForward(float distance);
    void moveBackward(float distance);
    void turnLeft(int time);
    void turnRight(int degrees);
    void setArmAngle(int LRangle, int UDangle);
    int getOvenCount();

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

    float PI;
    int TOTALCOUNTS;
    float WHEELRADIUS;
    int ovenCount;
    int startLightValue;
    int scoopLightValue;

    void printStartScreen();

};
#endif // ROBOT_H
