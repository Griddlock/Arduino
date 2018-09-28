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

unsigned long cnt[10];    // Real time loop counters

int temp_ext;             // External temperature voltage
int temp_int;             // Internal temperature voltage
int fuel_level = 180;     // Fuel level (countdown value)


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


  Serial.begin(9600);
  // For blanking out the display
  BLANK[0] = 10; BLANK[1] = 10; BLANK[2] = 10; BLANK[3] = 10; BLANK[4] = 0;
  
}

void loop()
{
    unsigned long mil = millis();

    // ################################## Time-based blocks

    if (cnt[0] < mil)
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
                LED[0] = 10;
                LED[1] = 13;
                LED[2] = 12;
                LED[3] = 10;
                fuel_level = -1;
            } else {
                LED[3] = 14;
                LED[2] = 15;
                LED[1] = 16;
                LED[0] = 17;
                fuel_level = 0;
            }
            cnt[0] = mil + 490;
        }
    }

    if (cnt[1] < mil)
    {
        cnt[1] = mil + 900;
        // read the external temperature
        temp_ext = analogRead(A0);    // (430 - 930)
        temp_ext = ((temp_ext - 500) * 10); // Shift the digits one to the left
        breakdown(TEMP1, temp_ext, true, false);
        TEMP1[0] = 18;  // Make digit 0 a "c"
    }
    
    if (cnt[2] < mil)
    {
        cnt[2] = mil + 400;
        temp_ext = analogRead(A0);
        temp_ext = ((temp_ext - 500) * 30); // Shift the digits one to the left
        breakdown(TEMP2, temp_ext, true, false);
        TEMP2[0] = 18;
        
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
