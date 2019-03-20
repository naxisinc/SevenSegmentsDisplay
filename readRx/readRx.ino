/**/

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
const int btn_pins[4] = {5, 4, 7, 6};
bool btn_states[4] = {HIGH};
bool last_btn_states[4] = {HIGH};

void setup()
{
    // Open serial communications
    Serial.begin(9600);

    pinMode(PWR_LED, OUTPUT);
    digitalWrite(PWR_LED, HIGH);
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            pinMode(digits[i][j], OUTPUT);
            // Setting the btn pins like INPUT and LOW by default
            pinMode(btn_pins[j], INPUT);
            digitalWrite(btn_pins[j], HIGH);
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

                    for (int i = 0; i < 4; i++)
                    {
                        Serial.print(display[i]);
                    }
                    Serial.println();

                    // Write the received numbers in the 7segments display
                    for (int i = 0; i < 4; i++)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            digitalWrite(digits[i][j], BCD[display[i]][j]);
                        }
                    }
                }
            }
            // cleaning the logs
            incomingData = "";
        }
    }

    // Reading btns
    for (int i = 0; i < 4; i++)
    {
        btn_states[i] = digitalRead(btn_pins[i]);
        if (btn_states[i] != last_btn_states[i])
        {
            if (digitalRead(btn_pins[i]) == LOW)
            {
                if (display[i] < 10)
                {
                    display[i]++;
                }
                else
                {
                    display[i] = 0;
                }
            }
        }
        last_btn_states[i] = btn_states[i];
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
