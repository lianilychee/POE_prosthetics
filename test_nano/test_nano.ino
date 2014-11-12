// Assign pins to components.
const float pot = A0;
float potVal = 0;

// Assign pin types.
void setup() {
  pinMode(pot, INPUT);
  Serial.begin(9600);
}

// Actual loop.
void loop() {
  potVal = analogRead(pot);
  Serial.println(potVal);
}
