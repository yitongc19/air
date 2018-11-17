#include <Servo.h>
#include <NewPing.h>

#define LEFT_ECHO 2
#define LEFT_TRIG 3        
#define RIGHT_ECHO 4
#define RIGHT_TRIG 5
#define MID_ECHO 9
#define MID_TRIG 10
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters).
                         //Maximum sensor distance is rated at 400-500cm.
                                      
class Sweeper
{
  Servo servo;              // the servo
  long pos;              // current servo position 
  long increment;        // increment to move for each interval
  long  updateInterval;      // interval between updates
  unsigned long lastUpdate; // last update of position
  bool stopServo;
 
public: 
  Sweeper(long interval, long incre)
  {
    updateInterval = interval;
    increment = incre;
    stopServo = false;
  }
  
  void Attach(int pin)
  {
    servo.attach(pin);
  }
  
  void Detach()
  {
    servo.detach();
  }

  void ChangeSpeed(long newIncre) {
    increment = newIncre;
  }
  
  void Update()
  {
    unsigned long currentMillis = millis();
    if((unsigned long) (currentMillis - lastUpdate) >= updateInterval)  // time to update
    {
      lastUpdate = millis();
      
      if (not stopServo) {
        pos += increment;
        servo.write(pos);
      }
      
      if ((pos >= 90) || (pos <= 0)) // end of sweep
      {
        // reverse direction
        increment = -increment;
      }
    }
  }

  void stop() {
     stopServo = true;
  }

  void restart() {
    stopServo = false;
  }
};

/*-----( Declare objects )-----*/
NewPing sonar_left(LEFT_TRIG, LEFT_ECHO, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar_right(RIGHT_TRIG, RIGHT_ECHO, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar_mid(MID_TRIG, MID_ECHO, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

Sweeper sweeper1(15, 1);
Sweeper sweeper2(25, 1);
Sweeper sweeper3(35, 1);
Sweeper sweeper4(45, 1);

void setup() 
{ 
  Serial.begin(9600);
  sweeper1.Attach(6);
  sweeper2.Attach(12);
  sweeper3.Attach(22);
  sweeper4.Attach(26);
} 
 
unsigned long lastUpdate = millis();
long updateIntervalSonar = 1000;
long loop_count;
void loop() 
{ 
  long dLeft;
  long dRight;
  long dMid;

  unsigned long curMillis = millis();
  if ((unsigned long) (curMillis - lastUpdate) >= updateIntervalSonar) {
     lastUpdate = millis();
     dLeft = sonar_left.ping_cm();
     dRight = sonar_right.ping_cm();
     dMid = sonar_mid.ping_cm();
//      Serial.print("Left: ");
//  Serial.print(dLeft);
//  Serial.println(" cm");
//  
//  Serial.print("Right: ");
//  Serial.print(dRight);
//  Serial.println(" cm");
//  
//  Serial.print("Mid: ");
//  Serial.print(dMid);
//  Serial.println(" cm");
//  loop_count++;
//  Serial.println(loop_count);
     bool leftward = false;
     bool rightward = false;
     bool forward = false;
     
     if (dLeft < 50 and dLeft != 0) {
        leftward = true;
     }
     if (dRight < 50 and dRight != 0) {
        rightward = true;  
     }
     if (dMid < 50 and dMid != 0) {
        forward = true;
     } 
     
//  /* ------ Move version ------- */
//  if (leftward) {
//    sweeper4.ChangeSpeed(4);
//  } 
//
//  if (rightward) {
//    sweeper1.ChangeSpeed(4);
//  }
//
//  if (forward) {
//    sweeper2.ChangeSpeed(4);
//    sweeper2.ChangeSpeed(4);
//  }

  /* ------- stop version ------- */
  if (leftward) {
    sweeper4.stop();
  } else {
    sweeper4.restart();
  }

    if (rightward) {
    sweeper1.stop();
  } else {
    sweeper1.restart();
  }

    if (forward) {
    sweeper2.stop();
    sweeper3.stop();
  } else {
     sweeper2.restart();
    sweeper3.restart();
  }
}
 

  
  sweeper1.Update();
  sweeper2.Update();
  sweeper3.Update();
  sweeper4.Update();
}
