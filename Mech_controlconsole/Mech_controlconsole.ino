
// 7-Segment Tubes
//                           0    1    2    3    4    5    6    7    8    9         -    n    o    F    U    E    L    c
unsigned char LED_CHARS[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xFF,0xBF,0xDC,0x9C,0x8E,0xC1,0x86,0xC7,0x9E};
unsigned char ADDR[] = { 0x01, 0x02, 0x04, 0x08 };
unsigned char TEMP1[5];
unsigned char TEMP2[5];
unsigned char LED[5];
unsigned char BLANK[5];
int SCLK[3];
int RCLK[3];
int DIO[3];

unsigned long cnt[10];    // Real time loop counters 0=fuel countdown, 1=ext.temp, 2=int.temp, 3=hazard, 4=hazard

// Thermometers
int temp_ext;             // External temperature voltage
int temp_int;             // Internal temperature voltage
int fuel_level = 180;     // Fuel level (countdown value)

// Hazard lights
unsigned long flashtime[2]; // The millis() count when the output is set to high
int flashing[2];            // The millis() count when the output is set to low
int flashdelay[2];          // The number of milliseconds until the next flash (flip-flops between values to make double-flash effect)
int flashdelay1[2];         // milliseconds delay between the double-flash
int flashdelay2[2];         // milliseconds delay until the next double-flash

void setup ()
{
    // These are the pin layouts for each device. You can add more, of course, on the analog pins, so one Arduino board can control even more displays at once
    SCLK[0] = 4; RCLK[0] = 3; DIO[0] = 2;
    SCLK[1] = 7; RCLK[1] = 6; DIO[1] = 5;
    SCLK[2] = 10; RCLK[2] = 9; DIO[2] = 8;
  
    // There are 5 pins on each tube (Vcc, SCLK, RCLK, DIO, Gnd). Vcc and Gnd are obvious. Hook up the pins from the tube to your arduino, as defined above. (ex. SCLK[device_number] = pin_number  ==> For device #0, hook to pin 4 of arduino)
    pinMode(SCLK[0],OUTPUT);
    pinMode(RCLK[0],OUTPUT);
    pinMode(DIO[0],OUTPUT);
    pinMode(SCLK[1],OUTPUT);
    pinMode(RCLK[1],OUTPUT);
    pinMode(DIO[1],OUTPUT);
    pinMode(SCLK[2],OUTPUT);
    pinMode(RCLK[2],OUTPUT);
    pinMode(DIO[2],OUTPUT);
  
    pinMode(A0, INPUT);   // External Temperature
    pinMode(A1, INPUT);   // Engine Temperature
    pinMode(A2, INPUT);   // Hazard light switch
    pinMode(A4, OUTPUT);  // Hazard light 1
    pinMode(A5, OUTPUT);  // Hazard light 2

    // Hazard light setup
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


    Serial.begin(9600);
    // For blanking out the display
    BLANK[0] = 10; BLANK[1] = 10; BLANK[2] = 10; BLANK[3] = 10; BLANK[4] = 0;
  
}

void loop()
{
    unsigned long mil = millis();

    // ################################## Time-based blocks

    if (cnt[0] < mil)   // Fuel countdown, if pin D11 is high
    {
        if (digitalRead(11) == HIGH)  // Only if the switch is on
        {
            cnt[0] = mil + 990;
            
    
            if (fuel_level > 0)
            {
                fuel_level--;  // countdown
                breakdown(LED, fuel_level, true, false);
            } else {
                // Flash "no / FUEL"
                if (fuel_level == 0)
                {
                    LED[3] = 10;
                    LED[2] = 12;  // n
                    LED[1] = 13;  // o
                    LED[0] = 10;
                    fuel_level = -1;
                } else {
                    LED[3] = 14;  // F
                    LED[2] = 15;  // U
                    LED[1] = 16;  // E
                    LED[0] = 17;  // L
                    fuel_level = 0;
                }
                cnt[0] = mil + 490;
            }
        } else {
            // No countdown, show full tank, then check for on-switch every 5 seconds
            LED[3] = 14;  // F
            LED[2] = 15;  // U
            LED[1] = 17;  // L
            LED[0] = 17;  // L
            fuel_level = 181;
            cnt[0] = mil + 5000;
        }
    }


    if (cnt[1] < mil)   // Thermistor for external
    {
        cnt[1] = mil + 900;
        // read the external temperature
        temp_ext = readThermistor(analogRead(A0)) * 10;
        breakdown(TEMP1, temp_ext, true, false);
        TEMP1[0] = 18;  // Make digit 0 a "c"
    }
    
    if (cnt[2] < mil)   // Thermistor for internal
    {
        cnt[2] = mil + 400;
        temp_ext = readThermistor(analogRead(A1)) * 10;
        breakdown(TEMP2, temp_ext, true, false);
        TEMP2[0] = 18;
    }


    if (digitalRead(A2) == HIGH)      // Hazard light switch is on
    {
        for (int x = 1; x <= 2; x++)
        {
            // flash the hazard lights on consistent interval, regardless of the stepper motor controller
            if (mil >= flashtime[x])
            {
                // time to flash again, and reset the flashtime
                flashtime[x] = mil + flashdelay[x];
                if (flashdelay[x] > 1000) { flashdelay[x] = flashdelay1[x]; } else { flashdelay[x] = flashdelay2[x]; }
                if (x == 1) { digitalWrite(A4, HIGH); } else { digitalWrite(A5, HIGH); }
                flashing[x] = mil + 100;
            }
        
            if ((mil > flashing[x]) && (flashing[x] > 0))
            {
                if (x == 1) { digitalWrite(A4, LOW); } else { digitalWrite(A5, LOW); }
                flashing[x] = 0;
            }
        }
    } else {
        digitalWrite(A4, LOW);
        digitalWrite(A5, LOW);
    }



    // ################################## Loop-based blocks

    // When you apply a digit, that digit stays visible until you latch another value into that same tube.
    // In order to maximize the amount of time that each 7-Seg display digit is on, display the first digit for all devices, then the second for all devices, etc.
    // This way, each tube has one digit on for the most time possible.
    LED4_Display(LED, 0, 0);
    LED4_Display(TEMP1, 1, 0);
    LED4_Display(TEMP2, 2, 0);
    LED4_Display(LED, 0, 1);
    LED4_Display(TEMP1, 1, 1);
    LED4_Display(TEMP2, 2, 1);
    LED4_Display(LED, 0, 2);
    LED4_Display(TEMP1, 1, 2);
    LED4_Display(TEMP2, 2, 2);
    LED4_Display(LED, 0, 3);
    LED4_Display(TEMP1, 1, 3);
    LED4_Display(TEMP2, 2, 3);

    // Clear digit 3 from each device so that it gets the exact same exposure as the other three, instead of the delay time until the next cycle (it would otherwize make the left digit appear brighter than all the others)
    LED4_Display(BLANK, 0, 3);
    LED4_Display(BLANK, 1, 3);
    LED4_Display(BLANK, 2, 3);

}

// This function breaks down a 4-digit (or fewer) number into its individual characters to make it easy to display them
unsigned char breakdown(unsigned char *l, int val, bool trm, bool dash)
{
    // l = an array [0-3] of where to store the individual character values, once split
    // val = The integer that you want to display
    // trm, if TRUE, will trim the left-side zeros from the number
    // dash, if TRUE, will make all the characters dashes if 'val' is zero (good for countdowns or null values)
    bool neg = false;
    if (val < 0) { val = abs(val); neg = true; }
    
    // Break down the number into digits
    l[3] = floor (val / 1000);
    l[0] = (val - (floor(val / 10) * 10));
    l[1] = ((val - (floor(val / 100) * 100)) - l[0]) / 10;
    l[2] = ((val - (l[3] * 1000)) - ((l[1] * 10) + l[0])) / 100;

    if (trm)
    {
        // Get rid of right-side zeros
        if (l[3] == 0) { l[3] = 10; }
        if ((l[2] == 0) && (l[3] == 10)) { l[2] = 10; }
        if ((l[1] == 0) && (l[2] == 10)) { l[1] = 10; }
        if ((l[0] == 0) && (l[1] == 10)) { l[0] = 10; }
    }
    if (dash)
    {
        if (val == 0) { l[0] = 11; l[1] = 11; l[2] = 11; l[3] = 11; }
    }

    if (neg)
    {
        l[3] = 11;
    }

}

void LED4_Display (unsigned char *led, int device, int digit)
{
    unsigned char *t;
    unsigned char *a;
        
    t = LED_CHARS + led[digit];     // Specify the character to use, in this case led[0,2,3,or 4]
    a = ADDR + digit;               // Get the mask for the panel, which makes the letter show on only one of the four displays
    LED_OUT(*t, device);            // Communicate the character to the tube
    LED_OUT(*a, device);            // Communicate the mask to the tube
    digitalWrite(RCLK[device],LOW); // Latch the data to the tube (i.e. make it go)
    digitalWrite(RCLK[device],HIGH);
}

void LED_OUT(unsigned char X, int device)
{
  unsigned char i;
  for(i=8;i>=1;i--)     // Latch each bit of X into the tube. This is a serial communication...one bit at a time.
  {
    digitalWrite(DIO[device],X & 0x80);   // Set the output bit to the value of the most significant bit in the character
    X<<=1; // Shift all bits in the character left (like a conveyor belt, so the next loop gets the next bit.
    digitalWrite(SCLK[device],LOW); // Latch the bit into the tube
    digitalWrite(SCLK[device],HIGH);
  }
}


double readThermistor(int pin) 
{
  // variables that live in this function
  double rThermistor = 0;            // Holds thermistor resistance value
  double tKelvin     = 0;            // Holds calculated temperature
  double tCelsius    = 0;            // Hold temperature in celsius
  double adcAverage  = 0;            // Holds the average voltage measurement
  double balance = 9945;
  double beta = 3974.0;
  double room_temp = 293.15;
  double r_room = 11825;



  rThermistor = balance * ( (933.0 / pin) - 1);
  tKelvin = (beta * room_temp) / (beta + (room_temp * log(rThermistor / r_room)));
  tCelsius = tKelvin - 273.15;  // convert kelvin to celsius 

  Serial.println(rThermistor);
  
  return tCelsius;    // Return the temperature in Celsius
}

