#include "tinyexpr.h"
#include <U8g2lib.h>

#define knoba 2
#define knobb 4
#define knobsw 3
#define button A3

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 
//static const unsigned char image_Layer_5_bits[] = {
  // 0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0xc0,0x00,0x00,
  // 0x00,0x00,0x00,0x40,0x01,0x00,0x00,0x00,0x00,0x20,0x01,
  // 0x00,0x00,0x00,0x00,0x10,0x01,0x38,0x00,0x00,0x00,0x08,
  // 0x01,0x24,0x00,0x00,0x00,0x04,0x01,0x23,0x00,0x00,0x00,
  // 0x02,0x82,0x21,0x00,0x00,0x00,0x02,0x62,0x20,0x00,0x00,
  // 0x00,0x01,0x1a,0x20,0x00,0x00,0x00,0x01,0x06,0x20,0x00,
  // 0x00,0x80,0x00,0x00,0x20,0x00,0x00,0x40,0x00,0x00,0x20,
  // 0x00,0x00,0x40,0x00,0x00,0x20,0x00,0x00,0x20,0x00,0x00,
  // 0x20,0x00,0x00,0x10,0x00,0x00,0x20,0x00,0xff,0x13,0x00,
  // 0x00,0x20,0x00,0x03,0x80,0x1f,0x00,0x20,0x00,0x06,0xc0,
  // 0x70,0xf8,0xe7,0x3f,0x18,0x40,0x5e,0x0c,0x0c,0x20,0x60,
  // 0x40,0x5e,0xf4,0x08,0x30,0x80,0x41,0x60,0xf4,0x08,0x0e,
  // 0x00,0xc2,0x3f,0x0c,0x0f,0x01,0x00,0x06,0x00,0xf8,0xc1,
  // 0x00,0x80,0x03,0x00,0x00,0x00,0x01,0xc0,0x00,0x02,0x21,
  // 0x00,0x03,0x30,0x00,0xfc,0x5e,0x00,0x04,0x08,0x00,0x80,
  // 0x80,0x00,0x0f,0xfc,0xff,0x01,0x83,0xfe,0x00,0x00,0x30,
  // 0x00,0x7c,0x18,0x00,0x00,0x18,0x00,0x00,0x30,0x00,0x00,
  // 0xfc,0x00,0x00,0x1c,0x00,0x00,0x80,0x00,0x00,0x04,0x00,
  // 0x00,0x40,0x00,0x00,0x0a,0x00,0x00,0x40,0x06,0x00,0x1e,
  // 0x00,0x00,0xe0,0x01,0x00,0x1c,0x00,0x00,0x00,0x00,0x00,
  // 0x10,0x00};


// int8_t knobpreva; 
volatile bool knobPressed = false, knobmov = false;
volatile int8_t modee = 0, knobv;
const int8_t modelim[] = {10, 11, 5, 10, 12, 3}; // NUMBER | SYMBOL | MENU | TRIGONOMETRY | FUNCTION | CONSTANCE
volatile int8_t opp1 = 0;
const char numberr[] = "0123456789";
const char operatorr[] = "+-*/^().,=#";
const char TRIG[10][6] = {"acos", "asin", "atan", "atan2", "cos", "sin", "tan", "cosh", "sinh", "tanh"};
const char FUNC[12][6] = {"fac", "ncr", "npr", "ceil", "floor", "abs", "exp", "pow", "sqrt", "ln", "log", "log10"};
const char CONS[12][6] = {"e", "pi", "g"};
char cureqa[64] = "";
double resultt[3];
int buttonprev;
char buf[20];

void setup() {
  Serial.begin(9600);
  u8g2.begin();
  asm(".global printf_flt");

  pinMode(knoba, INPUT);
  pinMode(knobb, INPUT);
  pinMode(knobsw, INPUT);
  pinMode(button, INPUT);

  // knobpreva = digitalRead(knoba);
  buttonprev = 1023;
  //Serial.println(digitalPinToInterrupt(6));

  attachInterrupt(digitalPinToInterrupt(knoba), rotaryint, FALLING);
  attachInterrupt(digitalPinToInterrupt(knobsw), rotarysw, FALLING);
}

void loop() {
  int buttoncur = analogRead(button), pressed=0; 
  if (buttonprev > 950 && buttoncur < 950) {
    if (buttoncur > 830) pressed=3;
    else if (buttoncur > 660) pressed=2;
    else pressed=1;
  }

  // ===== INPUT RESPOND =====
  // ===== BUTTON
  if (pressed) modee = pressed-1;
  // ===== KNOB
  if (knobmov) {
    knobmov=0;
    if (!knobv)
      opp1 = (opp1 + 1) % modelim[modee];
    else
      opp1 = (opp1 + modelim[modee] - 1) % modelim[modee];
  }
  // ===== KNOB BUTTON
  if (knobPressed) {
    noInterrupts();
    knobPressed = false;
    int8_t p = opp1;      // snapshot while interrupts are off
    int8_t m = modee;
    interrupts();

    if (m == 5) {
      strcat(cureqa, CONS[opp1]);
      modee = 0;
    } else if (m == 4) {
      strcat(cureqa, FUNC[opp1]);
      modee = 1;
    } else if (m == 3) {
      strcat(cureqa, TRIG[opp1]);
      modee = 1;
    } else if (m == 2) {
      modee = 3+p;
    } else {
      char c = (m == 0) ? numberr[p] : operatorr[p];
      int8_t l = strlen(cureqa);
      if (c == '#') {
        cureqa[l-1] = '\0';
      } else if (c == '=') {
        calres();
      } else if (l < (int)sizeof(cureqa) - 1) {
        cureqa[l] = c;
        cureqa[l + 1] = '\0';
      }
    }
  }

  // ===== DISPLAY COMPONENT =====
  u8g2.firstPage();
  do {
    if (modee == 7) {
      CATT();
    } else if (modee == 5) {
      CONSTMODE(opp1);
    } else if (modee == 4) {
      FUNCTIONMODE(opp1);
    } else if (modee == 3) {
      TRIGONOMETRYMODE(opp1);
    } else if (modee == 2) {
      drawMODE1(opp1);
    } else {
      drawscreen();
      draweqa();
      drawscroll(opp1);
      drawres();
    }
  } while (u8g2.nextPage());
  
  // u8g2.sendBuffer();
  buttonprev = buttoncur;
}

void drawscreen() {
  const unsigned char image_ButtonLeftSmall_bits[] = {0x04,0x06,0x07,0x06,0x04};
  const unsigned char image_Pin_pointer_bits[] = {0x04,0x0e,0x1f};
  const unsigned char image_Pin_pointer_copy_1_bits[] = {0x1f,0x0e,0x04};

  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  u8g2.drawXBM(1, 9, 3, 5, image_ButtonLeftSmall_bits);
  u8g2.drawXBM(1, 30, 3, 5, image_ButtonLeftSmall_bits);
  u8g2.drawXBM(1, 51, 3, 5, image_ButtonLeftSmall_bits);
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(6, 15, "NUM");
  u8g2.drawStr(6, 36, "S1");
  u8g2.drawStr(6, 57, "MODE");
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(34, 14, "RotaryCalBasic");
  u8g2.drawFrame(31, 3, 81, 15);
  u8g2.drawLine(34, 30, 98, 30);

  u8g2.drawXBM(122, 30, 3, 5, image_ButtonLeftSmall_bits);
  u8g2.drawXBM(114, 14, 5, 3, image_Pin_pointer_bits);
  u8g2.drawXBM(114, 48, 5, 3, image_Pin_pointer_copy_1_bits);
}

void drawscroll(int poss) {
  int limitt = modelim[modee];
  u8g2.setDrawColor(0);
  u8g2.drawBox(113, 19, 7, 27);
  u8g2.setDrawColor(1);
  u8g2.setCursor(114, 27);
  if (modee == 1) {
    u8g2.print(operatorr[(poss+limitt-1)%limitt]);
    u8g2.setCursor(114, 36);
    u8g2.print(operatorr[poss]);
    u8g2.setCursor(114, 45);
    u8g2.print(operatorr[(poss+limitt+1)%limitt]);
  } else {
    u8g2.print((poss+limitt-1)%limitt);
    u8g2.setCursor(114, 36);
    u8g2.print(poss);
    u8g2.setCursor(114, 45);
    u8g2.print((poss+limitt+1)%limitt);
  }
}

void drawMODE1(int p) {
    u8g2.setFont(u8g2_font_helvB08_tr);
    u8g2.drawStr(2, 9, "Seletctor");
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, 0, 128, 10);
    u8g2.setDrawColor(1);
    u8g2.drawStr(24, 20, "TRIGONOMETRY");
    u8g2.drawStr(24, 29, "FUNCTON");
    u8g2.drawStr(24, 38, "CONSTANT");
    u8g2.drawStr(24, 47, "");
    u8g2.drawStr(24, 56, "OWO");
    u8g2.drawStr(2, 20, "^");
    u8g2.drawStr(2, 61, "v");
    u8g2.drawStr(12, 20+p*9, ">");
}

void TRIGONOMETRYMODE(int p) {
    u8g2.setFont(u8g2_font_helvB08_tr);
    u8g2.drawStr(2, 9, "TRIGONOMETRY");
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, 0, 128, 10);
    u8g2.setDrawColor(1);
    int a = p%5, b = p-a;
    for (int i = 0; i < 5; i++) {
      u8g2.setCursor(24, 20+(i*9));
      u8g2.print(TRIG[b+i]);
    }
    u8g2.drawStr(2, 20, "^");
    u8g2.drawStr(2, 61, "v");
    u8g2.drawStr(12, 20+a*9, ">");
}

void FUNCTIONMODE(int p) {
    u8g2.setFont(u8g2_font_helvB08_tr);
    u8g2.drawStr(2, 9, "FUNCTION");
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, 0, 128, 10);
    u8g2.setDrawColor(1);
    int a = p%5, b = p-a;
    for (int i = 0; i < 5 && b+i < 12; i++) {
      u8g2.setCursor(24, 20+(i*9));
      u8g2.print(FUNC[b+i]);
    }
    u8g2.drawStr(2, 20, "^");
    u8g2.drawStr(2, 61, "v");
    u8g2.drawStr(12, 20+a*9, ">");
}

void CONSTMODE(int p) {
    u8g2.setFont(u8g2_font_helvB08_tr);
    u8g2.drawStr(2, 9, "CONSTANT");
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, 0, 128, 10);
    u8g2.setDrawColor(1);
    u8g2.drawStr(24, 20, "e");
    u8g2.drawStr(24, 29, "pi");
    u8g2.drawStr(24, 38, "Earth gravity");
    u8g2.drawStr(24, 47, "");
    u8g2.drawStr(24, 56, "");
    u8g2.drawStr(2, 20, "^");
    u8g2.drawStr(2, 61, "v");
    u8g2.drawStr(12, 20+p*9, ">");
}

void draweqa() {
  int a = strlen(cureqa)-12; if (a<0) a=0;
  // sprintf(buf, "%s", cureqa);
  u8g2.setCursor(34, 27);
  u8g2.print(cureqa+a);
}

void drawres() {  
  u8g2.setCursor(34, 41);
  dtostrf(resultt[0], 6, 2, buf);
  u8g2.print(buf);
  u8g2.setCursor(34, 50);
  dtostrf(resultt[1], 6, 2, buf);
  u8g2.print(buf);
  u8g2.setCursor(34, 59);
  dtostrf(resultt[2], 6, 2, buf);
  u8g2.print(buf);
}

void CATT() {
  u8g2.setFont(u8g2_font_6x13O_tr);
  u8g2.drawStr(3, 14, "MEOW MEOW");
  //u8g2.drawXBM(30, 25, 46, 37, image_Layer_5_bits);
  //u8g2.drawXBM(79, 25, 46, 37, image_Layer_5_bits);
  u8g2.drawStr(6, 57, "BACK");
}

void calres() {
  int error;
  resultt[2] = resultt[1];
  resultt[1] = resultt[0];
  resultt[0] = te_interp(cureqa, &error);
  cureqa[0] = '\0';
}

void rotaryint() {
  knobmov = 1;
  knobv = digitalRead(knobb);
}

void rotarysw() {
  knobPressed = true;
}