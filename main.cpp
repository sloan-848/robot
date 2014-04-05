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

    //Init Robot
    Robot hal;

    LCD.Clear(CLEARCOLOR);
    hal.setArmAngle(10,90);
    hal.setArmAngle(165,90);

    //Calibrate straightH and ramp position
    LCD.WriteLine("Please put me on an edge");
    Sleep(2.0);
    while(!hal.getSwitchPress());
    float rampX = hal.getX();
    float straightH = hal.getHeading();
    float rightH = straightH + 90;
    if(rightH >= 180){
        rightH -= 180;
    }
    Sleep(2.0);
    LCD.WriteLine("Tell me when you're ready!");
    Sleep(1.0);
    while(!hal.getSwitchPress());
    LCD.Clear(CLEARCOLOR);


    //Wait for light
    LCD.WriteLine("Waiting for light.");
    while(!hal.cdsStart());
    LCD.WriteLine("Lets do it!");
    hal.setArmAngle(165,170);
    hal.timeForward(1.0,hal.MEDIUM);

    //Press button
    for(int i = 0; i < hal.getOvenCount(); i++){
        hal.setArmAngle(165,170);
        hal.setArmAngle(180,170);
    }
    hal.setArmAngle(165,170);
    hal.timeBack(.5,hal.SLOW);
    hal.setArmAngle(167,121);
    hal.setArmAngle(0,121);  //EO - 3/23

    //Move to Switch
    hal.turnRight(80);
    hal.moveForward(24,hal.FAST);
    hal.turnLeftToHeading(straightH);
    hal.setArmAngle(116,165);
    hal.timeForward(2,hal.SUPERFAST);

    //Flip switch
    hal.setArmAngle(116,100);
    hal.setArmAngle(116,165);

    //Move to skid
    hal.timeForward(.5,hal.SUPERFAST);
    hal.moveBackward(0,5.5,hal.MEDIUM);
    hal.turnLeftToHeading(rightH);
    hal.setArmAngle(116,130);
    hal.setArmAngle(20,165);
    hal.moveBackward(9, hal.MEDIUM);  //EO - 3/23
    hal.turnRightToHeading(straightH);

    //Pick up skid
    hal.setArmAngle(20,5);
    hal.timeForward(2.2, hal.MEDIUM);
    hal.setArmAngle(20,25);
    Sleep(.2);
    hal.setArmAngle(20,50);
    hal.moveBackward(1.0,hal.SLOW);

    //Move to top of ramp
    hal.moveBackward(9,hal.MEDIUM);
    hal.turnLeftToHeading(rightH);
    hal.forwardToXPoint(rampX+4.0,hal.MEDIUM);
    hal.turnRightONE(straightH);

    //Move to light
    LCD.WriteLine("Moving down the ramp.");
    hal.moveBackwardToLight(7.5,hal.MEDIUM,38);  //TODO: adjust value of 30 in calibration

    //Read light
    LCD.WriteLine("Reading Light");
    while(hal.cdsColor() == 0);
    LCD.Write("COLOR: ");
    if(false){
        LCD.WriteLine("Red");

        //Move to Red Counter
        hal.turnLeft(35);
        hal.moveBackward(11,hal.MEDIUM);
        hal.turnRight(35);
        hal.timeBack(3,hal.SUPERFAST);


        //Drop Scoop


        //Move to chiller
        hal.moveForward(4,hal.MEDIUM);
        hal.turnLeft(90);
        hal.moveForward(11, hal.MEDIUM);  //EO - 3/23
        hal.turnRight(45);
        hal.moveForward(5,hal.MEDIUM);  //Guess - 3/23
        hal.turnRight(45);
        hal.setArmAngle(20,20);
        hal.moveForward(3, hal.MEDIUM);  //Distance for skid to be inside chiller

    }
    else if(true){
        LCD.WriteLine("Blue");

        //Move to Red Counter
        hal.turnRight(40);
        hal.moveBackward(12,hal.MEDIUM);
        hal.turnLeft(35);
        hal.timeBack(.75,hal.MAX);


        //Drop Scoop


        //Move to chiller
        hal.moveForward(2,hal.MEDIUM);
        hal.turnLeft(15);  //EO - 3/23
        hal.moveForward(6.5,hal.FAST); //Guess - 3/23
        hal.turnRightToHeading(straightH);
    }
    else{
        LCD.WriteLine("I'm Broken!");
        hal.timeBack(3.0, hal.MEDIUM);
        hal.moveForward(5.0, hal.MEDIUM);
        hal.turnLeft(90);
        hal.setArmAngle(20,20);
        hal.turnRight(45);
        hal.moveForward(5,hal.MEDIUM);  //Guess - 3/23
        hal.turnRight(45);
        hal.moveForward(3, hal.MEDIUM);  //Distance for skid to be inside chiller
    }

    //Drop skid in chiller
    hal.setArmAngle(20,0);
    hal.moveForward(5.0,hal.MEDIUM);
    hal.moveBackward(5.0,hal.MEDIUM);
    hal.moveBackward(4.0, hal.MEDIUM);
    hal.setArmAngle(20,135);
    hal.setArmAngle(116,135);
    hal.setArmAngle(116,165);

    //Square up, prep for ramp ascension
    hal.turnLeft(80);
    hal.timeForward(2.0, hal.FAST);
    hal.moveBackward(11, hal.FAST);  //EO - 3/23
    hal.turnRight(90);

    //Move up ramp and into ChargeZone
    hal.moveForward(31, hal.SUPERFAST);
    hal.turnLeft(95);
    hal.moveForward(40, hal.SUPERFAST);

    return 1;
}

