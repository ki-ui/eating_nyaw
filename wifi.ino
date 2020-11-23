const char SSID[] = "";
const char PASSWORD[] = "";

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
