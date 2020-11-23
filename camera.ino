// 上下反転する（コメントにするとしない）
#define VFLIP

// 左右反転する（コメントにするとしない）
#define HMIRROR

void camera_init(){
    //カメラ初期化
    Serial.println("camera initalizing...");
    camera_config_t config = {
        .pin_pwdn = -1,
        .pin_reset = 15,
        .pin_xclk = 27,
        .pin_sscb_sda = 22,
        .pin_sscb_scl = 23,
        .pin_d7 = 19,
        .pin_d6 = 36,
        .pin_d5 = 18,
        .pin_d4 = 39,
        .pin_d3 = 5,
        .pin_d2 = 34,
        .pin_d1 = 35,
        .pin_d0 = 32,
        .pin_vsync = 25,
        .pin_href = 26,
        .pin_pclk = 21,
        .xclk_freq_hz = 20000000,       // 20MHz
        .ledc_timer = LEDC_TIMER_0,     // 0番のタイマー使用
        .ledc_channel = LEDC_CHANNEL_0, // 0番のチャネル使用
        .pixel_format = PIXFORMAT_JPEG, // JPEG
        .frame_size = FRAMESIZE_UXGA,   // 解像度
        .jpeg_quality = 8,             // JPGE画質（小さいほど高画質）
        .fb_count = 1                   // フレームバッファ数（２つあれば２倍で処理できる？）
    };
    auto result = esp_camera_init(&config);
    if (result != ESP_OK)
    {
    Serial.printf("esp_camera_init error 0x%x\n", result);
    esp_restart();
    }

    #ifdef VFLIP
    {
        auto sensor = esp_camera_sensor_get();
        if (sensor != NULL)
        {
        sensor->set_vflip(sensor, 1); //上下反転
        }
    }
    #endif

    #ifdef HMIRROR
    {
        auto sensor = esp_camera_sensor_get();
        if (sensor != NULL)
        {
        sensor->set_hmirror(sensor, 1); //左右反転
        }
    }
    #endif

}
