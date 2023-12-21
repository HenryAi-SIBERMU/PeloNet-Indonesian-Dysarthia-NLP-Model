 
  #include <esp_now.h>
  #include <WiFi.h>
  #include <Servo.h>
  #include <driver/rtc_io.h>
  #define BUTTON_LEFT 21
  #define BUTTON_RIGHT 22
  #define BUTTON_SLEEP GPIO_NUM_15
  #define LEDREMOTE 23  
  RTC_DATA_ATTR int sleep_count = 0;
  esp_now_peer_info_t peerInfo;
  int BTN1 = 0;
  int BTN2 = 0;
  

// INIT ESP-NOW - void espnow1
void init_espnow() 
 {
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
 }
  
//  ESP MAC ADRESS 
  uint8_t broadcastAddress[] = {0x94,0xB9,0x7E,0xD4,0x12,0xE4};

// STRUCTURE SEND DATA
  typedef struct struct_message {
  int BTN1 ;
  int BTN2 ;

 } 
  struct_message;
  struct_message myData;

// CALLBACK DATA - void espnow 2
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// SEND DATA ESPNOW - void espnow 3
void send_data() 
{
  sizeof(myData);
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  if (result == ESP_OK) {
     Serial.println("Sent with success");
  }
}

// BUTTON REMOTE RIGHT
void remotebutton_right()
{
  if(digitalRead(BUTTON_RIGHT) == LOW)
  myData.BTN1 = 1;
  else {
  myData.BTN1 = 0;
  delay(5);
  }
}

// BUTTON REMOTE LEFT
void remotebutton_left  ()
{
  if(digitalRead(BUTTON_LEFT) == LOW)
  myData.BTN2 = 1;
  else {
  myData.BTN2 = 0;
  delay(5);
  }
}

// LED REMOTE RIGHT
void ledremote_right()
{
  if(digitalRead(BUTTON_RIGHT) == LOW){
  digitalWrite(LEDREMOTE, HIGH);
  }
  else {
  digitalWrite(LEDREMOTE, LOW);
  }
  delay(5);

}

//  LED REMOTE LEFT
void ledremote_left()
{
  if(digitalRead(BUTTON_LEFT) == LOW){
  digitalWrite(LEDREMOTE, HIGH);
  }
  else { 
  digitalWrite(LEDREMOTE, LOW); 
  }
  delay(5);
}

// TRIGGER WAKEUP BUTTON SLEEP MODE - void espnow 4
void show_wake_reason()
{
  sleep_count++;
  auto cause = esp_sleep_get_wakeup_cause();
  switch (cause){
    case ESP_SLEEP_WAKEUP_EXT0:
    Serial.println("Wakeup reason: EXT0");
    break; 
    default:
    Serial.printf("Wakeup reason: %d\n", cause);
   }
    Serial.printf("Count %d\n", sleep_count);
 }

// TRIGGER BUTTON SLEEP MODE - void espnow 5
void enter_sleep()
{
  Serial.println("Going to sleep...");
  delay(1000);
  Serial.println("3");
  delay(1000);
  Serial.println("2");
  delay(1000);
  Serial.println("1");
  delay(1000);
  #ifdef DEEP_SLEEP_EXT0 
  pinMode(BUTTON_SLEEP, INPUT_PULLUP);
  rtc_gpio_hold_en(BUTTON_SLEEP);
  esp_sleep_enable_ext0_wakeup(BUTTON_SLEEP, LOW);
  #endif
  esp_deep_sleep_start();
 }

 
void setup() {
  
  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_STA);
  pinMode(BUTTON_LEFT,INPUT_PULLUP); 
  pinMode(BUTTON_RIGHT,INPUT_PULLUP);
  pinMode(BUTTON_SLEEP, INPUT_PULLUP);
  pinMode(LEDREMOTE,OUTPUT);
  
  // REGISITER ALL ESPNOW VOID 
  init_espnow();
  show_wake_reason();
  esp_sleep_enable_ext0_wakeup(BUTTON_SLEEP,0 ); 
  esp_now_register_send_cb(OnDataSent);
  
  // REGSITER PEER
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // ADD PEER       
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
 }

 
void loop() {

  // REMOTE BUTTON SEND ESP NOW
  if(digitalRead(BUTTON_RIGHT) == LOW) {
  remotebutton_right() ;
  }
  else {
  remotebutton_right() ;
   }
  if(digitalRead(BUTTON_LEFT) == LOW) {
  remotebutton_left() ;
  }
  else {
  remotebutton_left() ;
  }
  send_data();
  

  // LED REMOTE WIHTOUT SEND DATA
  if(digitalRead(BUTTON_RIGHT) == LOW){
  ledremote_right() ; 
  }
  else {
  ledremote_right();
  }
  if(digitalRead(BUTTON_LEFT) == LOW){
  ledremote_left() ; 
  }
  else {
  ledremote_left() ;
  }
   
  // WIFI TIMER TIMEOUT SLEEP STATE 
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  if (result == ESP_OK) {   
     Serial.println("Sent with success");
  }
  else {
      Serial.println("Error sending the data");
      Serial.println("enter TIMER sleep...");
      esp_sleep_enable_timer_wakeup(5000000);
      esp_deep_sleep_start();
  }
  // BUTTON DEEP SLEEP STATE  
  if (digitalRead(BUTTON_SLEEP) == LOW){
     Serial.println("Button pressed - enter DEEP sleep\n");
     enter_sleep();
  }
}
