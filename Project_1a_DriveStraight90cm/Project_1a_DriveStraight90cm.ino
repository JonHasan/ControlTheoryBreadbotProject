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

     /**************
      * Project 1: 
      * 1. Optimal parameters for good run found to be targetcount = targetcount - 38, speed 160 (for now), offset 4 
      * 2. Optimal parameters for run 1 hold steady for speed of 180 
      * 3. Parameters for 1 do not hold for speed of 190, increasing offset to 6 - worked well but trying 5 for better results. 
      * Ended up performing worse than previously. Boosting to 6.5. Boosted to 7 and reduced targetcount to targetcount -= 32
      * Then to 29 , offset to 6.5 
      * 4. Decided to add in the second slow speed portion. Reducing speed of fast phase to 180, offset reduced to 4.5, slow speed is 120.
      * TargetCount back to original status. Result: robot overshot by large margin. It might be the countsperrev is incorrect. 
      * Changing to 188 instead of 192. 188 still overshot, changing to 184. Still overshot by a couple of centimeters. Reducing to 180.
      * Still overshot a tiny fraction. Reducing to 179. Just barely missed it.increasing to 179.5. THis seemed to work perfectly
      * Don't know why I needed to reduce the counter per rev. It should have been 192 but it overshoots beyond 90 cm. Maybe hall 
      * effect sensor are not as well placed as originally though. 
      * 
      * Current  setup: CountsPerRev 179.5, slowing distance = 550, stop distance = 350, initial speed 180, slowing speed 120, target count
      * untouched, offset = 4.5 
      * 
      * 5. Slowing distance to 650, stop to 250, slowing speed to 130. Bumping up counts per rev 180. 
      * Time: 3.832 seconds  Distance from 90 cm: almost dead center (mm difference) 
      * Time: 3.494      Distance from 90: multiple centimeter awa 
      * TIme: 3.384 seconds   Distance: a centimeter
      * TIme: 3.395 seconds   Distance: centimeter and a half 
      * 
      * Changing back to 179.5 
      * 
      * Time: 3.7699 seconds  Distance: a centimeter or so 
      * 
      * Moving around a bit much 
      * 
      * initial speed to 175, offset to 4 
      * 
      * A lot better, still off though reducing to 179.25
      * 
      * Going to 178
      * 
      * changed offset to 4.25 and then 4.65
      * 
      * Changed countsperrev to 177.5
      * 
      * Back to 178
      * 
      * Final setup: 
      * 
      * TIme: 3.7771   Distance: a centimeter
      * 
      * TIme: 3.919 seconds  Distance: couple centimeters 
      * 
      * 178.5 counts per rev and offset 4.5 
      * 
      * Offset to 4.3, countsperrev to 179.35
      * 
      * Final parameters - slowing distance: 650 , stop distance: 250, initial speed: 175, slowing speed: 130, offset 4.3, countsperrev: 178.8 
      */
    #include <RedBot.h>
    RedBotMotors motors;

    RedBotEncoder encoder = RedBotEncoder(A2, A3);
    int buttonPin = 12;
    int countsPerRev = 178.8;   // 4 pairs of N-S x 48:1 gearbox = 192 ticks per wheel rev. This should have worked but it overshot by half a foot 

    float wheelDiam = 65;  // diam = 65mm 
    float wheelCirc = PI*wheelDiam;  // Redbot wheel circumference = pi*D
    const float finalDistance = 900; // 90 cm, or 900 mm
    const float slowingDistance = 650;
    const float stopDistance = 250;
    const int initialSpeed = 175;
    const int slowingSpeed = 130;
      // variable used to offset motor power on right vs left to keep straight.
    const int offset = 4.3;  // offset amount to compensate Right vs. Left drive
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
        driveStraight(stopDistance,slowingSpeed);
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

      // variables for tracking the left and right encoder counts
      long prevlCount, prevrCount;

      long lDiff, rDiff;  // diff between current encoder count and previous count

      // variables for setting left and right motor power
      int leftPower = motorPower;
      int rightPower = motorPower;

      numRev = distance / wheelCirc;  // calculate the target # of rotations
      targetCount = numRev * countsPerRev;    // calculate the target count

      // debug
      Serial.print("driveStraight() ");
      Serial.print(distance);
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
