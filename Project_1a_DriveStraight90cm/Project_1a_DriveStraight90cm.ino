   /// Project 1a Drive 90cm 
   /// Group: Steve, Jon, Matt


   /// Original Source taken from the following example in the Redbot library
   /// and modified by the project team
    
    /***********************************************************************
     * Exp7_3_DriveStraight -- RedBot Experiment 7.3
     * 
     * Knowing where your robot is can be very important. The RedBot supports
     * the use of an encoder to track the number of revolutions each wheels has
     * made, so you can tell not only how far each wheel has traveled but how
     * fast the wheels are turning.
     * 
     * This sketch was written by SparkFun Electronics, with lots of help from 
     * the Arduino community. This code is completely free for any use.
     * 
     * 8 Oct 2013 M. Hord
     * Revised, 31 Oct 2014 B. Huang 
     ***********************************************************************/
    #include <RedBot.h>
    RedBotMotors motors;
    
    RedBotEncoder encoder = RedBotEncoder(A2, A3);
    int buttonPin = 12;
    int countsPerRev = 192;   // 4 pairs of N-S x 48:1 gearbox = 192 ticks per wheel rev
    
    float wheelDiam = 65;  // diam = 65mm            
    float wheelCirc = PI*wheelDiam;  // Redbot wheel circumference = pi*D
    const float finalDistance = 900; // 90 cm, or 900 mm
    const float slowingDistance = 900;
    const float stopDistance = 50;
    const int initialSpeed = 180;
    const int slowingSpeed = 100;
      // variable used to offset motor power on right vs left to keep straight.
    const int offset = 4;  // offset amount to compensate Right vs. Left drive   Changed from 5 to 4 
    const int motorDelay = 50; // small time delay to allow motors to respond to offset change
      
    void setup()
    {
      pinMode(buttonPin, INPUT_PULLUP);
      Serial.begin(9600);
    }
    
    void loop(void)
    {
      // set the power for left & right motors on button press
      if (digitalRead(buttonPin) == LOW)
      {
        driveStraight(slowingDistance, initialSpeed);
        //driveStraight(stopDistance,slowingSpeed);
        // now apply "brakes" to stop the motors.
        motors.brake();  
      }
    }
    
    void driveStraight(float distance, int motorPower)
    {
      long lCount = 0;
      long rCount = 0;
      long targetCount;
      float numRev;
    istance);
      Serial.print(" mm at ");
      Serial.print(motorPower);
      Serial.println(" power.");
    
      Serial.print("Target: ");
      Serial.print(numRev, 3);
      Serial.println(" revolutions.");
      Serial.println();
      
      // print out header
      Serial.print("Left\t");   // "Left" and tab
      Serial.print("Right\t");  // "Right" and tab
      Serial.println("Target count");
      Serial.println("============================");
    
      encoder.clearEnc(BOTH);    // clear the encoder count
      delay(100);  // short delay before starting the motors.
      
      motors.drive(motorPower);  // start motors 
    
      while (rCount < targetCount)
      {
        // while the right encoder is less than the target count -- debug print 
        // the encoder values and wait -- this is a holding loop.
        lCount = encoder.getTicks(LEFT);
        rCount = encoder.getTicks(RIGHT);
        Serial.print(lCount);
        Serial.print("\t");
        Serial.print(rCount);
        Serial.print("\t");
        Serial.println(targetCount);
    
        motors.leftDrive(leftPower);
        motors.rightDrive(rightPower);
    
        // calculate the rotation "speed" as a difference in the count from previous cycle.
        lDiff = (lCount - prevlCount);
        rDiff = (rCount - prevrCount);
    
        // store the current count as the "previous" count for the next cycle.
        prevlCount = lCount;
        prevrCount = rCount;
    
        // if left is faster than the right, slow down the left / speed up right
        if (lDiff > rDiff) 
        {
          leftPower = leftPower - offset;
          rightPower = rightPower + offset;
        }
        // if right is faster than the left, speed up the left / slow down right
        else if (lDiff < rDiff) 
        {
          leftPower = leftPower + offset;  
          rightPower = rightPower - offset;
        }
        delay(motorDelay);  // short delay to give motors a chance to respond.
      }
    }
