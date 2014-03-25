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


    //Set initial arm value
    hal.setArmAngle(165,170);


/*RPS check doesn't work
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
*/

    LCD.Clear(CLEARCOLOR);


    //wait for light to start
    LCD.WriteLine("Waiting for light.");
    while(!hal.cdsStart());
    LCD.WriteLine("Lets do it!");

    hal.timeForward(2.0,hal.SLOW);

    //press button the required number of times
    for(int i = 0; i < hal.getOvenCount(); i++){
        hal.setArmAngle(165,170);
        hal.setArmAngle(180,170);
    }
    hal.setArmAngle(165,170);

    hal.timeBack(1,hal.SLOW);

    hal.setArmAngle(0,121);  //EO - 3/23


    //turn to prep for next task
    hal.turnRight(80);


    //move forward to next task
    //hal.moveForward(25,hal.MEDIUM);  //EO - 3/23
    hal.moveForward(29,0,hal.MEDIUM);

    //Get into position to flip switch - also squares up
    hal.turnLeft(90);
    hal.setArmAngle(116,135);
    hal.timeForward(2,hal.SUPERFAST);


    //Flip switch - PT 3/21
    hal.setArmAngle(116,85);


    //Get in position to pull pin
    //hal.moveBackward(8, hal.MEDIUM);  //EO - 3/23
    hal.moveBackward(0,6,hal.MEDIUM);
    hal.turnRight(90);
    hal.setArmAngle(178,165);  //Arm angle to prep pin pull
    //hal.moveForward(11, hal.MEDIUM);  //EO - 3/23
    while(true){
        LCD.Write(hal.getX());
        LCD.Write(" , ");
        LCD.WriteLine(hal.getY());
    }
    hal.moveForward(3,0,hal.MEDIUM);


    //Pull pin and release pin
    hal.setArmAngle(178,126);  //Arm angle to pull pin
    hal.moveBackward(.5, hal.SLOW);  //Distance to get even with skid

    //Drop Pin
    hal.setArmAngle(0,126);
    hal.setArmAngle(0,67);

    //Prep for skid pickup
    hal.turnLeft(90);
    hal.setArmAngle(20,10);  //Arm angle prep to pick up skid
    hal.moveForward(7, hal.SLOW);  //EO - 3/23
    hal.setArmAngle(20,50);  //Arm angle to raise skid


    //Move to top of ramp
    hal.moveBackward(3, hal.MEDIUM);
    hal.turnRight(30);
    hal.moveBackward(7, hal.                MEDIUM);  //EO - 3/23
    hal.turnLeft(30);


    //Move down ramp - PT 3/21
    LCD.WriteLine("Moving down the ramp.");
    hal.moveBackward(34,hal.MEDIUM);  //EO - 3/23


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
        hal.moveForward(11, hal.MEDIUM);  //EO - 3/23
        hal.turnRight(45);
        hal.moveForward(5,hal.MEDIUM);  //Guess - 3/23
        hal.turnRight(45);
        hal.setArmAngle(20,30);
        hal.moveForward(3, hal.SLOW);  //Distance for skid to be inside chiller

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
    hal.setArmAngle(20,0);  //Arm angle to put down scoop
    hal.moveBackward(4.0, hal.MEDIUM);
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

