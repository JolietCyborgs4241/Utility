// cyborgs pwm motror driver
//
// this is intended to drive a Rev Robotics SparkMAX motoro controller using the PWM inputs
//
// in this mode, it operates just like a conventional servo
//
// The PWM is controlled using a potentiaometer connected to an analog input.  The pot only
// sets speed - direction is controlled by a DPDT center-off spring loaded rocker switch.
// Each of the positions is connected to a digital input and run the motor in the desired direction
// at the speed prortional to the pot setting - when not pressed, the inputs are pulled high
// using on-board pullup in the micro controller.


#include <Servo.h>


#define POT_0   A0
#define POT_1   A1
#define POT_2   A2
#define POT_3   A3

#define PWM_0   5
#define PWM_1   6
#define PWM_2   9
#define PWM_3   10

#define FWD_0   2
#define REV_0   3
#define FWD_1   4
#define REV_1   7
#define FWD_2   8
#define REV_2   11
#define FWD_3   12
#define REV_3   13

#define SERVO_FULL_REV    0   // corresponds to 0 degrees
#define SERVO_STOP        90  // mid point
#define SERVO_FULL_FWD    180 // max servo PWM value


typedef struct {
  int   motorSpeed;
  int   motorPwm;
  int   motorFwd;
  int   motorRev;
  Servo servo;
} IO_CONNS;


IO_CONNS  IoConns[] = {
  { POT_0, PWM_0, FWD_0, REV_0 },
  { POT_1, PWM_1, FWD_1, REV_1 },
  { POT_2, PWM_2, FWD_2, REV_2 },
  { POT_3, PWM_3, FWD_3, REV_3 },
};


#define SERIAL_BAUD_RATE    115200


#define PER_LOOP_PAUSE      0


void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println("Starting...");

  int i;
  
  for (i = 0 ; i < sizeof(IoConns)/sizeof(IoConns[0]) ; i++) {
    pinMode(IoConns[i].motorFwd, INPUT_PULLUP);
    pinMode(IoConns[i].motorRev, INPUT_PULLUP);
    pinMode(IoConns[i].motorPwm, OUTPUT);
    IoConns[i].servo.attach(IoConns[i].motorPwm);
  }
}

void loop() {
  
  int i, speedRead, fwdRead, revRead, servoVal;

  for (i = 0 ; i < (sizeof(IoConns)/sizeof(IoConns[0])) ; i++) {
    speedRead = analogRead(IoConns[i].motorSpeed);   // get the speed, 0-1023 range

    fwdRead = digitalRead(IoConns[i].motorFwd);
    revRead = digitalRead(IoConns[i].motorRev);

    if ((fwdRead == HIGH && revRead == HIGH) ||     // no direction selected
        (fwdRead == LOW  && revRead == LOW)) {       // invalid state - STOP!    
      servoVal = SERVO_STOP;
    } else {
      if (fwdRead == LOW) {                         // map forward
        servoVal = map(speedRead, 0, 1023, SERVO_STOP, SERVO_FULL_FWD);
      } else {                                      // else backward
        servoVal = map(speedRead, 0, 1023, SERVO_STOP, SERVO_FULL_REV);
      }
    }

#ifdef DEBUG
    Serial.print("Speed value for ");
    Serial.print(IoConns[i].motorSpeed);
    Serial.print(" is ");
    Serial.print(speedRead);
    Serial.print("(R ");
    Serial.print(revRead);
    Serial.print(" / F ");
    Serial.print(fwdRead);
    Serial.println(")");
    
    Serial.print("Servo value for ");
    Serial.print(IoConns[i].motorPwm);
    Serial.print(" is ");
    Serial.println(servoVal);   
#endif

    IoConns[i].servo.write(servoVal);
  }

  delay(PER_LOOP_PAUSE);                            // give things a chance to settle...
}
