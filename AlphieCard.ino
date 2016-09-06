// Simple sketch to read out BMA020 using SoftI2C
#define SDA_PORT PORTD
#define SDA_PIN 0
#define SCL_PORT PORTD
#define SCL_PIN 1
#define I2C_SLOWMODE 1
#include <SoftI2CMaster.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <TKLCD.h>

TKLCD_Local lcd = TKLCD_Local();

void upload_configuration() {
  i2c_start(0x22);
  i2c_write(0xA0);
  i2c_write(0x2F);
  i2c_write(0x3B);
  i2c_write(0x38);
  i2c_write(0x37);
  i2c_write(0x2D);
  i2c_write(0x32);
  i2c_write(0x3A);
  i2c_write(0x32);
  i2c_write(0x3F);
  i2c_write(0x34);
  i2c_stop();
  
  i2c_start(0x22);
  i2c_write(0xA1);  
  i2c_write(0x4D);
  i2c_write(0x48);
  i2c_write(0x47);
  i2c_write(0x48);
  i2c_write(0x4D);
  i2c_write(0x4B);
  i2c_write(0x48);
  i2c_write(0x4B);
  i2c_write(0x4F);
  i2c_write(0x4C);
  i2c_stop();
  
  i2c_start(0x22);  
  i2c_write(0xA2);  
  i2c_write(0x8B);
  i2c_write(0x88);
  i2c_write(0x88);
  i2c_write(0x88);
  i2c_write(0x8B);
  i2c_write(0x8A);
  i2c_write(0x88);
  i2c_write(0x8A);
  i2c_write(0x8C);
  i2c_write(0x8A);
  i2c_stop();
}

void setup(void) {
  Serial.begin(115200);
  lcd.begin();
  
  i2c_init();
    
  // First Read goodness
  i2c_start(0x23);
  i2c_write(0xE0);
  lcd.clear();
  lcd.print(i2c_read(true), HEX);  // Should be 0x4A
  i2c_stop();

  delay(500);  // So I can read the above

  lcd.clear();
  lcd.println(F("Uploading configuration"));
  upload_configuration();  
  
  i2c_start(0x23);
  lcd.print(i2c_read(false), HEX);  // F1
  lcd.print(i2c_read(false), HEX);  // Checksum for config data
  lcd.print(i2c_read(true), HEX);   // 00
  i2c_stop();
  
  delay(500);
}

void loop(void){
  int card1 = 0;
  int card2 = 0;
  
  i2c_start(0x22);
  i2c_write(0x30);
  i2c_stop();
  
  // From the logic dump
  delay(70);

  lcd.clear();
  i2c_start(0x23);
  i2c_write(0x20);
  card1 = i2c_read(false);
  card2 = i2c_read(true);
  i2c_stop();
  
  card1 = card2 << 8 | card1;
  
  switch (card1) {
    case -1:
      lcd.print(F("Unable to read"));
      break;
    
    case -6:
      lcd.print(F("No card"));
      break;
    
    case -16:
      lcd.print(F("Insert / Eject"));
      break;
  
    default:
      lcd.print(F("ID: "));
      lcd.print(card1);
  }

  // From logic dump
  delay(100);
}

