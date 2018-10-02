
unsigned long wire_timeout[4];
unsigned long mil;
int dir;
int step_location;
unsigned long pulse_timer;

const int pulse_duration = 20;
const int pulse_delay = 50;

void setup() {

    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);

    for (int x = 0; x < 4; x++)
    {
        wire_timeout[x] = millis();
    }
    step_location = 0;
    dir = 1;
}

void loop() {
    // put your main code here, to run repeatedly:
  
    mil = millis();

    // Set the next wire when the pulse_timer has expired
    if (pulse_timer <= mil)
    {
        if (dir != 0)
        {
            // initiate wire
            digitalWrite(step_location + 4, HIGH);
            wire_timeout[step_location] = mil + pulse_duration;        

            // Increment the wire for the next iteration
            step_location += dir;
            if (step_location < 0) { step_location = 3; }
            if (step_location > 3) { step_location = 0; }
        }
        
        pulse_timer = mil + pulse_delay;
    }

    // reset any wires that have timed out
    if (wire_timeout[0] <= mil) { digitalWrite(4, LOW); }
    if (wire_timeout[1] <= mil) { digitalWrite(5, LOW); }
    if (wire_timeout[2] <= mil) { digitalWrite(6, LOW); }
    if (wire_timeout[3] <= mil) { digitalWrite(7, LOW); }

}
