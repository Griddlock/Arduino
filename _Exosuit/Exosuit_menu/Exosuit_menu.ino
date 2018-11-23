/*
 * Menu Controller
 * - Changes output based on the control bits, basically, a fancy multiplexer
 * - 
 */




void setup() {
    pinMode(2, OUTPUT); //      Control bit 1
    pinMode(3, OUTPUT); // PWM  Control bit 2
    pinMode(4, OUTPUT); //      Control bit 3
    pinMode(5, OUTPUT); // PWM  Control activate (joystick button)
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
    pinMode(A5, INPUT);   // (move the joystick to here, maybe?)
    pinMode(A6, INPUT);   //
    pinMode(A7, INPUT);   //

}

void loop() {
  // put your main code here, to run repeatedly:

}
