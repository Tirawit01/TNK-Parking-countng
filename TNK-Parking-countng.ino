#include <Wire.h>  // ไลบรารีสำหรับการสื่อสารผ่าน I2C
#include <LiquidCrystal_PCF8574.h>  // ไลบรารีสำหรับควบคุมจอ LCD แบบ I2C

LiquidCrystal_PCF8574 lcd(0x27);  // สร้างออบเจ็กต์สำหรับจอ LCD ที่อยู่ I2C คือ 0x27

#define swCheckIN A0  // กำหนดขา A0 เป็นสวิตช์ CheckIN
#define swCheckOUT A1 // กำหนดขา A1 เป็นสวิตช์ CheckOUT

#define relay 4  // กำหนดขา 4 สำหรับควบคุมรีเลย์
      
int CheckIN = 1;  // ตัวแปรเก็บ CheckIN
int CheckOUT = 0;  // ตัวแปรเก็บ CheckOUT
int counter = 0;  // ตัวแปรนับจำนวนคน
const int maxCounter = 3;  // กำหนดค่าสูงสุดของจำนวนคน

void setup() {
    Serial.begin(9600);  // เริ่มการสื่อสารผ่านพอร์ตอนุกรมที่ความเร็ว 9600 bps
    pinMode(swCheckIN, INPUT);  // ตั้งค่าให้ขา swCheckIN เป็นอินพุต
    pinMode(swCheckOUT, INPUT); // ตั้งค่าให้ขา swCheckOUT เป็นอินพุต
    pinMode(relay, OUTPUT);     // ตั้งค่าให้ขา relay เป็นเอาต์พุต

    lcd.begin(16, 2); 
    lcd.setBacklight(255); 
    lcd.setCursor(0, 0); 
    lcd.print("     Welcome    ");  
    delay(3000);  // รอ 3 วินาที   [1000  = 1วิ] 
    lcd.clear();  // ล้างหน้าจอ LCD
}

void loop() {
    CheckIN = digitalRead(swCheckIN);  // CheckIN
    CheckOUT = digitalRead(swCheckOUT);  // CheckOUT

    // แสดงสถานะของสวิตช์และตัวนับบน Serial Monitor
    Serial.print("swCheckIN=");
    Serial.print(CheckIN);
    Serial.print("\t");
    Serial.print("swCheckOUT=");
    Serial.print(CheckOUT);
    Serial.print("\t");
    Serial.print("Counter=");
    Serial.print(counter);
    Serial.println();

    // ถ้าสวิตช์ CheckIN ถูกกด
    if (CheckIN == 0) {
        while (CheckOUT == 1) {
            CheckOUT = digitalRead(swCheckOUT);
        }
        if (counter < maxCounter) {
            counter++;
        }
        while (CheckOUT == 0) {
            CheckOUT = digitalRead(swCheckOUT);
        }
    }

    // ถ้าสวิตช์ CheckOUT ถูกกด
    if (CheckOUT == 0) {
        while (CheckIN == 1) {
            CheckIN = digitalRead(swCheckIN);
        }
        if (counter > 0) {
            counter--;
        }
        while (CheckIN == 0) {
            CheckIN = digitalRead(swCheckIN);
        }
    }

    // แสดงจำนวนคนบนจอ LCD
    lcd.setCursor(0, 0);
    lcd.print("car =");
    lcd.setCursor(10, 0);
    lcd.print(counter);
    lcd.print("  ");
    
    // แสดงสถานะของไฟและควบคุมรีเลย์
    lcd.setCursor(0, 1);
    lcd.print("car is ");
    if (counter >= maxCounter) {
        digitalWrite(relay, HIGH); 
        lcd.print("");
        lcd.setCursor(9, 1);
        lcd.print("Full"); 
    } else {
        digitalWrite(relay, LOW); 
        lcd.print("");
        lcd.setCursor(9, 1);
        lcd.print("          ");  // ลบข้อความ "เต็มที่ 20" ถ้าจำนวนน้อยกว่า 20   
    }
}

// โค้ดนี้จะนับจำนวนคนที่เข้าหรือออกจากพื้นที่โดยใช้สวิตช์สองตัว เมื่อจำนวนคนมากกว่า 0 รีเลย์จะเปิดไฟ และเมื่อจำนวนคนเท่ากับ 0 รีเลย์จะปิดไฟ สถานะต่างๆ จะแสดงบนจอ LCD
     