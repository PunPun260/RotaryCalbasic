#include <Arduino.h>
#include <U8g2lib.h>

#define knoba 6
#define knobb 7
#define knobsw 8

int8_t knobpreva;
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 
static const unsigned char image_ButtonLeftSmall_bits[] = {0x04,0x06,0x07,0x06,0x04};
static const unsigned char image_Pin_pointer_bits[] = {0x30,0x00,0x30,0x00,0xfc,0x00,0xfc,0x00,0xff,0x03,0xff,0x03};
static const unsigned char image_Pin_pointer_copy_1_bits[] = {0xff,0x03,0xff,0x03,0xfc,0x00,0xfc,0x00,0x30,0x00,0x30,0x00};

void setup() {
  Serial.begin(9600);
  u8g2.begin();

  pinMode(knoba, INPUT);
  pinMode(knobb, INPUT);
  pinMode(knobsw, INPUT);

  knobpreva = digitalRead(knoba);
}

void loop() {
  int8_t knobcura = digitalRead(knoba), pos=0; 
  if (knobpreva < knobcura) {
    if (digitalRead(knobpreva) != knobcura) pos++; // CW
    else pos--; // CCW
  }

  // ===== DISPLAY BASIC COMPONENT =====
  u8g2.clearBuffer();
  drawscreen();
  u8g2.sendBuffer();


  knobpreva = knobcura;
}

void drawscreen() {
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  u8g2.drawXBM(5, 9, 3, 5, image_ButtonLeftSmall_bits);
  u8g2.drawXBM(5, 30, 3, 5, image_ButtonLeftSmall_bits);
  u8g2.drawXBM(5, 51, 3, 5, image_ButtonLeftSmall_bits);
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(10, 15, "NUM");
  u8g2.drawStr(10, 36, "S1");
  u8g2.drawStr(10, 57, "Cf");
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(34, 14, "RotaryCalBasic");
  u8g2.drawFrame(31, 3, 81, 15);
  u8g2.drawLine(34, 30, 98, 30);
  
  u8g2.drawXBM(115, 2, 10, 6, image_Pin_pointer_bits);
  u8g2.drawEllipse(123, 20, 1, 1);
  u8g2.drawEllipse(122, 32, 2, 2);
  u8g2.drawEllipse(123, 44, 1, 1);
  u8g2.drawXBM(116, 30, 3, 5, image_ButtonLeftSmall_bits);
  u8g2.drawXBM(115, 56, 10, 6, image_Pin_pointer_copy_1_bits);
}