const int pin4 = 4;
const int pin5 = 5;
const int ledPin = 3;           // ledPin 
const int zwaaiPin = 2;         // zwaaiPin
const int trigPin = 7;          // trigger pin
const int echoPin = 6;          // echo pin

int num;
float echoTijd;                 // variabele voor tijdsduur als continue getal
float afstand;                  // variabele voor afstand als continue getal  
float afstandGem;   
float total = 0;                // variabele voor totaal in het gemiddelde
float afwijking = 0;
float afwijkingNorm = 4.5;        // Maximale afwijking voordat rem activeert
float afstandNorm = 67.5;         // Afstand naar ondergrond op een vlak terrein
float afwijkingAbs;
float remTijd = 0.1*1000000;    // Activatieduur voor de motoren
float wachtTijd;                 // Duratie geactiveerde remmen
int numReading = 0;             // variabele voor aantal metingen voor het gemiddelde


void setup() {
  Serial.begin(9600);                 // stel de seriële monitor in (plotten) 
  pinMode(trigPin, OUTPUT);           // stel de trigger pin in als uitvoer
  pinMode(echoPin, INPUT);            // stel de echo pin in als invoer
  pinMode(ledPin, OUTPUT);
  pinMode(zwaaiPin, OUTPUT);
  digitalWrite(zwaaiPin, HIGH); // uit
  digitalWrite(ledPin, HIGH); // aan
}

void RemOp()
{
 digitalWrite(pin4, HIGH);
 digitalWrite(pin5, LOW);
}
void RemAf()
{
 digitalWrite(pin4, LOW);
 digitalWrite(pin5, HIGH);
}
void RemUit ()
{
digitalWrite(pin4, LOW);
digitalWrite(pin5, LOW);
}



void loop() {
 if(numReading < 5)
 {
  digitalWrite(trigPin, LOW);         // zet de trigPin op uit
  delayMicroseconds(2);               // wacht 2 microseconden zodat de trigPin ook daadwerkelijk uit is

  digitalWrite(trigPin, HIGH);        // zet de trigPin aan om ultrasoon geluid te zenden
  delayMicroseconds(10);              // wacht 10 microseconden
  digitalWrite(trigPin, LOW);         // set de trigPin uit om het zenden te stoppen

  echoTijd = pulseIn(echoPin, HIGH);     // vraag de tijdsduur op hoe lang het duurde voor er geluid werd ontvangen
                                         // Let op dat pulseIn het programma pauseert totdat er een signaal is ontvangen
                                         // standaard is de timeout 1 seconden. Dus het programma staat maximaal 1 seconde stil en geeft dus maximaal 1 seconde geen afstandswaardes
  afstand = (echoTijd * 0.0343) / 2;     // bereken de afstand aan de hand van de snelheid van het geluid
                                         
   Serial.print("Afstand: ");             // Toon het resultaat op de seriële monitor
   Serial.println(afstand);

  
  if(afstand <= 500){
  if(afstand > 5){
  total = total + afstand;               // Voegt de gemeten afstand toe aan het totaal
  numReading++;                       
  }
 }
 } //einde ifNumreading
  else{
    
  afstandGem = total / 5;               // Berekent de gemiddelde afstand over 5 metingen
  Serial.print("AfstandGem: ");         // Print de gemiddelde afstand over 5 metingen
  Serial.println(afstandGem);

  
  numReading = 0;                       // reset numReading
  total = 0;                            // reset total
  num++;

  afwijking = afstandGem - afstandNorm;
  afwijkingAbs = abs(afwijking);
  Serial.print("afwijkingAbs: ");
  Serial.println(afwijkingAbs);

  if (afwijkingAbs >= afwijkingNorm){
    
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
  Serial.print("Test nummer: ");
  Serial.println(num);
} //einde else
} //Void einde
