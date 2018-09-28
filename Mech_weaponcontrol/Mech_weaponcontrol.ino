unsigned int mil;

int gun_status;       // 0 = stowed, 1 = ready
int rocket_status;    // 0 = inactive, 1 = running, 2 = hammer is firing
int pin[13];

int btn_press[3];   // current press state of button. 0=ready, 1=pressed & debouncing, 2=triggered
int btn_delay[3];   // debounce delay time


void setup() {
    // put your setup code here, to run once:
  
    pin[0] = A0;
    pin[1] = A2;
    pin[2] = A4;

    // Buttons and indicators
    pinMode(pin[0], INPUT);  // Weapon off
    pinMode(A1, OUTPUT);  // show weapon off
    pinMode(pin[1], INPUT);  // Gun select
    pinMode(A3, OUTPUT);  // show gun selected
    pinMode(pin[2], INPUT);  // Rocket select
    pinMode(A5, OUTPUT);  // show rocket selected

    // Weapons
    pinMode(2, OUTPUT);   // gun motor activating
    pinMode(3, OUTPUT);   // gun motor stowing
    pinMode(4, INPUT);    // gun ready position switch
    pinMode(5, INPUT);    // gun stow position switch
    pinMode(6, OUTPUT);   // gun motor
    
    pinMode(7, OUTPUT);   // rocket launcher tilt up
    pinMode(8, OUTPUT);   // rocket launcher tilt down
    pinMode(9, INPUT);    // rocket at top position switch
    pinMode(10, INPUT);   // rocket at bottom position switch
    pinMode(11, OUTPUT);  // rocket launch motors
    pinMode(A6, INPUT);   // rocket tilt adjustment


    pinMode(12, INPUT);   // Fire weapon button

    // Unused pins
    // pinMode(A7, OUTPUT);
    // pinMode(13, OUTPUT);

}

void loop() {

    mil = millis();


    // check for any three button presses
    for (int i = 0; i < 3; i++)
    {
        if (btn_delay[i] < mil)
        {
            if (btn_press[i] == 0)
            {
                // button is not being pressed, check for press
                if (digitalRead(pin[i]) == HIGH)
                {
                    btn_press[i] = 1;
                    btn_delay[i] = mil + 100;
                }
            } else {
                // button is pressed and delay is over, at this point
                if (digitalRead(pin[i]) == LOW)
                {
                    btn_press[i] = 2;
                    btn_delay[i] = mil + 100;
                }
            }
        }
    }

    if (btn_press[0] == 2)    // Weapons Off button has been triggered
    {
        btn_press[0] = 0;
        if (gun_status > 0) { digitalWrite(6, LOW); move_weapon(3, 5); gun_status = 0; }  // stow the gun, ensure motor is deactivated
        if (rocket_status > 0) { digitalWrite(11, LOW); } // turn off the flywheels
        show_weapon_status();
    }

    if (btn_press[1] == 2)    // Gun button has been triggered
    {
        btn_press[1] = 0;
        move_weapon(2, 4);
        gun_status = 1;
        show_weapon_status();
    }

    if (btn_press[2] == 2)    // Rocket button has been triggered
    {
        btn_press[2] = 0;
        digitalWrite(11, HIGH);   // turn on the flywheels
        delay(3000);
        rocket_status = 1;
        show_weapon_status();
    }


    // This is the place to add the rocket tilt adjustment control, but that is on hold

    // tilt = analogRead(A6)
    

    if (gun_status == 1)
    {
        if (digitalRead(12) == HIGH)
        {
            digitalWrite(6, HIGH);
        } else {
            digitalWrite(6, LOW);
        }
    }

    if (rocket_status == 1)
    {
        if (digitalRead(12) == HIGH)
        {
            rocket_status = 2;
            show_weapon_status();


            // TODO: make the hammer do its cycle
            // NOTE: rocket_status == 2 is not actually useful at this point, since it is not in a real-time loop

            delay(2000);    // Delay to allow the motors to speed up again
            rocket_status = 0;
            show_weapon_status();
        }
    }
}

void move_weapon(int pin_mo, int pin_sw)
{
    // This function pauses the real-time loop until completed
    unsigned int timeout = millis() + 5;
    
    digitalWrite(pin_mo, HIGH);
    while ((digitalRead(pin_sw) == LOW) && (timeout < millis()))
    {
        // wait until the gun is aligned or the timeout lapses
    }
    digitalWrite(pin_mo, LOW);
}

void show_weapon_status()
{
    if (gun_status == 1) { digitalWrite(A3, HIGH); } else { digitalWrite(A3, LOW); }
    if (rocket_status == 1) { digitalWrite(A5, HIGH); } else { digitalWrite(A5, LOW); }
    if ((gun_status == 0) && (rocket_status == 0)) { digitalWrite(A1, HIGH); } else { digitalWrite(A1, LOW); }
}

