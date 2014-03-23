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

    //wait for light to start
    while(!hal.cdsStart());
    LCD.WriteLine("Lets do it!");


    //press button the required number of times
    for(int i = 0; i < hal.getOvenCount(); i++){
        hal.setArmAngle(00,00);  //Prep for hit
        Sleep(1.0);
        hal.setArmAngle(00,00);  //Hit
        Sleep(1.0);
    }
    hal.setArmAngle(00,00);  //Prep for next task


    //turn to prep for next task
    hal.turnRight(90);


    //move forward to next task
    hal.moveForward(00,hal.MEDIUM);  //Distance to even with switch


    //Get into position to flip switch - also squares up
    hal.turnLeft(90);
    hal.timeForward(4,hal.SLOW);


    //Flip switch - PT 3/21
    hal.setArmAngle(116,122);
    Sleep(.6);
    hal.setArmAngle(116,95);


    //Get in position to pull pin
    hal.moveBackward(00, hal.MEDIUM);  //Distance to even with pin
    hal.turnRight(90);
    hal.setArmAngle(00,00);  //Arm angle to prep pin pull
    hal.moveForward(00, hal.MEDIUM);  //Distance to pin


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
    hal.moveForward(00, hal.SLOW);  //Distance to pick up skid
    hal.setArmAngle(00,00);  //Arm angle to raise skid


    //Move to top of ramp
    hal.moveBackward(1.0, hal.MEDIUM);
    hal.turnRight(45);
    hal.moveBackward(00, hal.MEDIUM);  //Distance to top of ramp at 45 degrees
    hal.turnLeft(45);


    //Move down ramp - PT 3/21
    LCD.WriteLine("Moving down the ramp.");
    hal.moveBackward(35,hal.MEDIUM);


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
        hal.moveForward(00, hal.MEDIUM);  //Distance to even with chiller
        hal.turnRight(90);
        hal.moveForward(00, hal.SLOW);  //Distance for skid to be inside chiller

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
        hal.turnLeft(15);
        hal.moveForward(1,hal.MEDIUM);
        hal.turnRight(15);
        hal.moveForward(00, hal.MEDIUM);  //Distance for skid to be inside chiller
    }
    else{
        LCD.WriteLine("I Lost The Light!");
        return 0;
    }


    //Drop skid in chiller and prep for ramp ascension
    hal.setArmAngle(00,00);  //Arm angle to put down scoop
    hal.moveBackward(3.0, hal.MEDIUM);
    hal.turnRight(90);
    hal.moveForward(00, hal.MEDIUM);  //Distance to even with ramp
    hal.turnLeft(90);


    //Move up ramp and into ChargeZone
    hal.moveForward(25, hal.FAST);
    hal.turnLeft(90);
    hal.moveForward(40, hal.FAST);

    //End program
    return 1;
}

