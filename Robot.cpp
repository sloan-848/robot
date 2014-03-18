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

    switch1 = new DigitalInputPin(FEHIO::P2_0);
    switch2 = new DigitalInputPin(FEHIO::P2_1);
    switch3 = new DigitalInputPin(FEHIO::P2_2);

    //declare constants
    startLightValue = 10;
    scoopLightValue = 20;
    PI = 3.14159265;
    TOTALCOUNTS = 20;
    WHEELRADIUS = 1.35;

    //Calibrate servos
    LRservo->SetMin(500);
    LRservo->SetMax(2366);
    UDservo->SetMin(500);
    UDservo->SetMax(2328);


    rps->InitializeMenu();
    rps->Enable();

    ovenCount = rps->Oven();

    LCD.WriteLine("Successfully initialized robot");
    LCD.Clear(FEHLCD::Black);
    printStartScreen();
    while(switch1->Value() && switch2->Value() && switch3->Value());
}

int Robot::getOvenCount(){
    return ovenCount;
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
        while(switch1->Value() && switch2->Value() && switch3->Value());

        //decision tree
        if(!switch1->Value()){
            LCD.WriteLine("UD servo selected. Please release button.");
            Sleep(1.0);
            bool innerFinish = false;
            while(!innerFinish){
                LCD.Clear(FEHLCD::Black);
                LCD.Write("UD servo angle: ");
                LCD.WriteLine(UDangle);
                UDservo->SetDegree(UDangle);
                //wait for input
                while(switch1->Value()&&switch2->Value()&&switch3->Value());
                if(!switch1->Value()){
                    UDangle--;
                }
                else if(!switch2->Value()){
                    innerFinish = true;
                }
                else if(!switch3->Value()){
                    UDangle++;
                }
                Sleep(.03);
            }
            finished = false;

        }
        else if(!switch2->Value()){
            finished = true;
        }
        else if(!switch3->Value()){
            LCD.WriteLine("LR servo selected. Please release button.");
            Sleep(1.0);
            bool innerFinish = false;
            while(!innerFinish){
                LCD.Clear(FEHLCD::Black);
                LCD.Write("LR servo angle: ");
                LCD.WriteLine(LRangle);
                LRservo->SetDegree(LRangle);
                //wait for input
                while(switch1->Value()&&switch2->Value()&&switch3->Value());
                if(!switch1->Value()){
                    LRangle--;
                }
                else if(!switch2->Value()){
                    innerFinish = true;
                }
                else if(!switch3->Value()){
                    LRangle++;
                }
                Sleep(.03);
            }
            finished = false;
        }
        Sleep(.5);
    }
}


bool Robot::cdsReady(int op){
    bool ready = false;

    if(op == START){
        if(cdsCell->Value() < startLightValue){
            ready = true;
        }
    }
    else if(op == SCOOP){
        if(cdsCell->Value() < scoopLightValue){
            ready = true;
        }
    }
    return ready;
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
        //LCD.Write("Left to right: ");
        //LCD.WriteLine(leftEncoder->Counts() - rightEncoder->Counts());
        //if still in first stage, ramp up
        /*
        if (avgCounts < 10){
            leftMotor->SetPercent(motorPercent*(avgCounts+1)/10.0);
            rightMotor->SetPercent(motorPercent*(avgCounts+1)/10.0);
        }
        //if coming to the end, slow down
        else if(avgCounts > estCounts - 10){
            leftMotor->SetPercent(motorPercent*(estCounts - avgCounts));
            rightMotor->SetPercent(motorPercent*(estCounts - avgCounts));
        }
        */
        //normal run
        if(avgCounts > 5){
            rightMotor->SetPercent(motorPercent+((leftEncoder->Counts()-1) - rightEncoder->Counts()));
        }
        avgCounts = (leftEncoder->Counts() + rightEncoder->Counts())/2.0;
        LCD.WriteLine(avgCounts);

    }
    //stop wheel movement
    leftMotor->SetPercent(0);
    rightMotor->SetPercent(0);
}

void Robot::moveBackward(float distance){
    int motorPercent= -50;

    leftEncoder->ResetCounts();
    rightEncoder->ResetCounts();


    int estCounts = (TOTALCOUNTS*distance)/(2*PI*WHEELRADIUS);

    leftMotor->SetPercent(motorPercent);
    rightMotor->SetPercent(motorPercent+5);

    int avgCounts = (leftEncoder->Counts() + rightEncoder->Counts())/2.0;
    //while the average counts between the wheels are less than the final desired count
    while(avgCounts < estCounts){
        //LCD.Write("Left to right: ");
        //LCD.WriteLine(leftEncoder->Counts() - rightEncoder->Counts());
        //if still in first stage, ramp up
        /*
        if (avgCounts < 10){
            leftMotor->SetPercent(motorPercent*(avgCounts+1)/10.0);
            rightMotor->SetPercent(motorPercent*(avgCounts+1)/10.0);
        }
        //if coming to the end, slow down
        else if(avgCounts > estCounts - 10){
            leftMotor->SetPercent(motorPercent*(estCounts - avgCounts));
            rightMotor->SetPercent(motorPercent*(estCounts - avgCounts));
        }
        */
        //normal run
        if(avgCounts > 5){
            rightMotor->SetPercent(motorPercent+(leftEncoder->Counts() - rightEncoder->Counts()));
        }
        avgCounts = (leftEncoder->Counts() + rightEncoder->Counts())/2.0;
        LCD.WriteLine(avgCounts);

    }
    //stop wheel movement
    leftMotor->SetPercent(0);
    rightMotor->SetPercent(0);
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
    LCD.Write("Final: ");
    LCD.WriteLine(finalDeg);
    Sleep(.5);
    leftMotor->SetPercent((-1)*motorPercent);
    rightMotor->SetPercent(motorPercent);

    while(rps->Heading() != finalDeg){
        LCD.Write("Current: ");
        LCD.Write(rps->Heading());
    }

    //stop wheel movement
    leftMotor->SetPercent(0);
    rightMotor->SetPercent(0);

}
/*Depreciated
void Robot::turnLeft(int time){
    leftMotor->SetPercent(-50);
    rightMotor->SetPercent(50);
    Sleep(time);
    leftMotor->SetPercent(0);
    rightMotor->SetPercent(0);
}*/

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

/*Depreciated
void Robot::turnRight(int time){
    leftMotor->SetPercent(50);
    rightMotor->SetPercent(-50);
    Sleep(time);
    leftMotor->SetPercent(0);
    rightMotor->SetPercent(0);
}*/

void Robot::setArmAngle(int LRangle, int UDangle){
    UDservo->SetDegree(UDangle);
    LRservo->SetDegree(LRangle);
}

void Robot::printStartScreen(){
    LCD.WriteLine("Connections: ");
    LCD.WriteLine("P0_0 - CDS Cell");
    LCD.WriteLine("P1_0 - Left Wheel Encoder");
    LCD.WriteLine("P1_1 - Right Wheel Encoder");
    LCD.WriteLine("P2_0..2 - Switch Board");
    LCD.WriteLine("MOT0 - Left Wheel Motor");
    LCD.WriteLine("MOT1 - Right Wheel Motor");
    LCD.WriteLine("SERVO0 - LR Arm Servo");
    LCD.WriteLine("SERVO1 - UD Arm Servo");
}

bool Robot::validRPS(){
    bool working = false;
    rps->Enable();
    LCD.Clear(FEHLCD::Black);
    LCD.Write("Please Move RPS Around.");
    for(int i = 0; i < 100; i++){
        LCD.WriteLine(rps->Heading());
        if(rps->Heading() != 0){
            working = true;
        }
        Sleep(.01);
    }
    LCD.Write("Waiting for Packet");
    if(rps->WaitForPacket() != 0){
        working = true;
    }
    else{
        working = false;
    }

    return working;
}
