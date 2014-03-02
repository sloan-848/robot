#ifndef FUNCTION_H
#define FUNCTION_H
#include <math.h>
#include <FEHIO.h>
#include <FEHMotor.h>

/* Moves the robot from the back wall of the bottom course to the front of the oven
 *   - Includes Turn
 *   - Does not use encoders
 *   - Non-scientific timing used to control motors
 */
int moveUpRamp(FEHMotor leftMotor, FEHMotor rightMotor)
{

    //Moving Forward
    leftMotor.SetPercent(70);
    rightMotor.SetPercent(70);
    Sleep(4900);

    //Turning Left
    leftMotor.SetPercent(0);
    Sleep(1300);

    //Moving Forward
    leftMotor.SetPercent(70);
    rightMotor.SetPercent(70);
    Sleep(4000);

    //Movement Finished
    leftMotor.SetPercent(0);
    rightMotor.SetPercent(0);
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
