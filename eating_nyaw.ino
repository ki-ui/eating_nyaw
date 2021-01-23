#include <WiFiClientSecure.h>
#include <esp_camera.h>
#include <FirebaseESP32.h>
#include "Base64.h"

#define DELAY 1 * 60 * 1000
#define SEND_SIZE 1000

String FIREBASE_HOST = "eating-nyaw.firebaseio.com";
String FIREBASE_AUTH = "uVxOmTcLhzcHdvtWp67XSvT8nEFv0K4DBGjQqpFU";

FirebaseData firebaseData;

void setup()
{
    Serial.begin(115200);
    wifi_init();
    camera_init();

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
    Firebase.setMaxRetry(firebaseData, 3);
    Firebase.setMaxErrorQueue(firebaseData, 30); 
    Firebase.enableClassicRequest(firebaseData, true);
    Firebase.setwriteSizeLimit(firebaseData, "unlimited");
}


void loop(){
    Serial.println("loop start");
    
    String photoPath = "/esp32-cam";
    String donePath = photoPath + "/Done";
    String base64 = Photo2Base64();

    // DBのデータを削除
    Firebase.deleteNode(firebaseData, photoPath);
    
    // base64の記述と完了フラグの記述
    for(int i = 0;;i++){
      int size = i * SEND_SIZE;
      String message = base64.substring(size, size + SEND_SIZE);

      // base64を記述
      if (setString(photoPath + "/" + i, message) == false){
        Serial.println("ERROR Restart");
        ESP.restart();
      }
        
      // base64の全内容が記述できたら、完了フラグを記述する
      if (message.length() < SEND_SIZE ){
        if (setString(donePath, "Done.") == false){
          Serial.println("ERROR Restart");
          ESP.restart();
        }
        break;
      }
    }

    Serial.println("loop end");
    delay(DELAY);
}

bool setString(String path, String message){
  if (Firebase.setString(firebaseData, path, message) == false){
    Serial.println("ERROR Reason");
    Serial.println(firebaseData.errorReason());
    return false;
  }
  
  return true;
}

String Photo2Base64() {
    camera_fb_t *fb = esp_camera_fb_get();  
    if (fb == NULL){
      Serial.println("Camera capture failed");
      return "";
    }
  
    String imageFile = "data:image/jpeg;base64,";
    char *input = (char *)fb->buf;
    char output[base64_enc_len(3)];
    for (int i=0;i<fb->len;i++) {
      base64_encode(output, (input++), 3);
      if (i%3==0) imageFile += urlencode(String(output));
    }
    esp_camera_fb_return(fb);
    
    return imageFile;
}

String urlencode(String str)
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
}
