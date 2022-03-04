#include <Arduino.h>
#include "RBControl_manager.hpp"
#include <Wire.h>
#include "time.hpp"
#include <stdint.h>
#include "stopwatch.hpp"
#include "nvs_flash.h"
#include <ODriveArduino.h>
#include "defines.h"

// #include <HardwareSerial.h>
// TODO funkce operator; nabijecky; Save pro konfiguraci Odrive; zprovozneni nejake komunikace primo s Odrive 
#define PNR 57.29577951308232

using rb::LED_GREEN;
void configureOdrive( ODriveArduino& odrive );

#define OTOCNY_MOTOR  rb::MotorId::M1 // motor pro otaceni servoruky
HardwareSerial odriveSerial(1);
ODriveArduino odrive(odriveSerial);
#include "function.h"

rb::Manager& rbc()
{
    static rb::Manager m(false,false);  // ve výchozím stavu se motory po puštění tlačítka vypínají, false zařídí, že pojedou, dokud nedostanou další pokyn
    return m;
}

timeout send_data { msec(1000) }; // timeout zajistuje posilani dat do PC kazdych 500 ms

// begin setup *************************************************************************************
void setup() {
    Serial.begin(115200);
    if (!SerialBT.begin("RoadAssistance")) //Bluetooth device name // na tomto pocitaci COM port 13
    {
        Serial.println("!!! Bluetooth initialization failed!");
        serial = &Serial;
    }
    else
    {
        serial = &SerialBT;
        SerialBT.println("!!! Bluetooth work!");
        Serial.println("!!! Bluetooth work!");
    }

    Serial.print ("Starting...\n");
    rbc();
    Serial.print ("RBC initialized\n");
 
    servo0.attach(27);
    servo0.write(position_servo0);
    servo1.attach(26);
    servo1.write(position_servo1);
    servo2.attach(4);
    servo2.write(position_servo2);
    servo3.attach(32);
    servo3.write(position_servo3);

    rbc().leds().blue( true );  // zapne modrou LED - tim zapne i Odrive 

    odriveSerial.begin(115200, SERIAL_8N1, 15, 13);

    Serial.println( "Setup odrive begin" );
    configureOdrive(odrive);

    odrive.initializeMotors( false );  // true - plná kalibrace,  false - kalibrace bez počátečního pískání
    if ( odrive.error() )
    {             // zjistí, jestli je chyba
        odrive.dumpErrors();           // vypíše chybu
        while ( true )
        {
            Serial.println( "Plese fix it!" );
            Serial.print("Voltage: ");
            Serial.println(odrive.inputVoltage() / 4); // vypíše průměrné napětí na článek
            delay( 10000 );
        }
    }
    Serial.println( "Done" );
    Serial.println( "Turning on" );
    odrive.turnOn();  // zapnutí odrive - fixovani motoru 
    if ( odrive.error() ) {
        odrive.dumpErrors();
    }
    Serial.println( "Turned on" );
    delay( 500 );

        odrive.move( 0, 0, max_speed ); // dojeď s osou 0 na pozici 0 rychlostí max_speed tiků na otáčku 
        odrive.move( 1, 0, max_speed ); 
        delay( 2000 );

        odrive.move( 0, otocka_kola, max_speed*2 );
        odrive.move( 1, otocka_kola, max_speed*2 ); 
        delay( 1000 );

        odrive.move( 0, 0, max_speed );
        odrive.move( 1, 0, max_speed );
        delay( 1000 );

        if ( odrive.error() )
            odrive.dumpErrors();

        vpred(200);

    send_data.restart();
}

void loop() {
    if (send_data)
    {
        send_data.ack();
        if (L_G_light) L_G_light = false; else  L_G_light = true;
        rbc().leds().green(L_G_light);
        Serial.println( millis() );
        SerialBT.println( millis() ); // na pocitaci Burda COM port 13
    }
    delay(10);
} 


