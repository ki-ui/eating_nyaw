// LINE通知用トークン
#define LINE_TOKEN ""

// 通知メッセージ（必須です）
#define MESSAGE "CPU温度 %.1f"

// 区切り用のランダム文字列
#define BOUNDARY "123456789000000000000987654321"

// メッセージヘッダ
#define MESSAGE_HEADER "\r\n--" BOUNDARY "\r\nContent-Disposition: form-data; name=\"message\"\r\n\r\n"

// 画像ヘッダ
#define IMAGE_HEADER "\r\n--" BOUNDARY "\r\nContent-Disposition: form-data; name=\"imageFile\"; filename=\"image.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n"

// 最後の区切り
#define BOUNDARY_LAST "\r\n--" BOUNDARY "--\r\n"

void line_init(){

    
}

bool connect(WiFiClientSecure* client){
    for (auto t = millis(); millis() - t < 15000 && !client->connect("notify-api.line.me", 443);)
        ;
    if (!client->connected()){
        Serial.println("https connect failed.");
        return false;
    }

    return true;
}

bool send_line(camera_fb_t *frame){
    WiFiClientSecure client;
    if (connect(&client) == false)
        return false;
    
    char message[64];
    auto messageLength = snprintf(message, sizeof(message), MESSAGE, temperatureRead());
    auto contentLength = strlen(MESSAGE_HEADER) + messageLength + strlen(IMAGE_HEADER) + frame->len + strlen(BOUNDARY_LAST);
    client.println("POST /api/notify HTTP/1.0");
    client.println("Authorization: Bearer " LINE_TOKEN);
    client.println("Content-Type: multipart/form-data;boundary=" BOUNDARY);
    client.println("Content-Length: " + String(contentLength));
    client.println();

    client.write((uint8_t *)MESSAGE_HEADER, strlen(MESSAGE_HEADER));
    client.write((uint8_t *)message, messageLength);
    client.write((uint8_t *)IMAGE_HEADER, strlen(IMAGE_HEADER));
    
    auto p = frame->buf;
    auto rest = frame->len;
    while (rest > 0 && client.connected()){
        auto n = client.write(p, rest > 1024 ? 1024 : rest); // 一度に大きなサイズを書き込むと失敗する
        p += n;
        rest -= n;
    }
    client.write((uint8_t *)BOUNDARY_LAST, strlen(BOUNDARY_LAST));
    client.flush();
    while (client.connected()){
        client.readStringUntil('\n');
    }
    client.stop();

    return true;
}
