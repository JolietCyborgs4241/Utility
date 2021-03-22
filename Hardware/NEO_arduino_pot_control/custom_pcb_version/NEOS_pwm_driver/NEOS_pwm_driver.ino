#include <Servo.h>


// When used with the Rev SPARK ontroller, the NEOS can be controlled using the same PWM
// frequencies used to control a servo motor.  A servo typically uses pulses bwteeen
// 1.5ms and 2.5ms to set the angle of the servo between 0-180 (proportionally).
//
// The NEOS motor controller responds the same way with 1.5ms being full reverse,
// 2.0ms being stopped, and 2.5ms being full forward (with proportional forward and
// backward speeds)
//
// We use a potentiometer to set a "speed" which is mapped to a PWM frequency as if we
// are controlling a servo.
//
// To make it easy to hit the "zero" or stopped position on the NEOS controller
// (2.0ms pulses), the code defines a "ZERO_ZONE" which is a +/- A2D value at which
// we just consider as a range of A2D input as "stopped".  This avoids having a touchy
// and exact "zero" point on the potentiometer.
//
// Without a ZERO_ZONE, the A2D value of 512 (and only 512) would be "zero";
// 511 would be very slow in reverse and 513 would be very slow in forward so it
// would be difficult to stop the motor completely.
//
// Using a ZERO_ZONE value of 50, any A2D input between 462 and 562 would be considered
// as "zero".  Moving off of "zero" would start the motor moving in the corresponding
// direction.
//
// We map the remaining values such that we get the full range of motor speed control
// at the cost of some reduced motor speed control resolution.  With a ZEEO_ZONE of 50,
// we map the full reverse speed (0 <= speed < 90) across the max A2D value minus the
// ZERO_ZONE:
//
// A2D_MAX / 2 - ZERO_ZONE -> 462 values
//
// 90 degrees / 462 values -> 0.19% motor speed resolution


//  pwm output pin

#define   PIN_PWM_OUT     9


// control input pin

#define   PIN_POT_IN      A0


// indicator LED pins

#define   PIN_RED_LED     5
#define   PIN_YELLOW_LED  6
#define   PIN_GREEN_LED   7

#define   LED_FWD         PIN_GREEN_LED
#define   LED_STOP        PIN_YELLOW_LED
#define   LED_REV         PIN_RED_LED

#define   LED_ON          HIGH
#define   LED_OFF         LOW


// any A2D inputs +/- this value from zero put out the 90 degree value
// (1.5ms PWM) whic is stopped on the NEOS controller - this prevents us from having
// to hit an exact singukar position on the control pot to stop the motor at the
// expense of loosing the very low speed forward and backward speeds

#define   ZERO_ZONE       50    

#define   PWM_STOP_VALUE  90      // "degrees" on the servo that will stop the NEOS

#define   A2D_MIN         0
#define   A2D_MAX         1023
#define   A2D_MID         ((A2D_MAX - A2D_MIN) / 2)

#define   ANGLE_MIN       0       // servos go from 0-180 degrees
#define   ANGLE_MAX       180     // but we could set bounds on the range if we like
#define   ANGLE_MID       ((ANGLE_MAX - ANGLE_MIN) / 2)


Servo pwmOut;   // we use the Servo library becauise that gives us the
                // oppropriate PWM drive capabilities (The NEOS controller
                // basically responds liek a servo motor)



void setup() {
  
  pwmOut.attach(PIN_PWM_OUT);         // initialize the "servo"

  pinMode(PIN_RED_LED, OUTPUT);       // setup LEDs
  pinMode(PIN_YELLOW_LED, OUTPUT);
  pinMode(PIN_GREEN_LED, OUTPUT);
  
}




void loop() {
  
  int value;

  while ( 1 ) {

    value = analogRead(PIN_POT_IN);     // returns an A2D value from 0 through A2D_MAX

    // are we in the "zero" zone where we want to stop the motor?
  
    if (value > (A2D_MID - ZERO_ZONE) || value < (A2D_MID + ZERO_ZONE)) {

      pwmOut.write(PWM_STOP_VALUE);

      // set the LEDs to indicate "stopped"
      
      digitalWrite(LED_FWD,  LED_OFF);
      digitalWrite(LED_STOP, LED_ON);
      digitalWrite(LED_REV,  LED_OFF);

      continue;
    }

    // We're out of the "zero" range so check to see what direction
    // we should be moving in
    //
    // We need to scale the A2D value - we do this using map() to go
    // from the A2D value outside of the "zero" range across the appropriate
    // half of the servo "angle" range using the corresponding portion of
    // the A2D range (offset by the "zero" range.
    
    if (value < A2D_MID) {              // reverse
      
      value = map(value, A2D_MIN, (A2D_MID - ZERO_ZONE), ANGLE_MIN, ANGLE_MID);

      digitalWrite(LED_FWD,  LED_OFF);
      digitalWrite(LED_STOP, LED_OFF);
      digitalWrite(LED_REV,  LED_ON);
      
    } else  {                           // forward
    
      value = map(value, (A2D_MID + ZERO_ZONE), A2D_MAX, ANGLE_MID, ANGLE_MAX);
      
      digitalWrite(LED_FWD,  LED_ON);
      digitalWrite(LED_STOP, LED_OFF);
      digitalWrite(LED_REV,  LED_OFF);
    }
        
    pwmOut.write(value);          // set the PWM frequency to control the NEOS
    
  }
}
