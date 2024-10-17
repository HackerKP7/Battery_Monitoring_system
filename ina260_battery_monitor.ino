#include <Adafruit_INA260.h>

Adafruit_INA260 ina260 = Adafruit_INA260();

float lower_battery_limit = 0;
float upper_battery_limit = 0;
int discrete_battery_percent = 0;
int datalog_gap = 5;
int status,current;

int lowest_num = 100;
int num_samples = 10000;
int once = 0;

void setup() {
  Serial.begin(9600);
  if (!ina260.begin()) {
    Serial.println("Couldn't find INA260 chip");
    while (1);
  }


  int bmstype = 2;

  if (bmstype == 2){
    lower_battery_limit = 3.4;
    upper_battery_limit = 8.4;
  }
  else if (bmstype == 4){
    lower_battery_limit = 10.8;
    upper_battery_limit = 16.8;
  }


}

void loop() {

  
  int current = ina260.readCurrent();
  int battery_percent = round((((ina260.readBusVoltage()/1000) - lower_battery_limit) * 100)/(upper_battery_limit - lower_battery_limit));
  
  float remainder = battery_percent % datalog_gap;


  if (remainder == 0 ){
    discrete_battery_percent = battery_percent;
  }
  else{
    discrete_battery_percent = battery_percent + (datalog_gap - remainder);
  }

  if (discrete_battery_percent < 0){
    discrete_battery_percent = 0;
  }
  if (discrete_battery_percent > 100){
    discrete_battery_percent = 100;  
  }


  if (current > 0){
    status = 1;
  }else{
    status = 0;
  }
  
  Serial.print(ina260.readBusVoltage()/1000);
  Serial.print(", ");
  Serial.print(discrete_battery_percent);
  Serial.print(", ");
  Serial.print(status);
  Serial.print(", ");
  Serial.println(ina260.readCurrent());
// Run this code and dont open the serial monitor, then run the vscode code and open the serial monitor from there. Data readings is done here and plotting and data loggingis been done in vscode.
//If you want proper percent value instead of discrete percent value ( what i have written is if the percent value is between 95 to 100 i will get percent 100 if percent between 90 to 95 we will get 90)
//so if you want proper percent value to get printed like 94,95,96 etc you can change the discrete_battery_percent to battery_percent in serial.println
  

  delay(1000);
}
