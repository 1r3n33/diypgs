#include "SPI.h"

#define RST 12
#define DC 10

void setup()
{
  pinMode(DC, OUTPUT);
  pinMode(RST, OUTPUT);

  SPI.begin(); // set SCK, MOSI, and SS to outputs, pulling SCK and MOSI low, and SS high

  digitalWrite(RST, LOW);
  digitalWrite(RST, HIGH);

  digitalWrite(SS, LOW); // Activate
  digitalWrite(DC, LOW); // DC pin is low for commands
  SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(0x21); // LCD extended commands
  SPI.transfer(0xB8); // set LCD Vop (contrast)
  SPI.transfer(0x04); // set temp coefficient
  SPI.transfer(0x14); // LCD bias mode 1:40
  SPI.endTransaction();
  digitalWrite(SS, HIGH); // Deactivate

  // Clear display data ram
  digitalWrite(SS, LOW); // Activate

  digitalWrite(DC, LOW); // DC pin is low for commands
  SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(0x20); // LCD basic commands
  SPI.transfer(0x0C); // normal display mode
  SPI.transfer(0x40); // y 0
  SPI.transfer(0x80); // x 0
  SPI.endTransaction();

  digitalWrite(DC, HIGH); // DC pin is high for data
  SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
  for (int i = 0; i < 48 * 84; i++)
  {
    SPI.transfer(0x00);
  }
  SPI.endTransaction();

  digitalWrite(SS, HIGH); // Deactivate
}

void loop()
{
  // LCD all segments on
  delay(1000);
  digitalWrite(SS, LOW); // Activate
  digitalWrite(DC, LOW); // DC pin is low for commands
  SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(0x20); // LCD basic commands
  SPI.transfer(0x09); // On
  SPI.endTransaction();
  digitalWrite(SS, HIGH); // Deactivate

  // LCD all segments off
  delay(1000);
  digitalWrite(SS, LOW); // Activate
  digitalWrite(DC, LOW); // DC pin is low for commands
  SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(0x20); // LCD basic commands
  SPI.transfer(0x08); // Off
  SPI.endTransaction();
  digitalWrite(SS, HIGH); // Deactivate

  // Draw 8x8 sphere
  delay(1000);
  digitalWrite(SS, LOW); // Activate

  digitalWrite(DC, LOW); // DC pin is low for commands
  SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(0x20); // LCD basic commands
  SPI.transfer(0x0C); // normal display mode
  SPI.transfer(0x40); // y 0
  SPI.transfer(0x80); // x 0
  SPI.endTransaction();

  digitalWrite(DC, HIGH); // DC pin is high for data
  SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(0x3C); // sprite data..
  SPI.transfer(0x7E);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0x7E);
  SPI.transfer(0x3C);
  SPI.endTransaction();

  digitalWrite(SS, HIGH); // Deactivate
}
