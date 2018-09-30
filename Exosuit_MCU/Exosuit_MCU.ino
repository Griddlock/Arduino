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
int cursor_item;

void setup() {

    // Left arm rockets, simple trigger, barrel recoil
    // Right arm gun, white muzzle flash, motor with metal slapper on it for sound effiect
    // Rear gun, multiple servos, must be very strong, needs to be extremely light
      // muzzle flash lights like right arm
      // Spring-assisted movement to help lift (think garage door opener, with spring-loaded pulley and cable)
    // arm gun, back gun, exit (think of all the controls needed)
    
    pinMode(2, OUTPUT); //        Cursor bit 1 to demultiplexer
    pinMode(3, OUTPUT); // PWM    Cursor bit 2 to demultiplexer
    pinMode(4, OUTPUT); //        Cursor bit 3 to demultiplexer
    pinMode(5, OUTPUT); // PWM    Selected bit 1    
    pinMode(6, OUTPUT); // PWM    Selected bit 2
    pinMode(7, OUTPUT); //        Selected bit 3
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
    if (stick_trig[0] == 1) { select_item = cursor_item; send_selected_value(selected_item); }
    
    // Up
    if (stick_trig[1] == -1) { cursor_item--; if (cursor_item < 0) { cursor_item = 7; } send_cursor_value(cursor_item); }
    
    // Down
    if (stick_trig[1] == 1) { cursor_item++; if (cursor_item > 7) { cursor_item = 0; } send_cursor_value(cursor_item); }

    // Reset the movement trigger, since they have been used now
    if (stick_trig[0] != 0) { stick_trig[0] = 0; }
    if (stick_trig[1] != 0) { stick_trig[0] = 1; }

    // *********************

    /*
     * Menu items:
     *    Ingress/Egress
     *    Rear weapon
     *    right arm gun
     */
    

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

void send_cursor_value(int val)
{
    digitalWrite(2, ((val & 1) ? HIGH : LOW));
    digitalWrite(3, ((val & 2) ? HIGH : LOW));
    digitalWrite(4, ((val & 4) ? HIGH : LOW));
}

void send_selected_value(int val)
{
    digitalWrite(5, ((val & 1) ? HIGH : LOW));
    digitalWrite(6, ((val & 2) ? HIGH : LOW));
    digitalWrite(7, ((val & 4) ? HIGH : LOW));
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

