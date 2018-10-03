
int levels[5];
int spd[5];
int del[5];


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  
  for(int x = 0; x < 5; x++)
  {
    levels[x] = random(0,1000);
    spd[x] = random(1,3);
  }
  
}



void loop() {

  for(int x = 0; x <= 3; x++)
  {
    if (del[x] > 0)
    {
      del[x] = del[x] - 1;
    } else {
      
      levels[x] = levels[x] + spd[x];
      if (levels[x] >= 1020) { levels[x] = random(0,300); spd[x] = random(1,3); del[x] = random(50, 250); }

      if (x == 0) { analogWrite(3, levels[x]); }
      if (x == 1) { analogWrite(5, levels[x]); }
      if (x == 2) { analogWrite(6, levels[x]); }
      if (x == 3) { analogWrite(9, levels[x]); }
    }
  }
  delay(10);
}
