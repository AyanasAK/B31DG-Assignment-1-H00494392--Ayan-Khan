#define dataP 25 // Yellow led pin for data signal A
#define syncP 26  // Green led pin for sdata signal B
#define enableBtn 18  // Button input for enabling signal A
#define slctBtn 17  // Button input for selecting the alternative waveform B

#define syncTime 50

bool signalEnbl=false;
bool altMode=false;

int KHAN[]={11,8,1,13};

int a= KHAN[0]*100;         // the starting pulse, also the repaeting one
int b= KHAN[1]*100;         // time off pulse
int c= KHAN[2]+4;           // number of time pulses would be repeated
int d=  KHAN[3]*500;        // idle time before the next pulse cycle

void setup() 
{
  Serial.begin(9600);
  pinMode(dataP, OUTPUT);
  pinMode(syncP,OUTPUT);
  pinMode(enableBtn,INPUT);
  pinMode(slctBtn,INPUT); 
}

void loop() 
{

  if(digitalRead(enableBtn)==HIGH)
  {
    signalEnbl=!signalEnbl;
    Serial.println("Signal A State changed");
    delay (500);
  }

  if (digitalRead(slctBtn)==HIGH)
  {
    altMode= !altMode;  
    Serial.println("Signal B State changed");
    delay (500);
  }
  
  if (signalEnbl)
  {
    normWave();
  }

  if (altMode)
  {
  // last Alpbabet 'K' = 13
  // 13%4= 1 +1 =2
  // so chosing the case 2 i.e. reversed waveform
  altWave();
  } 

}

void normWave()
{
      syncTrig();
      digitalWrite(dataP,HIGH);
      delayMicroseconds(a);

      digitalWrite(dataP,LOW);
      delayMicroseconds(b);
      // Serial.println(a);

      for(int n=2; n<=c;n++)
      {
        long Ton = a+((n-1)*50);  //ON time calculation

        digitalWrite(dataP,HIGH);
        delayMicroseconds(Ton);

        digitalWrite(dataP,LOW);
        delayMicroseconds(b);   // constant low time i.e. b= 1600us

        // Serial.println(Ton);

      }
      delayMicroseconds(d);

}

void altWave()
{

     syncTrig();
     delayMicroseconds(d);
       for(int n=c; n>=1;n--)
      {
        long Ton = a+((n-1)*50);  //ON time calculation

        digitalWrite(dataP,HIGH);
        delayMicroseconds(Ton);

        digitalWrite(dataP,LOW);
        delayMicroseconds(b);   // constant low time i.e. b= 1600us

        // Serial.println(Ton);
      }

      
}

void syncTrig()
{
    digitalWrite(syncP, HIGH);
    delayMicroseconds(syncTime);  // SYNC duration = 50µs
    digitalWrite(syncP, LOW);
}
