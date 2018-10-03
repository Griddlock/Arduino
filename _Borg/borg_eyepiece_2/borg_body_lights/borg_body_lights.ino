void setup() {
  // put your setup code here, to run once:

  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  for (int x = 0; x < 256; x++)
  {
    analogWrite(3, x);
    analogWrite(5, 256-x);
    delay(10);
  }
  for (int x = 255; x > 0; x--)
  {
    analogWrite(3, x);
    analogWrite(5, 256-x);
    delay(10);
  }
  

}
