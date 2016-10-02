//IIEST, logger manager for spazor event
int i,average,sum;
int s[7]={A0,A1,A2,A3,A4,A5,5};
int sensor[7];

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
for(i=0;i<7;i++)
pinMode(s[i],INPUT);
Serial.println("give the value of Kp,Ki,Kd")
   Serial.read( 
}

void loop() {
  // put your main code here, to run repeatedly:
 for(i=0;i<7;i++) 
sensor[i] = analogRead(s[i]);
for(i=0;i<7;i++)
{Serial.print(sensor[i]);
Serial.print("\t");}
average=0;
sum=0;
for(i=0;i<7;i++)
{
  average+=sensor[i]*i;
  sum+=sensor[i];
}
Serial.print(average);
Serial.print("\t");
Serial.print(sum);
Serial.print("\t");
Serial.print(average /sum);
Serial.print("\n");
delay(1000);
}
