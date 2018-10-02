unsigned int mil;
unsigned int rel;
unsigned int nextclick;
unsigned int checkmode;
int clicksleft;

boolean mode;

void setup() {
  // put your setup code here, to run once:
    pinMode(A0, INPUT);     // delay 20 seconds
    pinMode(A1, INPUT);     // delay 10 seconds
    pinMode(A2, INPUT);     // delay 1 second
    pinMode(A3, INPUT);     // delay 230ms (walking speed)
    pinMode(A4, INPUT);     // click 64 times, 150ms delay
    pinMode(A5, INPUT);     // click 512 times, 150ms delay
    pinMode(A6, INPUT);     // click 512 
    pinMode(A7, INPUT);     // HIGH = left button, LOW = right button
    
    pinMode(11, OUTPUT);    // Left button
    pinMode(12, OUTPUT);    // Right button

    mode = false;
    clicksleft = 0;
}

void loop() {
  // put your main code here, to run repeatedly:

    mil = millis();

    // Turn off the relay if its click time has expired (release)
    if (rel <= mil)
    {
        digitalWrite(11, LOW);
        digitalWrite(12, LOW);
    }

    
    if ((rel < (mil - 50)) && (nextclick <= mil))         // make sure there is at least a 50 ms delay between the last release and the next click
    {
        if (digitalRead(A0) == HIGH) { btnclick(100, 20000); }
        if (digitalRead(A1) == HIGH) { btnclick(100, 10000); }
        if (digitalRead(A2) == HIGH) { btnclick(100, 1000); }
        if (digitalRead(A3) == HIGH) { btnclick(100, 230); }

        if (digitalRead(A4) == HIGH) { if (clicksleft == 0) { clicksleft = 64; } }
        if (digitalRead(A5) == HIGH) { if (clicksleft == 0) { clicksleft = 512; } }
        if (digitalRead(A6) == HIGH) { clicksleft = 0; }


        if (clicksleft > 0)
        {
            btnclick(50, 150);
            clicksleft--;
        }
    }

    if (checkmode <= mil)
    {
        mode = digitalRead(A7);
        checkmode = mil + 3000;
    }
}

void btnclick(int ms, int del)
{
    if (mode)
    { digitalWrite(11, HIGH); } else { digitalWrite(12, HIGH); }
    rel = mil + ms;
    nextclick = mil + del;
}

