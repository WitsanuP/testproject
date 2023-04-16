
unsigned long ms_from_start = 0;
unsigned long ms_previous_read_LEDR  = 0;
unsigned long LEDR_interval = 50;
unsigned long ms_previous_read_LEDY  = 0;
unsigned long LEDY_interval = 150;
unsigned long ms_previous_read_LEDG  = 0;
unsigned long LEDG_interval = 1000;
unsigned long ms_previous_read_Dot  = 0;

#define CS 0x0A
#define DIN 0x0B
#define CLK 0x0D
#define LEDR 4
#define LEDY 5
#define LEDG 6
#define BUTTON1  2
#define BUTTON2  3

int buttonState1 = 0;
int buttonState2 = 0;
int LEDY_state = 0;
int LEDG_state = 0;
int LEDR_state = 0;
int Dot_state = 0;

void SendPackets(byte cmd, byte data);
void SPI_send(byte data);

void setup() {

  pinMode(DIN, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(CLK, OUTPUT);
  SendPackets(0x09, 0);         //decoding mode OFF
  SendPackets(0x0A, 0);         //light intensity (0-15)
  SendPackets(0x0B, 7);         //scan limit: all digits ON
  SendPackets(0x0C, 1);         //turn ON MAX7219
  for (byte i = 0; i < 8; i++) SendPackets(i + 1, 0xff);

  pinMode(LEDR, OUTPUT);
  pinMode(LEDY, OUTPUT);
  pinMode(LEDG, OUTPUT);

  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);

  digitalWrite(LEDG, HIGH);
}
void loop() {

  ms_from_start  = millis();
  //-----------------------------------------------------------------------------
  if (ms_from_start - ms_previous_read_LEDY > LEDY_interval) {

    buttonState1 = digitalRead(BUTTON1);
    if (buttonState1 == HIGH) {
      digitalWrite(LEDY, LOW);
    }
    else {

      ms_previous_read_LEDY = ms_from_start;
      if (LEDY_state == 0) LEDY_state = 1;  else LEDY_state = 0;
      digitalWrite(LEDY, LEDY_state);

    }
  }
  //-----------------------------------------------------------------------------
  if (ms_from_start - ms_previous_read_LEDG >  LEDG_interval) {
    ms_previous_read_LEDG = ms_from_start;
    if (LEDG_state == 0)  LEDG_state = 1; else LEDG_state = 0;
    digitalWrite(LEDG, LEDG_state);
  }
  //-----------------------------------------------------------------------------
  if (ms_from_start - ms_previous_read_LEDR >  LEDR_interval) {

    buttonState2 = digitalRead(BUTTON2);
    if (buttonState2 == HIGH) {
      digitalWrite(LEDR, LOW);
    }
    else {
      digitalWrite(LEDR, HIGH);
    }
  }
  //-----------------------------------------------------------------------------
  if (ms_from_start - ms_previous_read_Dot >  1000) {
    ms_previous_read_Dot = ms_from_start;
    if (Dot_state == 0) {
      Dot_state = 1; 
      for (byte i = 0; i < 8; i++) SendPackets(i + 1, 0x0f);
    }
    else if(Dot_state == 1) {
      Dot_state = 2;
      for (byte i = 0; i < 4; i++) SendPackets(i + 1, ~0xff);
      for (byte i = 4; i < 8; i++) SendPackets(i + 1, 0xff);
    }
    else if(Dot_state == 2) {
      Dot_state = 3;
      for (byte i = 0; i < 8; i++) SendPackets(i + 1, 0xf0);
    }
    else {
      Dot_state = 0;
      for (byte i = 0; i < 4; i++) SendPackets(i + 1, 0xff);
      for (byte i = 4; i < 8; i++) SendPackets(i + 1, ~0xff);
    }
    
  }
  //-----------------------------------------------------------------------------
}
void SendPackets(byte cmd, byte data)
{
  digitalWrite(CS, LOW);   //CS low pulse to enable MAX7219

  SPI_send(cmd);       //send command byte to MAX7219
  SPI_send(data);      //send data byte to MAX7219

  digitalWrite(CS, HIGH);  //CS high pulse to disable MAX7219
}
void SPI_send(byte data)
{
  for (byte i = 0; i < 8; i++)
  {
    digitalWrite(CLK, LOW);                   //CLK low pulse

    if (data & 0x80) digitalWrite(DIN, HIGH); //o/p high if MSB = 1
    else digitalWrite(DIN, LOW);              //o/p low if MSB = 0

    digitalWrite(CLK, HIGH);                  //CLK high pulse
    data = data << 1;                         //shift data left one bit
  }
}
