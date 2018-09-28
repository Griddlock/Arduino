int pinlevel[15];
int pintarget[15];
int pintimer[15];
int pinmin[15];
int pinmax[15];
int pindir[15];

void setup() {
  // Analog outputs
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  // Digital outputs
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);

  // Digital inputs
  pinMode(12, INPUT);
  
  // Setup
  pinlevel[2] = 0;
  pintimer[2] = 100;
  
  pinlevel[3] = 0;
  pinmin[3] = 0; // must be even
  pinmax[3] = 128; // must be even
  
  pinlevel[5] = 0;
  pinmin[5] = 0;
  pinmax[5] = 128;
  pindir[5] = 0;
  
  pinmin[6] = 0;
  pinmax[6] = 128;

}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(12) == false)
  {

  // ############################## Pin 2 - random frequency and duration flashes (LED on back)
  
  if (pintimer[2] <= 0)
  {
    // If on already, turn off. If off, turn on.
    if (pinlevel[2] == 0)
    {
      pinlevel[2] = 1;
      digitalWrite(2, true);
      pintimer[2] = random(12, 50);
    } else {
      pinlevel[2] = 0;
      digitalWrite(2, false);
      pintimer[2] = random(15, 200);
    }
  }
  pintimer[2]--;


  // ############################## Pin 3 - random brightness fluctuation (green triangle)
  
  if (pinlevel[3] == pintarget[3])
  {
    pintimer[3]--;
    if (pintimer[3] <= 0)
    {
      // timer is done, move to a new target brightness
      pintimer[3] = random(25, 200);
      pintarget[3] = (random(pinmin[3], pinmax[3] / 2)) * 2;
    }
  }
  if (pinlevel[3] < pintarget[3]) { pinlevel[3] += 2; }
  if (pinlevel[3] > pintarget[3]) { pinlevel[3] -= 2; }
  analogWrite(3, pinlevel[3]);


  // ############################## Pin 4 - regular flashing every 1000ms (LED on leg)
  
  if (pintimer[4] <= 0)
  {
    if (pinlevel[4] == 0)
    { pinlevel[4] = 1; digitalWrite(4, true); pintimer[4] = 12; }
      else
    { pinlevel[4] = 0; digitalWrite(4, false); pintimer[4] = 38; }
  }


  // ############################## Pin 5 - soft fade up and down (big glow stick)
  
  if ((pindir[5] == 0) && (pinlevel[5] < pinmax[5])) { pinlevel[5]++; if (pinlevel[5] >= pinmax[5]) { pindir[5] = 1; } }
  if ((pindir[5] == 1) && (pinlevel[5] > pinmin[5])) { pinlevel[5]--; if (pinlevel[5] <= pinmin[5]) { pindir[5] = 0; } }
  analogWrite(5, pinlevel[5]);


  // ############################## Pin 6 - random sudden brightness changes (red behind circuitboard)
  
  if (pintimer[6] <= 0)
  {
    // switch the brightness, reset the timer
    pinlevel[6] = random(pinmin[6], pinmax[6]);
    analogWrite(6, pinlevel[6]);
    pintimer[6] = random(12, 500); // between 240ms and 10s
  }
  pintimer[6]--;
  
  } else {  // digitalRead(12)
    digitalWrite(2, false);
    digitalWrite(4, false);
    if (pinlevel[3] > 0) { pinlevel[3]--; analogWrite(3, pinlevel[3]); }
    if (pinlevel[5] > 0) { pinlevel[5]--; analogWrite(3, pinlevel[5]); pindir[5] = 0; }
  }

  delay(20);
}
