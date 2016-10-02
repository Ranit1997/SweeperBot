//IIEST,ROBODARSHAN sweeper bot to clean the water on the floor, it uses the cordination with quadcopter to map the floor  
//initiative by punyajoy,vivek,sayan,ranit.....

//checks if there is wall at right,returns back,stops,turns right,goes forward a little,stops, turns left, loop goes to starting again

//.....pins for sonar.....
int trigL=14,echoL=15,trigF=48,echoF=46,trigR=18,echoR=19,trigB=16,echoB=17;
int thresh=25;
int proximity=2;

//....pins for motor.....
int RMF=10,RMB=11,LMF=8,LMB=9,ARMUP=6,ARMDOWN=7;

int water=A0,right_motorspeed=180,left_motorspeed=170,left_delay=2150,right_delay=2300,forward_delay=2000;
int temp=1,watersensor,three_flag=0,arm=1,flag=1,drivestate=1,flagarm=0,prox=0,k=0;
int pos=0;
void setup() {
  // put your setup code here, to run once:
pinMode(proximity,INPUT);
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
 Serial.println("value water sensor");
 Serial.println(watersensor);
 
 if(watersensor>150)
 {
  return 1;
 }
 else
 {
  return -1;
 }
}
/*void loop()
{
  prox=digitalRead(proximity);
  if(prox==1)
  {
    arm_stop();
    exit(0);
    
  }
  else
  {
    arm_up();
  }
    
}*/

void loop()
{
        

  int t;
  long sonarF;
  
 if(temp==1)  //skipping the error values of the sonar readings for the first time....
{
  sonar(trigF,echoF);
  delay(3000);
  temp=0;
}
  sonarF=sonar(trigF,echoF);
  Serial.println(sonarF);
  if(sonarF<=50&&sonarF>=20)
  {
    drivestate=0;
  }

if(drivestate==1)
 {
    forward();
    if(is_water()==1)
    {
      motor_stop();
      back();
      delay(3000);
      motor_stop();
      arm_down();
      delay(7500);
      arm_stop();
      for(t=0;t<4;t++)
     {
      forward();
      delay(4000);
      motor_stop();
      back();
      delay(2000);
      motor_stop();     
     }
      prox=digitalRead(proximity);
     while(prox==0)
     {
  prox=digitalRead(proximity);
  if(prox==1)
  {
    
    arm_stop();
    break;
  }
  else
  {
    arm_up();
  }
     }
    
    }
     
  } 
  if(drivestate==0)
  {
   back();
   delay(10000);
   motor_stop(); 
   }

  
 
}
