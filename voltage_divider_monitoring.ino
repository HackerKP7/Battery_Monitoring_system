float lower_battery_limit = 10.8;
float upper_battery_limit = 16.8;
float battery_offset = 0.35; //please change accodingly
int discrete_battery_percent = 0;

int lowest_num = 100;
float R1 = 36400;
float R2 = 15100;
int num_samples = 10000;
int once = 0;

void setup() {
  Serial.begin(9600);

}

void loop() {



  float total_analog_value = 0;
  for (int i = 0; i < num_samples; i++) {
    total_analog_value += analogRead(A0);

  }
  float analog_value = (total_analog_value / num_samples) ;
  // Serial.println(analog_value);
  float battery_volts_5V = ((analog_value*5)/1023);
  // Serial.println(battery_volts_5V);
  float battery_volts = (battery_volts_5V / (R2/(R1+R2))) - battery_offset; 
  int battery_percent = round(((battery_volts - lower_battery_limit) * 100)/(upper_battery_limit - lower_battery_limit));


  if (90 < battery_percent && battery_percent <= 100){
    discrete_battery_percent = 100;
  }
  else if(80 < battery_percent && battery_percent <= 90){
    discrete_battery_percent = 90;
  }
  else if(70 < battery_percent && battery_percent <= 80){
    discrete_battery_percent = 80;
  }
  else if(60 < battery_percent && battery_percent <= 70){
    discrete_battery_percent = 70;
  }
  else if(50 < battery_percent && battery_percent <= 60){
    discrete_battery_percent = 60;
  }
  else if(40 < battery_percent && battery_percent <= 50){
    discrete_battery_percent = 50;
  }
  else if(30 < battery_percent && battery_percent <= 40){
    discrete_battery_percent = 40;
  }
  else if(20 < battery_percent && battery_percent <= 30){
    discrete_battery_percent = 30;
  }
  else if(10 < battery_percent && battery_percent <= 20){
    discrete_battery_percent = 20;
  }
  else if(0 < battery_percent && battery_percent <= 10){
    discrete_battery_percent = 10;
  }
  else{
    discrete_battery_percent = 0;
  }

 
  Serial.print(battery_volts);
  Serial.print(", ");
  Serial.println(discrete_battery_percent);

  

  delay(1000);
}
