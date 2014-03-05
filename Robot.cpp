#include "robot.h"


/*Constructor for Robot Object
 *Initializes values for Robot
 */
Robot::Robot(){
    //Set values to pointers
    leftMotor = new FEHMotor(FEHMotor::Motor0);
    rightMotor = new FEHMotor(FEHMotor::Motor1);

    cdsCell = new AnalogInputPin(FEHIO::P0_0);

    LRservo = new FEHServo(FEHServo::Servo0);
    UDservo = new FEHServo(FEHServo::Servo1);

    rps = new FEHWONKA();

    leftEncoder = new FEHEncoder(FEHIO::P1_0);
    rightEncoder = new FEHEncoder(FEHIO::P1_1);

    myButtons = new ButtonBoard(FEHIO::Bank3);

    //Calibrate servos
    LRservo->SetMin(500);
    LRservo->SetMax(2366);
    UDservo->SetMin(500);
    UDservo->SetMax(2328);

    LCD.WriteLine("Initialized robot");
}

void Robot::findAngle(){
    bool finished = false;
    int LRangle = 90;
    int UDangle = 90;

    LRservo->SetDegree(LRangle);
    UDservo->SetDegree(UDangle);

    while(!finished){
        //print menu
        LCD.Clear(FEHLCD::Black);
        LCD.WriteLine("");
        LCD.WriteLine("Press button to select servo: ");

        //wait for input
        while(!myButtons->LeftPressed()&&!myButtons->MiddlePressed()&&!myButtons->RightPressed());

        //decision tree
        if(myButtons->LeftPressed()){
            LCD.WriteLine("UD servo selected. Please release button.");
            Sleep(1.0);
            bool innerFinish = false;
            while(!innerFinish){
                LCD.Clear(FEHLCD::Black);
                LCD.Write("UD servo angle: ");
                LCD.WriteLine(UDangle);
                UDservo->SetDegree(UDangle);
                //wait for input
                while(!myButtons->LeftPressed()&&!myButtons->MiddlePressed()&&!myButtons->RightPressed());
                if(myButtons->LeftPressed()){
                    UDangle--;
                }
                else if(myButtons->MiddlePressed()){
                    innerFinish = true;
                }
                else if(myButtons->RightPressed()){
                    UDangle++;
                }
                Sleep(.03);
            }
            finished = false;

        }
        else if(myButtons->MiddlePressed()){
            finished = true;
        }
        else if(myButtons->RightPressed()){
            LCD.WriteLine("LR servo selected. Please release button.");
            Sleep(1.0);
            bool innerFinish = false;
            while(!innerFinish){
                LCD.Clear(FEHLCD::Black);
                LCD.Write("LR servo angle: ");
                LCD.WriteLine(UDangle);
                LRservo->SetDegree(UDangle);
                //wait for input
                while(!myButtons->LeftPressed()&&!myButtons->MiddlePressed()&&!myButtons->RightPressed());
                if(myButtons->LeftPressed()){
                    LRangle--;
                }
                else if(myButtons->MiddlePressed()){
                    innerFinish = true;
                }
                else if(myButtons->RightPressed()){
                    LRangle++;
                }
                Sleep(.03);
            }
            finished = false;
        }
    }
}

/* Evaluates the input from the CDS sensor
 *   - Returns value between 0 and 100
 *   - Representative of the percentage of light
 */
int Robot::checkCDS(){
    int cdsPercentage = (cdsCell->Value() / 3.3) * 100;
    return cdsPercentage;
    Sleep(200);
}

/*Move forward a specific distance
 *distance given in INCHES
 *because we're in AMERICA
 *Land of the Free. Home of the Brave.

void Robot::moveForward(float distance){
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
*/
