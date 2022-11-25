/* I2C LCD with Arduino example code. More info: https://www.makerguides.com */

// Include the libraries:
// LiquidCrystal_I2C.h: https://github.com/johnrickman/LiquidCrystal_I2C
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD

#define C 850000
#define pulsador 7
#define sensorPin A0
#define multichip670 10
#define multichip770 4
#define multichip810 6
#define multichip850 5
#define multichip950 3

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered)
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,20,4) for 20x4 LCD.

// double sensorPin = A0;
// double multichip810 = 10;
double AC_Amplitud = 0;
double DC_Signal = 0; //Valor Minimo del array
double eHb = 71.708; // En gramos de Hemoglobina por Decilitro
double eHbO2 = 86.4; // En gramos de Hemoglobina por Decilitro
int L = 1; // en centímetros
double MaxVal = 0;
double MinVal = 9999999;
double funcion[200];
double PPG_Ratio = 0;
double hb = 0;

void cfg_multichip(bool a1, bool a2, bool a3, bool a4, bool a5){
  if (a1){
    pinMode(multichip670, OUTPUT);
  }

  if (a2){
     pinMode(multichip770, OUTPUT);
  }

  if (a3){
     pinMode(multichip810, OUTPUT);
  }

  if (a4){
    pinMode(multichip850, OUTPUT);
  }

  if (a5){
    pinMode(multichip950, OUTPUT);
  }

}

void set_multichip(bool a1, bool a2, bool a3, bool a4, bool a5){
  if (a1){
    digitalWrite(multichip670, LOW);
  }
  else
  {
    digitalWrite(multichip670, HIGH);
  }

  if (a2){
     digitalWrite(multichip770, LOW);
  }
  else
  {
    digitalWrite(multichip770, HIGH);
  }

  if (a3){
     digitalWrite(multichip810, LOW);
  }
  else
  {
    digitalWrite(multichip810, HIGH);
  }

  if (a4){
    digitalWrite(multichip850, LOW);
  }
  else
  {
    digitalWrite(multichip850, HIGH);
  }

  if (a5){
    digitalWrite(multichip950, LOW);
  }
  else
  {
    digitalWrite(multichip950, HIGH);
  }
}

void reset_multichip(bool a1, bool a2, bool a3, bool a4, bool a5){
  if (a1){
    digitalWrite(multichip670, HIGH);
  }
  else
  {
    digitalWrite(multichip670, LOW);
  }

  if (a2){
     digitalWrite(multichip770, HIGH);
  }
  else
  {
    digitalWrite(multichip770, LOW);
  }

  if (a3){
     digitalWrite(multichip810, HIGH);
  }
  else
  {
    digitalWrite(multichip810, LOW);
  }

  if (a4){
    digitalWrite(multichip850, HIGH);
  }
  else
  {
    digitalWrite(multichip850, LOW);
  }

  if (a5){
    digitalWrite(multichip950, HIGH);
  }
  else
  {
    digitalWrite(multichip950, LOW);
  }
}


void setup() {
  delay(500);
  Serial.begin(115200);
  Serial.println("Init");
  // Initiate the LCD:
  lcd.init();
  pinMode(pulsador, INPUT);
  pinMode(sensorPin, INPUT);
  cfg_multichip(false,false,true,true,false);
}

void loop() {
  lcd.clear();
  if(digitalRead(pulsador)==HIGH){
    MaxVal = 0;
    MinVal = 9999999;
    delay(1000);
    lcd.backlight();
    set_multichip(false,false,true,true,false);
    delay(1000);
    for (int i = 0; i < 200; ++i){
      funcion[i] = analogRead(sensorPin);
      if (funcion[i] <= MinVal){
        MinVal = funcion[i];
      }
      if (funcion[i] >= MaxVal){
        MaxVal = funcion[i];
      }
    }
    reset_multichip(false,false,true,true,false);

    //Cálculos//
    AC_Amplitud = MaxVal - MinVal;
    DC_Signal = MinVal;
    PPG_Ratio = AC_Amplitud/DC_Signal;
    hb = (PPG_Ratio)/((eHb + eHbO2)*L);

    Serial.print("AC_Amplitud = ");
    Serial.println(AC_Amplitud);
    
    Serial.print("DC_Signal = ");
    Serial.println(DC_Signal);

    Serial.print("PPG_Ratio = ");
    Serial.print((int)PPG_Ratio);
    Serial.print(".00");
    Serial.println((int)(PPG_Ratio*100000));
    

    Serial.print("MaxVal = ");
    Serial.println(MaxVal);
    Serial.print("MinVal = ");
    Serial.println(MinVal);

    delay (1000);
    lcd.print ("...Evaluando...");
    delay (2000);
    lcd.clear();
    lcd.print(C*hb);

    Serial.print((int)(hb*1000000));
    Serial.print(".");
    Serial.print((int)(hb*1000000));
    

    delay (2000);
    lcd.clear();

   //El rango es el estándar de concentración de sangre para que se considere que el paciente esté sano
    if (C*hb >= 11.5 and C*hb <= 15.5){
      lcd.print ("Sin anemia");
    }
    else{
      if (C*hb >= 11 and C*hb <= 11.4){
        lcd.print ("Anemia leve");
      }
      else{
        if (C*hb >= 8 and C*hb <= 10.9){
        lcd.print ("Anemia moderada");
        }
        else{
          if (C*hb >= 0 and C*hb < 8){
            lcd.print ("Anemia severa");
          }
          else{
            lcd.print("Medicion incorrecta, repetir la medicion");
          }
        }
      }
    }
    delay (3000);
    lcd.clear();
    delay(1000);
    lcd.noBacklight();
  }
}
