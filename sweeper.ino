//IIEST,ROBODARSHAN sweeper bot to clean the water on the floor, it uses the cordination with quadcopter to map the floor  
//initiative by punyajoy,vivek,sayan,ranit.....

//checks if there is wall at right,returns back,stops,turns right,goes forward a little,stops, turns left, loop goes to starting again

//.....pins for sonar.....
int trigL=14,echoL=15,trigF=48,echoF=46,trigR=18,echoR=19,trigB=16,echoB=17;
int thresh=25;

//....pins for motor.....
int RMF=10,RMB=11,LMF=8,LMB=9,ARMUP=6,ARMDOWN=7;

int water=A0,right_motorspeed=180,left_motorspeed=170,left_delay=2150,right_delay=2300,forward_delay=2000;
int temp=0,watersensor,three_flag=0,arm=1,flag=1,drivestate,flagarm=0;
int pos=0;
void setup() {
  // put your setup code here, to run once:
pinMode(trigL,OUTPUT);
pinMode(echoL,INPUT);
pinMode(trigR,OUTPUT);
pinMode(echoR,INPUT);
pinMode(trigF,OUTPUT);
pinMode(echoF,INPUT);
pinMode(trigB,OUTPUT);
pinMode(echoB,INPUT);
pinMode(ARMUP,OUTPUT);
pinMode(ARMDOWN,OUTPUT);
pinMode(RMF,OUTPUT);
pinMode(LMF,OUTPUT);
pinMode(RMB,OUTPUT);
pinMode(LMB,OUTPUT);
pinMode(5,OUTPUT);
pinMode(water,INPUT);
digitalWrite(12,HIGH);
Serial.begin(9600);
}

 

int arm_up()
{
analogWrite(ARMUP,160);
digitalWrite(ARMDOWN,LOW);

}


int arm_down()
{
analogWrite(ARMDOWN,160);
digitalWrite(ARMUP,LOW);
  
}
int arm_stop()
{
analogWrite(ARMDOWN,0);
digitalWrite(ARMUP,LOW);

  
}

int forward()
{
  analogWrite(LMF,left_motorspeed);
  digitalWrite(LMB,LOW);
  analogWrite(RMF,right_motorspeed);
  digitalWrite(RMB,LOW);
}


int back()
{
  analogWrite(LMB,left_motorspeed);
  digitalWrite(LMF,LOW);
  analogWrite(RMB,right_motorspeed);
  digitalWrite(RMF,LOW);
}


int turn_right()
{
  analogWrite(LMF,left_motorspeed);
  digitalWrite(LMB,LOW);
  analogWrite(RMB,right_motorspeed);
  digitalWrite(RMF,LOW);
}


int turn_left()
{
  analogWrite(RMF,right_motorspeed);
  digitalWrite(RMB,LOW);
  analogWrite(LMB,left_motorspeed);
  digitalWrite(LMF,LOW);
}


int motor_stop()
{
  digitalWrite(LMF,LOW);
  digitalWrite(LMB,LOW);
  digitalWrite(RMB,LOW);
  digitalWrite(RMF,LOW);
  delay(100);
}


long sonar(int trig,int echo)
{
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(5);
  digitalWrite(trig,LOW);
  long duration=pulseIn(echo,HIGH);
  long distance(duration/58.2);
  return distance;
}

int is_water() // checking the presence of water....
{
 watersensor=analogRead(water);
 if(watersensor>25)
 {
  return 1;
 }
 else
 {
  return -1;
 }
}
void loop()
{
long sonarF,sonarB,sonarL,sonarR;    //variables of sonar reading
int watersensor;                     // will turn 1 if detects water
//start taking sonar reading ... first few values are absurd hence rejected
if(temp==1)  //skipping the error values of the sonar readings for the first time....
{
  sonar(trigF,echoF);
  delay(1000);
  temp=0;
}
//....

sonarF=sonar(trigF,echoF);
Serial.print(sonarF);
sonarL=sonar(trigL,echoL);
Serial.print(sonarL);
sonarR=sonar(trigR,echoR);
Serial.print(sonarR);
sonarB=sonar(trigB,echoB);
Serial.print(sonarB);
Serial.println();  
//starting of the cleaning process deciding which position.... given by drive state
//checking the position of the bot....
//.... 1 for left corner,2 for right corner,3 for middle....
if(flag==1) //Determining drivestates. Drivestate 1 denotes going forward, coming back, turning right, going forward
{           //turning left. Drivestate 2 denotes going forward, coming back, turning left, going forward, turning right
  if((sonarL<=5) && (sonarB<=5)) //left corner
  {
    drivestate = 1;   //left corner 
  }
  else if((sonarR<=5) && (sonarB<=5))//right corner
  {
    drivestate = 2;   //right corner
  }
  else  //middle position
  {
    drivestate = 3;
  }
flag=0;//flushing the flag so that drivestates are not determined again
}
 if(drivestate==3) //the bot was in middle position. taking it to the left corner
{
  while(sonarL>4)
  {
  turn_left();
  sonarL=sonar(trigL,echoL);
  }   
  motor_stop();
  sonarF=sonar(trigF,echoF);
  while(sonarF>=15)
  {
    forward();
    sonarF=sonar(trigF,echoF);
  }
  
  while(sonarB>4)
  {turn_right();
   sonarB=sonar(trigB,echoB);
  }
  motor_stop();
  drivestate=1; //left corner reached, flushing the three_flag. it won't go left at the next loop
}
  //main loop for going forward.....
while(1)
{
  //Serial.print("loop 1");
  if((is_water()==1) && (arm==1))
  {
    back();
    delay(750);
    motor_stop();
    arm_down();
    delay(700);
    arm_stop();
    arm=0;//....storing the position of the arm....
  }
  forward();
  sonarF=sonar(trigF,echoF);
  if(sonarF<=30) //front wall is reached...
  {
    motor_stop();
    arm_up();
    delay(700);
    arm_stop();
    arm=1;
    break;
  }
  
}
//......
back();
//main loop for going backward....
while(1)
{
  //Serial.print("loop 2");
  sonarB=sonar(trigB,echoB);
  if(sonarB<=4) //back wall is reahed
  {
    motor_stop();
    break;
  }
  back();
}
//.....
sonarF=sonar(trigF,echoF);
Serial.print(sonarF);
sonarL=sonar(trigL,echoL);
Serial.print(sonarL);
sonarR=sonar(trigR,echoR);
Serial.print(sonarR);
sonarB=sonar(trigB,echoB);
Serial.print(sonarB);
Serial.println();  
if((drivestate==1&&sonarR<=4)||(drivestate==2&&sonarL<=4))
{
  motor_stop();
  exit(0);
} 


if(drivestate==1)
{
  while(sonarR>4)
  {
  turn_right();
  sonarR=sonar(trigR,echoR);
  }
  //delay(2300);
  forward();
  delay(2000);
  while(sonarB>4)
  {
  turn_left();
  sonarB=sonar(trigB,echoB);
  }
  motor_stop();
}
 else if(drivestate==2)
{
  while(sonarL>4)
  {
  turn_left();
  sonarL=sonar(trigL,echoL);
  }
 // delay(2150);
  forward();
 delay(2000);
  turn_right();
 //delay(2300);
 while(sonarB>4)
  {
  turn_right();
  sonarB=sonar(trigB,echoB);
  }
  motor_stop();
}

Serial.println();
Serial.println();
}





