int player[] = {A2,A3,A4,A5};
int sensitivity = 900; 

void setup() {
  Serial.begin(9600);
  Serial.println("Start ");
  for (int playerPin = 0; playerPin < (sizeof(player)/sizeof(int)); playerPin++) {
    pinMode(player[playerPin], OUTPUT);
  }
}

void loop() {
  for (int playerPin = 0; playerPin < (sizeof(player)/sizeof(int)); playerPin++) {
    if(analogRead(player[playerPin]) > sensitivity) {
      Serial.print("Player ");
      Serial.print(playerPin);
      Serial.println(" collided");
    } 
  }
}
