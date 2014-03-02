#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHWONKA.h>
#include <FEHServo.h>
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

    //calibrate servos
    LRservo.SetMin(500); //right-most
    LRservo.SetMax(2366); //left-most
    UDservo.SetMin(500); //on the ground
    UDservo.SetMax(2328); //in the air

    /*
     *rps.InitializeMenu();
     *rps.Enable();
     *int ovenPresses = rps.Oven();
     */

    //move arm to straight
    LRservo.SetDegree(30);
    UDservo.SetDegree(30);


    LCD.WriteLine("Waiting for the Light!");
    int cdsVal = checkCDS(cdsCell);
    while(cdsVal > 9){
        LCD.Clear(CLEARCOLOR);
        LCD.WriteLine(cdsVal);
        cdsVal = checkCDS(cdsCell);
    }

    LCD.WriteLine("Move Forward.");
    moveForward(34);
    LCD.WriteLine("Moved to Pin.");
    Sleep(3.0);
    LCD.WriteLine("Move Arm Up.");
    UDservo.SetDegree(60);
    Sleep(1.0);
    LCD.WriteLine("Move Backward");
    moveBackward(3);
    Sleep(1.0);
    LCD.WriteLine("Turning Right.");
    turnRight(100);
    Sleep(2.0);
    moveForward(30);

    //celebrate
    while(true){
        UDservo.SetDegree(0);
        Sleep(800);
        UDservo.SetDegree(40);
        Sleep(800);
    }
    return 0;
}

