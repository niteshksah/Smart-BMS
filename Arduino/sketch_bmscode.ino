#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define CELL1_NODE A0
#define CELL2_NODE A1
#define PACK_NODE  A2



#define CURRENT_PIN A3
#define TEMP_PIN    A4


#define RELAY_PIN 7

#define SOC_OUTPUT_PIN 6



#define BAL1_PIN 24
#define BAL2_PIN 23
#define BAL3_PIN 22



const float DIVIDER_RATIO = 11.0;

const float ADC_REF = 5.0;

const float ADC_MAX = 1023.0;


const float ACS_OFFSET = 2.50;

const float ACS_SENSITIVITY = 0.066;


const float RATED_CAPACITY_AH = 2.0;


const float AVAILABLE_CAPACITY_AH = 1.90;



const float LOW_LIMIT = 3.70;


const float BALANCE_DIFF = 0.05;


float soc = 100.0;


unsigned long previousMillis = 0;


float readNodeVoltage(int pin)
{
  int adcValue = analogRead(pin);

  float dividerVoltage =
    (adcValue * ADC_REF) / ADC_MAX;

  float actualVoltage =
    dividerVoltage * DIVIDER_RATIO;

  return actualVoltage;
}


float readCurrent()
{
  int adcValue = analogRead(CURRENT_PIN);

  float sensorVoltage =
    (adcValue * ADC_REF) / ADC_MAX;

  float current =
    (sensorVoltage - ACS_OFFSET)
    / ACS_SENSITIVITY;


  if (current > -0.05 && current < 0.05)
  {
    current = 0.0;
  }

  return current;
}



float readTemperature()
{
  int adcValue = analogRead(TEMP_PIN);

  float voltage =
    (adcValue * ADC_REF) / ADC_MAX;

  // LM35 = 10 mV / °C
  float temperature =
    voltage * 100.0;

  return temperature;
}


void setup()
{
  lcd.begin(20, 4);

  
  pinMode(SOC_OUTPUT_PIN, OUTPUT);

  
  pinMode(RELAY_PIN, OUTPUT);

 
  pinMode(BAL1_PIN, OUTPUT);
  pinMode(BAL2_PIN, OUTPUT);
  pinMode(BAL3_PIN, OUTPUT);


  
  digitalWrite(RELAY_PIN, HIGH);

  digitalWrite(BAL1_PIN, LOW);
  digitalWrite(BAL2_PIN, LOW);
  digitalWrite(BAL3_PIN, LOW);

  
  soc = 100.0;

  
  analogWrite(SOC_OUTPUT_PIN, 255);


 
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("3 CELL BMS");

  lcd.setCursor(0, 1);
  lcd.print("STATUS: LOADING...");

  delay(2000);

  lcd.clear();


  previousMillis = millis();
}


void loop()
{
  
  float node1 = readNodeVoltage(CELL1_NODE);

  float node2 = readNodeVoltage(CELL2_NODE);

  float packVoltage = readNodeVoltage(PACK_NODE);


  float cell1 = node1;

  float cell2 = node2 - node1;

  float cell3 = packVoltage - node2;



  float lowestCell = cell1;

  if (cell2 < lowestCell)
  {
    lowestCell = cell2;
  }

  if (cell3 < lowestCell)
  {
    lowestCell = cell3;
  }


  bool lowCell =
    (cell1 <= LOW_LIMIT) ||
    (cell2 <= LOW_LIMIT) ||
    (cell3 <= LOW_LIMIT);


  
  if (lowCell)
  {
    
    digitalWrite(RELAY_PIN, LOW);
  }
  else
  {
    // Battery okay
    digitalWrite(RELAY_PIN, HIGH);
  }


  // ===================================================
  // 6. READ CURRENT
  // ===================================================

  float current = readCurrent();


  // ===================================================
  // 7. COULOMB COUNTING
  // ===================================================

  unsigned long currentMillis = millis();

  float deltaTime =
    (currentMillis - previousMillis) / 3600000.0;

  previousMillis = currentMillis;


  // Positive current = discharge
  // Negative current = charging

  float deltaSOC =
    (current * deltaTime /
     RATED_CAPACITY_AH) * 100.0;


  // Subtracting positive current decreases SOC.
  // Subtracting negative current increases SOC.

  soc = soc - deltaSOC;


  // Limit SOC between 0 and 100 %

  if (soc > 100.0)
  {
    soc = 100.0;
  }

  if (soc < 0.0)
  {
    soc = 0.0;
  }


  // ===================================================
  // 7A. SOC PWM OUTPUT FOR PROTEUS GRAPH
  // ===================================================

  int socPWM =
    (int)(soc * 255.0 / 100.0);

  socPWM =
    constrain(socPWM, 0, 255);

  analogWrite(SOC_OUTPUT_PIN, socPWM);


  // ===================================================
  // 8. TEMPERATURE
  // ===================================================

  float temperature = readTemperature();


  // ===================================================
  // 9. SOH
  // ===================================================

  float soh =
    (AVAILABLE_CAPACITY_AH /
     RATED_CAPACITY_AH) * 100.0;


  bool balanceCell1 = false;

  bool balanceCell2 = false;

  bool balanceCell3 = false;


  if (lowCell)
  {
    if (cell1 > (lowestCell + BALANCE_DIFF))
    {
      balanceCell1 = true;
    }

    if (cell2 > (lowestCell + BALANCE_DIFF))
    {
      balanceCell2 = true;
    }

    if (cell3 > (lowestCell + BALANCE_DIFF))
    {
      balanceCell3 = true;
    }
  }



  digitalWrite(BAL1_PIN, balanceCell1);

  digitalWrite(BAL2_PIN, balanceCell2);

  digitalWrite(BAL3_PIN, balanceCell3);



  lcd.clear();

  lcd.setCursor(0, 0);

  lcd.print("C1:");
  lcd.print(cell1, 2);
  lcd.print("V ");

  lcd.print("C2:");
  lcd.print(cell2, 2);


  lcd.setCursor(0, 1);

  lcd.print("C3:");
  lcd.print(cell3, 2);
  lcd.print("V ");

  lcd.print("P:");
  lcd.print(packVoltage, 1);


  lcd.setCursor(0, 2);

  lcd.print("I:");
  lcd.print(current, 2);
  lcd.print("A ");

  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print("C");


  lcd.setCursor(0, 3);


  if (!lowCell)
  {
    lcd.print("LOAD:ON BAL:OFF");
  }
  else
  {
    if (balanceCell1 &&
        balanceCell2 &&
        balanceCell3)
    {
      lcd.print("BAL:C1 C2 C3");
    }

    else if (balanceCell1 &&
             balanceCell2)
    {
      lcd.print("BAL:C1 C2");
    }

    else if (balanceCell1 &&
             balanceCell3)
    {
      lcd.print("BAL:C1 C3");
    }

    else if (balanceCell2 &&
             balanceCell3)
    {
      lcd.print("BAL:C2 C3");
    }

    else if (balanceCell1)
    {
      lcd.print("BAL:C1");
    }

    else if (balanceCell2)
    {
      lcd.print("BAL:C2");
    }

    else if (balanceCell3)
    {
      lcd.print("BAL:C3");
    }

    else
    {
      lcd.print("BAL:NONE");
    }
  }


  delay(1500);


  lcd.clear();

  lcd.setCursor(0, 0);

  lcd.print("SOC : ");
  lcd.print(soc, 1);
  lcd.print("%");


  lcd.setCursor(0, 1);

  lcd.print("SOH : ");
  lcd.print(soh, 1);
  lcd.print("%");


  lcd.setCursor(0, 2);

  lcd.print("TEMP: ");
  lcd.print(temperature, 1);
  lcd.print(" C");


  lcd.setCursor(0, 3);


  

  if (temperature >= 45.0)
  {
    lcd.print("TEMP HIGH");
  }

  else if (lowCell)
  {
    lcd.print("LOAD OFF");
  }

  else
  {
    lcd.print("SYSTEM NORMAL");
  }


  delay(2000);
}