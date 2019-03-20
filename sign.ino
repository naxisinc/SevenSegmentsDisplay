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

int display[4] = {0, 0, 0, 0}; // store the current values for show

const int PWR_LED = 10;
const int ACT_LED = 11;
const int btn_pins[4] = {4, 5, 6, 7};
bool btn_states[4] = {HIGH};
bool last_btn_states[4] = {HIGH};
String cad = ":000000000X0X0X0X00a\n";
boolean iterator = false;

void setup()
{
  // Setting like transmitter
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  pinMode(PWR_LED, OUTPUT);
  digitalWrite(PWR_LED, HIGH);

  pinMode(ACT_LED, OUTPUT);
  digitalWrite(ACT_LED, LOW);

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      pinMode(digits[i][j], OUTPUT);
      // Setting the btn pins like INPUT and HIGH by default
      pinMode(btn_pins[j], INPUT);
      digitalWrite(btn_pins[j], HIGH);
    }
  }

  Serial.begin(9600);
}

void loop()
{
  for (int k = 0; k < 10; k++)
  {
    // Reading btns
    for (int i = 0; i < 4; i++)
    {
      btn_states[i] = digitalRead(btn_pins[i]);
      if (btn_states[i] != last_btn_states[i] && btn_states[i] == LOW)
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
      last_btn_states[i] = btn_states[i];
    }
    updateLocalDisplay();
    delay(100);
  }
  sendData();
}

void updateLocalDisplay()
{
  // Update the local digits
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      digitalWrite(digits[i][j], BCD[display[i]][j]);
    }
  }
}

void sendData()
{
  iterator = !iterator;
  cad.setCharAt(10, char(display[3]) + 48);
  cad.setCharAt(12, char(display[2]) + 48);
  cad.setCharAt(14, char(display[1]) + 48);
  cad.setCharAt(16, char(display[0]) + 48);
  cad.setCharAt(19, char(iterator) + 48);
  Serial.print(cad);
  digitalWrite(ACT_LED, iterator);
}
