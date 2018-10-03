
bool ButtonPressed[15];
bool OutputOn[15];
int eyeMode = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(3, OUTPUT); // Eye, Left LED
  pinMode(5, OUTPUT); // Eye, Top LED
  pinMode(6, OUTPUT); // Eye, Right LED
  pinMode(9, OUTPUT); // Eye, Bottom LED

  pinMode(2, OUTPUT);   // Eye, Laser LED
  pinMode(4, INPUT);    // Eye laser button
  pinMode(7, INPUT);    // Eye mode toggle
}



void loop() {

  if (eyeMode == 0)     // Awake mode
  {
    checkinput();
    analogWrite(3, 0);
    analogWrite(5, 0);
    analogWrite(6, 0);
    analogWrite(9, 0);

  }

     // Sleep mode
  if (eyeMode == 1) { operate(3,9); }
  if (eyeMode == 1) { operate(5,3); }
  if (eyeMode == 1) {  operate(6,5); }
  if (eyeMode == 1) {  operate(9,6); }
  
  if (eyeMode == 2)     // Waking up mode
  {
    for(int x = 0; x < 240; x+=2)
    {
      analogWrite(3, x);
      analogWrite(5, x);
      analogWrite(6, x);
      analogWrite(9, x);
      delay(5);
    }
    eyeMode = 0;
  }
}

int operate(int up, int dn)
{
    for(int x = 0; x < 255; x+= 4)
    {
      analogWrite(up, x);
      analogWrite(dn, 256 - x);
      checkinput();
      delay(10);
    }
    analogWrite(dn, 0);
    
    return 0;
}

int checkinput()
{

  // ************** Eye Laser toggle
  digitalWrite(2, debounce(4));
  
  // ************** Eye LED mode rotation
  if (debounce(7))
  {
    OutputOn[7] = false;
    eyeMode++;
    if (eyeMode > 2) { eyeMode = 0; }
  }
}


// btnPin - the pin to read and debounce
// returns - the new toggle value
bool debounce(int btnPin)
{
  if (!digitalRead(btnPin) && (ButtonPressed[btnPin]))
  {
    ButtonPressed[btnPin] = false;
    delay(20);
    // Toggle the eye laser
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

