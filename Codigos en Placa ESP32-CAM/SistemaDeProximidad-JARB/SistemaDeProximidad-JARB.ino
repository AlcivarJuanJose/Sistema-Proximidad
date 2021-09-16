//Librerías para camara esp32, conexiónes wifi, etc...
#include <Arduino.h>
#include "WiFi.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"


//Especificamos credenciales de conexión a red
const char* ssid = "PrototipoSP";
const char* password = "12345678"; 


//Conexión con el servidor 
String nombreServidor = "192.168.100.36";   //dirección ip del servidor 
String pathServidor = "/subir.php";         //ruta del servidor 
const int puertoServidor = 80;              // puerto de conexión


//Asignación de pines de la placa esp32 
//a sensor ultrasónico, led, bocina y pulasador 
const int trigPin = 2;                 //pin trigger del sensor ultrasonico
const int echoPin = 13;                //pin echo del sensor ultrasonico
const int Led = 15;                    //led
const int Bocina = 4;                  //bocina
const int Pulsador = 14;               //pulsador 


//Crea un cliente que se puede conectar 
//a una dirección IP de Internet y un puerto especificados
WiFiClient client;


//Asignación de pines de la cámara OV2640 
//para la placa ESP32-CAM AI-Thinker
#define CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22


void setup() {
  //Proceso de conexión a red Wifi
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("ESP32-CAM Dirección Ip: ");     //imprimir dirección ip de esp32 en monitor serial
  Serial.println(WiFi.localIP());


  //Configuración de pines de la placa esp32Cam 
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  //Iniciar con especificaciones altas para asignar búferes más grandes
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA; //tamaño del marco de la imagen (1600 x 1200)
    config.jpeg_quality = 10;           //calidad de la fotografía
    config.fb_count = 2;                //recuento de fotogramas
  } else {
    config.frame_size = FRAMESIZE_VGA;  //tamaño del marco de la imagen (640 x 480)
    config.jpeg_quality = 12;           //calidad de la fotografía
    config.fb_count = 1;                //recuento de fotogramas
  }

  
  //Inicio de la Cámara 
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) { 
    Serial.printf("Al iniciar la camara inció con un error 0x%x", err);
    delay(1000);
    ESP.restart();
  }

  //Configuración de OV2640: brillo, contraste, saturación, balance de blancos, exposición y más
  sensor_t * s = esp_camera_sensor_get();
  s->set_brightness(s, 0);     // -2 to 2
  s->set_contrast(s, 0);       // -2 to 2
  s->set_saturation(s, 0);     // -2 to 2
  s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
  s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
  s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
  s->set_wb_mode(s, 0);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
  s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
  s->set_aec2(s, 0);           // 0 = disable , 1 = enable
  s->set_ae_level(s, 0);       // -2 to 2
  s->set_aec_value(s, 300);    // 0 to 1200
  s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
  s->set_agc_gain(s, 0);       // 0 to 30
  s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
  s->set_bpc(s, 0);            // 0 = disable , 1 = enable
  s->set_wpc(s, 1);            // 0 = disable , 1 = enable
  s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
  s->set_lenc(s, 1);           // 0 = disable , 1 = enable
  s->set_hmirror(s, 0);        // 0 = disable , 1 = enable
  s->set_vflip(s, 0);          // 0 = disable , 1 = enable
  s->set_dcw(s, 1);            // 0 = disable , 1 = enable
  s->set_colorbar(s, 0);       // 0 = disable , 1 = enable


  //Configurción a entradas/salidas
  pinMode(trigPin, OUTPUT);         // Establece el trigPin como una salida
  pinMode(echoPin, INPUT);          // Establece el echoPin como una entrada
  digitalWrite(trigPin, LOW);       // Apagar trigPin 
  pinMode(Led, OUTPUT);             // Establece el led como una salida
  pinMode(Bocina, OUTPUT);          // Establece la bocia como una salida
  pinMode(Pulsador, INPUT);         // Establece el pulsador como una entrada
}

void loop (){
  //declaramos varibles con su tipo de dato
  long duracion;          //delcarar variable duración 
  long distanciaCm;       //declarar variable distancia en cm
  
  // Enviamos a TriggerPin Alto por 10 microsegundos
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  
  // Leemos EchoPin, retornando el tiempo en microsegundos 
  duracion = pulseIn(echoPin, HIGH);
  // Calculamos la distancia
  distanciaCm = duracion/59;

  // Código para evitar que tome fotos y encienda la bocina
  // mientras el pulsador este en HIGH
   if (digitalRead(Pulsador) == HIGH) {                               //pulsador en alto 
     digitalWrite(Bocina, LOW);                                       //bocina no ce enciende 
  }else if ((digitalRead(Pulsador) == LOW) && (distanciaCm == 15)){   //y si el pulsador esta bajo y la 
      enviarFoto();                                                   //distancia es la indicada tomará la fotografía
  }

  //Código para encender Led y Bocina 
  if(distanciaCm < 16 && distanciaCm > 0){                            //Si la distancia es la indicada
    digitalWrite(Led, HIGH);                                          //Enciende el Led y Bocina 
    digitalWrite(Bocina, HIGH);                                       //Hasta que el objeto ya no este en la distancia indicada
    }
  else {
    digitalWrite(Led, LOW);
    digitalWrite(Bocina, LOW);
    }
}

//Código para tomar foto con esp32 Cam
String enviarFoto() {
  String getAll;
  String getBody;
  
  Serial.println("Conectando con el servidor: " + nombreServidor);

  if (client.connect(nombreServidor.c_str(), puertoServidor)) {
    Serial.println("Conexión Exitosa!");    
    String head = "--SistemaProximidad\r\nContenido-Disposición: formulario-datos; nombre=\"archivoImagen\"; nombreArchivo=\"esp32-cam.jpg\"\r\nContenido-Tipo: imagen/jpeg\r\n\r\n";
    String tail = "\r\n--SistemProximidad--\r\n";

    uint32_t imageLen = fb->len;
    uint32_t extraLen = head.length() + tail.length();
    uint32_t totalLen = imageLen + extraLen;
  
    client.println("POST " + pathServidor + " HTTP/1.1");
    client.println("Host: " + nombreServidor);
    client.println("Contenido-Largo: " + String(totalLen));
    client.println("Tipo-Contenido: multiparte/formulario-datos; boundary=SistemaProximidad");
    client.println();
    client.print(head);
  
    uint8_t *fbBuf = fb->buf;
    size_t fbLen = fb->len;
    for (size_t n=0; n<fbLen; n=n+1024) {
      if (n+1024 < fbLen) {
        client.write(fbBuf, 1024);
        fbBuf += 1024;
      }
      else if (fbLen%1024>0) {
        size_t remainder = fbLen%1024;
        client.write(fbBuf, remainder);
      }
    }   
    client.print(tail);
    
    esp_camera_fb_return(fb);
    
    int timoutTimer = 10000;
    long startTimer = millis();
    boolean state = false;
    
    while ((startTimer + timoutTimer) > millis()) {
      Serial.print(".");
      delay(100);      
      while (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (getAll.length()==0) { state=true; }
          getAll = "";
        }
        else if (c != '\r') { getAll += String(c); }
        if (state==true) { getBody += String(c); }
        startTimer = millis();
      }
      if (getBody.length()>0) { break; }
    }
    Serial.println();
    client.stop();
    Serial.println(getBody);
  }
  else {
    getBody = "Conectando a " + nombreServidor +  " fallido.";
    Serial.println(getBody);
  }
}
