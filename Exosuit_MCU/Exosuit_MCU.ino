// This is the master CPU for the exosuit. It runs the joystick and the control panel, and delegates tasks to other controller boards

/*
 *              MCU
 *               |
 *         Strap  
 *         
 *         
 */


unsigned long mil;
boolean stick_debounce[2];
int stick_val[2];
int stick_trig[2];

int selected_item;

void setup() {

    // Left arm rockets, simple trigger, barrel recoil
    // Right arm gun, white muzzle flash, motor with metal slapper on it for sound effiect
    // Rear gun, multiple servos, must be very strong, needs to be extremely light
      // muzzle flash lights like right arm
      // Spring-assisted movement to help lift (think garage door opener, with spring-loaded pulley and cable)
    // arm gun, back gun, exit (think of all the controls needed)
    
    pinMode(2, OUTPUT); //        Menu bit 1 to demultiplexer
    pinMode(3, OUTPUT); // PWM    Menu bit 2 to demultiplexer
    pinMode(4, OUTPUT); //        Menu bit 3 to demultiplexer
    pinMode(5, OUTPUT); // PWM    Activate menu item
    pinMode(6, OUTPUT); // PWM
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT); // PWM
    pinMode(10, OUTPUT); // PWM
    pinMode(11, OUTPUT); // PWM
    pinMode(12, OUTPUT);

    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);   // Joystick button
    pinMode(A6, INPUT);   // Joystick X-axis
    pinMode(A7, INPUT);   // Joystick Y-axis
    

    stick_debounce[0] = false;
    stick_debounce[1] = false;
}

void loop() {

    mil = millis();


    // ********************* Joystick movement and cursor movement

    check_joystick();     // Set stick_trig[0] or stick_trig[1] when the user has pushed the joystick to a direction

    // Left
    if (stick_trig[0] == -1) { }

    // Right
    if (stick_trig[0] == 1) { }
    
    // Up
    if (stick_trig[1] == -1) { selected_item--; if (selected_item < 0) { selected_item = 7; } }
    
    // Down
    if (stick_trig[1] == 1) { selected_item++; if (selected_item > 7) { selected_item = 0; } }

    // Reset the movement trigger, since they have been used now
    if (stick_trig[0] != 0) { stick_trig[0] = 0; }
    if (stick_trig[1] != 0) { stick_trig[0] = 1; }

    // *********************

    // ********************* Menu item cursor

}


void check_joystick()
{
    // Within the real-time loop, determine if the joystick has been pushed in a direction to make the menu move

    for (int a = 0; a < 2; a++)
    {
        stick_val[a] = stick(a);
        
        if ((stick_val[a] != 0) && (!stick_debounce[a]))
        {
            stick_trig[a] = stick_val[a];   // trigger the stick direction
            stick_debounce[a] = true;
            delay(10);
            return;
        }

        if ((stick_val[a] == 0) && (stick_debounce[a]))
        {
            stick_debounce[a] = false;
            delay(10);
            return;
        }
        
    }
}

int stick(int axis)
{
    // return a digital conversion of the stick position for the specified axis (don't bother with the extra resistors in the circuit this time. It is 1 milliamp!)
    int a;
    if (axis == 0) { a = analogRead(A6); } else { a = analogRead(A7); }

    if (a < 400) { return -1; }
    if (a > 800) { return 1; }
    return 0;
}

