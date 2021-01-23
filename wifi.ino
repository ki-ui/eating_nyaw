//const char SSID[] = "aterm-2.4Ghz";
//const char PASSWORD[] = "neko4719A";
const char SSID[] = "rakuten mini";
const char PASSWORD[] = "aaaaaaaa";

void wifi_init(){
    Serial.println("WiFi connecting...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD); 

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println(" connected");
}
