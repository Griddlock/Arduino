// Ingress/Egress control board
// This board controls up to 4 of the h-bridged motors that open and close the straps
// Usage: High on pin 10 to tighten the straps, High on pin 11 to loosen straps. Pin 12 will output high until all four limit switches are simultaneously pressed.
// Action will cease if the driving pin (10 or 11) returns to low (so the process can be halted mid-way).

// Applications:
//    (shin, thigh, shoulder, forearm) x 2
//    

unsigned char mil;
boolean busy;

int motor_fwd[4];
int motor_rev[4];
int motor_fwd_sw[4];
int motor_rev_sw[4];


void setup() {
    pinMode(2, OUTPUT);   // motor 0
    pinMode(3, OUTPUT);   // motor 0
    pinMode(4, OUTPUT);   // motor 0
    pinMode(5, OUTPUT);   // motor 0
    pinMode(6, OUTPUT);   // motor 1
    pinMode(7, OUTPUT);   // motor 1
    pinMode(8, OUTPUT);   // motor 1
    pinMode(9, OUTPUT);   // motor 1
    
    pinMode(10, INPUT);   // directive to close
    pinMode(11, INPUT);   // directive to open
    pinMode(12, OUTPUT);  // feedback (LOW when idle)

    pinMode(A0, OUTPUT);   // motor 2
    pinMode(A1, OUTPUT);   // motor 2
    pinMode(A2, OUTPUT);   // motor 2
    pinMode(A3, OUTPUT);   // motor 2
    pinMode(A4, OUTPUT);   // motor 3
    pinMode(A5, OUTPUT);   // motor 3
    pinMode(A6, OUTPUT);   // motor 3
    pinMode(A7, OUTPUT);   // motor 3

    motor_fwd[0] = 2;
    motor_rev[0] = 3;
    motor_fwd_sw[0] = 4;
    motor_rev_sw[0] = 5;

    motor_fwd[1] = 6;
    motor_rev[1] = 7;
    motor_fwd_sw[1] = 8;
    motor_rev_sw[1] = 9;

    motor_fwd[2] = A0;
    motor_rev[2] = A1;
    motor_fwd_sw[2] = A2;
    motor_rev_sw[2] = A3;

    motor_fwd[3] = A4;
    motor_rev[3] = A5;
    motor_fwd_sw[3] = A6;
    motor_rev_sw[3] = A7;

    busy = false;
}


// Forward = closing
// Reverse = opening

void loop() {

    mil = millis();
  
    if (digitalRead(10) == HIGH)
    {
        busy = false;
        for (int i = 0; i < 4; i ++)  // For each motor
        {
            if (digitalRead(motor_fwd_sw[i]) == LOW)  // Run until the forward switch is triggered
            {
                digitalWrite(motor_fwd[i], HIGH);
                busy = true;
            } else {
                digitalWrite(motor_fwd[i], LOW);
            }
        }
    } else {
        for (int i = 0; i < 4; i++) { digitalWrite(motor_fwd[i], LOW); }
    }
  
    if (digitalRead(11) == HIGH)
    {
        busy = false;
  
        for (int i = 0; i < 4; i ++)  // For each motor
        {
            if (digitalRead(motor_rev_sw[i]) == LOW) // Run until the reverse switch is triggered
            {
                digitalWrite(motor_rev[i], HIGH);
            } else {
                digitalWrite(motor_rev[i], LOW);
            }
        }
    } else {
        for (int i = 0; i < 4; i++) { digitalWrite(motor_rev[i], LOW); }
    }

    digitalWrite(12, busy);   // Indicate to the MCU the status of the open/close
}



