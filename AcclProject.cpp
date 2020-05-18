#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

// Variables
int currentposition = 1;
int newposition = 0;
int pincorrect = 0;
volatile boolean unlocked = false;
volatile boolean writingpass = false;
volatile boolean passwordstored = false;
volatile boolean storingpass = false;

int initialposition = 1;
int passpointer = 0;
int passinput = 0;

const int buttonPin = PIN_SW0; // the number of the push-button pin
const int ledPin = PIN_LED_13; // the number of the LED pin

int x = 0;
int y = 0;
int z = 0;

int gesture;
int password[6] = {0, 0, 0, 0, 0, 0};
int passwordtry[6] = {0, 0, 0, 0, 0, 0};

volatile boolean ledOn = false;

void setup() {

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  attachInterrupt(buttonPin, startButton, CHANGE);
  
  // Initialize the Accelerometer parameters
  Serial.begin(9600);

  //If the accelerometer was not detected
  if(!accel.begin())
  {
    Serial.println("No ADXL345 sensor detected.");

    // If an error ocurrs (for example, the accelerometer isnt connected) we enter an infinite loop.
    while (1);
  }
}

void loop() {

  // Read from the accelerometer
   sensors_event_t event; 
   accel.getEvent(&event);

   // print to console
   // Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
   // Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
   // Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");
   // Serial.println("m/s^2 ");
   // Serial.println(currentposition);
   delay(500);

   if((event.acceleration.x > -2) && (event.acceleration.x < 2))
   {
      x = 0;
   }

   if((event.acceleration.y > -2) && (event.acceleration.y < 2))
   {
      y = 0;
   }

   if((event.acceleration.z > -2) && (event.acceleration.z < 2))
   {
      z = 0;
   }

   if((event.acceleration.x > 8) && (event.acceleration.x < 12))
   {
      x = 1;
   }
   
   if((event.acceleration.y > 8) && (event.acceleration.y < 12))
   {
      y = 1;
   }

   if((event.acceleration.z > 8) && (event.acceleration.z < 12))
   {
      z = 1;
   }
   
   if((event.acceleration.x > -12) && (event.acceleration.x < -8))
   {
      x = -1;
   }
   
   if((event.acceleration.x > -12) && (event.acceleration.x < -8))
   {
      x = -1;
   }

   if((event.acceleration.y > -12) && (event.acceleration.y < -8))
   {
      y = -1;
   }

   if((event.acceleration.z > -12) && (event.acceleration.z < -8))
   {
      z = -1;
   }

   // matrix
    int pos[3] = {x, y, z};

    // Available positions
//    int gesture1[3] = {0, 0, 1};
//    int gesture2[3] = {-1, 0, 0};
//    int gesture3[3] = {1, 0, 0};
//    int gesture4[3] = {0, 1, 0};
//    int gesture5[3] = {0, -1, 0};
//    int gesture6[3] = {0, 0, -1};

   // gesture
     if (x==0 && y==0 && z==1)
     {
        gesture = 1;
     }
     if (x==-1 && y==0 && z==0)
     {
        gesture = 2;
     }
     if (x==1 && y==0 && z==0)
     {
        gesture = 3;
     }
     if (x==0 && y==1 && z==0)
     {
        gesture = 4;
     }
     if (x==0 && y==-1 && z==0)
     {
        gesture = 5;
     }
     if (x==0 && y==0 && z==-1)
     {
        gesture = 6;
     }
   
    // Print acceleration and hand position
    Serial.print("Xa: "); Serial.print(event.acceleration.x); Serial.print("  "); Serial.print("Ya: "); Serial.print(event.acceleration.y); Serial.print("  "); Serial.print("Za: "); Serial.print(event.acceleration.z); Serial.print("  "); Serial.print("Hand Position:  "); Serial.println(gesture);

    if(passwordstored)
    {
      Serial.println ("Password Stored");
      digitalWrite(ledPin, HIGH);
    }else
    {
      //Serial.println("Password Missing!");
      digitalWrite(ledPin, LOW);
    }

    if(storingpass)
    {
      password[passpointer] = currentposition;
      newposition = gesture;

      if(currentposition != newposition)
      {
        passpointer = passpointer + 1;
        password[passpointer] = newposition;
        currentposition = newposition;
        passinput = passinput + 1;
      }

      if(passinput == 5)
      {
        storingpass = false;
        passwordstored = true;
        Serial.println("StoredPassword:   ");
        for(int j = 0; j <= 5; j = j+1)
        {
          Serial.print(password[j]);
        }
        delay(2000);
        passinput = 0;
        currentposition = 1;
        newposition = 0;
        passpointer = 0;
        gesture = 1;
        digitalWrite(ledPin, LOW);     
      }
      Serial.println(passinput);    
    }

    if(writingpass)
    {  
      passwordtry[passpointer] = currentposition;
      newposition = gesture;

      if(currentposition != newposition)
      {
        passpointer = passpointer + 1;
        passwordtry[passpointer] = newposition;
        currentposition = newposition;
        passinput = passinput + 1;
      }

      if(passinput == 5)
      {
        writingpass =  false;
        Serial.print("Tried Password:  "); 
        for(int j = 0; j <= 5; j = j+1)
        {
          Serial.print(passwordtry[j]);
        }

        for(int i = 0; i <= 5; i = i+1)
        {
          if(password[i] == passwordtry[i])
          {
            pincorrect = pincorrect + 1;
            passinput = 0;
            writingpass = false;
          }
          else
          {
            Serial.println("Wrong Password...See yoy later :(");
            delay(2000);
            unlocked = false;
            writingpass = false;
            passinput = 0;
            currentposition = 1;
            newposition = 0;
            passwordtry[0] = 0; passwordtry[1] = 0; passwordtry[2] = 0; passwordtry[3] = 0; passwordtry[4] = 0; passwordtry[5] = 0;
            passpointer = 0;
            break;
          }
        }

        if(pincorrect >= 5)
        {
          unlocked = true;
          unlockstuff();
         }      
      }
      Serial.println(passinput);   
    }    
}

//Interrupt
void startButton()
{
  delay(200);
  Serial.println("Interrupt!!");
  if(ledOn)
  {
    ledOn = false;
    digitalWrite(ledPin, LOW);
  } else
  {
    ledOn = true;
    digitalWrite(ledPin, HIGH);

    //check if password is already stored
    if(passwordstored)
    {
      writingpass = true;
    }else
    {
      storingpass = true;
    }

    passwordtry[0] = 0; passwordtry[1] = 0; passwordtry[2] = 0; passwordtry[3] = 0; passwordtry[4] = 0; passwordtry[5] = 0;
    int passpointer = 0;
    pincorrect = 0;
    passinput = 0;
    currentposition = 1;
    newposition = 0;
   }
}

void unlockstuff()
{
  //do stuff
  Serial.println("Correct Password!!");
  Serial.println("    ");
  delay(1000);
  Serial.println("Opening Door...");
  Serial.println("    ");
  Serial.println("    ");
  Serial.println("    ");
  
  // Door opener
  delay(3000);
  passinput = 0;
  currentposition = 1;
  newposition = 0;
  passwordtry[0] = 0; passwordtry[1] = 0; passwordtry[2] = 0; passwordtry[3] = 0; passwordtry[4] = 0; passwordtry[5] = 0;
  passpointer = 0;
}