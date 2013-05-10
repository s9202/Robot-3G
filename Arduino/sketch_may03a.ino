#include <Servo.h>
Servo servo1;
Servo servo2;



//--------------------------- wybranie portów które będziemy używać
void setup() {
	pinMode(13, OUTPUT);		//Port13 to akurat jest wbudowana dioda
	digitalWrite(13, LOW);	//port13
        servo1.attach(14); //analog A0
        servo2.attach(15); //analog A1
        
	Serial.begin(9600);		//Uruchomienie USB serialport z prędkością 9600
}

//------------------------------ Główna pętla z programem

void loop() {
       
	
	if( Serial.available() > 0 ) {			
		char letter = Serial.read();		
		
		if( letter == '1') {//przod - START
			digitalWrite(13, HIGH);
                        servo1.attach(14);  servo2.attach(15); 
                        servo1.write(135);  servo2.write(45);
			Serial.println(" { \"komunikat\": \"DIODA SWIECI\" } ");	
		        Serial.println(" \"1\" ");					
		}
		else if( letter == '3') {//lewo - START
			digitalWrite(13, HIGH);
                        servo1.attach(14);  servo2.attach(15); 
                        servo1.write(80);  servo2.write(80);
		}
		else if( letter == '5') {//tyl - START
			digitalWrite(13, HIGH);
                        servo1.attach(14);  servo2.attach(15); 
                        servo1.write(45);  servo2.write(135);
		}
                else if( letter == '7') {//prawo - START
			digitalWrite(13, HIGH);
                        servo1.attach(14);  servo2.attach(15); 
                        servo1.write(100);  servo2.write(100);
		}
                else if( letter == '0' || letter== '2' || letter=='4' || letter =='6'){//przod, lewo, prawy, tyl - STOP
                  	digitalWrite(13, LOW);
                        servo1.detach();  servo2.detach();
			Serial.println(" { \"komunikat\": \"DIODA GASNIE\" } ");
		}
  
	

        }
}
