int player[] = {A2,A3,A4,A5};
int speedRegulationPin = 3;

int intervalls[] = {0,63,127,191,255};
int sensitivity = 900; 

void setup() {
  Serial.begin(9600);
  Serial.println("Start ");
  // Initialize player colision pins
  for (int playerPin = 0; playerPin < (sizeof(player)/sizeof(int)); playerPin++) {
    pinMode(player[playerPin], OUTPUT);
  }
  // Initialize speed regulation pin
  pinMode(speedRegulationPin, OUTPUT);
}

void loop() {
  checkCollision();
  // ToDo Make use of a timer (delay not possible -> we need the collision detection)
  randomSpeed();
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

void randomSpeed(){
  int ri = random(5);
  analogWrite(3, intervalls[ri]);
}


