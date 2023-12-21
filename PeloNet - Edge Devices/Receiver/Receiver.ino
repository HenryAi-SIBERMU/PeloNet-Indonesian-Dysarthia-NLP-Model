 
#include <esp_now.h>
#include <WiFi.h>
#include <Servo.h>
#include <driver/rtc_io.h>
Servo myservo1; 
Servo myservo2; 
Servo myservo3; 
Servo myservo4;
Servo myservo5;
RTC_DATA_ATTR int sleep_count = 0;

// Inisialisasi Switch Button 4 servo + 1 thumbservo
int angle = 90; 
int angle_thumb = 90; 

// Inisialisasi Sensor INMPP441 4 servo + 1 thumbservo
int angle_inmp_ccw = 180;
int angle_thumb_ccw = 90 ;
int angle_inmp_cw = 0;
int angle_thumb_cw = 0 ;

// 5 derajat incremen untuk Switch Buton 
int angleStep = 5;
int angleStep_thumb = 2 ;

typedef struct struct_message {
 
  int BTN1 ;
  int BTN2 ;
  int tutupsengau ;
  int bukasengau ;
  int onn ;
  int off ;
}

struct_message;
struct_message myData;

// CALLBACK DATA - void espnow 2
void data_receive(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
    Serial.print("Bytes received: ");
    Serial.println();
   }

// INIT ESP-NOW - void espnow 2
void init_espnow() 
 {
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
 }
 
// 4 SERVO CCW - SWITCH BUTTON
void servo_ccw()
{
 while(digitalRead(myData.BTN1) == LOW){
 if (angle > 0 && angle <= 180) {
      angle = angle - angleStep;
       if(angle < 0){
        angle = 0;
       }else{
      myservo1.write(angle);
      myservo2.write(angle);
      myservo3.write(angle);
      myservo4.write(angle);
      Serial.print("Moved to: ");
      Serial.print(angle);   
      Serial.println(" degree");
       }
     }
 // 1 THUMB SERVO CCW - SWITCH BUTTON -- * note ini coding nya pakai yg cw (negasi nya ccw) , karena servo sempet short jadi polaritas kebalik  
 if (angle >= 0 && angle <= 180) {
      angle_thumb = angle_thumb + angleStep;
      if(angle_thumb >180){
        angle_thumb =180;
       }else{
      myservo5.write(angle_thumb);
       }   
     }
  delay(20);
  }
}

// 4 SERVO CW - SWITCH BUTTON
void servo_cw()
{
 while(digitalRead(myData.BTN2) == LOW){
   if (angle >= 0 && angle <= 180) {
      angle = angle + angleStep;
      if(angle >180){
        angle =180;
       }else{
      myservo1.write(angle);
      myservo2.write(angle);
      myservo3.write(angle);
      myservo4.write(angle);
      Serial.print("Moved to: ");
      Serial.print(angle);   
      Serial.println(" degree");
       }
   }
// 1 THUMB SERVO CW - SWITCH BUTTON -- * note ini coding nya pakai yg ccw (negasi nya cw) , karena servo sempet short jadi polaritas kebalik 
 if (angle_thumb > 0 && angle_thumb <= 180) {
      angle_thumb = angle_thumb - angleStep_thumb; // seharunsya pakai angleStep, karena short , konfigurasi harus terpisah
       if(angle_thumb < 0){
        angle_thumb = 0;
       }else{
      myservo5.write(angle_thumb);
       }
    }
  delay(20); 
  }
 }

 // 4 SERVO CCW + 1 THUMB SERVO - SENSOR INMP441
void servo_ccw_inmp441()
{
 while(digitalRead(myData.bukasengau) == 1){
 
      myservo1.write(angle_inmp_ccw);
      myservo2.write(angle_inmp_ccw);
      myservo3.write(angle_inmp_ccw);
      myservo4.write(angle_inmp_ccw);
      myservo5.write(angle_thumb_ccw);
     Serial.print("bukasengau Detect");
     Serial.println();
     delay(20);
 }
while(digitalRead(myData.onn) == 1){
 
      myservo1.write(angle_inmp_ccw);
      myservo2.write(angle_inmp_ccw);
      myservo3.write(angle_inmp_ccw);
      myservo4.write(angle_inmp_ccw);
      myservo5.write(angle_thumb_ccw);
     Serial.print("on Detect");
     Serial.println();
     delay(20);
   }
}
 

 // 4 SERVO CW + 1 THUMB SERVO - SENSOR INMP441
void servo_cw_inmp441()
{
 while (digitalRead(myData.tutupsengau) == 1){
      
      myservo1.write(angle_inmp_cw);
      myservo2.write(angle_inmp_cw);
      myservo3.write(angle_inmp_cw);
      myservo4.write(angle_inmp_cw);
      myservo5.write(angle_thumb_cw);
      Serial.print("tutupsengau Detect");
      Serial.println();
      delay(20);
 }
 while (digitalRead(myData.off) == 1){
      
      myservo1.write(angle_inmp_cw);
      myservo2.write(angle_inmp_cw);
      myservo3.write(angle_inmp_cw);
      myservo4.write(angle_inmp_cw);
      myservo5.write(angle_thumb_cw);
      Serial.print("off Detect");
      Serial.println();
      delay(20);
    }
  }
     

void timer_sleep()
{
  Serial.println("Going to sleep...");
  esp_sleep_enable_timer_wakeup(5000000);
  esp_deep_sleep_start();
 }

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_STA);
  myservo1.write(angle); 
  myservo2.write(angle);
  myservo3.write(angle);
  myservo4.write(angle);
  myservo5.write(angle);
  myservo1.attach(5);
  myservo2.attach(18);
  myservo3.attach(22);
  myservo4.attach(21);
  myservo5.attach(23);
  
  // REGISITER ALL ESPNOW VOID 
  init_espnow();
  esp_now_register_recv_cb(data_receive);
  
  if (digitalRead(myData.bukasengau) == 0){
    Serial.print("bukasengau detect");
    Serial.println();
    servo_ccw_inmp441() ;
    }
    

   if (digitalRead(myData.onn) == 0){
    Serial.print("on detect");
    Serial.println();
    servo_ccw_inmp441() ;
    }
   

  if (digitalRead(myData.tutupsengau) == 0){
    Serial.print("tutupsengau Detect");
    Serial.println();
    servo_cw_inmp441() ;
    }
   
    
  if (digitalRead(myData.off) == 0){
    Serial.print("off Detect"); 
    Serial.println();
    servo_cw_inmp441() ;
    }
    
}
 
void loop() {

  if(digitalRead(myData.BTN1) == LOW){
    Serial.print("Moved to: ");
    Serial.print(angle); 
    Serial.println(" degree");
    servo_cw () ;
    }
    else {
    servo_cw ();
    }

  if(digitalRead(myData.BTN2) == LOW){
    Serial.print("Moved to: ");
    Serial.print(angle); 
    Serial.println(" degree");
    servo_ccw () ;
    }
    else {
    servo_ccw () ;
    }
   
  }
