unsigned long mil;

unsigned long laser_delay[2];

void setup() {

    
}

/*
 * One bi-directional motor for raising/lowering luke
 * LEDs for light saber flash
 * LEDs for internal explosions
 * 
 * Laser shot LEDs
 * Laser barrel recoil servos
 * sound effects for lasers
 * 
 * (non-controlled)
 * cockpit red glow LED (hard switch)
 * Eight single-direcitonal motors for rotating the knees (hard switch)
 */

void loop() {
  
    mil = millis();

    // Laser shot takes one full second to complete

    for (int i = 0l i < 2; i++)
    {
        if (laser_delay[i] < mil)
        {

            laser_delay[i] = mil + 1000;
        }
    }

}
