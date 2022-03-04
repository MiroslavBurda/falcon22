// function.h

//  Serial.println( odrive.getPos( 0 ) );  // vraci pozici enkoderu osy 0
// odrive.move( 0, 0, max_speed ); // dojeď s osou 0 na pozici 0 rychlostí max_speed tiků na otáčku 
// void speed( int axis, float speed ); // pro osu axis nastavi rychlost speed v ticich za sekundu
// void setAccel( float accel );  // nastavi zrychleni pro dalsi pohyb

long max_speed = 40000; // pocet tiku za sekundu max cca 200000,  enkodéry zvládají cca 5000 otacek motoru za sekundu
const int LEVY_MOTOR = 0;
const int PRAVY_MOTOR = 1; 
float smer = 0.0;
const float PREVOD = 10; // prevodovy pomer 
const float PRUMER_KOLA = 60.0; // v mm 
int mm = 2400 * PREVOD / (3.1415927 * PRUMER_KOLA );  // // pocet tiku na mm 
// int otacka = 235; // pocet tiku na otacku kola - nevim jiste 
int zatoc = 2400 * PREVOD / (3.1415927 * PRUMER_KOLA / 4 );  // pocet tiku na zatoceni o 90 stupnu

struct Driven {
    int left_enc;
    int right_enc;
    bool ok;
};

// bool obstacle();
// {
//     Serial.print(d);  // OVERIT !!!!!



// }
// t = millis();

// while ( ! Serial1.available() )  
//         sleep(0);
//     c = Serial1.read();

// if ((millis() - t) > 4000) {
//             rbc().setMotors().stop(LEFT_MOTOR)
//                              .stop(RIGHT_MOTOR)
//                              .set();
//             Serial.println("Calibration failed (timeout)");
//             return false;
//         }

void drive(int levy, int pravy) // pocet tiku enkoderu levny motor, pravy motor , vraci neujetou vzdalenost, 0 znamena, ze dojel, jak ma 
{   
    Driven a;
    a.left_enc = levy;
    a.right_enc = pravy;
    // do
    { 
        int left_enc_begin = odrive.getPos( LEVY_MOTOR );
        int right_enc_begin = odrive.getPos( PRAVY_MOTOR );
        Serial.println("drive");
        while (true) // * ! (end_L and end_R)
        {
            // if  ( !( digitalRead(33) and digitalRead(5) and digitalRead(27) and digitalRead(26) and digitalRead(25) ) ) {
            //     Serial.println("obstacle detected");
            //     speed( int LEVY_MOTOR, 0 ); // stop
            //     speed( int PRAVY_MOTOR, 0 ); // stop
            //    if ( odrive.error() )
            //         odrive.dumpErrors();         
            //                
                   delay(100); // ceka, jestli souper neodjel
           // }
           // else {
                
                odrive.move( LEVY_MOTOR, a.left_enc, max_speed );
                odrive.move( PRAVY_MOTOR, a.right_enc, max_speed );
                delay(100);
                a.left_enc = levy - odrive.getPos( LEVY_MOTOR ) - left_enc_begin;
                a.right_enc = pravy - odrive.getPos( PRAVY_MOTOR ) - right_enc_begin;
                printf( "levy %i, pravy %i, a.left_enc %i, a.right_enc %i \n", levy, pravy , a.left_enc, a.right_enc );
                a.ok = ( ( ((levy >= 0) and (a.left_enc < 2)) or  ((levy <= 0) and (a.left_enc > 2)) ) and 
                       ( ((pravy >= 0) and (a.right_enc < 2)) or  ((pravy <= 0) and (a.right_enc > 2)) ) ) ? true : false ;
                Serial.println(a.ok ? "dojeto" : "nedojeto");
                if (a.ok) 
                    break;
           // }
        }  
        // delay(1000);
    } 
    // while (!a.ok); 
}

void vpred(float kolik)  // kolik pocet milimetru
{
    drive(kolik * mm, kolik * mm);  
}

void vpravo(float kolik) // kolik - pocet otocek o 90 stupnu
{
    drive(kolik * zatoc, 0);   
}

void vpravo_na_miste(float kolik) // kolik - pocet otocek o 90 stupnu
{
     drive(kolik*zatoc/2, -kolik*zatoc/2);   
}

void vlevo(float kolik) // kolik - pocet otocek o 90 stupnu
{
    drive(0, kolik * zatoc);     
}
