

void reset_db(){
    
    record_count = 0;
    EEPROM.put(100,record_count);
    EEPROM.commit();

}

int getDbLen(){
 EEPROM.get(100, record_count);
 return record_count;
}


void add_command_db(String device, String command_name, String command_code, decode_type_t protocol, uint16_t size, const uint8_t* state,bool exist){
  
  //read_to_List();
  
  if (exist){
    if (!isCommand(device, command_name)){

      EEPROM.get(100, record_count);
      mem_pos = 104 + record_count * DB_RECORD_LEN;
      Serial.println("record count " + record_count);
      Serial.println("mem_pos " + mem_pos);

      Serial.println("Addition of new command to db");
      delay(100);
      device.toCharArray(db.Device,20);
      command_name.toCharArray(db.command_name,20);
      command_code.toCharArray(db.command_code,10);
      db.protocol = protocol;
      db.size = size;
     
      memcpy(db.state, state, sizeof(db.state[0])*53);

      delay(100);
      record_count = record_count + 1;
      EEPROM.put(mem_pos, db);
      EEPROM.commit();
      EEPROM.put(100, record_count);
      EEPROM.commit();

      Serial.println("new mem_pos " + mem_pos);
      Serial.println("new record count " + record_count);
      Serial.println("Addition of new command to db Finished");

      read_to_List();
    }
  }else{

      EEPROM.get(100, record_count);
      mem_pos = 104 + record_count * DB_RECORD_LEN;
      Serial.println("record count " + record_count);
      Serial.println("mem_pos " + mem_pos);

      Serial.println("Addition of new command to db");
      delay(100);
      device.toCharArray(db.Device,20);
      command_name.toCharArray(db.command_name,20);
      command_code.toCharArray(db.command_code,10);
      db.protocol = protocol;
      db.size = size;
      memcpy(db.state, state, sizeof(db.state[0])*53);
      delay(100);
      record_count = record_count + 1;
      EEPROM.put(mem_pos, db);
      EEPROM.commit();
      EEPROM.put(100, record_count);
      EEPROM.commit();

      Serial.println("new mem_pos " + mem_pos);
      Serial.println("new record count " + record_count);
      Serial.println("Addition of new command to db Finished");
  }

}

void print_command_db(){

  Serial.println("reading start");
  EEPROM.get(100, record_count);
  Serial.print("record_count ");
  Serial.println(record_count);

  if (record_count != 0){
  
      for (int pos = 104; pos < (104 + record_count * DB_RECORD_LEN); pos += DB_RECORD_LEN){

        EEPROM.get(pos,db);
        Serial.println(db.Device);
        Serial.println(db.command_name);
        Serial.println(db.command_code);
        Serial.println("------------------------");

      }

   }
   Serial.println("reading finished");
}

bool isCommand(String device, String command){

  for (int h = 0;h < myList.size();h++){

    Database tmp = myList.get(h);
    if (device == tmp.Device)
      if (command == tmp.command_name){
        Serial.println("Exist in database");
        return true;
      }
  }
  return false;
}


void print_list(){

Serial.println("print_list");
for (int h = 0;h < myList.size();h++){

    Database tmp = myList.get(h);
    Serial.println(tmp.Device);
    Serial.println(tmp.command_name);
    Serial.println(tmp.command_code);
    Serial.println("------------------------");
    
      
  }

}

void read_to_List(){
  
  Serial.println("read to list start");

  myList = LinkedList<Database>();
  EEPROM.get(100, record_count);

  if (record_count != 0){
  
      for (int pos = 104; pos < (104 + record_count * DB_RECORD_LEN); pos += DB_RECORD_LEN){

        EEPROM.get(pos,db);
        myList.add(db);

      }
   }
   Serial.println("read to list finish");
}

void update_db(){
  Serial.println("Update db");
  record_count = 0;
  EEPROM.put(100, record_count);
  for (int h = 0;h < myList.size();h++){
          Database tmp = myList.get(h);
          add_command_db(tmp.Device,tmp.command_name,tmp.command_code,tmp.protocol,tmp.size,tmp.state,false);
          //bспользовать метод без проверки на наличие похожих команд
    }
  read_to_List();
}

void delete_command_db(String device, String toDelete_command){

  Serial.println("Delete command start");
  bool flag = false;
  for (int h = 0;h < myList.size();h++){

    Database tmp = myList.get(h);
    if (device == tmp.Device)
      if (toDelete_command == tmp.command_name){
        Serial.println("Found!");
        myList.remove(h);
        flag = true;
        break;
      }
  } 

  if (flag){
  update_db();
  }
  else {
    Serial.println("Not found");
  }
  Serial.println("Delete command finish");
}

