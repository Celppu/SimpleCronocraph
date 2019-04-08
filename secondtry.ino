/*
Made by Celppu

Do whatever you want

Should act as chronograph, but my coding skills are bad

*/
#include <LiquidCrystal.h>

uint8_t numSamples = 0;
long t, t0;
uint8_t x = 0;
uint8_t  raja1 = 159;
uint8_t  raja2 = 159;
uint8_t  minus = 3;
uint8_t currentlimit = raja1;
uint8_t jus =1;
int avg;
int portti = 1;
float speeD;
float joules;
float lasku;
unsigned long firsttime;
unsigned long secondtime;
unsigned long aika;
float aikaero;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


void setup()
{
  Serial.begin(2000000);
  Serial.println(analogRead(0) / 4);
  raja1 = analogRead(0) / 4 - minus;
  Serial.println(analogRead(1) / 4);
  raja2 = analogRead(1) / 4 - minus;

  currentlimit = raja1;
  lcd.begin(16, 2);

      lcd.setCursor(0, 0);
      lcd.print(":D");

  ADCSRA = 0;             // clear ADCSRA register
  ADCSRB = 0;             // clear ADCSRB register
  ADMUX = (1 << ADLAR) | (1 << REFS0) | 0; //Select ADC Channel
  // ADMUX |= (0 & 0x07);    // set A0 analog input pin
  // ADMUX |= (1 << REFS0);  // set reference voltage
  // ADMUX |= (1 << ADLAR);  // left align ADC value to 8 bits from ADCH register

  // sampling rate is [ADC clock] / [prescaler] / [conversion clock cycles]
  // for Arduino Uno ADC clock is 16 MHz and a conversion takes 13 clock cycles
  //ADCSRA |= (1 << ADPS2) | (1 << ADPS0);    // 32 prescaler for 38.5 KHz
  //ADCSRA |= (1 << ADPS2);                     // 16 prescaler for 76.9 KHz
  ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    // 8 prescaler for 153.8 KHz

  ADCSRA |= (1 << ADATE); // enable auto trigger
  ADCSRA |= (1 << ADIE);  // enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN);  // enable ADC
  ADCSRA |= (1 << ADSC);  // start ADC measurements
}

ISR(ADC_vect)
{
  x = ADCH;  // read 8 bit value from ADC
  //aika = micros();
/*
  if(jus <=100){
    avg += x;
    jus++;
  }else{
    raja1 = avg/100 - 50;
    if(portti = 1){
      currentlimit = raja1;
    }
    jus = 0;
  }
  */
  if (x < currentlimit) {
    aika = micros();
    Serial.print ("hah ");
    Serial.println (x);
    Serial.println (currentlimit);
    //ohitetaan yksi tulos
    if (portti > 2) {
      portti = portti - 1;
    }

    // If something goes past second port
    if (x < raja2 && portti == 2) {
      secondtime = aika;
      aikaero = secondtime - firsttime;
      portti = 1;
      Serial.print("tulos ");
      Serial.println(aikaero);
      lasku = aikaero / 1000000;
      Serial.println(lasku);
      Serial.print("metereitä sekuneisa ");
      Serial.println(0.3 / lasku);
  
      //Sama lcd näytölle
      lcd.setCursor(0, 0);
      lcd.print("speed ");
      lcd.setCursor(7, 0);
      lcd.print(0.3 / lasku);
      lcd.print(" m/s");

      speeD = 0.3 / lasku;
      joules = speeD * speeD;
      joules = joules * 0.00025;
      joules = joules / 2;
      lcd.setCursor(0, 1);
      lcd.print("0.25g ");
      lcd.print(joules);
      //vaihetaan toiseen porttiin
      ADCSRA = 0;             // clear ADCSRA register
      ADCSRB = 0;             // clear ADCSRB register
      ADMUX = (1 << ADLAR) | (1 << REFS0) | 0; //Select ADC Channel
      //ADCSRA |= (1 << ADPS2);                     // 16 prescaler for 76.9 KHz
      ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    // 8 prescaler for 153.8 KHz

      ADCSRA |= (1 << ADATE); // enable auto trigger
      ADCSRA |= (1 << ADIE);  // enable interrupts when measurement complete
      ADCSRA |= (1 << ADEN);  // enable ADC
      ADCSRA |= (1 << ADSC);    // Start A2D Conversions

      currentlimit = raja1;
      //
    }
    
    //if goes past first port
    if (x < raja1 && portti == 1) {
      firsttime = aika;
      //changing registers to watch second port
      /*
      ADMUX = (1 << ADLAR) | (1 << REFS0) | 1; //Select ADC Channel
      ADCSRA |= (1 << ADSC);    // Start A2D Conversions
      //alotetaan firsttime laskuri
      //Serial.println ("JEE");
*/
      ADCSRA = 0;             // clear ADCSRA register
      ADCSRB = 0;             // clear ADCSRB register
      ADMUX = (1 << ADLAR) | (1 << REFS0) | 1; //Select ADC Channel
      //ADCSRA |= (1 << ADPS2);                     // 16 prescaler for 76.9 KHz
      ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    // 8 prescaler for 153.8 KHz

      ADCSRA |= (1 << ADATE); // enable auto trigger
      ADCSRA |= (1 << ADIE);  // enable interrupts when measurement complete
      ADCSRA |= (1 << ADEN);  // enable ADC
      ADCSRA |= (1 << ADSC);  // Start A2D Conversions

      currentlimit = raja2;
      portti = 2; // for some reason i found this as good way to skip wrong results
    }
  }
  numSamples++;
}

void loop()
{
  if (numSamples >= 200)
  {
    //uint8_t samp = numSamples;
    t = micros() - t0; // calculate elapsed time
    
    //Serial.print("Sampling frequency: ");
    //Serial.print((float)1000000 / t);
    Serial.print(numSamples/((float)t/1000000));
    Serial.println(" KHz");
    //Serial.println(x);
    //Serial.println(t);
    //Serial.println(micros());

    //delay(2000);
    // restart
    t0 = micros();
    numSamples = 0;


        //If nothing got through second port
    if (micros() - firsttime > 500000 && portti > 1) {
      portti = 1;
      //ADMUX = (1 << ADLAR) | (1 << REFS0) | 0; //Select ADC Channel
      //ADCSRA |= (1 << ADSC);    // Start A2D Conversions
      currentlimit = raja1;
      Serial.println("miss");
      Serial.println(aika);
      lcd.setCursor(0, 1);
      lcd.print("miss");
      ADCSRA = 0;             // clear ADCSRA register
      ADCSRB = 0;             // clear ADCSRB register
      ADMUX = (1 << ADLAR) | (1 << REFS0) | 0; //Select ADC Channel
      //ADCSRA |= (1 << ADPS2);                     // 16 prescaler for 76.9 KHz
      ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    // 8 prescaler for 153.8 KHz

      ADCSRA |= (1 << ADATE); // enable auto trigger
      ADCSRA |= (1 << ADIE);  // enable interrupts when measurement complete
      ADCSRA |= (1 << ADEN);  // enable ADC
      ADCSRA |= (1 << ADSC);    // Start A2D Conversions


      raja2=x - minus;
      currentlimit = raja1 -minus;
    }

    // ADMUX = (1<<ADLAR) | (1<<REFS0) | 1; //Select ADC Channel
    // ADCSRA |= (1 << ADSC);    // Start A2D Conversions
    if(portti == 1){
      raja1 = x - minus;
      currentlimit = x - minus;
    }
  }
}
