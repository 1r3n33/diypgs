#define RST 12
#define CE 11
#define DC 10
#define DIN 9
#define CLK 8

void lcd_write_cmd(byte cmd)
{
  digitalWrite(DC, LOW); // DC pin is low for commands
  digitalWrite(CE, LOW);
  shiftOut(DIN, CLK, MSBFIRST, cmd); // transmit serial data
  digitalWrite(CE, HIGH);
}

void setup()
{
  pinMode(RST, OUTPUT);
  pinMode(CE, OUTPUT);
  pinMode(DC, OUTPUT);
  pinMode(DIN, OUTPUT);
  pinMode(CLK, OUTPUT);

  digitalWrite(RST, LOW);
  digitalWrite(RST, HIGH);

  lcd_write_cmd(0x21); // LCD extended commands
  lcd_write_cmd(0xB8); // set LCD Vop (contrast)
  lcd_write_cmd(0x04); // set temp coefficient
  lcd_write_cmd(0x14); // LCD bias mode 1:40
  lcd_write_cmd(0x20); // LCD basic commands

  // Clear display data ram
  digitalWrite(CE, LOW);
  digitalWrite(DC, LOW);              // DC pin is low for commands
  shiftOut(DIN, CLK, MSBFIRST, 0x0C); // normal display mode
  shiftOut(DIN, CLK, MSBFIRST, 0x40); // y 0
  shiftOut(DIN, CLK, MSBFIRST, 0x80); // x 0
  digitalWrite(DC, HIGH);             // DC pin is high for data
  for (int i = 0; i < 48 * 84; i++)
  {
    shiftOut(DIN, CLK, MSBFIRST, 0x00);
  }
  digitalWrite(DC, LOW); // DC pin is low for commands
  digitalWrite(CE, HIGH);
}

void loop()
{
  // LCD all segments on
  delay(1000);
  lcd_write_cmd(0x09);

  // LCD all segments off
  delay(1000);
  lcd_write_cmd(0x08);

  // Draw 8x8 sphere
  delay(1000);
  digitalWrite(CE, LOW); // Activate

  digitalWrite(DC, LOW);              // DC pin is low for commands
  shiftOut(DIN, CLK, MSBFIRST, 0x20); // basic instruction mode
  shiftOut(DIN, CLK, MSBFIRST, 0x0C); // normal display mode
  shiftOut(DIN, CLK, MSBFIRST, 0x40); // y 0
  shiftOut(DIN, CLK, MSBFIRST, 0x80); // x 0

  digitalWrite(DC, HIGH);             // DC pin is high for data
  shiftOut(DIN, CLK, MSBFIRST, 0x3C); // sprite data..
  shiftOut(DIN, CLK, MSBFIRST, 0x7E);
  shiftOut(DIN, CLK, MSBFIRST, 0xFF);
  shiftOut(DIN, CLK, MSBFIRST, 0xFF);
  shiftOut(DIN, CLK, MSBFIRST, 0xFF);
  shiftOut(DIN, CLK, MSBFIRST, 0xFF);
  shiftOut(DIN, CLK, MSBFIRST, 0x7E);
  shiftOut(DIN, CLK, MSBFIRST, 0x3C);

  digitalWrite(DC, LOW);  // DC pin is low for commands
  digitalWrite(CE, HIGH); // Deactivate
}
