#include <HTTPClient.h>               //Download: https://electronoobs.com/eng_arduino_httpclient.php
#include <WiFi.h>  

float q,V,cc,ocv;
const char* ssid = "bryan";              //Add your WIFI network name 
const char* password =  "ioioioio"; 
int fwd_cnst=512,dir_cnst=512;
unsigned int Actual_Millis, Previous_Millis;
char c = 'a',garbage;
String str="",data_to_send = "",forward_constant="",direction_constant="";
float inclination,ambient_temp,speed,weight,battery_level,battery_temp,timee,battery_rating=10000.0,charging_cycle,acceleration,battery_voltage,battery_current,humidity;
float speed_main;
double converted;
TaskHandle_t Task1;

int strtoint(String str){
  int integer = 0,i=0;

  while(str[i]!='\0'){
    integer=(integer*10)+(str[i]-48);
    i++;
  }
  return integer;
}

void Task1code( void * parameter) {
  for(;;) {
    while(Serial2.available()>0)
    {garbage = Serial2.read();}

    do{
      c=Serial2.read();
    }while(c != '&');
    inclination= Serial2.parseFloat();
    ambient_temp= Serial2.parseFloat();
    speed= Serial2.parseFloat();
    weight= Serial2.parseFloat();
    battery_level= Serial2.parseFloat();
    battery_temp= Serial2.parseFloat();
    timee= Serial2.parseFloat();
    battery_rating= Serial2.parseFloat();
    charging_cycle= Serial2.parseFloat();
    acceleration= Serial2.parseFloat();
    battery_voltage= Serial2.parseFloat();
    battery_current= Serial2.parseFloat();
    humidity = Serial2.parseFloat();
    q = Serial2.parseFloat();
    V = Serial2.parseFloat();
    cc = Serial2.parseFloat();
    ocv = Serial2.parseFloat();
    c='a';
    delay(200);
  }
}

void setup() {
  // put your setup code here, to run once:
  delay(10);

  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(27,OUTPUT);
  pinMode(33,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(35,OUTPUT);
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);
  digitalWrite(27,LOW);
  digitalWrite(33,LOW);
  digitalWrite(26,HIGH);
  digitalWrite(35,HIGH);
  Serial.begin(115200);
  Serial2.begin(115200);
  xTaskCreatePinnedToCore(
    Task1code, /* Function to implement the task */
    "Task1", /* Name of the task */
    10000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    1,  /* Priority of the task */
    &Task1,  /* Task handle. */
    0
  );

  WiFi.begin(ssid, password);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(500);
    Serial.print(".");
  }

  Serial.print("Connected, my IP: ");
  Serial.println(WiFi.localIP());
  Actual_Millis = millis();               //Save time for refresh loop
  Previous_Millis = Actual_Millis;
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(26,HIGH);
  digitalWrite(35,HIGH);
  Actual_Millis = millis();
  if(Actual_Millis - Previous_Millis > 1000){
    Previous_Millis = Actual_Millis;  
    if(WiFi.status()== WL_CONNECTED){                   //Check WiFi connection status  
      HTTPClient http;                                  //Create new client
      
      data_to_send = "inclination=" + (String)inclination + "&" + "ambient_temp=" + (String)ambient_temp + "&" + "speed=" + (String)speed + "&" + "weight=" + (String)weight + "&" + "battery_level=" + (String)battery_level + "&" + "battery_temp=" + (String)battery_temp + "&" + "time=" + (String)timee + "&" + "battery_rating=" + (String)battery_rating + "&" + "charging_cycle=" + (String)charging_cycle + "&" + "acceleration=" + (String)acceleration +"&battery_voltage=" + (String)battery_voltage + "&battery_current=" + (String)battery_current +"&humidity=" + (String)humidity +"&q=" + (String)q+"&V=" + (String)V+"&cc=" + (String)cc+"&ocv=" + (String)ocv;
      //Begin new connection to website       
      http.begin("https://ioioioio.in/ind.php");   //Indicate the destination webpage 
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");         //Prepare the header
      
      int response_code = http.POST(data_to_send);                                //Send the POST. This will giveg us a response code
      
      //If the code is higher than 0, it means we received a response
      if(response_code > 0){
        Serial.println(data_to_send);
        Serial.println("HTTP code " + String(response_code));                     //Print return code
      }//END of response_code > 0
      
      else{
       Serial.print("Error sending POST, code: ");
       Serial.println(response_code);
      }

      data_to_send = "forward_constant=";
      
      response_code = http.POST(data_to_send);                                //Send the POST. This will giveg us a response code
      
      //If the code is higher than 0, it means we received a response
      if(response_code > 0){
        Serial.println(data_to_send);
        Serial.println("HTTP code " + String(response_code));                     //Print return code
  
        if(response_code == 200){                                                 //If code is 200, we received a good response and we can read the echo data
          forward_constant = http.getString();                                //Save the data comming from the website
          Serial.print("Server reply fwd_cnst: ");                                         //Print data to the monitor for debug
          Serial.println(forward_constant);
          

        }//End of response_code = 200
      }//END of response_code > 0


      data_to_send = "direction_constant=";

      response_code = http.POST(data_to_send);                                //Send the POST. This will giveg us a response code
      
      //If the code is higher than 0, it means we received a response
      if(response_code > 0){
        Serial.println(data_to_send);
        Serial.println("HTTP code " + String(response_code));                     //Print return code
  
        if(response_code == 200){                                                 //If code is 200, we received a good response and we can read the echo data
          direction_constant = http.getString();                                //Save the data comming from the website
          Serial.print("Server reply dir_cnst: ");                                         //Print data to the monitor for debug
          Serial.println(direction_constant);
          

        }//End of response_code = 200
      }//END of response_code > 0
      
      else{
       Serial.print("Error sending POST, code: ");
       Serial.println(response_code);
      }
      http.end();  
      
      fwd_cnst = strtoint(forward_constant);
      dir_cnst = strtoint(direction_constant);   
      
      Serial.println(fwd_cnst);
      Serial.println(dir_cnst);     
    }//END of WIFI connected
    else{
      Serial.println("WIFI connection error");
    }
  }
}