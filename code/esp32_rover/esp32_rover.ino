/***  www.bradleyconn.space ***/

/**
 * This is the "production" code.
 * It strips out the "leds as motors" testing.
 * It leaves all the printing because it isn't that time sensitive with smoothing turned on.
 * This file is way longer than it should be but it's kept that
 * way to be really easy to import into the Arduino IDE and use.
 * 
 */

#include <Arduino.h>
// ESP32 needs a special analog write library
#include <analogWrite.h>

// pin definitions
const uint8_t pin_ch1 = 34;
const uint8_t pin_ch2 = 35;
const uint8_t pin_ch3 = 32;
const uint8_t pin_ch4 = 33;
const uint8_t pin_ch5 = 25;
const uint8_t pin_ch6 = 26;

const uint8_t pin_reverse_left = 12; // pin not good as an input
const uint8_t pin_reverse_right = 14;

const uint8_t pin_left = 13;
const uint8_t pin_right = 27;

/****************************************
  This is how the controller mapping goes
     right stick x    = ch1
     right stick y    = ch2
     left stick y     = ch3
     left stick x     = ch4
     top right switch = ch5
     top left wtich   = ch6
****************************************/

// TODO: Make a calibration routine with a button
// May only need to do it for one channel and apply to all
const int ch1_max = 2000;
const int ch1_min = 1000;
const int ch2_max = 1822;
const int ch2_min = 922;
const int ch3_max = 2000;
const int ch3_min = 1000;
const int ch4_max = 2000;
const int ch4_min = 1000;

#if 0
#define datapoints 5
int left_history[datapoints];
int right_history[datapoints];
// A function that can be used to reset the arrays
void reset_smoothing_function() {
  for (int i=0; i<datapoints; i++) {
    left_history[i] = 0;
    right_history[i] = 0;
  }
}
void smooth_driving(int * left, int * right) {
  // Shift the data
  // This is super inefficient but there's tons of performance overhead so going for the simple route
  for (int i=datapoints-1; i>0; i--) {
    left_history[i] = left_history[i-1];
    right_history[i] = right_history[i-1];
  }
  //Enter the newest data
  left_history[0] = *left;
  right_history[0] = *right;

  // Do the smoothing
  // For now try a simple moving average
  // XXX: In the future might need to care about maxing out the int
  int left_accum = 0;
  int right_accum = 0;
  for (int i=datapoints-1; i>0; i--) {
    left_accum = left_history[i];
    right_accum = right_history[i];
  }
  int left_smoothed = left_accum / datapoints;
  int right_smoothed = right_accum/datapoints;

  *left = left_smoothed;
  *right = right_smoothed;
}
#endif
#if 1

/**
 * Some smoothing params for the function below!
 * Globals are used for easy resetting
 */
// Step per second allowed.
// As a guess, don't go more than 50% in half a second? that might be slow...
// Range is from -500 to +500
#define max_step  75
#define step_time_ms  50
// Variables to store the historical data
int prev_left = 0;
int prev_right = 0;
// This needs to be updated on every call to smooth that updates AND every reset smooth
unsigned long prev_time = 0;
// A function that can be used to reset the values
void reset_smoothing_function() {
  prev_left = 0;
  prev_right = 0;
  prev_time = millis();
}

/**
 * This functions take a left wheel and right wheel signal and use historical data
 * to smooth the control experiance. The parameters are inout params
 * Motivation:
 * Quick changes in the motors is violent on the vehicle and its electronics.
 * Try to reduce instantanious large changes.
 * Keep some historical data to make informed corrections.
 * 
 * @param *left - in out param for the drive signal the motor
 * @param *right - in out param for the drive signal of the motor
 */
void smooth_driving(int * left, int * right) {
  Serial.print("Millis: ");
  Serial.println(millis());
  // This can all be done way better but for now take a simplistic approach and feel it out
  // Simple approach - find a good balance of maximum step function in one step, and frequency of steps.
  // TODO: Better approach - always update but limit the step to a step per ms ratio, with an additional maximum step constraint
  
  // If it's been to soon don't update
  if(millis() - prev_time < step_time_ms){
    // Too quick to update so use the previous values
    *left = prev_left;
    *right = prev_right;
    return;
  }

  // Over step time so do an update
  int target_left = *left;
  int target_right = *right;
  int left_smoothed = 0;
  int right_smoothed = 0;

  // Left side
  // If adding forward power
  if(target_left > prev_left) {
    int step_difference = target_left - prev_left;
    if(step_difference > max_step) {
      step_difference = max_step;
    }
    left_smoothed = prev_left + step_difference;
  }
  // Adding backwards power
  else {
    int step_difference = prev_left - target_left;
    if(step_difference > max_step) {
      step_difference = max_step;
    }
    left_smoothed = prev_left - step_difference;
  }

  // Right side
  // If adding forward power
  if(target_right > prev_right) {
    int step_difference = target_right - prev_right;
    if(step_difference > max_step) {
      step_difference = max_step;
    }
    right_smoothed = prev_right + step_difference;
  }
  // Adding backwards power
  else {
    int step_difference = prev_right - target_right;
    if(step_difference > max_step) {
      step_difference = max_step;
    }
    right_smoothed = prev_right - step_difference;
  }
  
  *left = left_smoothed;
  *right = right_smoothed;

  //update the values
  prev_left = *left;
  prev_right = *right;
  prev_time = millis();
}
#endif

/**
 * This is the function that takes the left and right drive strength and direction
 * and sends it out to the motor.
 * 
 * Left and right are ints where negative indicates going in the reverse direction
 * The values should be between -500 and 500
 * The motor driver splits it into a direction signal (binary) and a strength signal (variable)
 * 
 * @param int left - left drive value between -500 and 500
 * @param int right - right drive value between -500 and 500
 */
void drive(int left, int right) {
  // The rover can have pretty harsh instantaneous behavior
  // so apply and algorithm to smooth out the harshness
  // These are inout params
  smooth_driving(&left, &right);

  Serial.println(left);
  Serial.println(right);

  // Deduce the direction of each motor and send the signal
  // Also ensure the speed is a positive number 
  if (left < 0) {
    // reverse on
    digitalWrite(pin_reverse_left, HIGH);
    left = abs(left); 
    Serial.println("left reverse");
  }
  else {
    //reverse off
    digitalWrite(pin_reverse_left, LOW);
    Serial.println("left forward");
  }

  if (right < 0) {
    //reverse on
    digitalWrite(pin_reverse_right, HIGH);
    right = abs(right);
    Serial.println("right reverse");
  }
  else {
    //reverse off
    digitalWrite(pin_reverse_right, LOW);
    Serial.println("right forward");
  }

 //clip the values over 500, and set anything over 450 to max
  if (left > 450) {
    left = 500; 
  }
  if (right > 450) {
    right = 500;
  }
 //clip the values under 100 to keep sensitivity down
  if (left < 100) {
    left = 0; 
  }
  if (right < 100) {
    right = 0;
  }

  uint32_t left_mapped = (uint32_t)map(left, 0, 500, 0, 255);
  uint32_t right_mapped = (uint32_t)map(right, 0, 500, 0, 255);
  //The left wheel is about 9/10th the speed of the right wheel.
  //Limit the right wheel by that much
  uint32_t  right_mapped_calibrated = (right_mapped * 9) / 10;
  analogWrite(pin_left, left_mapped);
  analogWrite(pin_right, right_mapped_calibrated);

  Serial.print("left power");
  Serial.println(map(left, 0, 500, 0, 255));
  Serial.print("right power ");
  Serial.println(map(right, 0, 500, 0, 255));
}

/**
 * This function takes left-right, up-down controller axis, maps it to a left
 * and right wheel. Then sends it to the motor for actuation.
 * 
 * @param ch_x_mapped - the left-right value. Left is negative.
 * @param ch_y_mapped - the up-down value. Down is negative.
 */
void single_stick_drive(int ch_x_mapped, int ch_y_mapped) {
  // Cool little "algorithm". Easy enough to think through.
  int left = ch_y_mapped + ch_x_mapped;
  int right = ch_y_mapped - ch_x_mapped;

  // if we're definitely going backwards switch which wheel to reverse on.
  if(ch_y_mapped<-100) {
    left = ch_y_mapped - ch_x_mapped;
    right = ch_y_mapped + ch_x_mapped;
  }
  if (left > 500) {
    left = 500;
  }
  else if (left < -500) {
    left = -500;
  }
  if (right > 500) {
    right = 500;
  }
  else if (right < -500) {
    right = -500;
  }
  drive(left, right);
}


/**
 * This function takes left-right, up-down controller axis, maps it to a left
 * and right wheel. Then sends it to the motor for actuation.
 * 
 * @param left - the left stick value. Down is negative.
 * @param right - the Right stick value. Down is negative.
 */
void dual_stick_drive(int left, int right) {
  drive(left, right);
}

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(pin_ch1, INPUT); // Set our input pins as such
  pinMode(pin_ch2, INPUT);
  pinMode(pin_ch3, INPUT);
  pinMode(pin_ch4, INPUT);
  pinMode(pin_ch5, INPUT);
  pinMode(pin_ch6, INPUT);
  
  //control pins
  pinMode(pin_reverse_left, OUTPUT);
  pinMode(pin_reverse_right, OUTPUT);
  pinMode(pin_left, OUTPUT);
  pinMode(pin_right, OUTPUT);

  Serial.begin(115200);

  //disable all motor output on start up.
  digitalWrite(pin_right, LOW);
  digitalWrite(pin_left, LOW);
  digitalWrite(pin_reverse_right, LOW);
  digitalWrite(pin_reverse_left, LOW);
}

void loop() {
  // Read the pulse width of each channel
  int ch1 = pulseIn(pin_ch1, HIGH, 25000); // Read the pulse width of 
  int ch2 = pulseIn(pin_ch2, HIGH, 25000); // each channel
  int ch3 = pulseIn(pin_ch3, HIGH, 25000);
  int ch4 = pulseIn(pin_ch4, HIGH, 25000);
  int ch5 = pulseIn(pin_ch5, HIGH, 25000);
  int ch6 = pulseIn(pin_ch6, HIGH, 25000);

  int ch1_mapped = map(ch1, ch1_min,ch1_max,-500,500);
  int ch2_mapped = map(ch2, ch1_min,ch1_max,-500,500);
  int ch3_mapped = map(ch3, ch1_min,ch1_max,-500,500);
  int ch4_mapped = map(ch4, ch1_min,ch1_max,-500,500);
  int ch5_mapped = map(ch5, ch1_min,ch1_max,-500,500);
  int ch6_mapped = map(ch6, ch1_min,ch1_max,-500,500);

  int right_stick_up_down = ch2_mapped;
  int right_stick_left_right = ch1_mapped;
  int left_stick_up_down = ch3_mapped;
  int left_stick_left_right = ch4_mapped;
  int left_toggle = ch6_mapped;
  int right_toggle = ch5_mapped;

  Serial.print("Right stick updown: "); // center at 0
  Serial.println(right_stick_up_down);
  Serial.print("Right stick leftright: ");
  Serial.println(right_stick_left_right);
  Serial.print("Left stick updown: ");
  Serial.println(left_stick_up_down);
  Serial.print("Left stick leftright: ");
  Serial.println(left_stick_left_right);
  Serial.println("");

  bool single_stick = false;
  bool dual_stick = false;
  if(left_toggle>0){
    Serial.println("Left Switch: Engaged");
    dual_stick = true;
    //single_stick = false;
  } 
  else {
    Serial.println("Left Switch: Disengaged");
    single_stick = true;
    //dual_stick = false;
  }

  bool controls_forwards = false;
  bool controls_backwards = false;
  if(right_toggle>0){
    Serial.println("Right Switch: Engaged");
    controls_forwards = true;
    //controls_backwards = false;
  } 
  else {
    Serial.println("Right Switch: Disengaged");
    controls_backwards = true;
    //controls_forwards = false;
  }

  // Check to see if the controller is connected by seeing if the values read are within the range.
  // Check the 4 important channels to make sure there's no loose wires which could cause the vehicle to move unintentionally
  // Use a decent buffer to reduce potential misinterpretations
  int buff = 300;
  // If the controller isn't connected
  if(  ch1 < (ch1_min - buff) || ch1 > (ch1_max + buff)
    || ch2 < (ch2_min - buff) || ch2 > (ch2_max + buff)
    || ch3 < (ch3_min - buff) || ch3 > (ch3_max + buff)
    || ch4 < (ch4_min - buff) || ch4 > (ch4_max + buff)) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)

    //disable everything
    digitalWrite(pin_right, LOW);
    digitalWrite(pin_left, LOW);
    digitalWrite(pin_reverse_right, LOW);
    digitalWrite(pin_reverse_left, LOW);

    // Controller isn't connected so reset the drive state for when it comes back online.
    reset_smoothing_function();
  }
  // Controller is connected let's do this!
  else {
    //enable on
    digitalWrite(LED_BUILTIN, LOW);  
    if (single_stick) {
      if(!controls_backwards) {
        single_stick_drive(right_stick_left_right, right_stick_up_down);
      }
      else {
        //negate the values which will flip them
        right_stick_left_right = -right_stick_left_right;
        right_stick_up_down = -right_stick_up_down;
        single_stick_drive(right_stick_left_right, right_stick_up_down);
      }
    }
    else if (dual_stick) {
      if(!controls_backwards) {
        dual_stick_drive(left_stick_up_down, right_stick_up_down);
      }
      else {
        //swap left and right and forward and back
        int new_right_stick_up_down = -left_stick_up_down;
        int new_left_stick_up_down = -right_stick_up_down;
        dual_stick_drive(new_left_stick_up_down, new_right_stick_up_down);
      }
    }
  }
}
