

void start_mainPortal(){
  ui.attachBuild(build);
  ui.start();
  ui.attach(action2);
  isMain_start = 1;
}

void build() {
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);
  GP.PAGE_TITLE("Smart remote");   


  if (ui.uri("/scan")){


    GP.TEXT("device", "Device",db.Device);
    GP.TEXT("command_name", "Command_name",db.command_name);
    GP.BREAK();
    GP.LABEL("Code Scan");
    //run code scanner
    GP.TEXT("code", "scanned", db.command_code);
    GP.UPDATE("code",10);
    GP.BUTTON_LINK("/", "Back");
    
  }

  if (ui.uri("/")){
 // первый блок навигации
  GP.NAV_TABS_M("navA","Control,Scan,C-Manager,About");

  GP.NAV_BLOCK_BEGIN("navA", 0);
  
  read_to_List();

   // включаем границы таблиц (ДЛЯ НАГЛЯДНОСТИ)
  GP.TABLE_BORDER(true);

    // делаем таблицу вручную
  GP.TITLE("Device Control");
  GP.TABLE_BEGIN();

  GP.TR();
  GP.TD();
  GP.LABEL("Device");
  GP.TD();
  GP.LABEL("C name");
  GP.TD();
  GP.LABEL("IR Code");
  GP.TD();

  String s;
  for (int i = 0; i < myList.size();i++ ){

    Database tmp = myList.get(i);
    GP.TR();
    GP.TD();
    GP.LABEL(tmp.Device);
    GP.TD();
    //GP.LABEL(tmp.command_name);
    delay(300);
    s = "run_command_" + String(i);
    GP.BUTTON(s,tmp.command_name);  
    GP.TD();
    GP.LABEL(tmp.command_code);

  }
  GP.TABLE_END();
  GP.NAV_BLOCK_END();
 /////////////////////////////////////////////////////////////////////
  GP.NAV_BLOCK_BEGIN("navA", 1);

  GP.LABEL("Scan");
  GP.HR(GP_GREEN);
  GP.FORM_BEGIN("/scan");                 // начать форму с именем (имя)
  GP.TEXT("device", "Device");
  GP.TEXT("command_name", "Command_name");
  GP.BREAK();
  GP.LABEL("Code Scan");
  GP.SUBMIT("Scan");
  //GP.BUTTON("scan","Scan");  
  GP.FORM_END(); 
  
  GP.NAV_BLOCK_END();
/////////////////////////////////////////////////////////////////
  GP.NAV_BLOCK_BEGIN("navA", 2);

  GP.TITLE("Code manager");
  GP.TABLE_BEGIN();

  GP.TR();
  GP.TD();
  GP.LABEL("Device");
  GP.TD();
  GP.LABEL("C name");
  GP.TD();
  GP.LABEL("IR Code");
  GP.TD();
  GP.LABEL("protocol");
  GP.TD();
  GP.LABEL("code uint64");
  GP.TD();
  GP.LABEL("size");
  GP.TD();
  GP.LABEL("Action");
  GP.TD();

  for (int i = 0; i < myList.size();i++ ){

    Database tmp = myList.get(i);
    GP.TR();
    GP.TD();
    GP.LABEL(tmp.Device);
    GP.TD();
    GP.LABEL(tmp.command_name);
    GP.TD();
    GP.LABEL(tmp.command_code);
    GP.TD();
    GP.LABEL(String(tmp.protocol));
    GP.TD();
    GP.LABEL("" + String(getUInt64fromHex(tmp.command_code)));
    GP.TD();
    GP.LABEL(String(tmp.size));
    GP.TD();


    GP.CONFIRM("delete_"+ String(i), "Are you confirm deleting " + String(myList.get(i).command_name) + "?");
    GP.BUTTON("del" + String(i),"delete");  
    GP.UPDATE_CLICK("delete_"+ String(i), "del" + String(i));
  
  }
  GP.TABLE_END();

  GP.NAV_BLOCK_END();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  GP.NAV_BLOCK_BEGIN("navA", 3);
  GP.LABEL("Short Instruction");

  GP.NAV_BLOCK_END();

  }

  GP.BUILD_END();
}

void action2(GyverPortal& p) {

  if (p.form("/scan")) {      // кнопка нажата
    p.copyStr("device", db.Device);  // копируем себе
    p.copyStr("command_name", db.command_name);
    //run scanner
    //scanned = "FFFFFF";
    add_command_db(db.Device,db.command_name,scanned,db.protocol,db.size, db.state, true);

    //db.command_code = scanned;
    //add command
    
   }

  if (ui.update()) {
      //Serial.println("update");
      ui.answer(1);
  }

  bool flag = ui.getBool();

  if (ui.click()) {
    String str = ui.clickName();

    if (ui.clickName().startsWith("run_command_")){

      str.replace("run_command_","");
      int pos = str.toInt();

      Database tmp = myList.get(pos);
      uint64_t command = getUInt64fromHex(tmp.command_code);


      Serial.println("---SEND------------------");
    
      Serial.println(tmp.protocol);
      
      for (int i = 0; i < 53;i++){
        Serial.print(tmp.state[i]);
      }
      Serial.println(" ");
      Serial.println(command);
      Serial.println(tmp.size);
      Serial.println("---SEND---END----");


      send_IR(tmp.protocol, tmp.state,command,tmp.size);
      Serial.println("----run------");
      /*Serial.println("----run------");
      Serial.println(tmp.Device);
      Serial.println(tmp.command_name);
      Serial.println(tmp.command_code);
      Serial.println("----run------");*/

      //run IR sender here
      //if successful output the alert
    }

    if (ui.clickName().startsWith("delete_") && flag == 1){

      str.replace("delete_","");
      int pos = str.toInt();
      Database tmp = myList.get(pos);
      //delete command from database here
      Serial.println("delete!!!!");
      delete_command_db(tmp.Device, tmp.command_name);
      //pop up
    }
    Serial.println(ui.clickName());
  }
}









