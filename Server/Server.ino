#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <FS.h>

char* ssid = "0";
char* password = "0";
String Str_ssid = "";
String Str_password = "";
String showIP = "";
String Str_ip = "";
#define ssidAddr 0
#define passwordAddr 30
int ssidLen;
int passwordLen;
#define ssidLenAddr 60
#define passwordLenAddr 61

IPAddress apIP(6, 6, 6, 6);
//IPAddress myIP;
ESP8266WebServer server(80);

String getContentType(String filename) {
  if (server.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

const int led = 13;
String html = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><style>  body {    color:#434343;    font-family:\"Helvetica Neue\",Helvetica,Arial,sans-serif;    font-size:14px;    line-height:1.42857142857143;    padding:20px;  }  .container {    margin:0 auto;    max-width:350px;  }  form .field-group {    box-sizing:border-box;    clear:both;    padding:4px 0;    position:relative;    margin:1px 0;    width:100%;  }  form .field-group > label {    color:#757575;    display:block;    margin:0 0 5px 0;    padding:5px 0 0;    position:relative;    word-wrap:break-word;  }  input[type=text] {    background:#fff;    border:1px solid #d0d0d0;    border-radius:2px;    box-sizing:border-box;    color:#434343;    font-family:inherit;    font-size:inherit;    height:2.5em;    line-height:1.4285714285714;    padding:4px 5px;    margin:0;    width:100%;  }  @media all and (orientation :portrait){.container{padding-top:0px}} input[type=text]:focus {    border-color:#2ECC71;    outline:0;  }  .button-container {    box-sizing:border-box;    clear:both;    margin:1px 0 0;    padding:4px 0;    position:relative;    width:100%;  }  button[type=submit] {    box-sizing:border-box;    background:#f5f5f5;    border:1px solid #bdbdbd;    border-radius:2px;    color:#434343;    cursor:pointer;    display:block;    font-family:inherit;    font-size:14px;    font-variant:normal;    font-weight:400;    height:2.14285714em;    line-height:1.42857143;    margin:30px auto;    padding:4px 30px;    text-decoration:none;    vertical-align:baseline;    white-space:nowrap;  }  #right-div{      position: fixed;      bottom: 3%;      left: calc(50% - 100px);      width: 200px;      font-size: 12px;      line-height:1;      color: #999;    }    .right{      text-align: center;    }  @media (max-height: 400px) {    #right-div{      display: none;    }  }</style></head><body><div class=\"container\"><h1 style=\"text-align: center;\">WiFi &#x914D;&#x7F6E;</h1><form method=\"get\" action=\"./pin\"><div class=\"field-group\"><label>SSID</label><input name=\"ssid\" type=\"text\" length=32></div><div class=\"field-group\"><label>Password</label><input name=\"password\" type=\"text\" length=64></div><div class=\"button-container\"><button type=\"submit\">&#x8FDE;&#x63A5;</button></div></form></div><div id=\"right-div\"><p class=\"right\">© CopyRight Tasker&#x5DE5;&#x4F5C;&#x5BA4;</p><p class=\"right\">QQ&#x7FA4;&#xFF1A;652946621</p></div></body><script></script></html>";
void handleRoot() {
  Serial.println(EEPROM.read(passwordLenAddr));
  if (EEPROM.read(passwordLenAddr) > 7) {   
    File file = SPIFFS.open("/index.html", "r");
    size_t sent = server.streamFile(file, "text/html");
    file.close(); 
    return;
  } else {
    server.send(200, "text/html", html);
    return;
  }
}

String re_ip() {
  showIP.concat("<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF-8\">  <title>(&#xFFE3;&#x25BD;&#xFFE3;)&#xFF0F;</title> <style>   body {      color: #434343;     font-family: \"Helvetica Neue\",Helvetica,Arial,sans-serif;     font-size: 34px;      line-height: 1.42857142857143;      padding: 20px;    }   .container {      margin: 0 auto;     max-width: 90%;     text-align: center;   }   .ip{      margin-top: 2em;    }   .button-container {            box-sizing: border-box;            clear: both;            margin: 1px 0 0;            padding: 4px 0;            position: relative;            width: 4em;        }    input[type=submit] {      box-sizing: border-box;     background: #f5f5f5;      border: 1px solid #bdbdbd;      border-radius: 2px;     color: #434343;     cursor: pointer;      display: block;     font-family: inherit;     font-size: 14px;      font-variant: normal;     font-weight: 400;     height: 2.14285714em;     line-height: 1.42857143;      margin: 30px auto;      padding: 4px 30px;      text-decoration: none;      vertical-align: baseline;     white-space: nowrap;    }   input{      font-size: 40px;    } </style></head><body> <div class=\"container\">   <h1 style=\"text-align: center;\">&#x5DF2;&#x6210;&#x529F;&#x8FDE;&#x63A5;&#x5230;");
  showIP.concat(ssid);
  showIP.concat("</h1>   <h2 class=\"ip\">IP&#x5730;&#x5740;</h2>    <h2><a href=\"http://");
  showIP.concat(WiFi.localIP()[0]);
  showIP.concat(".");
  showIP.concat(WiFi.localIP()[1]);
  showIP.concat(".");
  showIP.concat(WiFi.localIP()[2]);
  showIP.concat(".");
  showIP.concat(WiFi.localIP()[3]);
  showIP.concat("\">");
  showIP.concat(WiFi.localIP()[0]);
  showIP.concat(".");
  showIP.concat(WiFi.localIP()[1]);
  showIP.concat(".");
  showIP.concat(WiFi.localIP()[2]);
  showIP.concat(".");
  showIP.concat(WiFi.localIP()[3]);
  showIP.concat("</a></h2><br><h3>WiFi&#x70ED;&#x70B9;&#x5373;&#x5C06;&#x5173;&#x95ED;&#xFF0C;&#x8BF7;&#x8FDE;&#x63A5;&#x4E0A;&#x4F60;&#x81EA;&#x5DF1;&#x7684;WiFi</h3><h3>&#x4F60;&#x4EE5;&#x540E;&#x53EF;&#x4EE5;&#x901A;&#x8FC7;&#x8BBF;&#x95EE;&#x8FD9;&#x4E2A;IP&#x5730;&#x5740;&#x8FDB;&#x5165;&#x63A7;&#x5236;&#x9875;&#x9762;<br>&#x8BD5;&#x4E00;&#x4E0B;&#x5427;</h3><br></div></body></script></html>");
  return showIP;
}

void pin() {
  if (server.hasArg("ip")) {
    server.send(200, "text/html", re_ip());
    showIP = "";
//    delay(18000);
//    WiFi.mode(WIFI_STA);
  } else if (server.hasArg("ssid") && server.hasArg("password")) {
    if (ssid == "0" && password == "0") {
      //      WiFi.mode(WIFI_STA);
      Str_ssid = server.arg("ssid");//获取用户输入的ssid赋值给String变量
      Str_password = server.arg("password");
      //      Serial.println(Str_ssid);
      //      Serial.println(Str_password);
      ssidLen = Str_ssid.length();//获取ssid String变量长度  （int）
      passwordLen = Str_password.length();
      //      Serial.println(passwordLen);
      EEPROM.write(ssidLenAddr, ssidLen);
      EEPROM.write(passwordLenAddr, passwordLen);
      //      Serial.println(EEPROM.read(ssidLenAddr));
      //      Serial.println(EEPROM.read(passwordLenAddr));
      Serial.println();
      //      Serial.println();
      for (int i = 0; i < ssidLen; i++) {
        EEPROM.write(ssidAddr + i, Str_ssid[i]);
        //        Serial.println(EEPROM.read(ssidAddr + i));
      }
      for (int i = 0; i < passwordLen; i++) {
        EEPROM.write(passwordAddr + i, Str_password[i]);
        //        Serial.println((char)EEPROM.read(passwordAddr + i));
      }
      //      server.send(200, "text/html", "ok");
      ssid = "";
      password = "";
      for (int i = 0; i < EEPROM.read(ssidLenAddr); i++) {
        ssid += (char)EEPROM.read(ssidAddr + i);
        //        Serial.println(ssid);
      }
      for (int i = 0; i < EEPROM.read(passwordLenAddr); i++) {
        password += (char)EEPROM.read(passwordAddr + i);
        //        Serial.println(password);
      }
      //      Serial.println(Str_ssid.c_str());
      //      Serial.println(Str_password.c_str());
      //            ssid = (char*)Str_ssid.c_str();
      //            password = (char*)Str_password.c_str();
      ssid = (char*)Str_ssid.c_str();
      password = (char*)Str_password.c_str();
      EEPROM.commit();
      Serial.println("SSID: " + Str_ssid);
      Serial.println("Password: " + Str_password);
      server.send(200, "text/html", "<!DOCTYPE html><html><head>     <meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><style>            body {                color: #434343;                font-family: \"Helvetica Neue\",Helvetica,Arial,sans-serif;                font-size: 14px;                line-height: 1.42857142857143;                padding: 20px;                margin-top: 100px;            }</style></head><body>     <center>        <h1>&#x6B63;&#x5728;&#x8FDE;&#x63A5;&#xFF0C;&#x8BF7;&#x7A0D;&#x7B49;&#x3002;&#x3002;&#x3002;</h1>     </center></body><script> window.setTimeout(\"location='./pin?ip=show'\", 5000);</script></html>" );
      WiFi.begin(ssid, password);
      Serial.print("WiFi Connecting");
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
//      WiFi.mode(WIFI_STA);
      //      ip = (String)WiFi.localIP();
      //      long ttt = (int)WiFi.localIP();
      //      long longIP = WiFi.localIP();
      //      long y = longIP % 256;
      //      long m = (longIP - y) / (256 * 256 * 256);
      //      long n = (longIP - 256 * 256 * 256 * m - y) / (256 * 256);
      //      long x = (longIP - 256 * 256 * 256 * m - 256 * 256 * n - y) / 256;
      //      String ip = "";
      //      ip.concat((String)y);
      //      ip.concat(".");
      //      ip.concat((String)n);
      //      ip.concat(".");
      //      ip.concat((String)x);
      //      ip.concat(".");
      //      ip.concat((String)m);
      //      showIP.concat("<center><h1>IP Address: ");
      //      showIP.concat(ip);
      //      showIP.concat("</h1></center>");
      Serial.println("");
      Serial.println("connected successfully");
      //    Serial.println(ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      Str_ip.concat(WiFi.localIP()[0]);
      Str_ip.concat(".");
      Str_ip.concat(WiFi.localIP()[1]);
      Str_ip.concat(".");
      Str_ip.concat(WiFi.localIP()[2]);
      Str_ip.concat(".");
      Str_ip.concat(WiFi.localIP()[3]);
      //      server.send(200, "text/html", "IP address:" + WiFi.localIP() );
    }
  } else if (server.hasArg("light")) {
    if (server.arg("light") == "ON") {
      digitalWrite(D4, LOW);
      digitalWrite(D2, HIGH);
      server.send(200, "text/html", "on"); return;
    } else if (server.arg("light") == "OFF") {
      digitalWrite(D4, HIGH);
      digitalWrite(D2, LOW);
      server.send(200, "text/html", "off"); return;
    }
  } else {
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("ESP8266");
  }
}



void handleNotFound() {
  if ((Str_ip != "0.0.0.0") || (Str_ip != "")) {
    String path = server.uri();
    Serial.print("load url:");
    Serial.println(path);
    String contentType = getContentType(path);
    String pathWithGz = path + ".gz";
    if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
      if (SPIFFS.exists(pathWithGz))
        path += ".gz";
      File file = SPIFFS.open(path, "r");
      size_t sent = server.streamFile(file, contentType);
      file.close();
      return;
    }
    digitalWrite(led, 1);
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
  }
}

void setup(void) {
  pinMode(D4, OUTPUT);
  //  WiFi.mode(WIFI_AP_STA);
  SPIFFS.begin();
  EEPROM.begin(512);
  //  for (int i = 0; i < 512; i++) {
  //    EEPROM.write(i, 0);
  //  }
  //  EEPROM.commit();
//  Serial.println(EEPROM.read(passwordLenAddr));
  Serial.begin(115200);
  //  WiFi.begin(ssid, password);
  Serial.println("");
  //    Serial.println(EEPROM.read(passwordLenAddr));

  //  for (int i = 0; i < EEPROM.read(ssidLenAddr); i++) {
  //    ssid += (char)EEPROM.read(ssidAddr + i);
  //    //        Serial.println(ssid);
  //  }
  if (EEPROM.read(passwordLenAddr) > 5) {
    //    Serial.println("SSID length: " + EEPROM.read(ssidLenAddr));
    //    Serial.println("Password length: " + EEPROM.read(passwordLenAddr));
    for (int i = 0; i < EEPROM.read(ssidLenAddr); i++) {
      Str_ssid += (char)EEPROM.read(ssidAddr + i);
    }
    for (int i = 0; i < EEPROM.read(passwordLenAddr); i++) {
      Str_password += (char)EEPROM.read(passwordAddr + i);
    }
    ssid = "";
    password = "";
    ssid = (char*)Str_ssid.c_str();
    password = (char*)Str_password.c_str();
    Serial.println("SSID: " + Str_ssid);
    Serial.println("Password: " + Str_password);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("WiFi Connecting");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("connected successfully");
    //    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
//    WiFi.softAPdisconnect();
    server.begin();
    Serial.println("HTTP server started");
    //    int ttt = WiFi.localIP()[0];
    //    Serial.println(ttt);
  } else if (ssid == "0" && password == "0") {
    Serial.println("");
    Serial.println("For the first time to use the ESP8266");
    Serial.println("You need configure it");
    Serial.println("Please Connect the WiFi and use a browser to access 192.168.1.1");
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("ESP8266");
    server.begin();
    Serial.println("HTTP server started");
  }

  server.on("/", handleRoot);
  server.on("/pin", HTTP_GET, pin);
  //  server.on("/testpin", HTTP_GET, testpin);
  server.onNotFound(handleNotFound);

//    server.begin();
  //  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
