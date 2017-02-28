#include "pitches.h"

// Player pins
//rot, lila, grün, blau
int echos[] = {A5,A3,A4,9999};
// rot, lila, grün, blau
int trigger[] = {6,13,7,9999};
// rot, lila, grün, blau
long dauer[] = {0,0,0,0};
// rot, lila, grün, blau
int collisions[] = {0,0,0,0};
// rot, lila, grün, GELB
int entfernung[] = {0,0,0,0};

// Speed regulation pin
int speedRegulationPin = 3;

// Intervals for the speed regulation
int intervalls[] = {90,130,150,200,255};
// Sensitivity for the speed regulation resistor
int sensitivity = 900; 

// pump pin
const uint8_t pump_pin   = 12;

// delays for the pump
int delay_pump = 100;
int delay_valve = 100;

// valve pins
const uint8_t valve_pin[4] {11,10,9,8};

//temp
int counter = 3;
int hasToDrink = 0; 
const int pingPin = A0;
long duration = 0;
int soundPin = A1;

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};


void playSound(){
    for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(soundPin, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(soundPin);
  }
}

void setup() {
  playSound();
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 65000;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
  
  Serial.begin(9600);
  Serial.println("Start ");
  // Initialize player colision pins
  for (int playerPin = 0; playerPin < counter; playerPin++) {
    pinMode(trigger[playerPin], OUTPUT); 
    pinMode(echos[playerPin], INPUT); 
  }


  // Initialize pump pins
  pinMode(pump_pin, OUTPUT);
  for(int i=0; i<4; i++) {
    pinMode(valve_pin[i], OUTPUT);
  }
  
  // Initialize speed regulation pin
  pinMode(speedRegulationPin, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(4,HIGH);
  pinMode(5, OUTPUT);
  digitalWrite(5,LOW);
  analogWrite(3, intervalls[0]);

}

void loop() {
  hasToDrink = 0;
  for(int i=0;i<counter;i++){
    digitalWrite(trigger[i], LOW); //Hier nimmt man die Spannung für kurze Zeit vom Trigger-Pin, damit man später beim senden des Trigger-Signals ein rauschfreies Signal hat.
    digitalWrite(trigger[i], HIGH); //Jetzt sendet man eine Ultraschallwelle los.
    digitalWrite(trigger[i], LOW);//Dann wird der „Ton“ abgeschaltet.

    dauer[i] = pulseIn(echos[i], HIGH, 10000);
    entfernung[i] = (dauer[i]/2) * 0.03432;
    hasToDrink = checkCol();
    if(hasToDrink>0) {
        failed(hasToDrink); 
    }
 // Serial.print(i);
 // Serial.print(": ");
 // Serial.println(entfernung[i]);
  }
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH,10000);
  entfernung[3] = duration/ 29 / 2;
    // Serial.print("3: ");
    // Serial.println(entfernung[3]);
      hasToDrink = checkCol();
    if(hasToDrink>0) {
    failed(hasToDrink);
    }
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  randomSpeed();
}

void randomSpeed(){
  int x = random(5);
  if(x==1){
  int ri = random(4);
    analogWrite(3, intervalls[ri]);
    Serial.println(ri);
    if(random(4)>=3){
      digitalWrite(4,LOW);
      digitalWrite(5,HIGH);
    }else{
      digitalWrite(4,HIGH);
      digitalWrite(5,LOW);
    }
  }
}

void give_shot(int valve) {
    start_pump(); 
    delay(delay_pump);
    open_valve(valve);
    delay(delay_valve);
    close_valve(valve);
    delay(delay_pump);
    stop_pump();
}

void start_pump() {
    digitalWrite(pump_pin,HIGH);
    Serial.println("Start Pump");
}

void stop_pump() {
    digitalWrite(pump_pin,LOW);
    Serial.println("Stop Pump");
}

void open_valve(int valve) {
    digitalWrite(valve_pin[valve],HIGH);
    Serial.println("Open Valve");
}

void close_valve(int valve) {
    digitalWrite(valve_pin[valve],LOW);
    Serial.println("Close Valve");
    
}

int checkCol(){
  for(int i=0;i<4;i++){
    if(entfernung[i] <10 && entfernung[i]>8){
        Serial.print("Player: ");
        Serial.print(i+1);
        Serial.println(" collided.");
        collisions[i]++;
        entfernung[i]=0;
        return i+1; 
    }
  }
  return 0;
}


void failed(int a){
  playSound();
  delay(2000);
  give_shot(a);
  delay(2000);
  tone(soundPin, 600, 1000);
}

