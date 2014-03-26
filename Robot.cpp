#include "robot.h"


/*
 *Constructor for Robot Class
 *Initializes sensor and motor values for Robot.
 *Enables RPS and initializes menu.
 *Displays start screen listing connections.
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
    rightEncoder = new FEHEncoder(FEHIO::P1_7);

    switch1 = new DigitalInputPin(FEHIO::P2_0);
    switch2 = new DigitalInputPin(FEHIO::P2_1);
    switch3 = new DigitalInputPin(FEHIO::P2_2);

    //declare constants
    startLightValue = 18;
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
    LCD.WriteLine("Sucessfully Initialized RPS");

    rps->Enable();
    LCD.WriteLine("Sucessfully Enabled RPS");
    startX = getX();
    startY = getY();

    ovenCount = rps->Oven();

    LCD.WriteLine("Successfully initialized robot");
    Sleep(1.0);
    LCD.Clear(FEHLCD::Black);
    printStartScreen();
    while(switch1->Value() && switch2->Value() && switch3->Value());
}

/*
 *Returns the oven value returned by the RPS
 */
int Robot::getOvenCount(){
    return ovenCount;
}

/*
 *Sleeps a specific amount of time
 */
void Robot::wait(float time){
    Sleep(time);
}

/*
 *Subroutine that allows the user to use switchboard
 *to move arm servos to a specific position.
 */
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

/*
 *Returns the value of the CDS cell as an integer value
 *between 0 and 100.
 */
int Robot::checkCDS(){
    int cdsPercentage = (cdsCell->Value() / 3.3) * 100;
    return cdsPercentage;
}

/*
 *Checks if the starting light is on.
 *Used at beginning of run.
 *Returns true if the light is on.
 */
bool Robot::cdsStart(){
    bool ready = false;
    if(checkCDS() < startLightValue){
        ready = true;
    }
    return ready;
}

/*
 *Returns the color of the light under the CDS cell.
 *Used in shop.
 */
int Robot::cdsColor(){
    if(checkCDS() < 12){
        return REDLIGHT;
    }
    else{
        return BLUELIGHT;
    }
}

/*
 *Returns X value of robot using RPS.
 */
float Robot::getX(){
    return rps->X();
}

/*
 *Returns Y value of robot using RPS.
 */
float Robot::getY(){
    return rps->Y();
}

/*
 *Move forward a specific distance
 *distance given in INCHES
 *because we're in AMERICA
 *Land of the Free. Home of the Brave.
 *Also, requires the power percentage that the motors will use.
 */
void Robot::moveForward(float distance, int power){
    int motorPercent = power;

    leftEncoder->ResetCounts();
    rightEncoder->ResetCounts();


    int estCounts = (TOTALCOUNTS*distance)/(2*PI*WHEELRADIUS);

    leftMotor->SetPercent(motorPercent);
    rightMotor->SetPercent(motorPercent-(power/15.0));

    int avgCounts = (leftEncoder->Counts() + rightEncoder->Counts())/2.0;

    //while the average counts between the wheels are less than the final desired count
    int i = 0;
    while(avgCounts < estCounts){
        i++;
        //normal run
        if(avgCounts > 4){
            rightMotor->SetPercent(motorPercent+((leftEncoder->Counts()) - rightEncoder->Counts()));
        }
        avgCounts = (leftEncoder->Counts() + rightEncoder->Counts())/2.0;
    }
    //stop wheel movement
    leftMotor->SetPercent(0);
    rightMotor->SetPercent(0);
    LCD.WriteLine(i);
    wait(SHORT);
}

/*
 *Move forward a specific coordinate distance. One parameter must be zero.
 */
void Robot::moveForward(float distX, float distY, int power){
	int motorPercent = power;
	int tolerance = 2;
	
    rps->Enable();

    int initX = rps->X();
    int initY = rps->Y();

    leftEncoder->ResetCounts();
    rightEncoder->ResetCounts();

    leftMotor->SetPercent(motorPercent);
    rightMotor->SetPercent(motorPercent-(power/15.0));

    int avgCounts = 0;
	
    //split for navigation  
    if(distX == 0){
		//move until get to y coordinate
        while(!((abs(rps->Y()-initY) <= distY+tolerance)&&(abs(rps->Y()-initY) >= distY-tolerance))){
			//normal run
			if(avgCounts > 4){
				rightMotor->SetPercent(motorPercent+((leftEncoder->Counts()) - rightEncoder->Counts()));
			}
			avgCounts = (leftEncoder->Counts() + rightEncoder->Counts())/2.0;
		}  
	}
    else if(distY == 0){
		//move until get to y coordinate
        while(!((abs(rps->X() - initX) <= distX+tolerance)&&(abs(rps->X()-initX) >= distX-tolerance))){
			//normal run
			if(avgCounts > 4){
				rightMotor->SetPercent(motorPercent+((leftEncoder->Counts()) - rightEncoder->Counts()));
			}
			avgCounts = (leftEncoder->Counts() + rightEncoder->Counts())/2.0;
		}  
	}
	else{
		LCD.WriteLine("You fail at method contracts. Shame on you.");
	}
	//stop wheel movement
    leftMotor->SetPercent(0);
    rightMotor->SetPercent(0);
	wait(SHORT);
}	

/*
 *Move forward to a specific x point away from our starting position
 */
void Robot::forwardToXPoint(float pointX, int power){
    int motorPercent = power;
    float tolerance = .25;

    rps->Enable();

    leftEncoder->ResetCounts();
    rightEncoder->ResetCounts();

    leftMotor->SetPercent(motorPercent);
    rightMotor->SetPercent(motorPercent-(power/15.0));

    int avgCounts = 0;

    while(!((getX() <= pointX+tolerance)&&(getX() >= pointX-tolerance))){
        //normal run
        if(avgCounts > 4){
            rightMotor->SetPercent(motorPercent+(leftEncoder->Counts() - rightEncoder->Counts()));
        }
        avgCounts = (leftEncoder->Counts() + rightEncoder->Counts())/2.0;
    }

    //stop wheel movement
    leftMotor->SetPercent(0);
    rightMotor->SetPercent(0);
    wait(SHORT);
}

/*
 *Move forward a specific distance given in INCHES
 *Also, requires the power percentage that the motors will use.
 */
void Robot::moveBackward(float distance, int power){
    int motorPercent= power*(-1);

    leftEncoder->ResetCounts();
    rightEncoder->ResetCounts();


    int estCounts = (TOTALCOUNTS*distance)/(2*PI*WHEELRADIUS);

    leftMotor->SetPercent(motorPercent);
    rightMotor->SetPercent(motorPercent);

    int avgCounts = (leftEncoder->Counts() + rightEncoder->Counts())/2.0;
    //while the average counts between the wheels are less than the final desired count
    while(avgCounts < estCounts){
        //normal run
        if(avgCounts > 4){
            rightMotor->SetPercent(motorPercent+(rightEncoder->Counts() - leftEncoder->Counts()));
        }
        avgCounts = (leftEncoder->Counts() + rightEncoder->Counts())/2.0;
    }
    //stop wheel movement
    leftMotor->SetPercent(0);
    rightMotor->SetPercent(0);
    wait(SHORT);
}

/*
 *Move forward to a specific coordinate. One parameter must be zero.
 */
void Robot::moveBackward(float finalX, float finalY, int power){
	int motorPercent = power*(-1);
	int tolerance = 2;
	
    int initX = rps->X();
    int initY = rps->Y();

    rps->Enable();

    leftEncoder->ResetCounts();
    rightEncoder->ResetCounts();

    leftMotor->SetPercent(motorPercent);
    rightMotor->SetPercent(motorPercent);

    int avgCounts = 0;
	
    //split for navigation  
	if(finalX == 0){
		//move until get to y coordinate
        while(!((abs(rps->Y()-initY) <= finalY+tolerance)&&(abs(rps->Y()-initY) >= finalY-tolerance))){
			if(avgCounts > 4){
				rightMotor->SetPercent(motorPercent+(rightEncoder->Counts() - leftEncoder->Counts()));
			}
			avgCounts = (leftEncoder->Counts() + rightEncoder->Counts())/2.0;
		}  
	}
	else if(finalY == 0){
		//move until get to y coordinate
        while(!((abs(rps->X()-initX) <= finalX+tolerance)&&(abs(rps->X()-initX) >= finalX-tolerance))){
			if(avgCounts > 4){
				rightMotor->SetPercent(motorPercent+(rightEncoder->Counts() - leftEncoder->Counts()));
			}
			avgCounts = (leftEncoder->Counts() + rightEncoder->Counts())/2.0;
		}  
	}
	else{
		LCD.WriteLine("You fail at method contracts. Shame on you.");
	}
	//stop wheel movement
    leftMotor->SetPercent(0);
    rightMotor->SetPercent(0);
	wait(SHORT);
}


/*
 *Move the motors forward for a specified amount of time.
 */
void Robot::timeForward(float time, int power){
    leftMotor->SetPercent(power);
    rightMotor->SetPercent(power-(power/15.0));
    Sleep(time*1.0);
    leftMotor->SetPercent(0);
    rightMotor->SetPercent(0);
    wait(SHORT);
}


/*
 *Move the motors backward for a specified amount of time.
 */
void Robot::timeBack(float time, int power){
    leftMotor->SetPercent(power*(-1));
    rightMotor->SetPercent(power*(-1));
    Sleep(time*1.0);
    leftMotor->SetPercent(0);
    rightMotor->SetPercent(0);
    wait(SHORT);
}

/*
 *Returns the robot's current heading, as reported by the RPS.
 */
float Robot::getHeading(){
    return (rps->Heading());
}

/*
 *Turns the robot left for a certain number of degrees.
 *Uses RPS headings.
 */
void Robot::turnLeft(int degrees){
    int motorPercent= 40;
    int tolerance = 3;

    rps->Enable();


    float startDeg = getHeading();
    LCD.Write("Start: ");
    LCD.WriteLine(startDeg);

    float finalDeg = startDeg + degrees;
    if(finalDeg >= 180){
        finalDeg -= 180;
    }

    LCD.Write("Final: ");
    LCD.WriteLine(finalDeg);
    leftMotor->SetPercent((-1)*motorPercent);
    rightMotor->SetPercent(motorPercent);

    if(degrees > 20){
        Sleep(1.5);
    }
    else{
        Sleep(.5);
    }

    while(!((getHeading() < (finalDeg + tolerance))&&( (finalDeg - tolerance) < getHeading())));

    //stop wheel movement
    leftMotor->SetPercent(0);
    rightMotor->SetPercent(0);

    LCD.Write("Actual Final: ");
    LCD.WriteLine(getHeading());
    wait(SHORT);
}

/*
 *Turns the robot right for a certain number of degrees.
 *Uses RPS headings.
 */
void Robot::turnRight(int degrees){
    int motorPercent= 40;
    int tolerance = 3;

    rps->Enable();

    float startDeg = getHeading();
    LCD.Write("Start: ");
    LCD.WriteLine(startDeg);

    float finalDeg = startDeg-degrees;
    if(finalDeg < 0){
        finalDeg += 180;
    }

    LCD.Write("Final: ");
    LCD.WriteLine(finalDeg);

    leftMotor->SetPercent(motorPercent);
    rightMotor->SetPercent((-1)*motorPercent);

    if(degrees > 20){
        Sleep(1.5);
    }
    else{
        Sleep(.5);
    }

    while(!((getHeading() < finalDeg + tolerance)&&( finalDeg - tolerance < getHeading())));

    //stop wheel movement
    leftMotor->SetPercent(0);
    rightMotor->SetPercent(0);

    LCD.Write("Actual Final: ");
    LCD.WriteLine(getHeading());
    wait(SHORT);
}

/*
 *Sets the arm's servo motors to specified values. UD first, then LR.
 */
void Robot::setArmAngle(int LRangle, int UDangle){
    LRservo->SetDegree(LRangle);
    wait(SHORT);
    UDservo->SetDegree(UDangle);
    wait(SHORT);
}

/*
 *Print a splash screen displaying the robot's connections to the LCD.
 *Used in initialization of robot class.
 */
void Robot::printStartScreen(){
    LCD.WriteLine("Connections: ");
    LCD.WriteLine("P0_0 - CDS Cell");
    LCD.WriteLine("P1_0 - Left Wheel Encoder");
    LCD.WriteLine("P1_7 - Right Wheel Encoder");
    LCD.WriteLine("P2_0..2 - Switch Board");
    LCD.WriteLine("MOT0 - Left Wheel Motor");
    LCD.WriteLine("MOT1 - Right Wheel Motor");
    LCD.WriteLine("SERVO0 - LR Arm Servo");
    LCD.WriteLine("SERVO1 - UD Arm Servo");
}

/*
 *Check if the RPS is functioning properly.
 */
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
