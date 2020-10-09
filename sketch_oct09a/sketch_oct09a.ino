#include <avr/wdt.h>

#define INTERVAL    10 // 600s = 10 min

void pump();

long int iteration = 0;
const int sensor = 2;
const int RELAY1 = 4;

void setup(){
  
  Serial.begin(9600);

  pinMode(sensor, INPUT_PULLUP); // ACHTUNG ... reversed voltage logic on sensor ... connect it to GND to logic true
  pinMode(RELAY1, OUTPUT);
  Serial.println("RESET");
  delay(2000); // Delay for starting
  wdt_enable(WDTO_8S); // Watchdog for 8s enable

  cli();//stop interrupts
  
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  if(digitalRead(sensor) == 1)
  {
    pump();
  }
  sei();//allow interrupts
  }

ISR(TIMER1_COMPA_vect){
  iteration++;
  wdt_reset();
  Serial.print("NEXT pump sequence in: ");
  Serial.print(INTERVAL - iteration);
  Serial.print(" s");
  Serial.print("\n");
  if (iteration >= INTERVAL)
  {
    cli();//stop interrupt
    iteration = 0;
    toggle();
    sei();//allow interrupts
    }
  }

void pump()
{
  digitalWrite(RELAY1, true);
  while(!digitalRead(sensor)){
    Serial.println(digitalRead(sensor));
    wdt_reset();
    Serial.println("PUMPING! ... waiting for water level to drop ...");
    }
  digitalWrite(RELAY1,false);
  Serial.println("PUMP STOP! ... time delay enabled ...");
}

void toggle()
{
  while(digitalRead(sensor))
  {
    wdt_reset();
    Serial.println("PUMP READY ... waiting for water level ...");
  }
  Serial.print("Water level DETECTED .... starting to PUMP ...");
  pump();
  }


void loop(){}
