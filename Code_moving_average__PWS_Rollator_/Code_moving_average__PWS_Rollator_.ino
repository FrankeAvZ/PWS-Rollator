const int pin4 = 4;
const int pin5 = 5;
const int ledPin = 3;           // ledPin 
const int zwaaiPin = 2;         // zwaaiPin
const int trigPin = 7;          // trigger pin
const int echoPin = 6;          // echo pin
const int trigPinHor = 10;      //
const int echoPinHor = 9;

float echoTijd;                   // variabele voor tijdsduur als continue getal
float echoTijdHor;
float afstand;                    // variabele voor afstand als continue getal  
float metingen[5];
float afstandHor;
float afstandGem;   
float afstandGemHor;
float total = 0;                  // variabele voor totaal in het gemiddelde
float totalHor = 0;
float afwijking = 0;
float afwijkingNorm = 4.5;        // Maximale afwijking voordat rem activeert
float afstandNorm = 89;           // Afstand naar ondergrond op een vlak terrein
float afwijkingAbs;
float remTijd = 0.1*1000000;      // Activatieduur voor de motoren
float wachtTijd = 5000000;        // Duratie geactiveerde remmen
float maxAfstand = 20;
float totalD;
const int numReadings = 5;
int readIndex;
int geremd = 0;                   // Status van remmen (0 = niet geremd, 1 = wel geremd)
int numReading = 0;               // variabele voor aantal metingen voor het gemiddelde


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
  digitalWrite(trigPinHor, LOW);
  
  delayMicroseconds(2);               // wacht 2 microseconden zodat de trigPin ook daadwerkelijk uit is

  digitalWrite(trigPin, HIGH);        // zet de trigPin aan om ultrasoon geluid te zenden
  digitalWrite(trigPinHor, HIGH);
  
  delayMicroseconds(10);              // wacht 10 microseconden
  
  digitalWrite(trigPin, LOW);         // set de trigPin uit om het zenden te stoppen
  digitalWrite(trigPinHor, LOW);
  
  echoTijd = pulseIn(echoPin, HIGH);     // vraag de tijdsduur op hoe lang het duurde voor er geluid werd ontvangen
                                         // Let op dat pulseIn het programma pauseert totdat er een signaal is ontvangen
                                         // standaard is de timeout 1 seconden. Dus het programma staat maximaal 1 seconde stil en geeft dus maximaal 1 seconde geen afstandswaardes
  afstand = (echoTijd * 0.0343) / 2;     // bereken de afstand aan de hand van de snelheid van het geluid
                                         
   Serial.print("Afstand: ");             // Toon het resultaat op de seriële monitor
   Serial.println(afstand);

  echoTijdHor = pulseIn(echoPinHor, HIGH);
  afstandHor = (echoTijdHor * 0.0343) / 2;   

  Serial.print("Afstand Horizontaal: ");  // Toon het resultaat op de seriële monitor
  Serial.println(afstandHor);

  total = total - metingen[readIndex];                            //MOVING AVERAGE FILTER//
  // read from the sensor:
  metingen[readIndex] = afstand;
  // add the reading to the total:
  total = total + metingen[readIndex];
  // advance to the next position in the array:
  readIndex++;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }    
  
  afstandGem = total / numReadings;
  Serial.print("Gemiddelde afstand= ");
  Serial.println(afstandGem);
  Serial.print("van: ");
    for(int i = 0; i < 5; i++)
  {
    Serial.println(metingen[i]);
  }

  for(int i = 0; i < 5; i++){
   totalD = totalD + sq(metingen[i] - afstandGem);  
  }

  float stdDev = sqrt(totalD/numReadings);
  Serial.print("Standaard deviatie= ");
  Serial.println(stdDev);
  
 
 
   
 } //einde ifNumreading
  else{
    
  afstandGem = total / 5;               // Berekent de gemiddelde afstand over 5 metingen
  Serial.print("Gemiddelde afstand: ");         // Print de gemiddelde afstand over 5 metingen
  Serial.println(afstandGem);

  afstandGemHor = ( totalHor / 5);
  Serial.print("Gemiddelde afstand Horizontaal: ");         // Print de gemiddelde afstand over 5 metingen
  Serial.println(afstandGem);
  
  numReading = 0;                       // reset numReading
  total = 0;                            // reset total
  

  afwijking = afstandGem - afstandNorm;
  afwijkingAbs = abs(afwijking);
  Serial.print("afwijkingAbs: ");
  Serial.println(afwijkingAbs);

  if (afwijkingAbs >= afwijkingNorm){    
    digitalWrite(zwaaiPin, LOW);        // Zwaailamp AAN
    
    RemOp;
    delay(remTijd);
    RemUit;
    
    delay(wachtTijd);
    
    RemAf;
    delay(remTijd);
    RemUit;    
    
    digitalWrite(zwaaiPin, HIGH);       // Zwaailamp UIT
    delay(5000000);
  }
  else{
  if (afstandGemHor > maxAfstand){
    digitalWrite(zwaaiPin, LOW);        // Zwaailamp AAN
    
    RemOp;
    delay(remTijd);
    RemUit;
    
    delay(wachtTijd);
    
    RemAf;
    delay(remTijd);
    RemUit;    
    
    digitalWrite(zwaaiPin, HIGH);       // Zwaailamp UIT
    delay(5000000);
  }
  } 
} //einde else
} //Void einde
