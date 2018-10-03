
bool pinOn[12];
int pindel[12];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(2, INPUT);
  
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  for (int i = 2; i <= 12; i++)
  {
    pinOn[i] = false;
    pindel[12] = random(4,20);
  }

}

void loop() {
  // put your main code here, to run repeatedly:

  for(int i = 2; i <= 12; i++)
  {
    pindel[i]--;
    if (pindel[i] <=0)
    {
      if (pinOn[i] == true)
      {
        pinOn[i] = false;
        pindel[i] = random(4,20);
      } else {
        pinOn[i] = true;
        pindel[i] = 2;
      }
      digitalWrite(i, pinOn[i]);
    }
  }
  delay(50);
}
