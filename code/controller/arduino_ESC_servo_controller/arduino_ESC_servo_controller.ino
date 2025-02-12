#include <M5Unified.h>
#include <ESP32Servo.h>

Servo ESC;
Servo steering_servo;

int start_motor_value = 50;
int min_motor_value = 85;
int max_motor_value = 110;

int motor_value;
float accelerator = 0.0;
int steering = 0;

int no_signal_count = 0;
int no_signal_threshhold = 10000;
unsigned long last_update = 0;
const int motor_update_interval = 10; // Adjust speed update interval (ms)

// Multiplier settings
float esc_multiplier = 1.0;       // Can be 1.0 (normal) or -1.0 (reverse)
float steering_multiplier = -1.0;  // Can be 1.0 (normal) or -1.0 (invert steering)

// Previous values to detect changes
int prev_motor_value = -1;
int prev_steering = -1;

void setup() 
{
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Display.setTextSize(2);
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  
  // Attach servos
  steering_servo.attach(7);   // Use GPIO3 for steering
  ESC.attach(8, 1100, 2000);  // Use GPIO2 for ESC
  
  Serial.begin(115200);

  motor_value = start_motor_value;
  ESC.write(motor_value);
  steering_servo.write(90);
}

void loop() 
{
   if (Serial.available())
   {
      String input = Serial.readStringUntil('\n');
      int commaIndex = input.indexOf(',');
      if (commaIndex != -1) 
      {
         accelerator = constrain(input.substring(0, commaIndex).toInt() / 100.0, 0.0, 1.0);
         steering = input.substring(commaIndex + 1).toInt() - 50;
         no_signal_count = 0;
      }
   }
   else
   {
      no_signal_count++;
   }

   if (no_signal_count >= no_signal_threshhold)
   {
      accelerator = 0.0;
   }

   // Apply multipliers
   float adjusted_accelerator = accelerator * esc_multiplier;
   float adjusted_steering = steering * steering_multiplier;

   // Gradually adjust motor speed
   if (millis() - last_update >= motor_update_interval) 
   {
      last_update = millis();
      int target_motor_value = int(min_motor_value + (max_motor_value - min_motor_value) * adjusted_accelerator);
      
      if (motor_value < target_motor_value)
      {
         motor_value++;
      }
      else if (motor_value > target_motor_value)
      {
         motor_value--;
      }
   }

   // Always write new values to servos
   ESC.write(motor_value);
   steering_servo.write(90+adjusted_steering);

   // Only update display if values change
   if (steering != prev_steering || motor_value != prev_motor_value)
   {
      M5.Display.fillRect(0, 0, 128, 50, TFT_BLACK);
      M5.Display.setCursor(0, 0);
      M5.Display.printf("Steering: %d\n", (int)adjusted_steering);
      M5.Display.printf("ESC Value: %d\n", motor_value);
      //M5.Display.printf("ESC Mult: %.1f\n", esc_multiplier);
      //M5.Display.printf("Steer Mult: %.1f\n", steering_multiplier);
      
      prev_steering = steering;
      prev_motor_value = motor_value;
   }

   // Adjust multipliers using M5 buttons
   //M5.update();
   //if (M5.BtnA.wasPressed()) esc_multiplier *= -1.0;  // Toggle ESC forward/reverse
   //if (M5.BtnB.wasPressed()) steering_multiplier *= -1.0; // Invert steering
}
#include <M5Unified.h>
#include <ESP32Servo.h>

Servo ESC;
Servo steering_servo;

int start_motor_value = 50;
int min_motor_value = 85;
int max_motor_value = 110;

int motor_value;
float accelerator = 0.0;
int steering = 0;

int no_signal_count = 0;
int no_signal_threshhold = 10000;
unsigned long last_update = 0;
const int motor_update_interval = 10; // Adjust speed update interval (ms)

// Multiplier settings
float esc_multiplier = 1.0;       // Can be 1.0 (normal) or -1.0 (reverse)
float steering_multiplier = -1.0;  // Can be 1.0 (normal) or -1.0 (invert steering)

// Previous values to detect changes
int prev_motor_value = -1;
int prev_steering = -1;

void setup() 
{
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Display.setTextSize(2);
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  
  // Attach servos
  steering_servo.attach(7);   // Use GPIO3 for steering
  ESC.attach(8, 1100, 2000);  // Use GPIO2 for ESC
  
  Serial.begin(115200);

  motor_value = start_motor_value;
  ESC.write(motor_value);
  steering_servo.write(90);
}

void loop() 
{
   if (Serial.available())
   {
      String input = Serial.readStringUntil('\n');
      int commaIndex = input.indexOf(',');
      if (commaIndex != -1) 
      {
         accelerator = constrain(input.substring(0, commaIndex).toInt() / 100.0, 0.0, 1.0);
         steering = input.substring(commaIndex + 1).toInt() - 50;
         no_signal_count = 0;
      }
   }
   else
   {
      no_signal_count++;
   }

   if (no_signal_count >= no_signal_threshhold)
   {
      accelerator = 0.0;
   }

   // Apply multipliers
   float adjusted_accelerator = accelerator * esc_multiplier;
   float adjusted_steering = steering * steering_multiplier;

   // Gradually adjust motor speed
   if (millis() - last_update >= motor_update_interval) 
   {
      last_update = millis();
      int target_motor_value = int(min_motor_value + (max_motor_value - min_motor_value) * adjusted_accelerator);
      
      if (motor_value < target_motor_value)
      {
         motor_value++;
      }
      else if (motor_value > target_motor_value)
      {
         motor_value--;
      }
   }

   // Always write new values to servos
   ESC.write(motor_value);
   steering_servo.write(90+adjusted_steering);

   // Only update display if values change
   if (steering != prev_steering || motor_value != prev_motor_value)
   {
      M5.Display.fillRect(0, 0, 128, 50, TFT_BLACK);
      M5.Display.setCursor(0, 0);
      M5.Display.printf("Steering: %d\n", (int)adjusted_steering);
      M5.Display.printf("ESC Value: %d\n", motor_value);
      //M5.Display.printf("ESC Mult: %.1f\n", esc_multiplier);
      //M5.Display.printf("Steer Mult: %.1f\n", steering_multiplier);
      
      prev_steering = steering;
      prev_motor_value = motor_value;
   }

   // Adjust multipliers using M5 buttons
   //M5.update();
   //if (M5.BtnA.wasPressed()) esc_multiplier *= -1.0;  // Toggle ESC forward/reverse
   //if (M5.BtnB.wasPressed()) steering_multiplier *= -1.0; // Invert steering
}
