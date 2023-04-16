


void getIRcommand(){
    if (irrecv.decode(&results)){
      scanned = String(results.value, HEX);
      Serial.println("RECEIVE:");
      led.setRGB(0, 255, 255);
      led.setBrightness(255);
      delay(2000);
      
      irrecv.resume();
      Serial.println(scanned);
      Serial.println(results.value);
      db.protocol = results.decode_type;
      db.size = results.bits;
      memcpy(db.state, results.state, sizeof(db.state[0])*53);

      for (int i = 0; i < 53;i++){
        Serial.print(db.state[i]);
      }
      Serial.println("--------------------------");
      Serial.println("Another:");
      for (int i = 0; i < 53;i++){
        Serial.print(results.state[i]);
      }
      Serial.println(" ");
      Serial.print("protocol ");
      Serial.println(db.protocol);
      Serial.print("size ");
      Serial.println(db.size);
      led.setRGB(0, 0, 255);
      led.setBrightness(100);
    }
}

uint64_t getUInt64fromHex(char const *str)
{
    uint64_t accumulator = 0;
    for (size_t i = 0 ; isxdigit((unsigned char)str[i]) ; ++i)
    {
        char c = str[i];
        accumulator *= 16;
        if (isdigit(c)) /* '0' .. '9'*/
            accumulator += c - '0';
        else if (isupper(c)) /* 'A' .. 'F'*/
            accumulator += c - 'A' + 10;
        else /* 'a' .. 'f'*/
            accumulator += c - 'a' + 10;

    }

    return accumulator;
}

void send_IR(const decode_type_t protocol, const uint8_t* state,uint64_t value, const uint16_t size){

    bool success;
    if (hasACState(protocol)) {  // Does the message require a state[]?
      // It does, so send with bytes instead.
      success = irsend.send(protocol, state, (size / 8));
      Serial.println("var2");
    } else {  // Anything else must be a simple message protocol. ie. <= 64 bits
      Serial.println("var3");
      Serial.println(protocol);
      Serial.println(value);
      Serial.println(size);
      success = irsend.send(protocol, value, size);
      Serial.println("var3");
    }
    led.setRGB(255, 0, 0);
    led.setBrightness(255);
    delay(1000);
    led.setRGB(0, 0, 255);
    led.setBrightness(100);
    Serial.println(success); 
}