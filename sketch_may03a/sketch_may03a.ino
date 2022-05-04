#define p1 17
#define p2 2

void setup() {
  pinMode(p1, INPUT);
  pinMode(p2, INPUT);
  Serial.begin(115200);
}

void loop() {
 Serial.print(digitalRead(p1));
 Serial.print(",");
  Serial.print(digitalRead(p2));
  Serial.println();
  delay(1000);
}
