const int pin4 = 4;
const int pin5 = 5;
const int ledPin = 3;           // ledPin 
const int zwaaiPin = 2;         // zwaaiPin
const int trigPin = 7;          // trigger pin
const int echoPin = 6;          // echo pin
const int trigPinHor = 10;      
const int echoPinHor = 9;

//Algemeen

float afstandNorm = 89;           // Afstand naar ondergrond op een vlak terrein
float remTijd = 0.1*1000;         // Activatieduur voor de motoren
float wachtTijd = 5000;           // Duratie geactiveerde remmen
bool geremd = false;              // Status van remmen 

//Vertical sensor
float echoTijd;                   // Tijd die het ultrasone geluid heeft gedaan over de afstand
float afstand;                    // Afstand naar obstakel
float total = 0;                  // variabele voor totaal in het gemiddelde
float afwijking = 0;              // Afwijking van afstand tot de vloer ten opzichte van de normale afstand
float afwijkingNorm = 4.5;        // Maximale afwijking voordat rem activeert
float afwijkingAbs;               // Absoluut van afwijking



void setup() {
  Serial.begin(9600);                 // stel de seriële monitor in (plotten) 
  pinMode(trigPin, OUTPUT);           // stel de trigger pin in als uitvoer
  pinMode(echoPin, INPUT);            // stel de echo pin in als invoer
  pinMode(ledPin, OUTPUT);
  pinMode(zwaaiPin, OUTPUT);
  digitalWrite(zwaaiPin, HIGH); // uit
  digitalWrite(ledPin, HIGH); // aan
}

void RemOp()                      //Functie om rem in te drukken
{
 digitalWrite(pin4, HIGH);
 digitalWrite(pin5, LOW);
}
void RemAf()                      //Functie om rem er af te halen
{
 digitalWrite(pin4, LOW);
 digitalWrite(pin5, HIGH);
}
void RemUit ()                    //Functie om de rem stil te zetten
{
digitalWrite(pin4, LOW);
digitalWrite(pin5, LOW);
}



void loop() {
  digitalWrite(trigPin, LOW);         // zet de trigPin op uit  
  digitalWrite(trigPinHor, LOW);      
  
  delayMicroseconds(2);               // wacht 2 microseconden zodat de trigPin ook daadwerkelijk uit is

  digitalWrite(trigPin, HIGH);        // zet de trigPin aan om ultrasoon geluid te zenden
  digitalWrite(trigPinHor, HIGH);
  
  delayMicroseconds(10);              // wacht 10 microseconden
  
  digitalWrite(trigPin, LOW);         // set de trigPin uit om het zenden te stoppen
  digitalWrite(trigPinHor, LOW);
  
  echoTijd = pulseIn(echoPin, HIGH);     // vraag de tijdsduur op hoe lang het duurde voor er geluid werd ontvangen
  
  afstand = (echoTijd * 0.0343) / 2;     // bereken de afstand aan de hand van de snelheid van het geluid
  afwijking = afstand-afstandNorm;
  afwijkingAbs = abs(afwijking);

if(afwijking > afwijkingNorm){
    if(geremd == false){
    geremd = true;
    digitalWrite(zwaaiPin, LOW);        // Zwaailamp AAN
    
    RemOp;                                                                        
    delay(remTijd);                     // Aantal seconden dat de motor draait
    RemUit;
    
    delay(wachtTijd);                   // Wachttijd totdat de rem er af wordt gehaald
    
    RemAf;                              
    delay(remTijd);                     // Aantal seconden dat de motor draait
    RemUit;    
    
    digitalWrite(zwaaiPin, HIGH);       // Zwaailamp UIT
    delay(5000);                        // 5 seconden tijd om weg te komen bij het obstakel
   }
   }
   geremd = false;
   Serial.println("");                  // Blanco lijn tussen resultaten
}
   
 
 
