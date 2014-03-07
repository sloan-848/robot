/*#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHWONKA.h>
#include <FEHServo.h>*/
#include "robot.h"
#include "Function.h"

#define FONTCOLOR FEHLCD::White
#define CLEARCOLOR FEHLCD::Scarlet
#define PI 3.14159265
#define TOTALCOUNTS 20
#define WHEELRADIUS 1.35

//motor initialization
FEHMotor leftMotor(FEHMotor::Motor0);
FEHMotor rightMotor(FEHMotor::Motor1);

//cds cell initialization
AnalogInputPin cdsCell(FEHIO::P0_0);

//arm servo initialization
FEHServo LRservo(FEHServo::Servo0);
FEHServo UDservo(FEHServo::Servo1);

//WONKA unit initialization
//FEHWONKA rps;
FEHEncoder leftEncoder(FEHIO::P1_0);
FEHEncoder rightEncoder(FEHIO::P1_1);

//buttonboard initialization
ButtonBoard buttons (FEHIO::Bank3);

void moveForward(float distance){
    int motorPercent= 50;

    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();


    int estCounts = (TOTALCOUNTS*distance)/(2*PI*WHEELRADIUS);

    leftMotor.SetPercent(motorPercent);
    rightMotor.SetPercent(motorPercent-5);

    while((leftEncoder.Counts() < estCounts )||(rightEncoder.Counts() < estCounts)){
        LCD.Write("Left: ");
        LCD.Write(leftEncoder.Counts());
        LCD.Write("  -  Right: ");
        LCD.WriteLine(rightEncoder.Counts());
        if(leftEncoder.Counts() - rightEncoder.Counts() > 2){
            rightMotor.SetPercent(motorPercent + 10);
        }
        else if(leftEncoder.Counts() - rightEncoder.Counts() < -2){
            rightMotor.SetPercent(motorPercent - 10);
        }
        else{
            rightMotor.SetPercent(motorPercent);
        }
    }

    leftMotor.SetPercent(0);
    rightMotor.SetPercent(0);
}

void moveBackward(float distance){
    int motorPercent= 50;

    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();

    int estCounts = (TOTALCOUNTS*distance)/(2*PI*WHEELRADIUS);

    leftMotor.SetPercent(-1*motorPercent);
    rightMotor.SetPercent((-1*motorPercent)+5);

    while((leftEncoder.Counts() < estCounts )||(rightEncoder.Counts() < estCounts)){
        //Replace with adjusting code eventually
    }

    leftMotor.SetPercent(0);
    rightMotor.SetPercent(0);
}

void turnLeft(int degrees){
    float wheelbaseRadius = 7.0;
    int countDistance = (wheelbaseRadius*TOTALCOUNTS*degrees)/(WHEELRADIUS*360*1.5);

    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    leftMotor.SetPercent(-40);
    rightMotor.SetPercent(40);

    while(rightEncoder.Counts() < countDistance){
        //do nothing
    }
    leftMotor.SetPercent(0);
    rightMotor.SetPercent(0);
}

void turnRight(int degrees){
    float wheelbaseRadius = 7.0;
    int countDistance = (wheelbaseRadius*TOTALCOUNTS*degrees)/(WHEELRADIUS*360*1.75);

    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    leftMotor.SetPercent(40);
    rightMotor.SetPercent(-40);

    while(leftEncoder.Counts() < countDistance){
        //do nothing
    }
    leftMotor.SetPercent(0);
    rightMotor.SetPercent(0);
}


int main(void)
{

    LCD.Clear(CLEARCOLOR);
    LCD.SetFontColor(FONTCOLOR);

    Robot hal;

    hal.setArmAngle(160,50);

    LCD.WriteLine("Waiting for the light.");
    while(hal.checkCDS() < hal.lightValue){
        //wait patiently
    }

    LCD.WriteLine("Moving to in-front of skid.");
    hal.moveForward(20);
    LCD.WriteLine("Turning Left.");
    hal.turnLeft(90);
    LCD.WriteLine("Lowering Arm.");
    hal.setArmAngle(160,20);
    LCD.WriteLine("Moving into position");
    hal.moveForward(3);
    LCD.WriteLine("Raising Arm.");
    hal.setArmAngle(160,30);
    LCD.WriteLine("Turning Right 20 degrees.");
    hal.turnRight(20);
    LCD.WriteLine("Moving backwards");
    //move backwards


    return 0;
}

