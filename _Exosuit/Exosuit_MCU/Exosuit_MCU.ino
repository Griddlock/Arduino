// This is the master CPU for the exosuit. It runs the joystick and the control panel, and delegates tasks to other controller boards

unsigned long mil;
boolean stick_debounce[2];
int stick_val[2];
int stick_trig[2];

int selected_item;
int cursor_item;
int trigger1;
int trigger2;
int trigger3;
boolean standing_lock;

void setup() {

    // Left arm rockets, simple trigger, barrel recoil
    // Right arm gun, white muzzle flash, motor with metal slapper on it for sound effiect
    // Rear gun, multiple servos, must be very strong, needs to be extremely light
      // muzzle flash lights like right arm
      // Spring-assisted movement to help lift (think garage door opener, with spring-loaded pulley and cable)
    // arm gun, back gun, exit (think of all the controls needed)
    
    pinMode(2, OUTPUT);  //       Cursor bit 1 to demultiplexer
    pinMode(3, OUTPUT);  // PWM   Cursor bit 2 to demultiplexer
    pinMode(4, OUTPUT);  //       Selected bit 1 to demultiplexer
    pinMode(5, OUTPUT);  // PWM   Selected bit 2 to demultiplexer
    pinMode(6, OUTPUT);  // PWM   Speaker
    pinMode(7, OUTPUT);  //       COMMAND: Locking mechanism (0 = open, 1 = close);
    pinMode(8, OUTPUT);  //       COMMAND: Straps, close
    pinMode(9, OUTPUT);  // PWM   COMMAND: Straps, open
    pinMode(10, OUTPUT); // PWM   COMMAND: Rear gun, present
    pinMode(11, OUTPUT); // PWM   COMMAND: Rear gun, stow
    pinMode(12, OUTPUT); //       COMMAND: Rear gun fire

    pinMode(A0, OUTPUT); //       COMMAND: Right arm gun fire
    pinMode(A1, OUTPUT); //       COMMAND: Left arm gun fire
    pinMode(A2, OUTPUT); //       
    pinMode(A3, INPUT); //        Trigger 2 ('go forward')
    pinMode(A4, INPUT); //        Trigger 3 ('go back')
    pinMode(A5, INPUT); //        Joystick button
    pinMode(A6, INPUT); //        Joystick X-axis
    pinMode(A7, INPUT); //        Joystick Y-axis

    trigger1 = A5;
    trigger2 = A3;
    trigger3 = A4;
    

    stick_debounce[0] = false;
    stick_debounce[1] = false;

    standing_lock = true;     // Assume that the lock is on. On power-on, the suit locks by default (probably best ease of use)
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
    if (stick_trig[1] == -1) { cursor_item--; if (cursor_item < 0) { cursor_item = 3; } send_cursor_value(cursor_item); }
    
    // Down
    if (stick_trig[1] == 1) { cursor_item++; if (cursor_item > 3) { cursor_item = 0; } send_cursor_value(cursor_item); }

    // Reset the movement trigger, since they have been used now
    if (stick_trig[0] != 0) { stick_trig[0] = 0; }
    if (stick_trig[1] != 0) { stick_trig[0] = 1; }

    // *********************

    /*
     * Menu items to decide what the right-had controls do: joystick button, top trigger, bottom trigger
     *    0 - "Safety": (all attempts make error sound)
     *    1 - "Rear Gun": present / fire / stow
     *    2 - "Arm Guns": nothing / fire right / fire left
     *    3 - "Ingress": toggle standing locks / straps close / straps open
     *    
     */


    // 0 - "Safety"
    if (select_item == 0)
    {
        // nothing happens when any trigger is pressed
        if ((digitalRead(trigger1) == HIGH) || (digitalRead(trigger2) == HIGH) || (digitalRead(trigger3) == HIGH))
        {
            alertsound(1);
        }
    }


    // 1 - "Rear Gun": present / fire / stow
    if (select_item == 1)
    {
        digitalWrite(10, digitalRead(trigger1));    // bring gun out to ready position
        digitalWrite(11, digitalRead(trigger3));    // stow gun
        digitalWrite(12, digitalRead(trigger2));    // Fire gun
    }

    // 2 - "Arm Guns": nothing / fire right / fire left
    if (select_item == 2)
    {
        //digitalRead(trigger1)
        digitalWrite(A0, digitalRead(trigger2));
        digitalWrite(A1, digitalRead(trigger3));
    }

    // 3 - "Ingress": toggle locks / straps close / straps open
    if (select_item == 3)
    {
        if (digitalRead(trigger1))
        {
            // This will delay the real-time loop momentarily
            delay(30); // debounce
            while (digitalRead(trigger1) == HIGH)
            {
              // wait for release
              delay(10);
            }
            delay(30);
            standing_lock = !standing_lock;
            if (standing_lock) { alertsound(2); } else { alertsound(3); }
            digitalWrite(7, standing_lock);  // Invert the locking mechanism
        }
        
        digitalWrite(8, digitalRead(trigger2));   // Close the straps
        digitalWrite(9, digitalRead(trigger3));   // Open the straps
    }

}



void alertsound(int id)
{
    // id = identification of which sound to play
    // 1 = error, 2 = stand lock on, 3 = stand lock off, 4 = rear gun up, 5 = rear gun stowed

    boolean spk = false;
    // Create a sound generator instead of using tone()
    if (id == 1)
    {
        for (int i = 0; i < 20; i++)
        {
            spk = !spk;
            digitalWrite(6, spk);
            delay(30);
        }
    }

    if (id == 2)
    {
        for (int i = 0; i < 30; i++)
        {
            spk = !spk;
            digitalWrite(6, spk);
            delay(50 - i);
        }
    }

    if (id == 3)
    {
        for (int i = 0; i < 30; i++)
        {
            spk = !spk;
            digitalWrite(6, spk);
            delay(50 - i);
        }
    }
    
    
    digitalWrite(6, LOW);
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
}

void send_selected_value(int val)
{
    digitalWrite(4, ((val & 1) ? HIGH : LOW));
    digitalWrite(5, ((val & 2) ? HIGH : LOW));
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

