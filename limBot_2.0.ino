#include <Servo.h>

Servo q1;
Servo q2;
Servo q3;
Servo q4;
double lastLeftFilter = 90;
double lastRightFilter = 90;
double filter = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial);
  q1.attach(2);
  q2.attach(3);
  q3.attach(4);
  q4.attach(5);
  pinMode(8,INPUT);
  pinMode(9,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  double pwmDrive = pulseIn(8,HIGH);
  double pwmSteer = pulseIn(9,HIGH);

  double angleDrive = map(pwmDrive,1000,2000,0,180);
  double angleSteer = map(pwmSteer,1000,2000,0,180);

  double left = angleDrive + .5 * (angleSteer-90);
  double right = angleDrive - .5 * (angleSteer-90);

  if (right > 180){
    double offset = right-180;
    right -= offset;
    left -= offset;
  }
  else if (right < 0){
    double offset = 0-right;
    right += offset;
    left += offset;
  }
  else if (left > 180){
    double offset = left-180;
    left -= offset;
    right -= offset;
  }
  else if (left < 0){
    double offset = 0-left;
    left += offset;
    right += offset;
  }

  double filteredLeft = (left + (lastLeftFilter * filter)) / (filter + 1);
  lastLeftFilter = filteredLeft;
  double filteredRight = (right + (lastRightFilter * filter)) / (filter + 1);
  lastRightFilter = filteredRight;
  
  if (filteredRight < 95 and filteredRight > 85){
    filteredRight = 90;
  }
  else if (filteredRight > 95){
    filteredRight -= 5;
  }
  else{
    filteredRight += 5;
  }
  
  if (filteredLeft < 95 and filteredLeft > 85){
    filteredLeft = 90;
  }
  else if (filteredLeft > 95){
    filteredLeft -= 5;
  }
  else{
    filteredLeft += 5;
  }

  if(pwmSteer > 750){
    Serial.print("OK");
    Serial.print(" , ");
    Serial.print(filteredRight);
    Serial.print(" , ");
    Serial.print(filteredLeft);
    //Serial.print(" , ");
    //Serial.print(pwmDrive);
    //Serial.print(" , ");
    //Serial.print(pwmSteer);
    Serial.println();
    
    q1.write(filteredRight);
    q2.write(filteredLeft);
    q3.write(filteredLeft);
    q4.write(filteredRight);
  }
  else{
    Serial.print("NOT OK");
    Serial.println();
    
    q1.write(90);
    q2.write(90);
    q3.write(90);
    q4.write(90);
  }

}
