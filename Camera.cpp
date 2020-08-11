#include "Camera.h"

Camera::Camera(){
  
}

void Camera::camera1Trigger(){
  downloadAndSaveFile("/snapshot.jpg","http://"+ IPcam1 +"/webcapture.jpg?command=snap&channel1=0");
  for (int i = 0; i < 2; i++){
    Serial.println("Send image " + i +1);
    LINE.setToken(tokens[i]);
    LINE.notifyPicture("Sensor 1 triggered", SPIFFS, "/snapshot.jpg");  
  }
}

void Camera::camera2Trigger(){
  downloadAndSaveFile("/snapshot.jpg","http://"+ IPcam2 +"/webcapture.jpg?command=snap&channel1=0");
  for (int i = 0; i < 2; i++){
    Serial.println("Send image " + i +1);
    LINE.setToken(tokens[i]);
    LINE.notifyPicture("Sensor 2 triggered", SPIFFS, "/snapshot.jpg");  
  }
}

void Camera::downloadAndSaveFile(String fileName, String  url){
  
  HTTPClient http;

  Serial.println("[HTTP] begin...\n");
  Serial.println(fileName);
  Serial.println(url);
  http.begin(url);
  
  Serial.printf("[HTTP] GET...\n", url.c_str());
  // start connection and send HTTP header
  int httpCode = http.GET();
  if(httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      Serial.printf("[FILE] open file for writing %d\n", fileName.c_str());
      
      File file = SPIFFS.open(fileName, "w");

      // file found at server
      if(httpCode == HTTP_CODE_OK) {

          // get lenght of document (is -1 when Server sends no Content-Length header)
          int len = http.getSize();

          // create buffer for read
          uint8_t buff[128] = { 0 };

          // get tcp stream
          WiFiClient * stream = http.getStreamPtr();

          // read all data from server
          while(http.connected() && (len > 0 || len == -1)) {
              // get available data size
              size_t size = stream->available();
              if(size) {
                  // read up to 128 byte
                  int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
                  // write it to Serial
                  //Serial.write(buff, c);
                  file.write(buff, c);
                  if(len > 0) {
                      len -= c;
                  }
              }
              delay(1);
          }

          Serial.println();
          Serial.println("[HTTP] connection closed or file end.\n");
          Serial.println("[FILE] closing file\n");
          file.close();
          
      }
      
  }
  http.end();
}
