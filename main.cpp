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
        hal.turnLeft(35);
        Sleep(1.0);

        LCD.WriteLine("Backing up to drop");
        hal.moveBackward(11,50);

        LCD.WriteLine("Turning to match up with drop point");
        hal.turnRight(35);

        LCD.WriteLine("Back up for drop.");
        hal.timeBack(3,50);

        Sleep(2.0);

        LCD.WriteLine("Move Forward");
        hal.moveForward(4,50);

        LCD.WriteLine("Turning Right");
        hal.turnLeft(20);

        LCD.WriteLine("Going Forward");
        hal.moveForward(12,50);

        LCD.WriteLine("Turn to go forward up ramp.");
        hal.turnRight(15);
    }
    else if(hal.cdsColor() == hal.BLUELIGHT){
        LCD.WriteLine("Blue");
        Sleep(1.0);
        hal.turnRight(35);
        Sleep(1.0);

        LCD.WriteLine("Backing up to drop");
        hal.moveBackward(11,50);

        LCD.WriteLine("Turning to match up with drop point");
        hal.turnLeft(35);

        LCD.WriteLine("Back up for drop.");
        hal.timeBack(3,50);

        Sleep(2.0);

        LCD.WriteLine("Move Forward");
        hal.moveForward(4,50);

        LCD.WriteLine("Turning Right");
        hal.turnRight(25);

        LCD.WriteLine("Going Forward");
        hal.moveForward(12,50);

        LCD.WriteLine("Turn to go forward up ramp.");
        hal.turnLeft(20);
    }
    else{
        LCD.WriteLine("I Lost The Light!");
        return 0;
    }

    LCD.WriteLine("Go forward up ramp.");
    hal.moveForward(25,70);

    LCD.WriteLine("Turning Left");
    hal.turnLeft(90);

    LCD.WriteLine("To the charger!");
    hal.moveForward(40,60);


    return 1;
}

