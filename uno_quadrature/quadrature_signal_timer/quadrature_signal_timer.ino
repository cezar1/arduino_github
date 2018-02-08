#define ledPin    13
#define ch_A_Pin  14//A0
#define ch_AN_Pin 15//A1
#define ch_B_Pin  16//A2
#define ch_BN_Pin 17//A3

bool setup_done=false;
int quad_state=0;

void prepare_counter(int value)
{
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = value;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}
void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(ch_A_Pin, OUTPUT);
  pinMode(ch_AN_Pin, OUTPUT);
  pinMode(ch_B_Pin, OUTPUT);
  pinMode(ch_BN_Pin, OUTPUT);

  digitalWrite(ch_A_Pin, 0);
  digitalWrite(ch_AN_Pin, 0);
  digitalWrite(ch_B_Pin, 0);
  digitalWrite(ch_BN_Pin, 0);
  Serial.begin(9600);
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  digitalWrite(ledPin, digitalRead(ledPin) ^ 1);   // toggle LED pin

  quad_state++;
  quad_state%=4;
      
  if (quad_state==0)
  {
    digitalWrite(ch_A_Pin, 1);
    digitalWrite(ch_AN_Pin, 0);
  }
  else if (quad_state==1)
  {
    digitalWrite(ch_B_Pin, 1);
    digitalWrite(ch_BN_Pin, 0);
  }
  else if (quad_state==2)
  {
    digitalWrite(ch_A_Pin, 0);
    digitalWrite(ch_AN_Pin, 1);
  }
  else if (quad_state==3)
  {
    digitalWrite(ch_B_Pin, 0);
    digitalWrite(ch_BN_Pin, 1);
  }

}

void loop()
{
  if (!setup_done)
  {
    setup_done=true;
    prepare_counter(31250);
  }
   while (Serial.available() > 0) {
    int req_freq = Serial.parseInt();
    
    if (Serial.read() == '\n') {
      prepare_counter(req_freq);
    }
  }
}
