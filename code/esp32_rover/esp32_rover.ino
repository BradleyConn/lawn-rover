#include <Arduino.h>
#include <analogWrite.h>

// pin definitions
int pin_ch1 = 34;
int pin_ch2 = 35;
int pin_ch3 = 32;
int pin_ch4 = 33;
int pin_ch5 = 25;
int pin_ch6 = 26;
// array approach is probably better
// pins for channels 1 to 6
int pin_ch[6] = {
  34, 35, 32, 33, 25, 26};

int pin_reverse_left = 12; // not good as an input
int pin_reverse_right = 14;

int pin_left = 13;
int pin_right = 27;

int pin_led_up = 5;
int pin_led_down = 17;
int pin_led_left = 16;
int pin_led_right = 4;


// channel values
int ch1;
int ch2;
int ch3;
int ch4;
int ch5;
int ch6;
// array approach is probably better
int ch[6];

// right stick x = ch1
// right stick y  = ch2
// left stick y = ch 3
// left stick x = ch 4
// top right switch = ch 5
// top left wtich = ch 6




// TODO: Make a calibration routine with a button
// May only need to do it for one channel and apply to all
int ch1_max = 2000;
int ch1_min = 1000;
int ch2_max = 1822;
int ch2_min = 922;

void led_display_off() {
  digitalWrite(pin_led_up, LOW);
  digitalWrite(pin_led_down, LOW);
  digitalWrite(pin_led_left, LOW);
  digitalWrite(pin_led_right, LOW);
}
void led_display_on() {
  digitalWrite(pin_led_up, HIGH);
  digitalWrite(pin_led_down, HIGH);
  digitalWrite(pin_led_left, HIGH);
  digitalWrite(pin_led_right, HIGH);
}
void single_stick_led_display(int ch_x_mapped, int ch_y_mapped) {
  led_display_off();
  //left
  if(ch_x_mapped < -100) {
    digitalWrite(pin_led_left, HIGH);
  }
  //right
  else if (ch_x_mapped > 100) {
    digitalWrite(pin_led_right,HIGH);
  }
  //down
  if(ch_y_mapped < -100) {
    digitalWrite(pin_led_down,HIGH);
  }
  //up
  else if (ch_y_mapped > 100) {
    digitalWrite(pin_led_up, HIGH);
  }
}
void dual_stick_led_display(int left, int right) {
  led_display_off();
  if(abs(left) > 150) {
    digitalWrite(pin_led_left,HIGH);
  }
  if(abs(right) > 150) {
    digitalWrite(pin_led_right,HIGH);
  }
}

// Quick changes in the motors is violent on the vehicle and its electronics.
// Try to reduce instantanious large changes.
// Keep some historical data to make informed corrections.
// Turn off moving average for now
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
// Step per second allowed.
// As a guess, don't go more than 50% in half a second? that might be slow...
// Range is from -500 to +500
#define max_step  75
#define step_time_ms  50
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
void drive(int left, int right) {

  smooth_driving(&left, &right);
  /*if(left <100 && left >-100) {
    left = 0;
  }
  if(right <100 && right >-100) {
    right = 0;
  }*/

Serial.println(left);
Serial.println(right);

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


 //clip the values over 500, and set anything over 400 to max
  if (left > 450) {
    left = 500; 
  }
  if (right > 450) {
    right = 500;
  }
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

void single_stick_drive(int ch_x_mapped, int ch_y_mapped) {
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

  //leds
  pinMode(pin_led_up, OUTPUT);
  pinMode(pin_led_down, OUTPUT);
  pinMode(pin_led_left, OUTPUT);
  pinMode(pin_led_right, OUTPUT);

  Serial.begin(115200); // Pour a bowl of Serial

  //disable everything on start up.
  digitalWrite(pin_right, LOW);
  digitalWrite(pin_left, LOW);
  digitalWrite(pin_reverse_right, LOW);
  digitalWrite(pin_reverse_left, LOW);



}

bool pulse = false;
void loop() {
  // Read the pulse width of each channel
  ch1 = pulseIn(pin_ch1, HIGH, 25000); // Read the pulse width of 
  ch2 = pulseIn(pin_ch2, HIGH, 25000); // each channel
  ch3 = pulseIn(pin_ch3, HIGH, 25000);
  ch4 = pulseIn(pin_ch4, HIGH, 25000);
  ch5 = pulseIn(pin_ch5, HIGH, 25000);
  ch6 = pulseIn(pin_ch6, HIGH, 25000);


  //Serial.print("Right Stick X:"); // Ch3 was x-axis 
  //Serial.print(ch3); // center at 0
  //Serial.print(" mapped: "); // center at 0
  //Serial.println(map(ch3, 922,1822,-500,500)); // center at 0
  //Serial.print("Right Stick Y:"); // Ch2 was y-axis
  //Serial.println(map(ch2, 922,1822, -500,500)); // center at 0
  //Serial.println(); //make some room

  //int ch3_mapped = map(ch3, ch1_min,ch1_max,-500,500);
  //int ch2_mapped = map(ch2, ch1_min,ch1_max,-500,500);
  //use the old min and max
  //Serial.println(ch1); // center at 0

  int ch1_mapped = map(ch1, ch1_min,ch1_max,-500,500);
  /*Serial.print("ch1 raw: "); // center at 0
  Serial.print(ch1); // center at 0
  Serial.print("    ch1 mapped: "); // center at 0
  Serial.println(ch1_mapped); // center at 0
  */
  int ch2_mapped = map(ch2, ch1_min,ch1_max,-500,500);
  /*Serial.print("ch2 raw: "); // center at 0
  Serial.print(ch2); // center at 0
  Serial.print("    ch2 mapped: "); // center at 0
  Serial.println(ch2_mapped); // center at 0
  */
  int ch3_mapped = map(ch3, ch1_min,ch1_max,-500,500);
  //Serial.print("ch3 mapped: "); // center at 0
  //Serial.println(ch3_mapped); // center at 0
  int ch4_mapped = map(ch4, ch1_min,ch1_max,-500,500);
  //Serial.print("ch4 mapped: "); // center at 0
  //Serial.println(ch4_mapped); // center at 0
  int ch5_mapped = map(ch5, ch1_min,ch1_max,-500,500);
  //Serial.print("ch5 mapped: "); // center at 0
  //Serial.println(ch5_mapped); // center at 0
  int ch6_mapped = map(ch6, ch1_min,ch1_max,-500,500);
  //Serial.print("ch6 mapped: "); // center at 0
  //Serial.println(ch6_mapped); // center at 0
  //Serial.println(); // center at 0
  

  int right_stick_up_down = ch2_mapped;
  int right_stick_left_right = ch1_mapped;
  int left_stick_up_down = ch3_mapped;
  int left_stick_left_right = ch4_mapped;
  int left_toggle = ch6_mapped;
  int right_toggle = ch5_mapped;

  Serial.print("Right stick updown: "); // center at 0
  Serial.println(right_stick_up_down); // center at 0
    Serial.print("Right stick leftright: "); // center at 0
  Serial.println(right_stick_left_right); // center at 0
    Serial.print("Left stick updown: "); // center at 0
  Serial.println(left_stick_up_down); // center at 0
    Serial.print("Left stick leftright: "); // center at 0
  Serial.println(left_stick_left_right); // center at 0

  Serial.println("");

  for (int p=0;p<0;p++){
    Serial.println(); // center at 0
  }

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
    || ch3 < (ch1_min - buff) || ch3 > (ch1_max + buff)
    || ch4 < (ch1_min - buff) || ch4 > (ch1_max + buff)) {
    led_display_off();
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)

    //disable everything
    digitalWrite(pin_right, LOW);
    digitalWrite(pin_left, LOW);
    digitalWrite(pin_reverse_right, LOW);
    digitalWrite(pin_reverse_left, LOW);

    // Controller isn't connected so reset the drive statefor when it comes back online.
    reset_smoothing_function();
  }
  else {
    //enable on
    digitalWrite(LED_BUILTIN, LOW);  
    if (single_stick) {
      if(!controls_backwards) {
        //single_stick_led_display(right_stick_left_right, right_stick_up_down);
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
        //dual_stick_led_display(left_stick_up_down, right_stick_up_down);
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


  //delay(1000);// I put this here just to make the terminal 
  // window happier
  pulse = !pulse;
  if (pulse) {
  //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else{
  //delay(2000);                       // wait for a second
  //digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  //delay(1000);                       // wait for a second
  }
  //analogWrite(pin_left, 150);//map(left, 100, 500, 0, 255));
  //analogWrite(pin_right, 150);//map(right, 100, 500, 0, 255));

}
