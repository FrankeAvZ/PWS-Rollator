const int pin4 = 4;
const int pin5 = 5;
const int ledPin = 3;           // ledPin 
const int zwaaiPin = 2;         // zwaaiPin
const int trigPin = 7;          // trigger pin
const int echoPin = 6;          // echo pin
const int trigPinHor = 10;      
const int echoPinHor = 9;

//Algemeen
int num;
float afstandNorm = 88.5;         // Afstand naar ondergrond op een vlak terrein
float remTijd = 0.1*1000;         // Activatieduur voor de motoren
float wachtTijd = 2000;           // Duratie geactiveerde remmen
bool geremd = false;              // Status van remmen 
int numReadings = 5;              // Het aantal metingen waaruit een gemiddelde wordt genomen
int readIndex;                    // Positie in array
bool senHor = true;               // Horizontale sensor aan/uit zetten
bool gestart = false;

//Horizonale sensor
float echoTijdHor;                // Tijd die het ultrasone geluid heeft gedaan over de afstand
float afstandHor;                 // Afstand naar obstakel
float afstandGemHor;              // Gemiddelde afstand naar obstakel
float totalHor = 0;               // Som van afgelopen 5 metingen
float minAfstand = 20;            // Minimale afstand tot obstakel
float metingenHor[5];             // Array van de laatste 5 metingen
float totalDHor;                  // Som van laatste 5 afwijkingen ten opzichte van gemiddelde
float totalCorHor;                // Som van "correcte" metingen
float deviatieHor;                // Afwijking van een meting ten opzichte van gemiddelde
float afstandGemCorHor;           // Gemiddelde Horizontale afstand gecorrigeerd
int numCorHor;                    // Aantal "correcte" metingen

//Vertical sensor
float metingen[5];
float echoTijd;                   // Tijd die het ultrasone geluid heeft gedaan over de afstand
float afstand;                    // Afstand naar obstakel
float afstandGem;                 // Gemiddelde afstand naar obstakel
float total = 0;                  // variabele voor totaal in het gemiddelde
float afwijking = 0;              // Afwijking van afstand tot de vloer ten opzichte van de normale afstand
float afwijkingNorm = 5;          // Maximale afwijking voordat rem activeert
float afwijkingAbs;               // Absoluut van afwijking
float totalD;                     // Som van laatste 5 afwijkingen ten opzichte van gemiddelde 
float totalCor;                   // Som van "correcte" metingen
float deviatie;                   // Afwijking van een meting ten opzichte van gemiddelde
int numCor;                       // Aantal "correcte" metingen



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
  for(int i = 0; i < numReadings; i++){  
  digitalWrite(trigPin, LOW);         // zet de trigPin op uit  
  digitalWrite(trigPinHor, LOW);      
  
  delayMicroseconds(2);               // wacht 2 microseconden zodat de trigPin ook daadwerkelijk uit is

  digitalWrite(trigPin, HIGH);        // zet de trigPin aan om ultrasoon geluid te zenden
  digitalWrite(trigPinHor, HIGH);
  
  delayMicroseconds(10);              // wacht 10 microseconden
  
  digitalWrite(trigPin, LOW);         // set de trigPin uit om het zenden te stoppen
  digitalWrite(trigPinHor, LOW);
 
  

  //Vertical sensor
  echoTijd = pulseIn(echoPin, HIGH);     // vraag de tijdsduur op hoe lang het duurde voor er geluid werd ontvangen
  afstand = (echoTijd * 0.0343) / 2;     // bereken de afstand aan de hand van de snelheid van het geluid
  
  total= total + afstand;
  metingen[i] = afstand;


  //Horizontale sensor
  if(senHor == true){
  echoTijdHor = pulseIn(echoPinHor, HIGH);
  afstandHor = (echoTijdHor * 0.0343) / 2;  
    
  totalHor = totalHor - metingenHor[i];     // Oudste van de vijf metingen van het totaal aftrekken
  metingenHor[i] = afstandHor;              // Oudste van de vijf metingen vervangen door de nieuwste waarde
  totalHor = totalHor + metingenHor[i];     // Nieuwste meting bij het totaal optellen  
  }
  
  }

  // Verticale Sensor
  afstandGem = total / numReadings;      // Het gemiddelde van de vijf waardes wordt berekent
  Serial.print("Gemiddelde afstand= ");  // Resultaat printen op de seriële monitor (SM)
  Serial.println(afstandGem);
  
  for(int i = 0; i < 5; i++){                       // het kwadraat van de afwijkingen van de metingen ten opzichte van het gemiddelde worden bij elkaar opgeteld
   totalD = totalD + sq(metingen[i] - afstandGem);  
  }

  float stdDev = sqrt(totalD/numReadings);          // Standaard deviatie wordt berekent door de wortel te nemen van de opgetelde afwijking gedeeld door het aantal metingen
  Serial.print("Standaard deviatie= ");             // resultaat printen op SM
  Serial.println(stdDev);
  

   
   for(int i = 0; i < 5; i++){                     // Alle metingen waarvan de afwijking ten opzichte van het gemiddelde kleiner is dan de standaard deviatie worden bij elkaar opgeteld
   deviatie = metingen[i]-afstandGem;
   deviatie = abs(deviatie);
   
    if(deviatie <= stdDev){
      totalCor = totalCor + metingen[i];
      numCor++;
      Serial.println(metingen[i]);
    }
   }

  Serial.print("totalCor= ");                         // Resultaten printen op de SM
  Serial.println(totalCor);
  Serial.print("Aantal correcte metingen: ");
  Serial.println(numCor);
  
   
   afwijking = afstandNorm - (totalCor/numCor);       // Afwijking van het gecorrigeerde gemiddelde ten opzichte van de afstand tot de vloer berekenen
   afwijkingAbs = abs(afwijking);

  Serial.print("Afwijking= ");
  Serial.println(afwijkingAbs);
   
   //Horizontale Sensor
   if(senHor == true){
   afstandGemHor = totalHor / numReadings;            // Het gemiddelde van de vijf waardes wordt berekent
    }
     
   num++;
  
   if(num > 5){
   if(afwijkingAbs > afwijkingNorm){                // Indien de absoluut van deze afwijking groter is dan de maximale afwijking OF afstand tot een obstakel is kleiner dan de minimale afstand: 
    geremd = true;
    Serial.println("rem door hoogte");
    
    
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
   
   
   num = 0;
   
   }
   if(senHor == true){
   if(afstandGemHor < minAfstand){
    if(geremd == false){
    geremd = true;
    Serial.println("rem door obstakel");
    
    
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
   
       
   num = 0;
    }
   }
  }
 }
   
   totalCor = 0; numCor = 0; totalD = 0; 

   Serial.print("Meting nummer: ");
   Serial.println(num);
   Serial.println("");                  // Blanco lijn tussen resultaten
  
   total = 0;
   geremd = false;
 } // Einde loop
 
