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

    rps.InitializeMenu();
    rps.Enable();

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
 *Land of the Free. Home of the Brave.*/

void Robot::moveForward(float distance){
    int motorPercent= 50;

    leftEncoder->ResetCounts();
    rightEncoder->ResetCounts();


    int estCounts = (TOTALCOUNTS*distance)/(2*PI*WHEELRADIUS);

    leftMotor->SetPercent(motorPercent);
    rightMotor->SetPercent(motorPercent-5);

    int avgCounts = (leftEncoder->Counts() + rightEncoder->Counts())/2.0;
    //while the average counts between the wheels are less than the final desired count
    while(avgCounts < estCounts){
        LCD.Write("Left to right: ");
        LCD.WriteLine(leftEncoder->Counts() - rightEncoder->Counts());
        //if still in first stage, ramp up
        if (avgCounts < 10){
            leftMotor->SetPercent(motorPercent*(avgCounts+1)/10.0);
            rightMotor->SetPercent(motorPercent*(avgCounts+1)/10.0);
        }
        //if coming to the end, slow down
        else if(avgCounts > estCounts - 10){
            leftMotor->SetPercent(motorPercent*(estCounts - avgCounts));
            rightMotor->SetPercent(motorPercent*(estCounts - avgCounts));
        }
        //normal run
        else{
            rightMotor->SetPercent(motorPercent*(leftEncoder->Counts() - rightEncoder->Counts())*.05);
        }
    }
    //stop wheel movement
    leftMotor->SetPercent(0);
    rightMotor->SetPercent(0);
}

void Robot::moveBackward(float distance){
    /*TODO:
      Refine code for moveForward(), then modify to move backwards()
     */
}

void Robot::turnLeft(int degrees){
    int motorPercent= 50;

    leftEncoder->ResetCounts();
    rightEncoder->ResetCounts();

    int startDeg = rps->Heading();

    int finalDeg = startDeg-degrees;
    if(finalDeg < 0){
        finalDeg += 180;
    }

    while(leftEncoder->Counts() < 5){
        leftMotor->SetPercent((-1)*motorPercent*(leftEncoder->Counts()+1)/5.0);
        rightMotor->SetPercent(motorPercent*(leftEncoder->Counts()+1)/5.0);
    }
    while(rps->Heading() != finalDeg){
        //wait
    }

    //stop wheel movement
    leftMotor->SetPercent(0);
    rightMotor->SetPercent(0);

}

void Robot::turnRight(int degrees){
    int motorPercent= 50;

    leftEncoder->ResetCounts();
    rightEncoder->ResetCounts();

    int startDeg = rps->Heading();

    int finalDeg = startDeg + degrees;
    if(finalDeg >= 180){
        finalDeg -= 180;
    }

    while(leftEncoder->Counts() < 5){
        leftMotor->SetPercent(motorPercent*(leftEncoder->Counts()+1)/5.0);
        rightMotor->SetPercent((-1)*motorPercent*(leftEncoder->Counts()+1)/5.0);
    }
    while(rps->Heading() != finalDeg){
        //wait
    }

    //stop wheel movement
    leftMotor->SetPercent(0);
    rightMotor->SetPercent(0);
}

void Robot::setArmAngle(int LRangle, int UDangle){
    LRservo->SetDegree(LRangle);
    UDservo->SetDegree(UDangle);
}
