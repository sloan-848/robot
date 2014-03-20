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

  /*LCD.WriteLine("Waiting for light");
    while(!hal.cdsReady(START));
    LCD.WriteLine("Lets do it.");
    Sleep(1.0);
    */

    LCD.Clear(CLEARCOLOR);

    LCD.WriteLine("Flipping Switch");
    hal.setArmAngle(116,122);
    Sleep(.6);

    hal.setArmAngle(116,95);

    LCD.WriteLine("Moving down the ramp.");
    hal.moveBackward(35,55);

    Sleep(.5);

    LCD.WriteLine("Reading Light");
    while(hal.cdsColor() == 0);
    LCD.Write("COLOR: ");
    if(hal.cdsColor() == hal.REDLIGHT){
        LCD.WriteLine("Red");
        Sleep(1.0);
        hal.turnLeft(40);
        Sleep(1.0);

        LCD.WriteLine("Backing up to drop");
        hal.moveBackward(5,55);

        Sleep(2.0);

        LCD.WriteLine("Moving Forward");
        hal.moveForward(5,45);

        LCD.WriteLine("Turning Left");
        hal.turnLeft(30);

        LCD.WriteLine("Going Forward");
        hal.moveForward(5,45);

        LCD.WriteLine("Turn to go forward up ramp.");
        hal.turnRight(30);
    }
    else if(hal.cdsColor() == hal.BLUELIGHT){
        LCD.WriteLine("Blue");
        Sleep(1.0);
        hal.turnRight(35);
        Sleep(1.0);

        LCD.WriteLine("Backing up to drop");
        hal.moveBackward(8,50);

        LCD.WriteLine("Turning to match up with drop point");
        hal.turnLeft(35);

        LCD.WriteLine("Back up for drop.");
        hal.timeBack(5,50);

        Sleep(2.0);

        LCD.WriteLine("Move Forward");
        hal.moveForward(4,50);

        LCD.WriteLine("Turning Right");
        hal.turnRight(15);

        LCD.WriteLine("Going Forward");
        hal.moveForward(12,50);

        LCD.WriteLine("Turn to go forward up ramp.");
        hal.turnLeft(15);
    }
    else{
        LCD.WriteLine("I Lost The Light!");
        return 0;
    }

    LCD.WriteLine("Go forward up ramp.");
    hal.moveForward(28,70);

    LCD.WriteLine("Turning Left");
    hal.turnLeft(90);

    LCD.WriteLine("To the charger!");
    hal.moveForward(40,55);

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

