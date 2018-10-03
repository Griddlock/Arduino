bool ButtonPressed[15];
bool OutputOn[15];
int eyeMode = 2;
int eyeLevel[10];
int randBlink = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, OUTPUT);   // Eye, Laser LED
  pinMode(3, OUTPUT); // Eye, Left LED
  pinMode(5, OUTPUT); // Eye, Top LED
  pinMode(6, OUTPUT); // Eye, Right LED
  pinMode(9, OUTPUT); // Eye, Bottom LED
  pinMode(10, OUTPUT); // Eye laser indicator
  pinMode(12, OUTPUT); // Vocal Modulator
  
  
  pinMode(4, INPUT);    // Eye laser button
  pinMode(7, INPUT);    // Eye mode toggle
  pinMode(8, INPUT);    // Eye flicker function
}


void loop() {

  // ********** "Going to sleep"
  if (eyeMode == 0)
  {
    setAllToLevel(0, 32, 1, 40);
    
    // start the first LED in the sleeping sequence and turn off the eye laser
    for (int x = 0; x < 16; x++)
    {
      sleepSet(9, x);
      delay(40);
    }
    digitalWrite(2, false);
    digitalWrite(10, false);
    OutputOn[4] = false;
    eyeMode++;
  }

  // ********** "Sleeping"
  if (eyeMode == 1)
  {

    
    for (int x = 0; x < 128; x++)
    {
      if ((x >= 0) && (x < 16)) { sleepSet(3, x - 0); }           // 3 up
      if ((x >= 16) && (x < 32)) { sleepSet(9, 31 - x); }     // 9 down
      if ((x >= 32) && (x < 48)) { sleepSet(5, x - 32); }     // 5 up
      if ((x >= 48) && (x < 64)) { sleepSet(3, 63 - x); }     // 3 down
      if ((x >= 64) && (x < 80)) { sleepSet(6, x - 64); }     // 6 up
      if ((x >= 80) && (x < 96)) { sleepSet(5, 95 - x); }     // 5 down
      if ((x >= 96) && (x < 112)) { sleepSet(9, x - 96); }     // 9 up
      if ((x >= 112) && (x < 128)) { sleepSet(6, 127 - x); }     // 6 down

      if (checkToggle_Eyemode()) { x = 64; eyeMode++; }

      delay(40);
    }
  }

  // ********** "Waking up"
  if (eyeMode == 2)
  {
    setAllToLevel(0, 16, 1, 30);
    //OutputOn[4] = true;
    setAllToLevel(48, 0, 1, 20);
    setAllToLevel(16, 48, 1, 20);
    eyeMode++;
  }

  
  // ********** "Awake"
  if (eyeMode == 3)
  {
    checkToggle_Laser();
    checkToggle_Flicker();
    if (checkToggle_Eyemode()) { eyeMode++; }
    delay(10);

    // on rare occasion, blink one of the LEDs
    randBlink = random(4000);

    if (randBlink == 200) { sleepSet(3, 1); delay(random(50,250)); sleepSet(3, 16); }
    if (randBlink == 400) { sleepSet(5, 1); delay(random(50,250)); sleepSet(5, 16); }
    if (randBlink == 600) { sleepSet(6, 1); delay(random(50,250)); sleepSet(6, 16); }
    if (randBlink == 800) { sleepSet(9, 1); delay(random(50,250)); sleepSet(9, 16); }
  }


  // Reset the cycle
  if (eyeMode > 3) { eyeMode = 0; }
}


bool checkToggle_Eyemode()
{
  // Toggle the eye mode
  if (debounce(7))
  {
    OutputOn[7] = false;
    return true;
  } else {
    return false;
  }
}

bool checkToggle_Flicker()
{
  // Toggle the flicker mode
  if (debounce(8))
  {
    OutputOn[8] = false;
    digitalWrite(12, true);
    
    do
    {
      if (random(100) > 50) { if (random(100) > 50) { sleepSet(3, 64); } else { sleepSet(3, 0); } }
      if (random(100) > 50) { if (random(100) > 50) { sleepSet(5, 64); } else { sleepSet(5, 0); } }
      if (random(100) > 50) { if (random(100) > 50) { sleepSet(6, 64); } else { sleepSet(6, 0); } }
      if (random(100) > 50) { if (random(100) > 50) { sleepSet(9, 64); } else { sleepSet(9, 0); } }
      delay(50);
    }
    while (debounce(8) == false);
    OutputOn[8] = false;
    
    setAllToLevel(16,0, 4, 10);
    digitalWrite(12, false);
  }
  return false;
}


bool checkToggle_Laser()
{
  // Toggle the laser, if needed
  bool laz = false;
  laz = debounce(4);
  digitalWrite(2, laz);
  digitalWrite(10, laz);
  return false;
}


bool debounce(int btnPin)
{
  if (!digitalRead(btnPin) && (ButtonPressed[btnPin]))
  {
    ButtonPressed[btnPin] = false;
    delay(20);
    OutputOn[btnPin] = !OutputOn[btnPin];
  }

  // When the button is pushed and AFTER previously released
  if (digitalRead(btnPin) && !ButtonPressed[btnPin])
  {
    // Button is pressed
    ButtonPressed[btnPin] = true;
    delay(20);
  }
  
  return OutputOn[btnPin];
}

int sleepSet (int pin, int val)
{
  if (val > 0) { analogWrite(pin, val); } else { digitalWrite(pin, false); }
  eyeLevel[pin] = val;
  return 0;
}

bool setAllToLevel(int target, int start, int stp, int del)
{
  int st = 1;
  if (start < target)
  {
    for(int x = start; x <= target; x+=stp)
    {
      if (eyeLevel[3] < x) { sleepSet(3, x); }
      if (eyeLevel[5] < x) { sleepSet(5, x); }
      if (eyeLevel[6] < x) { sleepSet(6, x); }
      if (eyeLevel[9] < x) { sleepSet(9, x); }
      delay(del);
    }
  } else {
    for(int x = start; x >= target; x-=stp)
    {
      if (eyeLevel[3] > x) { sleepSet(3, x); }
      if (eyeLevel[5] > x) { sleepSet(5, x); }
      if (eyeLevel[6] > x) { sleepSet(6, x); }
      if (eyeLevel[9] > x) { sleepSet(9, x); }
      delay(10);
    }
  }
  sleepSet(3, target);
  sleepSet(5, target);
  sleepSet(6, target);
  sleepSet(9, target);
  
}

