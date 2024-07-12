//Masukkan Library yang dibutuhkan yaitu BOT Telegram(CTBot) dan Loadcell(HX711) // 

#include "CTBot.h"
#include "HX711.h"

HX711 scale;
CTBot myBot;
TBMessage tMessage;
//void KonekWifi();

// Deklarasi GPIO
#define DOUT_PIN 14
#define CLK_PIN 12
#define SIMPANDATA_BUTTON 13
//#define TOTAL_BUTTON 4

// Konfigurasi Telegram Bot
String ssid = "Rumah Yuda";
String password = "yudazaky";
String token = "6852392391:AAFwMIRt5WLeX8R-Now02rYqhGM2PAxoHg8";
//const unsigned chat_id = 6224438098; 

bool pressed = false;
float berat, beratTotal;
int harga, hargaTotal, jml_ikan;
const float calibration_factor = -153000; // Hasil kalibrasi

void setup() {
  Serial.begin(115200);
  // Mengecek Koneksi WiFi Dengan Telegram
  KonekWifi();

  // Inisialisasi Load Cell
  scale.begin(DOUT_PIN, CLK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare();
  // Inisialisasi Tombol
  pinMode(SIMPANDATA_BUTTON, INPUT_PULLUP);
}

void loop() {  
  berat = scale.get_units(),4;
  berat *= 1000;
  
  Serial.println(berat);

  if (myBot.getNewMessage(tMessage)){       
    Serial.println(tMessage.text);
       if (tMessage.messageType == CTBotMessageText){           
        if (tMessage.text.equalsIgnoreCase("/start")){
               String reply = "";               
               reply += "Sistem sudah aktif! Selamat Memancing \n";
               myBot.sendMessage(tMessage.sender.id, reply);           
        } else if (tMessage.text.equalsIgnoreCase("/cek-harga-total")){
               hargaTotal = beratTotal * 21; 
               
               String reply = "";               
               reply += "Total Tangkapan anda sebagai berikut:  \n";
               reply += "- Total Ikan: ";
               reply += jml_ikan;
               reply += "\n";
               reply += "- Total Berat Ikan: ";
               reply += beratTotal;
               reply += "\n";
               reply += "- Harga yang harus dibayar: ";
               reply += hargaTotal;
               reply += " \n";
              
               
               myBot.sendMessage(tMessage.sender.id, reply);           
         }
     }
  }

  NotifMakanIkan();
  delay(500);

  Tombol();
  delay(1000);
  
}

void KonekWifi() {
  Serial.println("menghubungkan ke Wifi");
  myBot.wifiConnect(ssid, password);
  while (!myBot.testConnection()) {
    myBot.setTelegramToken(token);
    Serial.println("telegram Connected!");
  }
}

void NotifMakanIkan() {
  // Mengecek apakah ada berat 100 gram
  if (berat > 50.00  && berat <100.00) { 
    // Jika melebihi 100 gram akan mengirim pesan ke Telegram
    String message = "Ikan sedang memakan umpan!";
    myBot.sendMessage(6224438098, message);
    Serial.print(message);
  }
}

void Tombol() {
  bool beratHargaIkan = digitalRead(SIMPANDATA_BUTTON);

  // Jika Tombol ditekan akan mengirimkan data berat dan harga
  if (beratHargaIkan == pressed) {
    HargaIkan();
    jml_ikan++;
  }
}

void HargaIkan() {
  int harga = berat * 21;  // Menghitung harga ikan dikalikan dengan berat ikan
  // Mengirim data berat dan harga ikan ke Telegram
  String message = "Berat ikan: " + String(berat) + " gram dan Harga yang harus dibayarkan: " + " Rp " + String(harga) + " rupiah";
  myBot.sendMessage(6224438098, message);

  beratTotal += berat;
}


/* Projek ini menggunakan ESP8266 Sebagai Mikrokontroller 
 * Terakhir diupdate : 24 Januari 2024 01:30 WIB
 * 
 * Kode ini digunakan untuk: 
 * Lomba YNSF : Inovasi Alat Pancing Untuk Mendapatkan Notifikasi Jika Ikan Memakan Umpan dan Mengetahui Berat & Harga Ikan
 * Coded by Tim Kecil 
 * Copyright 2024 
*/
