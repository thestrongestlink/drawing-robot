// Include the AccelStepper library:
#include <AccelStepper.h>
#include <MultiStepper.h>

// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPinLowerArm 2
#define stepPinLowerArm 3
#define dirPinUpperArm 4
#define stepPinUpperArm 5
#define dirPinBase 8
#define stepPinBase 9
#define motorInterfaceType 1

// coordinates of wrist relative to shoulder
double XX2;
double YY2;
double ZZ2;
double RR2;
double XX2_new;
double YY2_new;
double ZZ2_new;
double RR2_new;
double AA1_new;   // horizontal angle of R
double AA2_new;   // vertical angle of R
double AA3_new;   // upper arm angle relative to R
double AA4_new;   // lower arm angle relative to upper arm

double LL1 = 160; // upper arm length
double LL2 = 340; // lower arm length

// start angles at stepper 0 positions
double AAO1 = 0.0000;  // angle shoulder at start
double AAO3 = 0.0392;  // angle lower arm at start
double AAO4 = 0.6211;  // angle upper arm at start

// min max angles
double AAO3min = 0.0387;  // angle lower arm
double AAO4min = 0.5233;  // angle upper arm
double AAO3max = 3.15000;  // angle lower arm
double AAO4max = 3.15000;  // angle upper arm
double RR2_min = 115.0; // minimum possible radius
double RR2_max = LL1 + LL2; // maximum possible radius

int steps1 = 7000; // number of steps for 180 degrees rotation shoulder
int steps2 = 4600; // number of steps for 180 degrees rotation upper arm
int steps3 = 3600; // number of steps for 180 degrees rotation lower arm

double new_position_d0;
double new_position_d1;
double new_position_d2;
long new_position_l0;
long new_position_l1;
long new_position_l2;
long new_position[3];
double new_position_d[3];

// Create a new instance of the AccelStepper class:
AccelStepper stepperUpperArm = AccelStepper(motorInterfaceType, stepPinUpperArm, dirPinUpperArm);
AccelStepper stepperBase = AccelStepper(motorInterfaceType, stepPinBase, dirPinBase);
AccelStepper stepperLowerArm = AccelStepper(motorInterfaceType, stepPinLowerArm, dirPinLowerArm);
MultiStepper steppers;

void setup() {
   // put your setup code here, to run once:
   Serial.begin(9600);

  // Set the maximum speed and acceleration:
  stepperLowerArm.setMaxSpeed(400);
  stepperLowerArm.setAcceleration(25);
  stepperLowerArm.setPinsInverted(true);
  stepperUpperArm.setMaxSpeed(400);
  stepperUpperArm.setAcceleration(25);
  stepperBase.setMaxSpeed(400);
  stepperBase.setAcceleration(25);

  // add steppers to multistepper
  steppers.addStepper(stepperBase);
  steppers.addStepper(stepperUpperArm);
  steppers.addStepper(stepperLowerArm);
 
   // reset position of stepper motors
   new_position[0] = 0;
   new_position[1] = 0;
   new_position[2] = 0;
 
   // move to starting position
   XX2_new = 350;
   YY2_new = cos(0) * 50;
   ZZ2_new = 125 + sin(0) * 50;
   move_to_carth(XX2_new, YY2_new, ZZ2_new);
   delay(2000);

    // draw a circle with R = 50, in X-Z plane maintaining Y
   for (int i=0;i<721;i++) {
     XX2_new = 350;
     YY2_new = cos(PI * i / 360) * 50;
     ZZ2_new = 125 + sin(PI * i / 360) * 50;
     move_to_carth(XX2_new, YY2_new, ZZ2_new);
   }
   XX2_new = 350;
   YY2_new = 50;
   ZZ2_new = 75;
   move_to_carth(XX2_new, YY2_new, ZZ2_new);
   XX2_new = 350;
   YY2_new = -50;
   ZZ2_new = 75;
   move_to_carth(XX2_new, YY2_new, ZZ2_new);
   XX2_new = 350;
   YY2_new = -50;
   ZZ2_new = 175;
   move_to_carth(XX2_new, YY2_new, ZZ2_new);
   XX2_new = 350;
   YY2_new = 50;
   ZZ2_new = 175;
   move_to_carth(XX2_new, YY2_new, ZZ2_new);
   XX2_new = 350;
   YY2_new = 50;
   ZZ2_new = 75;
   move_to_carth(XX2_new, YY2_new, ZZ2_new);

   delay(5000);

   // move to origin
   new_position[0] = 0;
   new_position[1] = 0;
   new_position[2] = 0;
   steppers.moveTo(new_position);
   steppers.runSpeedToPosition(); // Blocks until all are in position

}

void loop() {
   // put your main code here, to run repeatedly:
   // no code in loop for now
}

void move_to_carth(double X, double Y, double Z) {
   if (carth_to_spher(X, Y, Z)) {
     new_position_d0 = steps1 / 3.14 * (AA1_new - AAO1);  // absolute steps to new position shoulder
     new_position_d1 = steps2 / 3.14 * (AA3_new - AAO3);  // absolute steps to new position upper arm
     new_position_d2 = steps3 / 3.14 * (AA4_new - AAO4);  // absolute steps to new position lower arm
     new_position_l0 = (long) new_position_d0;
     new_position_l1 = (long) new_position_d1;
     new_position_l2 = (long) new_position_d2;
     new_position[0] = new_position_l0;
     new_position[1] = new_position_l1;
     new_position[2] = new_position_l2;
   // activate steppers to go to new_position[n] - current_position[n]
     steppers.moveTo(new_position);
     steppers.runSpeedToPosition(); // Blocks until all are in position  
   } else {
     Serial.println("Could not calculate position");
   }
}

boolean carth_to_spher(double X, double Y, double Z) {
   RR2_new = sqrt(X * X + Y * Y + Z * Z);
   AA1_new = atan(Y / X);
   AA2_new = asin(Z / RR2_new);
   // check if carth coordinate fits in spherical range of robot arm
   if (RR2_new > RR2_max) {
     Serial.println("Max R exceeded");
     return(false);
   } else if (RR2_new < RR2_min) {
     Serial.println("Min R exceeded");
     return(false);
   } else {
     AA3_new = PI - (AA2_new + acos((LL1 * LL1 + RR2_new * RR2_new - LL2 * LL2) / (2 * LL1 * RR2_new)));
     AA4_new = acos((LL2 * LL2 + LL1 * LL1 - RR2_new * RR2_new) / (2 * LL2 * LL1));
     if (AA3_new >= AAO3min && AA3_new <= AAO3max && AA4_new >= AAO4min && AA4_new <= AAO4max) {
       return(true);
     }
     else {
      Serial.println("Angles exceeded");
      return(false);
     }
   }
}
