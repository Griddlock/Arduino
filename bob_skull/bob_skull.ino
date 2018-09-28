
unsigned long mil;
int eye[6];
bool goodmode;
int flicker_target[6];
int current_bright[6];
int min_bright[6];
unsigned long flicker_delay[6];


void setup() {
    eye[0] = 3;
    eye[1] = 5;
    eye[2] = 6;
    eye[3] = 9;
    eye[4] = 10;
    eye[5] = 11;
    
    pinMode(eye[0], OUTPUT);  // Eye 1, red
    pinMode(eye[1], OUTPUT);  // Eye 1, yellow
    pinMode(eye[2], OUTPUT);  // Eye 1, blue
    pinMode(eye[3], OUTPUT);  // Eye 2, red
    pinMode(eye[4], OUTPUT);  // Eye 2, yellow
    pinMode(eye[5], OUTPUT);  // Eye 2, blue

    pinMode(A0, INPUT);       // Turn good
    pinMode(A1, INPUT);       // Turn bad

    goodmode = true;

    // red
    min_bright[0] = 128;
    min_bright[3] = 128;
    // yellow
    min_bright[1] = 128;
    min_bright[4] = 128;
    // blue
    min_bright[2] = 128;
    min_bright[5] = 128;

    for (int i = 0; i < 6; i++)       // Set all brightness targets to the lowest setting (makes a slow fade on).
    {
        flicker_target[i] = min_bright[i];
    }

}

void loop() {
  
    mil = millis();


    for (int i = 0; i < 6; i++)
    {
        // Soft flickering
        if (abs(flicker_target[i] - current_bright[i]) <= 3)
        {
            if (flicker_delay[i] <= mil)
            {
                current_bright[i] += ((flicker_target[i] - current_bright[i]) / 3);
                flicker_delay[i] = mil + (random(10,40));   // Make the next jump take a random amount of time
            }
        } else {

            // Set new brightness level for the LEDs if in the proper mode. Set to zero if in the wrong mode (should make a smooth transition that way
            if (((goodmode) && ((i != 2) && (i != 5))) || ((!goodmode) && ((i == 2) || (i == 5))))
            {
                flicker_target[i] = random(min_bright[i], 255);
            } else {
                flicker_target[i] = 0;
            }
        }

        analogWrite(eye[0], current_bright[0]);
    }

    if (digitalRead(A0) == HIGH) { goodmode = true; }
    if (digitalRead(A1) == HIGH) { goodmode = false; }

}



