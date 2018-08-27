#include <LiquidCrystal.h>

int numSamples = 0;
long t, t0;
int x = 0;
int raja1 = 159;
int raja2 = 159;
int portti = 1;
float speeD;
float joules;
float lasku;
unsigned long valiaika;
unsigned long valiaika2;
unsigned long aika;
float aikaero;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


void setup()
{
  Serial.begin(2000000);
  Serial.println(analogRead(0) / 4);
  raja1 = analogRead(0) / 4 - 4;
  Serial.println(analogRead(1) / 4);
  raja2 = analogRead(1) / 4 - 4;

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
  ADCSRA |= (1 << ADPS2);                     // 16 prescaler for 76.9 KHz
  //ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    // 8 prescaler for 153.8 KHz

  ADCSRA |= (1 << ADATE); // enable auto trigger
  ADCSRA |= (1 << ADIE);  // enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN);  // enable ADC
  ADCSRA |= (1 << ADSC);  // start ADC measurements
}

ISR(ADC_vect)
{
  x = ADCH;  // read 8 bit value from ADC
  aika = micros();

  if (x < 140) {
    Serial.print ("hah ");
    Serial.println (x);
    //ohitetaan yksi tulos
    if (portti > 2) {
      portti = portti - 1;
    }
    //jos jotain meni ohi ekasta
    if (x < raja1 && portti == 1) {
      valiaika = aika;
      //vaihetaan toiseen porttiin
      ADMUX = (1 << ADLAR) | (1 << REFS0) | 1; //Select ADC Channel
      ADCSRA |= (1 << ADSC);    // Start A2D Conversions
      //alotetaan valiaika laskuri
      Serial.println ("JEE");
      portti = 3;
    }
  
    // jos jotain menee toisesta
    if (x < raja2 && portti == 2) {
      valiaika2 = aika;
      aikaero = valiaika2 - valiaika;
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
      ADCSRA |= (1 << ADPS2);                     // 16 prescaler for 76.9 KHz
      //ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    // 8 prescaler for 153.8 KHz

      ADCSRA |= (1 << ADATE); // enable auto trigger
      ADCSRA |= (1 << ADIE);  // enable interrupts when measurement complete
      ADCSRA |= (1 << ADEN);  // enable ADC
      ADCSRA |= (1 << ADSC);    // Start A2D Conversions
      //
    }

  }

  numSamples++;
}

void loop()
{
  if (numSamples >= 1000)
  {
    t = micros() - t0; // calculate elapsed time

    Serial.print("Sampling frequency: ");
    Serial.print((float)1000000 / t);
    Serial.println(" KHz");
    Serial.println(x);
    Serial.println(micros());

    delay(2000);

    // restart
    t0 = micros();
    numSamples = 0;

        //jos mitään ei mennyt ohi
    if (aika - valiaika > 500000 && portti > 1) {
      portti = 1;
      ADMUX = (1 << ADLAR) | (1 << REFS0) | 0; //Select ADC Channel
      ADCSRA |= (1 << ADSC);    // Start A2D Conversions
      Serial.println("miss");
      Serial.println(aika);
      lcd.setCursor(0, 1);
      lcd.print("miss");
    }

    // ADMUX = (1<<ADLAR) | (1<<REFS0) | 1; //Select ADC Channel
    // ADCSRA |= (1 << ADSC);    // Start A2D Conversions
  }
}
