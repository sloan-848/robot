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

    //Set initial arm value
    hal.setArmAngle(165,90);

    //wait for light to start
    LCD.WriteLine("Waiting for light.");
    while(!hal.cdsStart());
    LCD.WriteLine("Lets do it!");

    hal.setArmAngle(165,170);

    hal.timeForward(1.0,hal.MEDIUM);

    //press button the required number of times
    for(int i = 0; i < hal.getOvenCount(); i++){
        hal.setArmAngle(165,170);
        hal.setArmAngle(180,170);
    }
    hal.setArmAngle(165,170);

    hal.timeBack(.9,hal.SLOW);

    //prep arm for next task
    hal.setArmAngle(167,121);
    hal.setArmAngle(0,121);  //EO - 3/23


    //turn to prep for next task
    hal.turnRight(80);


    //move forward to next task
    hal.moveForward(26.5,hal.FAST);  //Move forward
    //hal.moveForward(29,0,hal.FAST);


    //Get into position to flip switch - also squares up
    hal.turnLeft(90);
    hal.setArmAngle(116,165);
    hal.timeForward(2,hal.SUPERFAST);


    //Flip switch - PT 3/21
    hal.setArmAngle(116,100);

    //Re-square up with wall
    hal.timeForward(.5,hal.SUPERFAST);

    float straightHeading = hal.getHeading();

    //Get in position to pull pin
    //hal.moveBackward(8, hal.MEDIUM);  //EO - 3/23
    hal.moveBackward(0,6,hal.MEDIUM);

    hal.turnRight(89);
    hal.moveForward(6, hal.MEDIUM);  //EO - 3/23





    /*WORKS TO HERE*/












    /*
    hal.setArmAngle(178,136);  //Arm angle to pull pin
    hal.setArmAngle(0,126);
    hal.setArmAngle(0,67);
    */

    //Prep for skid pickup
    hal.turnLeft(45);
    hal.moveForward(1.5,hal.MEDIUM);
    hal.turnToHeading(straightHeading);

    //Get ready to square up
    hal.timeForward(3.0,hal.SUPERFAST);

    hal.moveBackward(8,hal.MEDIUM);

    hal.setArmAngle(20,10);  //Arm angle prep to pick up skid
    hal.timeForward(1.5, hal.FAST);  //EO - 3/23
    hal.setArmAngle(20,50);  //Arm angle to raise skid

    if(hal.getHeading() < 160){
        hal.turnToHeading(straightHeading);
    }

    /* Move to top of ramp - V1.0
    hal.moveBackward(5, hal.MEDIUM);
    hal.turnRight(35);
    hal.moveBackward(3, hal.MEDIUM);
    hal.turnRight(20);
    hal.moveBackward(9, hal.MEDIUM);  //EO - 3/23
    hal.turnLeft(45);*/

    //Move to top of ramp - V2.0
    for(int i = 0; i < 5; i++){
        hal.moveBackward(2,hal.MEDIUM);
        hal.turnRight(10);
    }
    hal.moveBackward(9.5,hal.MEDIUM);



    //Align to ramp
    hal.turnToHeading(straightHeading);


    //Move down ramp - PT 3/21
    LCD.WriteLine("Moving down the ramp.");
    hal.moveBackward(24,hal.MEDIUM);  //EO - 3/23


    //Check CDS color - PT 3/21
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
        hal.turnRight(35);
        hal.moveBackward(11,hal.MEDIUM);
        hal.turnLeft(35);
        hal.timeBack(3,hal.SUPERFAST);


        //Drop Scoop


        //Move to chiller
        hal.moveForward(1,hal.MEDIUM);
        hal.turnLeft(25);  //EO - 3/23
        hal.moveForward(3,hal.MEDIUM); //Guess - 3/23
        hal.turnRight(25);  //EO - 3/23
        hal.moveForward(5, hal.MEDIUM);  //EO - 3/23
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


    //Drop skid in chiller and prep for ramp ascension
    hal.setArmAngle(20,0);  //Arm angle to put down scoop
    hal.moveBackward(4.0, hal.MEDIUM);
    hal.setArmAngle(20,165);
    hal.setArmAngle(116,165);
    hal.turnLeft(90);
    hal.timeForward(2.0, hal.FAST);
    hal.moveBackward(11, hal.FAST);  //EO - 3/23
    hal.turnRight(90);


    //Move up ramp and into ChargeZone

    hal.moveForward(30, hal.SUPERFAST);
    hal.turnLeft(100);
    hal.moveForward(40, hal.SUPERFAST);

    //End program

    return 1;
}

