#include <WiFiClientSecure.h>
#include <esp_camera.h>

void setup()
{
    // シリアル通信開始
    Serial.begin(9600);
    Serial.setDebugOutput(true);

    // WiFi接続
    wifi_init();

    // camera init
    camera_init();

    // line init
    line_init();
}

void loop(){
    // 写真を撮影
    camera_fb_t *frame = esp_camera_fb_get();
    if (frame == NULL){
      Serial.println("esp_camera_fb_get failed.");
    }
    Serial.printf("image size: %u\n", frame->len);

    send_line(frame);

    esp_camera_fb_return(frame);

    delay(1000);
//    delay(600000);
}
