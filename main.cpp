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

    //check for valid RPS. Necessary for turning.
    if(!hal.validRPS()){
        LCD.Clear(CLEARCOLOR);
        LCD.WriteLine("RPS FAIL. LOL.");
        //TODO: Run alternative time-based turning program.
        return 0;
    }
    else{
        LCD.Clear(CLEARCOLOR);
        LCD.WriteLine("RPS Good.");
        Sleep(2.0);
    }

    LCD.Clear(CLEARCOLOR);


    //Set initial arm value
    hal.setArmAngle(70,160);  //EO - 3/23


    //wait for light to start
    while(!hal.cdsStart());
    LCD.WriteLine("Lets do it!");


    //press button the required number of times
    for(int i = 0; i < hal.getOvenCount(); i++){
        hal.setArmAngle(0,67);  //EO - 3/23
        Sleep(1.0);
        hal.setArmAngle(21,67);  //EO - 3/23
        Sleep(1.0);
    }
    hal.setArmAngle(0,121);  //EO - 3/23



    //turn to prep for next task
    hal.turnRight(90);


    //move forward to next task
    hal.moveForward(25,hal.MEDIUM);  //EO - 3/23


    //Get into position to flip switch - also squares up
    hal.turnLeft(90);
    hal.timeForward(4,hal.SLOW);


    //Flip switch - PT 3/21
    hal.setArmAngle(116,122);
    Sleep(.6);
    hal.setArmAngle(116,95);


    //Get in position to pull pin
    hal.moveBackward(9, hal.MEDIUM);  //EO - 3/23
    hal.turnRight(90);
    hal.setArmAngle(00,00);  //Arm angle to prep pin pull
    hal.moveForward(13, hal.MEDIUM);  //EO - 3/23


    //Pull pin and release pin
    hal.setArmAngle(00,00);  //Arm angle to pull pin
    hal.moveBackward(.5, hal.SLOW);  //Distance to get even with skid
    hal.turnRight(90);
    hal.timeForward(00, hal.SLOW);  //move to edge of over-hang
    hal.setArmAngle(00,00);  //Arm angle to drag pin off edge
    hal.moveBackward(.5, hal.SLOW);


    //Prep for skid pickup
    hal.turnRight(90);
    hal.turnRight(90);
    hal.setArmAngle(00,00);  //Arm angle prep to pick up skid
    hal.moveForward(8.5, hal.SLOW);  //EO - 3/23
    hal.setArmAngle(00,00);  //Arm angle to raise skid


    //Move to top of ramp
    hal.moveBackward(1.0, hal.MEDIUM);
    hal.turnRight(45);
    hal.moveBackward(7, hal.MEDIUM);  //EO - 3/23
    hal.turnLeft(45);


    //Move down ramp - PT 3/21
    LCD.WriteLine("Moving down the ramp.");
    hal.moveBackward(39,hal.MEDIUM);  //EO - 3/23


    //Check CDS color - PT 3/21
    LCD.WriteLine("Reading Light");
    while(hal.cdsColor() == 0);
    LCD.Write("COLOR: ");
    if(hal.cdsColor() == hal.REDLIGHT){
        LCD.WriteLine("Red");

        //Move to Red Counter
        hal.turnLeft(35);
        hal.moveBackward(11,hal.SLOW);
        hal.turnRight(35);
        hal.timeBack(3,hal.SLOW);


        //Drop Scoop


        //Move to chiller
        hal.moveForward(4,hal.SLOW);
        hal.turnLeft(90);
        hal.moveForward(13.5, hal.MEDIUM);  //EO - 3/23
        hal.turnRight(45);
        hal.moveForward(2,hal.MEDIUM);  //Guess - 3/23
        hal.turnRight(45);
        hal.moveForward(4, hal.SLOW);  //Distance for skid to be inside chiller

    }
    else if(hal.cdsColor() == hal.BLUELIGHT){
        LCD.WriteLine("Red");

        //Move to Red Counter
        hal.turnRight(35);
        hal.moveBackward(11,hal.SLOW);
        hal.turnLeft(35);
        hal.timeBack(3,hal.SLOW);


        //Drop Scoop


        //Move to chiller
        hal.moveForward(1,hal.MEDIUM);
        hal.turnLeft(25);  //EO - 3/23
        hal.moveForward(3,hal.MEDIUM); //Guess - 3/23
        hal.turnRight(25);  //EO - 3/23
        hal.moveForward(5, hal.MEDIUM);  //EO - 3/23
    }
    else{
        LCD.WriteLine("I Lost The Light!");
        return 0;
    }


    //Drop skid in chiller and prep for ramp ascension
    hal.setArmAngle(00,00);  //Arm angle to put down scoop
    hal.moveBackward(3.0, hal.MEDIUM);
    hal.turnRight(90);
    hal.moveForward(11, hal.MEDIUM);  //EO - 3/23
    hal.turnLeft(90);


    //Move up ramp and into ChargeZone
    hal.moveForward(25, hal.FAST);
    hal.turnLeft(90);
    hal.moveForward(40, hal.FAST);

    //End program

    return 1;
}

