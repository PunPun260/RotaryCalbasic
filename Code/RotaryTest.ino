#define knoba 6
#define knobb 7
#define knobsw 8

int8_t knobpreva;

void setup() {
  Serial.begin(9600);

  pinMode(knoba, INPUT);
  pinMode(knobb, INPUT);
  pinMode(knobsw, INPUT);

  knobpreva = digitalRead(knoba);
}

void loop() {
  int8_t knobcura = digitalRead(knoba), pos=0; 
  if (knobpreva < knobcura) {
    if (digitalRead(knobpreva) != knobcura) pos++; // CW
    else pos--; // CCW
  }
  knobpreva = cura
}
