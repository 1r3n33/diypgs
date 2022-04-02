#define RST 12
#define CE 11
#define DC 10
#define DIN 9
#define CLK 8

void lcd_write_cmd(byte cmd)
{
  digitalWrite(DC, LOW); //DC pin is low for commands
  digitalWrite(CE, LOW);
  shiftOut(DIN, CLK, MSBFIRST, cmd); //trasmit serial data
  digitalWrite(CE, HIGH);
}

void setup() {
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
  lcd_write_cmd(0x09); // LCD all segments on
  
}

void loop() {
  delay(1000);
  lcd_write_cmd(0x08); // LCD all segments off

  delay(1000);
  lcd_write_cmd(0x09); // LCD all segments on
}
