// global variables
double AA1_new;   // horizontal angle of R
double AA2_new;   // vertical angle of R
double AA3_new;   // upper arm angle relative to R
double AA4_new;   // lower arm angle relative to upper arm
double LL1 = 160; // upper arm length
double LL2 = 340; // lower arm length
/ min max angles
double AAO3max = 3.15000;  // angle lower arm
double AAO4max = 3.15000;  // angle upper arm
ddouble RR2_min = 115.0; // minimum possible radius
double RR2_max = LL1 + LL2; // maximum possible radius

boolean carth_to_spher(double X, double Y, double Z) {
   RR2_new = sqrt(X * X + Y * Y + Z * Z);
   AA1_new = atan(Y / X);
   AA2_new = asin(Z / R);
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
