void setup() {
  // put your setup code here, to run once:

  pinMode(3, OUTPUT);       // Motor Power
  pinMode(7, OUTPUT);       // Motor Direction
  pinMode(4, INPUT);        // Trigger button

  pinMode(8, OUTPUT);       // green: green
  pinMode(9, OUTPUT);       // gray: yellow
  pinMode(10, OUTPUT);      // brown: red
  pinMode(11, OUTPUT);      // blue: green
  pinMode(12, OUTPUT);      // white: white
  

  digitalWrite(3,0);
  digitalWrite(7,0);
}

void loop() {
  // put your main code here, to run repeatedly:


if (digitalRead(4) == HIGH)
{
  flashy_lights();
  release_ball();
}

}

void flashy_lights()
{
  delay(100);
  // make the lights flash in a random pattern

  doLight(8);
  doLight(12);
  doLight(9);
  doLight(10);
  doLight(11);
  doLight(8);
  
}


void doLight(int p)
{
  digitalWrite(p, 1);
  delay(200);
  digitalWrite(p, 0);
  delay(100);
}

void release_ball()
{
  open_door();
  delay(1000);
  close_door();
}


void open_door()
{
  digitalWrite(7,0);
  digitalWrite(3,1);
  delay(900);
  digitalWrite(3,0);
}


void close_door()
{
  digitalWrite(7,1);
  delay(50);
  digitalWrite(3,1);
  delay(1000);
  digitalWrite(3,0);
  delay(50);
  digitalWrite(7,0);
}

