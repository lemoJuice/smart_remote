


void loginPortal() {
  Serial.println("Captive start");

  // запускаем точку доступа
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP("WiFi Config");

  // запускаем портал
  captive.attachBuild(build_captive);
  captive.start();
  captive.attach(action);

  // работа портала
  while (captive.tick());
}

void action(GyverPortal& p) {

  if (p.form("/auth")) {      // кнопка нажата
    p.copyStr("lg", lp.ssid);  // копируем себе
    p.copyStr("ps", lp.pass);
    EEPROM.put(0, lp);              // сохраняем
    EEPROM.commit();                // записываем

    //delay(2000);
    reconnect_wifi();
    main_ip = WiFi.localIP().toString();
    //delay(20000);
    captive.stop();
    Serial.println("Captive stop"); 
  }
}

void build_captive(){
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);
  GP.PAGE_TITLE("Smart remote");   

  
  if (captive.uri("/portal")){

    GP.TITLE("Please Select the Net","m1",GP_WHITE,0);
    int numSsid = WiFi.scanNetworks();

    for (int thisNet = 0; thisNet < numSsid; thisNet++) {
      GP.HR(GP_GREEN);
      String ssd;
      ssd += WiFi.SSID(thisNet) + "  Signal: " + WiFi.RSSI(thisNet) + " dBm";
       
      GP.BOX_BEGIN(GP_JUSTIFY, "100%");
      GP.LABEL(ssd,"m2",GP_WHITE,0);
      GP.BREAK(); 
      GP.BOX_END();
      GP.FORM_BEGIN("/login_" + String(thisNet));
      GP.SUBMIT("Select Net");
      GP.FORM_END();
      GP.HR(GP_GREEN);  
    }
  } 

  if (captive.uri().startsWith("/login_")){

    GP.TITLE("Please Input Pass","m",GP_WHITE,0);
    String url = captive.uri();
    url.replace("/login_","");
    Serial.println(url);
    Serial.println(WiFi.SSID(url.toInt()) +"");
    WiFi.SSID(url.toInt()).toCharArray(lp.ssid,20);
    String str = "";
    str.toCharArray(lp.pass,20);
    GP.FORM_BEGIN("/auth");
    GP.TEXT("lg", "Login", lp.ssid);
    GP.BREAK();
    GP.TEXT("ps", "Password", lp.pass);
    //GP.TEXT("ip", "IP-Address", main_ip);
    //GP.UPDATE("ip",10);
    GP.SUBMIT("Submit");
    GP.FORM_END();
    GP.BUTTON_LINK("/portal", "Back");

  }

  if (captive.uri("/auth")){

    GP.LABEL("Successfully Connected!");
    GP.LABEL("Use following IP address:");
    GP.TEXT("ip", "IP-Address", main_ip);
    GP.UPDATE("ip",10);

  }

  GP.BUILD_END();
}

void reconnect_wifi(){

  WiFi.disconnect();
  Serial.print("Connect to: ");
  Serial.println(lp.ssid);
  //WiFi.mode(WIFI_STA);
  WiFi.begin(lp.ssid, lp.pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! Local IP: ");
  Serial.println(WiFi.localIP());
  }


void reset_wifi_settings(){

  Serial.println("Reset wi-fi settings");
  lp.isFirstStart = 13;
  EEPROM.put(0, lp);              // сохраняем
  EEPROM.commit();
  delay(100);                // записываем
  Serial.println("Reset wi-fi settings successfull");
  ESP.restart();

}


