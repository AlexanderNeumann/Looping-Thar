// kate: syntax c++

 // global Variable defintions
 
const uint8_t pump_pin   = 12;

int delay_pump = 500;
//int delay_valve = 100;


const uint8_t valve_pin[4] {
  11,
  10,
  9,
  8,
};


void give_shot(int valve) {
    start_pump(); 
    //for(int i=0; i<10; i++) {
    delay(delay_pump);
    open_valve(valve);
    delay(delay_valve);
    close_valve(valve);
    delay(delay_pump);
    
   //}
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
 
void setup() {
  // put your setup code here, to run once:
  
Serial.begin(115200);
  
pinMode(pump_pin, OUTPUT);
for(int i=0; i<4; i++) {
pinMode(valve_pin[i], OUTPUT);
}




give_shot(0);
}

void loop() {



delay(3000);

}
