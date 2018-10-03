void setup() {
  // put your setup code here, to run once:

    pinMode(6, OUTPUT); // speaker

    pinMode(2, INPUT); // 10K resistor to ground, to button that contacts Vcc
    pinMode(3, INPUT); // this too
    pinMode(4, INPUT); // this too
}

void loop() {
  // put your main code here, to run repeatedly:

    if (digitalRead(2) == HIGH) { alertsound(1); delay(500); }
    if (digitalRead(3) == HIGH) { alertsound(2); delay(500); }
    if (digitalRead(4) == HIGH) { alertsound(3); delay(500); }
}

void alertsound(int id)
{
    // id = identification of which sound to play
    // 1 = error, 2 = stand lock on, 3 = stand lock off, 4 = rear gun up, 5 = rear gun stowed

    boolean spk = false;
    // Create a sound generator instead of using tone()
    if (id == 1)
    {
        for (int i = 0; i < 200; i++)
        {
            spk = !spk;
            digitalWrite(6, spk);
            if (i > 130) { delay(random(1,3)); } else { delay(2); }
            if (i == 130) { delay(50); }
        }
    }

    if (id == 2)
    {
        for (int j = 0; j < 5; j++)
        {
        for (int i = 50; i < 150; i++)
        {
            spk = !spk;
            digitalWrite(6, spk);
            delay(200/i);
        }
        delay(50);
        }
    }

    if (id == 3)
    {
        for (int j = 0; j < 3; j++)
        {
        for (int i = 10 + (j*6); i < 100 + (j*10); i++)
        {
            spk = !spk;
            digitalWrite(6, spk);
            delay(i/20);
        }
        }
    }

    // (under devlopment...just have to add two more sounds)
    
    
    digitalWrite(6, LOW);
}
