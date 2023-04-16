
// Variables will change:
int lastState = LOW;      
int currentState;
int timer = 0;
int count_button = 0;
unsigned long timer_start = 0;
unsigned long timer_end = 0;
unsigned long hold_timer_start = 0;
unsigned long hold_timer_end = 0;
int total_hold_time = 0;
bool isHold = false;

void button() {
  // read the state of the the input pin:
  currentState = digitalRead(BUTTON_PIN);
  delay(50);


  if (lastState == HIGH && currentState == HIGH){//LOW LOW

    if (!isHold){
      hold_timer_start = millis();
      isHold = true;
    }
    if (isHold && millis() - hold_timer_start > 1500ul){
      
      Serial.println("hold");

    }

  }

  if (lastState == HIGH && currentState == LOW){//LOW HIGH

    if (isHold){
      hold_timer_end = millis();
      isHold = false;
      Serial.println("hold is end ");
      total_hold_time = (((hold_timer_end - hold_timer_start) / 1000ul) % 3600ul) % 60ul;
      Serial.println("Holded: ");
      Serial.print(total_hold_time);
      // обработать секунд зажатой кнопки
      /*if (total_hold_time == RESET_RANGE_BUTTON){
        reset_wifi_settings();
      }*/
    }
  }

  if (lastState == LOW && currentState == HIGH){//HIGH LOW
    Serial.println("Sensor is touched");

    if (timer == 1){
      count_button = count_button + 1;
      //Serial.println(count_button);
    }

    if (timer == 0) {
      timer = 1;
      timer_start = millis();
      //Serial.println("Timer start " + timer_start);
      count_button = count_button + 1;
    } 
    
  }

  if (timer == 1 ){
    timer_end = millis();
    if (timer_end - timer_start > 4000ul){
      timer = 0;
      // обработать число нажатий 
      Serial.println(count_button);
      if (count_button == RESET_CLICK_BUTTON){
        reset_wifi_settings();
      }
      count_button = 0;
      



    }
    
  }

  // save the the last state
  lastState = currentState;
  
}