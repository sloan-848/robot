#include <math.h>
#include <FEHIO.h>
#include <FEHServo.h>
#include <FEHMotor.h>
#include <FEHLCD.h>

#ifndef FUNCTION_H
#define FUNCTION_H


/* Assist programmer in finding correct servo
 * angles for a desired arm movement
 */
void findAngle(FEHServo LRservo, FEHServo UDservo, ButtonBoard myButtons){
    bool finished = false;
    int LRangle = 90;
    int UDangle = 90;

    LRservo.SetDegree(LRangle);
    UDservo.SetDegree(UDangle);

    while(!finished){
        //print menu
        LCD.WriteLine();
        LCD.WriteLine("Press button to select servo: ");

        //wait for input
        while(!myButtons.LeftPressed()&&!myButtons.MiddlePressed()&&!myButtons.RightPressed()){}

        //decision tree
        if(myButtons.LeftPressed()){
            LCD.WriteLine("UD servo selected. Please release button.");
            Sleep(1.0);
            while(){
                LCD.Clear(FEHLCD::Black);
                LCD.Write("UD servo angle: ");
                LCD.WriteLine(UDangle);
                //wait for input
                while(!myButtons.LeftPressed()&&!myButtons.RightPressed()){}
                if(myButtons.LeftPressed()){

                }
                else if(myButtons.RightPressed()){}
            }

        }
        else if(myButtons.MiddlePressed()){
            finished = true;
        }
        else if(myButtons.RightPressed()){
            /* TODO:
             *   -Mirror code from myButtons.LeftPressed()
             */
        }

    }
}

/* Evaluates the input from the CDS sensor
 *   - Returns value between 0 and 100
 *   - Representative of the percentage of light
 */
int checkCDS(AnalogInputPin cdsCell){
    int cdsPercentage = (cdsCell.Value() / 3.3) * 100;
    return cdsPercentage;
    Sleep(200);
}

/*Move forward a specific distance
 *distance given in INCHES
 *because we're in AMERICA
 *Land of the Free. Home of the Brave.
 */


#endif // FUNCTION_H
