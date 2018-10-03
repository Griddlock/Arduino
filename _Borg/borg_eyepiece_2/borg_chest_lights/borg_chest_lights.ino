int pindir3 = 0;
bool pinflag3 = false;
int pindir5 = 1;
int pinval5 = 0;
int pindir6 = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(3, OUTPUT); // Circuitboard
  pinMode(5, OUTPUT); // Glowstick
  pinMode(6, OUTPUT); // Transponder
}

void loop() {
  // put your main code here, to run repeatedly:

// Circuitboard
// Randomly blink off momentarily randomly
pindir3--;
if (pindir3 <=0 )
{
  if (pinflag3 == false)
  {
    digitalWrite(3, 0);
    pindir3 = 10;
    pinflag3 = true;
  } else {
    analogWrite(3,32);
    pindir3 = random(0, 1500);
    pinflag3 = false;
  }
}

// Glowstick
// Fade high and low continuously
pinval5 += pindir5;

if (pinval5 <= 0)
{
  pinval5 = 0;
  pindir5 = 1;
}
if (pinval5 >= 64)
{
  pinval5 = 64;
  pindir5 = -1;
}
analogWrite(5, pinval5);


// Transponder
// Make the light jump from one random brightness to another
pindir6--;
if (pindir6 <= 0)
{
  // Choose a random value
    analogWrite(6,random(0, 24));
    pindir6 = random(1,100);
}

delay(20);
}
