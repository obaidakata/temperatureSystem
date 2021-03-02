#include <math.h>

//Symbols stuff
class Digit// class of number of matrix 
{
  private:
    byte ledArr[5][3];
  public:
    Digit(byte A[5][3]);
    byte getVal(int row, int col) {
      return ledArr[col][row];
    }
};

Digit::Digit(byte A[5][3])// copy past the array led
{
  for (int i = 0 ; i < 5 ; i++)
  {
    for (int j = 0 ; j < 3 ; j++)
    {
      ledArr[i][j] = A[i][j];
    }
  }
}
Digit* digits;// the number print
void initializeDigits()
{
  byte digit0[5][3] = 
   {{1, 1, 1},
    {1, 0, 1},
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 1}
  };

  byte digit1[5][3] = {{0, 0, 1},
                       {0, 0, 1},
                       {0, 0, 1},
                       {0, 0, 1},
                       {0, 0, 1}};

  byte  digit2[5][3] = {{1, 1, 1},
                        {0, 0, 1},
                        {1, 1, 1},
                        {1, 0, 0},
                        {1, 1, 1}};

  byte digit3[5][3] = {{1, 1, 1},
                       {0, 0, 1},
                       {1, 1, 1},
                       {0, 0, 1},
                       {1, 1, 1}};

  byte digit4[5][3] = 
   {{1, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 0, 1},
    {0, 0, 1}
  };

  byte digit5[5][3] = 
   {{1, 1, 1},
    {1, 0, 0},
    {1, 1, 1},
    {0, 0, 1},
    {1, 1, 1}
  };

  byte digit6[5][3] = 
   {{1, 1, 1},
    {1, 0, 0},
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1}
  };

  byte digit7[5][3] = 
   {{1, 1, 1},
    {0, 0, 1},
    {1, 1, 1},
    {0, 0, 1},
    {0, 0, 1}
  };

  byte digit8[5][3] = 
   {{1, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1}
  };

  byte digit9[5][3] = 
   {{1, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 0, 1},
    {1, 1, 1}};
  digits = new Digit[10] {digit0, digit1, digit2, digit3, digit4,
                          digit5, digit6, digit7, digit8, digit9
                         };
}
//------------------------------------------------------------
//led matrix stuff

class ledmatrix
{
  private:
#define matrixSize 8
    byte matrix[matrixSize][matrixSize];
    const int DATA = 3;
    const int CLK = 2;
    const int CS = 4;
    byte* currLeftDigit[5][3], *currRightDigit[5][3];
    byte* fractionsLine[matrixSize];
    byte* TemperatureToBigLine[matrixSize];
    byte cLetter[7][5] = {{0,1,1,1,0},
                          {1,0,0,0,1},
                          {1,0,0,0,0},
                          {1,0,0,0,0},
                          {1,0,0,0,0},
                          {1,0,0,0,1},
                          {0,1,1,1,0}};
                          
    byte fLetter[7][5] = {{1,1,1,1,1},
                          {1,0,0,0,0},
                          {1,0,0,0,0},
                          {1,1,1,1,0},
                          {1,0,0,0,0},
                          {1,0,0,0,0},
                          {1,0,0,0,0}};
  private:
    void latchBuf();// Latch the entire buffer
    void writeBit(bool b);
    void updateDigit(byte* digit[5][3] , int num);
    void decoder3x8(byte row);
    void printOutPut();
    void turnOff();
    void fractionDegrees(float fraction);
    void printTemperatureSymbol();
  public:
    void initialize();
    void printLetter(char ch);
    void printTemperature(float num);
};

// matrix methouds

void ledmatrix::latchBuf() // Latch the entire buffer
{
  digitalWrite(CS, LOW);
  digitalWrite(CS, HIGH);
}

void ledmatrix::writeBit(bool b) // Write 1 bit to the buffer
{
  digitalWrite(DATA, b);
  digitalWrite(CLK, LOW);
  digitalWrite(CLK, HIGH);
}

void ledmatrix::initialize()  // initial the matrix led
{
  pinMode(DATA, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  
  //  Set registers: decode mode, scan limit, and shutdown (0x900, 0xB07, 0xC01)
  for (int i = 0; i < 4; i++) writeBit(LOW);
  writeBit(HIGH);
  for (int i = 0; i < 2; i++) writeBit(LOW);
  writeBit(HIGH);
  for (int i = 0; i < 8; i++) writeBit(LOW);
  latchBuf();

  for (int i = 0; i < 4; i++) writeBit(LOW);
  writeBit(HIGH);
  writeBit(LOW);
  writeBit(HIGH);
  writeBit(HIGH);
  for (int i = 0; i < 5; i++) writeBit(LOW);
  for (int i = 0; i < 3; i++) writeBit(HIGH);
  latchBuf();

  for (int i = 0; i < 4; i++) writeBit(LOW);
  for (int i = 0; i < 2; i++) writeBit(HIGH);
  for (int i = 0; i < 2; i++) writeBit(LOW);
  for (int i = 0; i < 7; i++) writeBit(LOW);
  writeBit(HIGH);
  latchBuf();
  turnOff();
  
  for (int i = 0 ; i < 5; i++)// save the pointer of matrix
  {
    for (int j = 0; j < 3; j++)
    {
      currLeftDigit[i][j] = &matrix[j][i];
      currRightDigit[i][j] = &matrix[j + 5][i];
    }
  }
    
   for(int i=0;i<matrixSize;i++)// save the pointer of line of 100
   {
    fractionsLine[i] = &matrix[i][7];
    TemperatureToBigLine[i] = &matrix[i][6];
   }
}

void ledmatrix::printTemperature(float num)// print the temperature
{
  if(num >= 100)// num superior to 100
  {
    num-=100;
    for(int i=0;i<matrixSize;i++)
    {
        *TemperatureToBigLine[i] = HIGH;// print the line of 100
    }
  }

  int leftDigit = int(num) / 10;// tens digit
  int rightDigit = int(num) % 10;// unity digit
  float fraction = num - (int)num;// decimal digit
  fractionDegrees(fraction);// decimal functiun
  updateDigit(currLeftDigit, leftDigit);//save to us array the digit
  updateDigit(currRightDigit, rightDigit);// save to us array the digit
  
  printOutPut();
}
void ledmatrix::fractionDegrees(float fraction) // the deimal functiun 
{
  int numLedToTurnON = 0;// how many led need to turn on
  if (fraction == 0)
  {
    numLedToTurnON = 0;
  }
  else if(fraction <= 0.25)
  {
    numLedToTurnON = 2;
  }
  else if(fraction <= 0.5)
  {
    numLedToTurnON = 4;
  }
  else if(fraction <= 0.75)
  {
    numLedToTurnON = 6;
  }
  else
  {
    numLedToTurnON = 8;
  }
  for(int i=0;i < matrixSize ;i++)
  {
    if(i < numLedToTurnON)
      *fractionsLine[i] = HIGH;
    else
      *fractionsLine[i] = LOW;
  }
}
void ledmatrix::updateDigit(byte* digit[5][3] , int num)// change the digit value
{
  for (int i = 0 ; i < 5; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      *digit[i][j] = digits[num].getVal(j, i);
    }
  }
}

void ledmatrix::turnOff()// initial the led matrix empty
{
  for (int i = 0; i < matrixSize; i++)
  {
    for (int j = 0; j < matrixSize; j++)
    {
      matrix[i][j] = LOW;// set the decimal to 0
    }
  }
  printOutPut();
}

void ledmatrix::printOutPut()// print all matrix give the date to us array
{
  for (int i = 1; i <= matrixSize; i++) // 8x8
  {
    for (int j = 0; j < 4 ; j++) writeBit(LOW);

    decoder3x8(i);

    for (int j = 0; j < matrixSize; j++)
      writeBit(matrix[(i - 1)][(matrixSize - 1) - j]); // all column turn off
    latchBuf();
  }
}
void ledmatrix::printLetter(char ch)// print the letter to change the state of temperature
{
  turnOff();
  byte (*arr)[7][5] = (ch == 'c')?&cLetter:&fLetter;
   for(byte i=0;i<7;i++)
      for(byte j=0;j<5;j++)
        matrix[j+3][i+1] = (*arr)[i][j];
        
   matrix[0][0] = HIGH;
   matrix[1][0] = HIGH;
   matrix[0][1] = HIGH;
   matrix[1][1] = HIGH;
  printOutPut();
  delay(500);
  turnOff();
}


void ledmatrix::decoder3x8(byte row) // print the row bit  with number
{
  if (row == 8)
  {
    writeBit(HIGH);
    row = row - 8;
  }
  else
  {
    writeBit(LOW);
  }

  if (row >= 4)
  {
    writeBit(HIGH);
    row = row - 4;
  }
  else
  {
    writeBit(LOW);
  }

  if (row >= 2)
  {
    writeBit(HIGH);
    row = row - 2;
  }
  else
  {
    writeBit(LOW);
  }
  if (row == 1)
  {
    writeBit(HIGH);
  }
  else
  {
    writeBit(LOW);
  }

}
ledmatrix matrix;
//---------------------------------------------------------------

// Temperature stuff
double Celsius, Fahrenheit;
double currDegrees , prevDegrees;
char temperatureState = 'c';
bool pressedButton = false;

const int TemperatureButton = 12;// temperature button
const int sensorPin = A5; // Declaration of the input pin

const int GreenLed = 10;
const int RedLed = 11;
int intensity = 1;

const double sensitivity = 0.01;
int redSens = 0,greenSens = 0;

void switchState()
{
  temperatureState = (temperatureState == 'c')?'f':'c';
  matrix.printLetter(temperatureState);
}

void initializeTemperature()
{
  pinMode(TemperatureButton,  INPUT_PULLUP);// initailise the  Celcuis /Farheineigth button
  pinMode(sensorPin, INPUT); //initials temperatur sensor
  pinMode(GreenLed, OUTPUT);// initialise the red led
  pinMode(RedLed, OUTPUT);// initialise the green led
  updateTemperature();
}

void updateTemperature()// save the celsius and the farenheigth
{
  Celsius =  ThermistorCelsius((int)analogRead(sensorPin));
  Fahrenheit = CelsiusToFahrenheit(Celsius);
  currDegrees = (temperatureState == 'c') ? Celsius :Fahrenheit;
}

double ThermistorCelsius(float RawADC) // class temperatur
{
  double Temp;
  Temp = log(10000.0 * ((1024.0 / RawADC - 1)));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  Temp = Temp - 273.15;            // Convert Kelvin to Celcius
  return Temp;
}

double CelsiusToFahrenheit(double celsius)// return the fareineigth value
{
  return ((celsius * 1.8f) + 32);
}

bool buttonPressed()// if button is pressed
{
  return (digitalRead(TemperatureButton) == LOW);
}
void turnLeds()// turn led green or red and upgrade intensity
{
  double  amount = prevDegrees - currDegrees;
  if(-amount > sensitivity)// if its downgroad the temperature
  {
    Serial.println("Temperature UP");
    if(greenSens <235)
       greenSens +=20;
    redSens = 0;
  }
  else if(amount > sensitivity)// if it upgrade the temperature
  {
    Serial.println("Temperature DOWN");
     if(redSens <235)
       redSens +=20;
    
    greenSens = 0;
  }
  else// its for the debug comments
  {
    Serial.println("Temperature no change");
  }
  analogWrite(RedLed,redSens);
  analogWrite(GreenLed,greenSens);
}

//---------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  initializeDigits();// initial the digit
  matrix.initialize();// initial the matrix 
  initializeTemperature();// the lm sensor
}


void loop()// the loop program
{
  for(int i=0;i<5;i++)// 500 ms
  {
     if(buttonPressed())// change button temperature
      {
        switchState();// switch temperature
      }
    prevDegrees = currDegrees;// save for the comparaison
    delay(100);
    updateTemperature();// save the celsius and fareineigth
    turnLeds(); // turn on off the led green or red
  }
  matrix.printTemperature(currDegrees);// print to the matrix led
  printDataToConsole(); // Output on the serial interface
}

void printDataToConsole()// the comment debugger
{
  Serial.print("Current temperature Celsius is:");
  Serial.print(Celsius);
  Serial.print(char(186)); //Output <°> Symbol
  Serial.println("Cf");
  Serial.print("Current temperature Fahrenheit is:");
  Serial.print(Fahrenheit);
  Serial.print(char(186)); //Output <°> Symbol
  Serial.println("F");
  Serial.println("---------------------------------------");
}
