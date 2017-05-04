#include<Wire.h>
#include<Adafruit_Sensor.h>
#include<Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include<math.h>

/* Assign a unique ID to the sensors */
Adafruit_L3GD20_Unified        gyro = Adafruit_L3GD20_Unified(20);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
/*Sensor Variables*/
sensors_event_t gyro_event;
sensors_event_t accel_event;
sensors_vec_t   curr_est;
sensors_vec_t   past_est;
long past_time;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initSensors();
  past_time=millis();
  accel.getEvent(&accel_event);
  gyro.getEvent(&gyro_event);
  past_time=updateAngle(&curr_est,&past_est,&accel_event,&gyro_event,past_time);
}

void loop() {
    accel.getEvent(&accel_event);
    gyro.getEvent(&gyro_event);
    past_time=updateAngle(&curr_est,&past_est,&accel_event,&gyro_event,past_time);
    Serial.println(curr_est.roll);
    delay(5);
}

long updateAngle(sensors_vec_t *curr_event, sensors_vec_t *past_event,sensors_event_t *accel_event,sensors_event_t *gyro_event,long past_time){
    past_event=curr_event;
    long curr_time=gyro_event->timestamp;
    double dt=double(curr_time-past_time)/1000.0;
    float gyro_rateX=gyro_event->gyro.x;
    gyro_rateX*=(180.0 / PI);
    //Serial.println(gyro_rateX);
    double accAngleX= (double)atan2(accel_event->acceleration.y, accel_event->acceleration.z) * (180.0 / PI);
    curr_event->roll  = 0.98*(past_event->roll + (gyro_rateX*dt)) + 0.02*accAngleX;
    return curr_time;
}

void initSensors()
{
  if(!accel.begin()){
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");
    while(1);
  }
  if(!gyro.begin()){
    /* There was a problem detecting the LM303 ... check your connections */
    Serial.println("Ooops, no LM303 detected ... Check your wiring!");
    while(1);
  }
}
