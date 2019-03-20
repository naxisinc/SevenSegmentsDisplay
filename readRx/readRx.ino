/*
  Este es el firmware que va en el sign exterior del tren.
  Utiliza el 75ALS176 como receiver poniendo el pin 3 en LOW.
  La cadena q recive (en caso de ser generada por nuestro transmitter)
  llevara en la posicion 19 el estado del ACT_LED, de esta forma se
  sincronizan ambos y en caso q se interrumpa la comunicacion el ACT_LED
  en este terminal dejara de blinkear.
*/

String incomingData = "";      // string to hold the input
String currentData = "";       // current data
int display[4] = {0, 0, 0, 0}; // store the current values for show

// BCD codes
byte BCD[11][4] = {
    {0, 0, 0, 0},  // 0
    {1, 0, 0, 0},  // 1
    {0, 1, 0, 0},  // 2
    {1, 1, 0, 0},  // 3
    {0, 0, 1, 0},  // 4
    {1, 0, 1, 0},  // 5
    {0, 1, 1, 0},  // 6
    {1, 1, 1, 0},  // 7
    {0, 0, 0, 1},  // 8
    {1, 0, 0, 1},  // 9
    {1, 1, 1, 1}}; // 10 = all off

// A, B, C, D inputs
const int digits[4][4] = {
    {44, 43, 42, 41},
    {40, 39, 38, 37},
    {28, 29, 30, 31},
    {32, 33, 34, 35}};

const int PWR_LED = 10;
const int ACT_LED = 11;

void setup()
{
    // Open serial communications
    Serial.begin(9600);

    // Setting like receiver
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);

    pinMode(ACT_LED, OUTPUT);
    pinMode(PWR_LED, OUTPUT);
    digitalWrite(PWR_LED, HIGH);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            pinMode(digits[i][j], OUTPUT);
        }
    }
}

void loop()
{
    while (Serial.available() > 0)
    {
        char inByte = Serial.read();

        incomingData += inByte;

        if (inByte == '\n')
        {
            // check if the incoming data is valid
            if (incomingData.length() == 21)
            {
                if (incomingData != currentData)
                {
                    currentData = incomingData;
                    int index = 3;
                    for (int i = 10; i <= 16; i += 2)
                    {
                        if (isDigit(currentData[i]))
                        {
                            display[index] = currentData[i] - 48;
                        }
                        else
                        {
                            display[index] = 10;
                        }
                        index--;
                    }
                }
                digitalWrite(ACT_LED, incomingData[19] - 48);
            }
            // cleaning the logs
            incomingData = "";
        }
    }

    // Write the received numbers in the 7segments display
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            digitalWrite(digits[i][j], BCD[display[i]][j]);
        }
    }
}
