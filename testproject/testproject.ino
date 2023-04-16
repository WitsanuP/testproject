
unsigned long ms_from_start = 0;
unsigned long ms_previous_read_LEDR  = 0;
unsigned long LEDR_interval = 50;
unsigned long ms_previous_read_LEDY  = 0;
unsigned long LEDY_interval = 150;
unsigned long ms_previous_read_LEDG  = 0;
unsigned long LEDG_interval = 1000;

#define LEDR 4
#define LEDY 5
#define LEDG 6

#define button1  2
#define button2  3

int buttonState1 = 0;
int buttonState2 = 0;

int LEDY_state = 0;
int LEDG_state = 0;
int LEDR_state = 0;

void setup() {
  pinMode(LEDR, OUTPUT);
  pinMode(LEDY, OUTPUT);
  pinMode(LEDG, OUTPUT);
  
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

  digitalWrite(LEDG, HIGH);
}
void loop() {
  ms_from_start  = millis();

  if (ms_from_start - ms_previous_read_LEDY > LEDY_interval) {

    buttonState1 = digitalRead(button1);
    if (buttonState1 == HIGH) {
      digitalWrite(LEDY, LOW);
    }
    else {
      //***************
      ms_previous_read_LEDY = ms_from_start;
      if (LEDY_state == 0) LEDY_state = 1;  else LEDY_state = 0;
      digitalWrite(LEDY, LEDY_state);
      //***************
    }
  }

  if (ms_from_start - ms_previous_read_LEDG >  LEDG_interval) {
    ms_previous_read_LEDG = ms_from_start;
    if (LEDG_state == 0)  LEDG_state = 1; else LEDG_state = 0;
    digitalWrite(LEDG, LEDG_state);
  }

  if (ms_from_start - ms_previous_read_LEDR >  LEDR_interval) {

    buttonState2 = digitalRead(button2);
    if (buttonState2 == HIGH) {
      digitalWrite(LEDR, LOW);
    }
    else {
      digitalWrite(LEDR, HIGH);
    }
  }
}
