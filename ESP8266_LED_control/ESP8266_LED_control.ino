// ESP8266

//Libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>  //https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h

//Constants
#define LED     D4
#define inter   client.println("<td style='color: grey; background-color: grey'>.</td>")
#define t_d     client.println("<hr><table  style='background-color: lightgrey' border='1'>")
#define t_f     client.println("</table>")

#define anaRval(analogpin,analogPIN)        client.println("<td>" + analogpin + "</td><td><input value=" \
                                            + analogRead(analogPIN) + " size='1'></input></td>")
                                  
#define anaRcur(anapin, anaPIN, max)        client.println("<td><input type='range' name=" + anapin + " id=" \
                                            + anapin + " min='0' max=" + max + " value=" + analogRead(anaPIN) + \
                                            " size='1'></td>")

#define anaW(anapin, anaPIN, min, max)   client.println("<form action=/" + anapin + "><td>" + anapin + "</td> \
                                            <td><input type='number' name=" + anapin + " id=" + anapin +  " \
                                            min=" + min + " max=" + max + " value=" + anaPIN + " size='1'></td> \
                                            <td><input value=" + anaPIN + " size='1'></input></td> \
                                            <td><input type='range' name=" + anapin + " id=" + anapin +  " \
                                            min=" + min + " max=" + max + " value=" + anaPIN + " size='1'></td> \
                                            <td><input type='submit' value='Envoyer'></input></td></form>")

#define anaWval(anapin, anaPIN, min, max)   client.println("<form action=/" + anapin + "><td>" + anapin + "</td> \
                                            <td><input type='number' name=" + anapin + " id=" + anapin +  " \
                                            min=" + min + " max=" + max + " value=" + anaPIN + " size='1'></td> \
                                            <td><input value=" + anaPIN + " size='1'></input></td> \
                                            <td><input type='submit' value='Envoyer'></input></td></form>")

#define anaWcur(anapin, anaPIN, min, max)   client.println("<form action=/" + anapin + "><td>" + anapin + "</td> \
                                            <td><input type='range' name=" + anapin + " id=" + anapin +  " \
                                            min=" + min + " max=" + max + " value=" + anaPIN + " size='1'></td> \
                                            <td><input value=" + anaPIN + " size='1'></input></td> \
                                            <td><input type='submit' value='Envoyer'></input></td></form>")



#define BP(digitalPIN)                      client.println("<td>" + digitalPIN + "</td><td><a href=/" + digitalPIN + "on> \
                                            <button style='background-color: lightgreen'>1</button></a></td> \
                                            <td><a href=/" + digitalPIN + "off> \
                                            <button style='background-color: LightCoral'>0</button></a></td>")

#define test(digitalpin)                    if (digitalRead(digitalpin)) { \
                                              client.print("<td style='background-color: lightgreen'>1</td>");} \
                                            else {client.print("<td style='background-color: lightCoral'>0</td>");}

//variables
String request;
String nom = "ESP8266";
char* ssid = "alcatel IDOL 4";
char* password = "eudp6503";
int pwm0 = 0;
int pwm1 = 0;
int pwm2 = 0;

//Objects
WiFiServer server(80);

void setup() {
  //Init Serial USB
  Serial.begin(115200);
  Serial.println(F("Initialize System"));
  //Init ESPBrowser
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  // Connect to Wifi network.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  server.begin();
  Serial.println();
  Serial.println(F("ESPBrowser initialized"));
  Serial.print(F("IP Address: "));
  Serial.println(WiFi.localIP());
  //Init WiFi server
  server.begin();
  pinMode(LED, OUTPUT);
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  // Read the first line of the request
  if (client){
    String request = client.readStringUntil('\r');
    client.flush();
    handleRequest(request);
    webpage(client);  //Return webpage
    Serial.println("Last page");
  }
}

// function handleRequest 
void handleRequest(String request) {
  //Handle web client request
  if (request.indexOf("/LEDon") > 0) {
    digitalWrite(LED, HIGH);  // light when low
  }
  if (request.indexOf("/LEDoff") > 0) {
    digitalWrite(LED, LOW); // light when low
  }    
  if (request.indexOf("/D4on") > 0) {
    digitalWrite(D4, HIGH);
  }
  if (request.indexOf("/D4off") > 0) {
    digitalWrite(D4, LOW);
  }
  if (request.indexOf("/PWM0") > 0) {
    pwm0 = extractVal(request);
    Serial.println("*******");
    Serial.print("PWM0 = ");
    Serial.println(pwm0);
    Serial.println("*******");
  }
  if (request.indexOf("/PWM1") > 0) {
    pwm1 = extractVal(request);
    Serial.println("*******");
    Serial.print("PWM1 = ");
    Serial.println(pwm1);
    Serial.println("*******");
  }
  if (request.indexOf("/PWM2") > 0) {
    pwm2 = extractVal(request);
    Serial.println("*******");
    Serial.print("PW21 = ");
    Serial.println(pwm2);
    Serial.println("*******");
  }
}

// function extract string request as int
int extractVal(String request){
    int colonPosition1 = request.indexOf('=');
    int colonPosition2 = request.indexOf('H');
    String temp;
    int extract_val;
    for (int i = colonPosition1 + 1; i < colonPosition2 - 1; i++){
      temp += (char)request.charAt(i);
    }
    extract_val = temp.toInt();
    Serial.println("+++++++");
    Serial.println(request);
    Serial.println("+++++++");
    Serial.println(extract_val);
    Serial.println("+++++++");
    return extract_val;
}

// function webpage 
void webpage(WiFiClient client) {
  // Send wbepage to client
  // https://developer.mozilla.org/fr/docs/Web/HTML/Element/
  Serial.println(request);
  String digitalPIN;
  String analogPIN;
  int pinName;
  int pwmValue;
  int min;
  int max;
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");  // do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta http-equiv='refresh' content='1'>");  // auto update time ; 1s
  client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
  client.println("<meta name='apple-mobile-web-app-status-bar-style' content=''grey'-translucent' />");
  client.println("</head>");
  client.println("<body style='background-color: grey'>");
  client.println("<hr>");
  client.println("<h1 style='color : #3AAA35;'><center><strong> " + nom + " </strong> Device Control </center></h1>");
  client.println("<hr>");
  client.println("<br>");
  
  // Analog
  t_d;
  //A0
  analogPIN = "A0";
  pinName = A0;
  max = 1023;
  anaRval(analogPIN, pinName);
  anaRcur(analogPIN,pinName, max);
  t_f;
  
  // PWM
  t_d;
  // PWM0 
  analogPIN = "PWM0";
  pwmValue = pwm0;
  min = 0;
  max = 255;
  anaWcur(analogPIN, pwmValue, min, max);   
  //anaWval(analogPIN, pwmValue, min, max);
  //anaW(analogPIN, pwmValue, min, max);
  /*client.println("<form action='/PWM0'><td>PWM0</td>");
  client.println("<td><input value=" + String(pwm0) + " readonly size='1'></input></td>");
  client.println("<td><input type='range' name='pwm0' id='pwm0' min='0' max='255' value='" + String(pwm0) + "' size='1'></td>");
  client.println("<td><input type='submit' value='Envoyer'></input></td></input></td></form>");*/
  // PWM1
  inter;
  analogPIN = "PWM1";
  pwmValue = pwm1;
  min = 0;
  max = 255;
  //anaWcur(analogPIN, pwmValue, min, max);   
  anaWval(analogPIN, pwmValue, min, max);
  //anaW(analogPIN, pwmValue, min, max);
  t_f;
  t_d;
  // PWM2
  analogPIN = "PWM2";
  pwmValue = pwm2;
  min = 0;
  max = 255;
  //anaWcur(analogPIN, pwmValue, min, max);   
  //anaWval(analogPIN, pwmValue, min, max);
  anaW(analogPIN, pwmValue, min, max);
  t_f;
  
  // Digital
  t_d;
  //  Built-in LED (D4)
  digitalPIN = "LED"; // action
  pinName = LED;
  BP(digitalPIN);
  test(pinName);
  //D0
  inter;
  digitalPIN = "D0"; // action
  pinName = D0;
  BP(digitalPIN);
  test(pinName);
  //  D1
  inter;
  digitalPIN = "D1"; // action
  pinName = D1;
  BP(digitalPIN);
  test(pinName);
//  D2
  inter;
  digitalPIN = "D2"; // action
  pinName = D2;
  BP(digitalPIN);
  test(pinName);
//  D3
  inter;
  digitalPIN = "D3"; // action
  pinName = D3;
  BP(digitalPIN);
  test(pinName);
//  D4
  inter;
  digitalPIN = "D4"; // action
  pinName = D4;
  BP(digitalPIN);
  test(pinName);
  t_f;
  client.println("</body></html>");
  client.println();
  delay(1);
}