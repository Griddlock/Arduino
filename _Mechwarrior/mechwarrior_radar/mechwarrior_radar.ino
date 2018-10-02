// BOARD SELECTION INDICATOR
int board_delay;
int board_flash_count;

// BOARD SELECTION 0 Globals
// radar variables
int spinmode;     // The state of the motor program
int dir;          // Direction of the motor
int spd;          // Speed of the motor, from 0 - 20
int current;      // Current pin that the motor is stepping
int stepsleft;    // the number of remaining steps left (from the random number chosen)
int spinpulse;    // The 20 millisecond delay between turning the motor pin on and off
unsigned long spindelay;  // The millis() count when the next pulse should happen

unsigned long flashtime[2]; // The millis() count when the output is set to high
int flashing[2];            // The millis() count when the output is set to low
int flashdelay[2];          // The number of milliseconds until the next flash (flip-flops between values to make double-flash effect)
int flashdelay1[2];         // milliseconds delay between the double-flash
int flashdelay2[2];         // milliseconds delay until the next double-flash

bool btn_activate;
bool btn_exit;
bool ind_ae;
int ind_timer_ae;

int BOARD_SELECT;

// BOARD SELECTION 1 Globals
int radar_mode;
int radar_current;
unsigned long radar_delay;
unsigned long radar_pulse;
int radar_location;
int radar_range;

void setup() {

  pinMode(2, INPUT);  // Board select bit 0
  pinMode(3, INPUT);  // Board select bit 1

    if ((digitalRead(2) == LOW) && (digitalRead(3) == LOW))     // BOARD SELECTION 0: External Radar, Hazard lights, Activate/Exit, Stabilizers
    {
        BOARD_SELECT = 0;
    
        pinMode(A0, INPUT); // Radar control switch
        pinMode(A1, INPUT); // Hazard lights switch
        pinMode(A2, INPUT); // Activate button
        pinMode(A3, INPUT); // Exit button
        pinMode(A4, INPUT); // Stabilizer position input
      
        pinMode(4, OUTPUT); // Stepper pin 1
        pinMode(5, OUTPUT); // Stepper pin 2
        pinMode(6, OUTPUT); // Stepper pin 3
        pinMode(7, OUTPUT); // Stepper pin 4
      
        pinMode(8, OUTPUT); // Activate indicator
        pinMode(9, OUTPUT); // Exit Indicator
        pinMode(10, OUTPUT);// Stabilizer status light (on when deployed)
      
        pinMode(11, OUTPUT); // Hazard lights output 1
        pinMode(12, OUTPUT); // Hazard lights output 2
        // radar variables
        spinmode = 0;
        dir = 0;
        spd = 0;
        current = 4;
        stepsleft = 0;
        spindelay = 0;
      
        flashtime[1] = millis();
        flashing[1] = 0;
        flashdelay[1] = 250;
        flashdelay1[1] = 250;
        flashdelay2[1] = 1750;
        
        flashtime[2] = millis();
        flashing[2] = 0;
        flashdelay[2] = 1200;
        flashdelay1[2] = 250;
        flashdelay2[2] = 1951;  // Delay  is offset to make the flashes unsynchronized and untimed
      
        btn_activate = false;
        btn_exit = false;
        ind_ae = false;
        ind_timer_ae = 0;
    
    }

    if ((digitalRead(2) == HIGH) && (digitalRead(3) == LOW))    // BOARD SELECTION 1: Display radar (motor and lights)
    {
        pinMode(A0, INPUT);   // Trigger for radar interaction
        pinMode(A1, INPUT);   // Radar limit button
        
        BOARD_SELECT = 1;
        radar_mode = 0;   // mode of operation
        radar_current = 4;      // current stepper motor pin
        radar_delay = millis();
        radar_pulse = millis();
        radar_location = 0;
        
    }

    if ((digitalRead(2) == LOW) && (digitalRead(3) == HIGH))    // BOARD SELECTION 2:
    {
        BOARD_SELECT = 2;
    }

    if ((digitalRead(2) == HIGH) && (digitalRead(3) == HIGH))   // BOARD SELECTION 3:
    {
        BOARD_SELECT = 3;
    }
}




void loop()
{

    if (BOARD_SELECT == 0)
    {
 
        if (digitalRead(A0) == HIGH)    // Radar switch is on
        {
            // Radar "spinmode"
            // 0 = stopped
            // 1/5 = accellerating 0/1
            // 2/6 = cruising 0/1
            // 3/7 = decellerating 0/1
            
            if (millis() >= spindelay)
            {
        
                if (spinmode == 0)  // stopped, choose a new direction;
                {
                    if (random(0, 1000) < 500) { dir = 0; } else { dir = 1; }
                    spd = 0;
                    spinmode = 1 + (dir * 4);
                }
          
                if ((spinmode == 1) || (spinmode == 5)) // accellerating
                {
                    if (spd < 10)
                    {
                        spd++;
                    } else {
                        spinmode++;
                    }
                }
          
                if ((spinmode == 2) || (spinmode == 6)) // cruising
                {
                    stepsleft--;
                    if (stepsleft <= 0)
                    {
                        spinmode++;
                        stepsleft = random(30,100);
                    }
                }
          
                if ((spinmode == 3) || (spinmode == 7)) // decellerating
                {
                    if (spd > 0)
                    {
                       spd--;
                    } else {
                        spinmode = 0;
                    }
                }
          
                if (spd > 0)
                {
                    if (spinmode < 4)
                    {
                        current++;
                        if (current > 7) { current = 4; }
                    } else {
                        current--;
                        if (current < 4) { current = 7; }
                    }
                    digitalWrite(current, HIGH);
                    spinpulse = millis() + 50;  // set the timeout for turing off the pin on the motor
                }
        
                spindelay = millis() + (200 - (spd * 10));
            }
        
            if (millis() >= spinpulse)
            {
                digitalWrite(current, LOW);
            }
        } else {
            if (spinmode > 0)
            {
                digitalWrite(4, LOW);
                digitalWrite(5, LOW);
                digitalWrite(6, LOW);
                digitalWrite(7, LOW);
                spinmode = 0;
            }
        }
      
      
      
        if (digitalRead(A1) == HIGH)      // Hazard light switch is on
        {
            for (int x = 1; x <= 2; x++)
            {
                // flash the hazard lights on consistent interval, regardless of the stepper motor controller
                if (millis() >= flashtime[x])
                {
                    // time to flash again, and reset the flashtime
                    flashtime[x] = millis() + flashdelay[x];
                    if (flashdelay[x] > 1000) { flashdelay[x] = flashdelay1[x]; } else { flashdelay[x] = flashdelay2[x]; }
                    digitalWrite(10 + x, HIGH);
                    flashing[x] = millis() + 100;
                }
            
                if ((millis() > flashing[x]) && (flashing[x] > 0))
                {
                    digitalWrite(10 + x, LOW);
                    flashing[x] = 0;
                }
            }
        } else {
            digitalWrite(11, LOW);
            digitalWrite(12, LOW);
        }
      
      
        if (digitalRead(A2) == HIGH)     // Activate button pressed
        {
            btn_exit = false;
            btn_activate = true;
        } else {
            if (digitalRead(A4) == HIGH)    // Stabilizers drop
            {
                btn_activate = false;
                digitalWrite(9, LOW);
                digitalWrite(10, HIGH);
            }
        }
        
      
        if (digitalRead(A3) == HIGH)      // Exit button pressed
        {
            btn_activate = false;
            btn_exit = true;
        } else {
            if (digitalRead(A4) == LOW)     // Stabilizers rise
            {
                btn_exit = false;
                digitalWrite(8, LOW);
                digitalWrite(10, HIGH);
            }
        }
      
        // Process the status lights for Activate and Exit
        if ((btn_activate) || (btn_exit))
        {
            if (millis() > ind_timer_ae)
            {
                // Toggle the light
                if (ind_ae)
                {
                    ind_ae = false;
                    ind_timer_ae = millis() + 400;
                    digitalWrite(8, LOW);
                    digitalWrite(9, LOW);
                } else {
                    ind_ae = true;
                    ind_timer_ae = millis() + 200;
                    if (btn_exit) { digitalWrite(9, HIGH); } else { digitalWrite(8, HIGH); }
                }
            }
        } else {
          digitalWrite(8, LOW);
          digitalWrite(9, LOW);
          digitalWrite(10, LOW);
        }
      
    }        

    if (BOARD_SELECT == 1)
    {
        // radar_mode:
            // 0 = resetting
            // 1 = at reset location
            // 2 = setting to random location
            // 3 = tracking incoming

        if (radar_mode == 0)    // Resetting to initial location
        {
            if ((digitalRead(A1) == LOW) && (radar_delay <= millis()))
            {
                // continue turning
                digitalWrite(radar_current, LOW);
                radar_current++;
                if (radar_current > 7) { radar_current = 4; }
                digitalWrite(radar_current, HIGH);
                radar_delay = millis() + 100;
                radar_pulse = millis() + 50;
            }

            if (radar_pulse <= millis())
            {
              digitalWrite(radar_current, LOW);
            }

            if (digitalRead(A1) == HIGH)    // Hit the reset button
            {
                digitalWrite(4, LOW); digitalWrite(5, LOW); digitalWrite(6, LOW); digitalWrite(7, LOW);
                radar_mode = 1;
                radar_location = 0;
            }
        }

        if ((digitalRead(A0) == HIGH) && (radar_mode == 1))
        {
            radar_mode = 2;
            radar_location = random(2, 20);
        }

        if (radar_mode == 2)
        {
            if (radar_delay <= millis())
            {
                digitalWrite(radar_current, LOW);
                radar_current--;
                radar_location--;
                if (radar_current < 4) { radar_current = 7; }
                if (radar_location > 0)
                {
                    digitalWrite(radar_current, HIGH);
                    radar_delay = millis() + 100;
                    radar_pulse = millis() + 50;
                } else {
                    digitalWrite(4, LOW); digitalWrite(5, LOW); digitalWrite(6, LOW); digitalWrite(7, LOW);
                    radar_mode = 3;
                    radar_range = 8;
                }
            }
        }

        if (radar_mode == 3)
        {

            if (radar_delay <= millis())
            {
                radar_range--;
                digitalWrite(8, radar_range & 1);
                digitalWrite(9, radar_range & 2);
                digitalWrite(10, radar_range & 4);

                radar_delay = millis() + 2000;
            }

            //This is not finished yet!
            
        }
        
    }

    if (BOARD_SELECT == 2)
    {
    }

    if (BOARD_SELECT == 3)
    {
    }

}


