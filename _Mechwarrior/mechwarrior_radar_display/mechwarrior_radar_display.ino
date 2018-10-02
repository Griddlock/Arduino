int radar_mode;
int radar_current;
unsigned long radar_delay;
unsigned long radar_pulse;
int radar_location;
int radar_range;



void setup() {
    pinMode(A0, INPUT);   // Trigger for radar interaction
    pinMode(A1, INPUT);   // Radar limit button
    
    BOARD_SELECT = 1;
    radar_mode = 0;   // mode of operation
    radar_current = 4;      // current stepper motor pin
    radar_delay = millis();
    radar_pulse = millis();
    radar_location = 0;
}


void loop()
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
}


void do_pulse(
