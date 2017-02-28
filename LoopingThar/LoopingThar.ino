// Player pins
// rot, lila, grün, blau
int echos[] = {A1,A2,A3,A4};
// rot, lila, grün, blau
int trigger[] = {A5,6,7,13};
// rot, lila, grün, blau
long dauer[] = {0,0,0,0};
// rot, lila, grün, blau
int collisions[] = {0,0,0,0};
// rot, lila, grün, blau
int entfernung[] = {0,0,0,0};

// Speed regulation pin
int speedRegulationPin = 3;

// Intervals for the speed regulation
int intervalls[] = {0,130,150,200,255};
// Sensitivity for the speed regulation resistor
int sensitivity = 900; 

// pump pin
const uint8_t pump_pin   = 12;

// delays for the pump
int delay_pump = 500;
int delay_valve = 5000;

// valve pins
const uint8_t valve_pin[4] {11,10,9,8};

void setup() {
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
  for (int playerPin = 0; playerPin < (sizeof(trigger)/sizeof(int)); playerPin++) {
    pinMode(trigger[playerPin], OUTPUT); 
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
  int hasToDrink = 0;
  for(int i=0;i<4;i++){
    digitalWrite(trigger[i], LOW); //Hier nimmt man die Spannung für kurze Zeit vom Trigger-Pin, damit man später beim senden des Trigger-Signals ein rauschfreies Signal hat.
    digitalWrite(trigger[i], HIGH); //Jetzt sendet man eine Ultraschallwelle los.
    digitalWrite(trigger[i], LOW);//Dann wird der „Ton“ abgeschaltet.

    dauer[i] = pulseIn(echos[i], HIGH);
    entfernung[i] = (dauer[i]/2) * 0.03432;
    hasToDrink = checkCol();
    if(hasToDrink>0) {
      // Rikus ihm seine Funktion
    }
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
    if(random(2)==1){
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
        Serial.print(" collided.");
        collisions[i]++;
        entfernung[i]=0;
        return i+1; 
    }
  }
  return 0;
}

