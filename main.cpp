/*#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHWONKA.h>
#include <FEHServo.h>*/
#include "robot.h"

#define FONTCOLOR FEHLCD::White
#define CLEARCOLOR FEHLCD::Scarlet

int main(void)
{

    LCD.Clear(CLEARCOLOR);
    LCD.SetFontColor(FONTCOLOR);

    Robot hal;


    hal.setArmAngle(30,30);

    Sleep(1.0);

    if(!hal.validRPS()){
        LCD.Clear(CLEARCOLOR);
        LCD.WriteLine("RPS FAIL. LOL.");
        return 0;
    }
    else{
        LCD.Clear(CLEARCOLOR);
        LCD.WriteLine("RPS Good.");
        Sleep(2.0);
    }

    LCD.WriteLine("Waiting for light");
    while(!hal.cdsReady(START));
    LCD.WriteLine("Lets do it.");
    Sleep(1.0);
    LCD.Clear(CLEARCOLOR);
    LCD.WriteLine("Moving to in-front of skid.");
    hal.moveForward(30);

    Sleep(.5);

    LCD.WriteLine("Turning Left.");
    hal.turnLeft(90);

/*
    Sleep(.5);

    LCD.WriteLine("Lowering Arm.");
    hal.setArmAngle(20,8);

    Sleep(.5);

    LCD.WriteLine("Moving into position");
    hal.moveForward(15);

    Sleep(.5);

    LCD.WriteLine("Raising Arm.");
    hal.setArmAngle(20,22);

    Sleep(.5);

    LCD.WriteLine("Moving backwards");
    hal.moveBackward(15);

    Sleep(.5);

    LCD.WriteLine("Turn Left");
    hal.turnLeft(2300);

    Sleep(.5);

    LCD.WriteLine("Move Down Ramp.");
    hal.moveBackward(35);

    Sleep(.5);

    LCD.WriteLine("Turn Left.");
    hal.turnLeft(1300);

    Sleep(.5);

    LCD.WriteLine("Move into chiller");
    hal.moveForward(8);
*/
    return 1;
}

