
unsigned long wire_timeout[4];
unsigned long mil;
int dir;
int dir_count;
int step_location;
unsigned long pulse_timer;

const int pulse_duration = 20;
const int pulse_delay = 30;       // pulse_delay must never be less than half of pulse_duration

// scenario
int radar_flag;
int radar_target;
int incoming_location;
unsigned long incoming_delay;
bool incoming_type;
bool launched_flare;
bool launched_decoy;

void setup() {

    //pinMode(2, );
    //pinMode(3, );
    pinMode(4, OUTPUT);           // Stepper pin 0
    pinMode(5, OUTPUT);           // Stepper pin 1
    pinMode(6, OUTPUT);           // Stepper pin 2
    pinMode(7, OUTPUT);           // Stepper pin 3
    pinMode(8, OUTPUT);           // Blip address bit 0
    pinMode(9, OUTPUT);           // Blip address bit 1
    pinMode(10, OUTPUT);          // Blip address bit 2
    pinMode(11, OUTPUT);          // Indicator: Heat lock
    pinMode(12, OUTPUT);          // Indicator: Radar lock
    
    pinMode(A0, INPUT);           // Scenario start button
    pinMode(A1, INPUT);           // radar arm reset position button
    pinMode(A2, INPUT);           // Flare button
    pinMode(A3, INPUT);           // Decoy button
    //pinMode(A4, );
    //pinMode(A5, );
    //pinMode(A6, );
    //pinMode(A7, );
    

    for (int x = 0; x < 4; x++)
    {
        wire_timeout[x] = millis();
    }
    step_location = 0;
    dir = 0;
    dir_count = 0;

    radar_flag = 0;
    radar_target = 0;
    incoming_location = 0;
    incoming_delay = 0;
    launched_flare = false;
    launched_decoy = false;
    incoming_type = false;
}

void loop() {
    // put your main code here, to run repeatedly:
  
    mil = millis();


    // Initiate the attack scenario


    // *********************** Radar Screen control
    // To reset: set dir = -1;
    // To initiate: set dir = 1, set dir_count = rand(3, radar_range);

    if ((digitalRead(A0) == HIGH) && (radar_flag == 0))
    {
        // Initiate the scenario
        radar_flag = 1;
        radar_target = random(3, 20);
        dir = 1;
    }

    
    if (radar_flag > 0)
    {
        if (radar_flag == 1)        // Set the radar arm to a random location
        {
            if (radar_target > 0)
            {
                radar_target--;
            } else {
                dir = 0;
                radar_flag = 2;   // reached target location
                incoming_location = 8;
                launched_flare = false;
                launched_decoy = false;
                if (random(0, 100) > 50) { incoming_type = true; } else { incoming_type = false; }
            }
        }

        if (radar_flag == 2)
        {
            // incoming fire
            if (incoming_delay <= mil)
            {
                if (incoming_location <= 0)
                {
                    // Hit!
                    radar_flag = 3;

                    // Clear the radar
                    digitalWrite(8, LOW); digitalWrite(9, LOW); digitalWrite(10, LOW); 
                    
                } else {
                    incoming_delay = mil + 2000;
                    incoming_location--;
    
                    // set the radar blip (output goes to the demultiplexer
                    if ((incoming_location & 1) > 0) { digitalWrite(8, HIGH); } else { digitalWrite(8, LOW); }
                    if ((incoming_location & 2) > 0) { digitalWrite(9, HIGH); } else { digitalWrite(9, LOW); }
                    if ((incoming_location & 4) > 0) { digitalWrite(10, HIGH); } else { digitalWrite(10, LOW); }

                    if ((incoming_location > 0) && (incoming_location < 4))
                    {
                        // check for countermeasures
                        if (digitalRead(A2) == HIGH) { launched_flare = true; }
                        if (digitalRead(A3) == HIGH) { launched_decoy = true; }
                    }

                    if (incoming_location == 1)
                    {
                        if ((launched_flare == true) && (incoming_type == false)) { radar_flag = 4; }
                        if ((launched_decoy == true) && (incoming_type == true)) { radar_flag = 4; }
                    }
                }
            }
        }

        if (radar_flag == 3)
        {
            // play alarm (3 blares) and then booming sound


            // reset
            radar_flag = 5;
        }


        if (radar_flag == 4)
        {
            // Successful defense, play 'all clear' sound
            radar_flag = 5;
            
        }

        if (radar_flag == 5)
        {
            dir = -1;
            // Reset the arm
            // If the limit button is being pressed, stop the motor immediately.
            if (digitalRead(A1) == HIGH)
            {
                dir = 0;
                radar_flag = 0;
            }
        }
    }

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
            
            pulse_timer = mil + pulse_delay;
        } else {
            pulse_timer = mil + 500;     // Since the motor is off, give a longer delay, like in sleep mode
        }
    }

    // reset any wires that have timed out
    if (wire_timeout[0] <= mil) { digitalWrite(4, LOW); }
    if (wire_timeout[1] <= mil) { digitalWrite(5, LOW); }
    if (wire_timeout[2] <= mil) { digitalWrite(6, LOW); }
    if (wire_timeout[3] <= mil) { digitalWrite(7, LOW); }

}
