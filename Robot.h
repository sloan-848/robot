#ifndef ROBOT_H
#define ROBOT_H
#include <math.h>
/*
#include <FEHIO.h>
#include <FEHServo.h>
#include <FEHMotor.h>
#include <FEHLCD.h>
#include <FEHUtility.h>
#include <FEHWONKA.h>
*/
#include "robot.cpp"

class Robot{
public:
    Robot();

    void findAngle();
    int checkCDS();
    void moveForward(float distance);

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

    ButtonBoard* myButtons;
};
#endif // ROBOT_H
