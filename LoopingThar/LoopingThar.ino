int player1 = A2;
int player2 = A3;
int player3 = A4;
int player4 = A5;
int sensitivity = 900; 

void setup() {
  Serial.begin(9600);
  Serial.println("Start ");
  pinMode(player1, OUTPUT);
  pinMode(player2, OUTPUT);
  pinMode(player3, OUTPUT);
  pinMode(player4, OUTPUT);
}

void loop() {
  if(analogRead(player1) > sensitivity) {
    Serial.println("Player 1 collided");
  }
  if(analogRead(player2) > sensitivity) {
    Serial.println("Player 2 collided");
  }
  if(analogRead(player3) > sensitivity) {
    Serial.println("Player 3 collided");
  }
  if(analogRead(player4) > 900) {
    Serial.println("Player 4 collided");
  }

}
