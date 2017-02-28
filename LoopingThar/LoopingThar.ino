// Player pins
int player[] = {A2,A3,A4,A5};

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
  for (int playerPin = 0; playerPin < (sizeof(player)/sizeof(int)); playerPin++) {
    pinMode(player[playerPin], OUTPUT);
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
  checkCollision();
  // ToDo Make use of a timer (delay not possible -> we need the collision detection)
}

void checkCollision(){
  for (int playerPin = 0; playerPin < (sizeof(player)/sizeof(int)); playerPin++) {
    if(analogRead(player[playerPin]) > sensitivity) {
      Serial.print("Player ");
      Serial.print(playerPin);
      Serial.println(" collided");
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


