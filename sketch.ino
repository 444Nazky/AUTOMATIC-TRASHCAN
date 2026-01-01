// Definisi template
#define BLYNK_TEMPLATE_ID "TMPL6h4tlercF" // ID template Blynk
#define BLYNK_TEMPLATE_NAME "Smart Trash" // Nama template Blynk
#define BLYNK_AUTH_TOKEN "RgJ3QtoWBn1czaZcSMcctVcFHNMS_rYp" // Token autentikasi Blynk
#define BLYNK_PRINT Serial // Mengarahkan keluaran Blynk ke Serial

#include <ESP32Servo.h> // Menyertakan library ESP32Servo untuk mengontrol servo
#include <WiFi.h> // Menyertakan library WiFi untuk konektivitas WiFi
#include <WiFiClient.h> // Menyertakan library WiFiClient untuk komunikasi WiFi
#include <BlynkSimpleEsp32.h> // Menyertakan library Blynk untuk ESP32
#include <LiquidCrystal_I2C.h> // Menyertakan library untuk LCD I2C
#include <NewPing.h> // Menyertakan library NewPing untuk sensor ultrasonik

// Deklarasi variabel koneksi Blynk dan WiFi
char auth[] = BLYNK_AUTH_TOKEN; // Token autentikasi Blynk
char ssid[] = "Wokwi-GUEST"; // Nama SSID WiFi
char pass[] = ""; // Kata sandi wifi

// Definisi pin
#define TRIGGER_PIN1 5 // Pin trig sensor ultrasonik 1
#define ECHO_PIN1 18 // Pin echo sensor ultrasonik 1
#define TRIGGER_PIN2 4 // Pin trig sensor ultrasonik 2
#define ECHO_PIN2 15 // Pin echo sensor ultrasonik 2
#define MAX_DISTANCE 400 // Jarak maksimal pengukuran sensor ultrasonik dalam cm
#define SERVO_PIN 13 // Pin kontrol servo
#define MAX_CAPACITY 100 // Kapasitas maksimum tong sampah dalam persen

// Objek untuk sensor ultrasonik dan servo
NewPing sinar_ultrasonik1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE); 
NewPing sinar_ultrasonik2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);
Servo servo;

// Objek untuk LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
BlynkTimer timer;

bool manualControl = false; // Menyimpan status kontrol manual
bool trashFullNotified = false; // Menyimpan status notifikasi tong sampah penuh
bool servoOpen = false; // Menyimpan status servo (buka/tutup)

// Fungsi untuk menangani perubahan kontrol manual melalui widget switch di Blynk
BLYNK_WRITE(V4) {
  int switchValue = param.asInt(); // Mengambil nilai dari widget switch
  manualControl = (switchValue == 1); // Mengaktifkan atau menonaktifkan kontrol manual  
  if (manualControl) {
    servo.write(110); // Buka tutup tong sampah
    servoOpen = true; // Menyimpan status servo sebagai buka
  } else {
    servo.write(0); // Tutup tong sampah secara manual
    servoOpen = false; // Menyimpan status servo sebagai tutup
  }
}

// Fungsi untuk mengukur jarak menggunakan sensor ultrasonik
void ukurJarak() {
  if (!manualControl) { // Jika kontrol manual tidak aktif, jalankan pengukuran otomatis
    unsigned int jarak1 = sinar_ultrasonik1.ping_cm(); // Sensor Ultrasonik 1
    unsigned int jarak2 = sinar_ultrasonik2.ping_cm(); // Sensor Ultrasonik 2

    // Mencetak hasil pengukuran ke Serial Monitor
    Serial.print("Jarak Objek: ");
    Serial.print(jarak1);
    Serial.print(" cm, Jarak Sampah: ");
    Serial.print(jarak2);
    Serial.println(" cm");

    // Menghitung kapasitas tong sampah dalam persen
    int capacity = map(jarak2, 16, 2, 0, 100); // Kapasitas berdasarkan jarak
    Blynk.virtualWrite(V2, capacity); // Mengirimkan kapasitas ke V2

    // Mengatur servo berdasarkan jarak dari sensor ultrasonik 1
    if (jarak1 > 0 && jarak1 <= 20) {
      if (!servoOpen) { // Jika servo belum terbuka
        servo.write(110); // Buka tutup tong sampah
        servoOpen = true; // Menyimpan status servo sebagai buka
        timer.setTimeout(5000L, []() { // Delay 5 detik untuk memberikan waktu membuang sampah
          if (servoOpen) {
            servo.write(0); // Tutup tutup tong sampah
            servoOpen = false; // Menyimpan status servo sebagai tutup
          }
        });
      }
    }

    // Mengirim status tong sampah ke V3 dan menampilkan pesan di LCD
    if (jarak2 <= 2) { // Status tong sampah penuh jika jarak <= 2 cm
      Blynk.virtualWrite(V3, "Tong Sampah Penuh"); // Mengirimkannya ke Blynk

      // Menampilkan pesan Kapasitas di LCD bersama "Sampah Penuh" di LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Kapasitas: ");
      lcd.print("100%");
      lcd.setCursor(0, 1);
      lcd.print("Sampah Penuh");

      // Mengirim notifikasi kepada pengguna di smartphone jika sampah penuh
      if (!trashFullNotified) {
        Blynk.logEvent("message", "Tempat Sampah Penuh! Harap Segera Di Angkut");
        trashFullNotified = true;
      }
    } else if (capacity >= 85) { // Kapasitas 85% atau lebih
      Blynk.virtualWrite(V3, "Tempat Sampah Hampir Penuh"); // Mengirimkan status ke Blynk

      // Menampilkan pesan di LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Kapasitas: ");
      lcd.print(capacity);
      lcd.print("%");
      lcd.setCursor(0, 1);
      lcd.print("Hampir Penuh");

      // Reset notifikasi tong sampah penuh
      trashFullNotified = false;
    } else if (jarak2 >= 16) { // Status tong sampah kosong jika jarak >= 16 cm
      Blynk.virtualWrite(V3, "Tong Sampah Kosong"); // Mengirimkannya ke Blynk

      // Menampilkan pesan "Tempat Sampah Kosong" di LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Tempat Sampah");
      lcd.setCursor(0, 1);
      lcd.print("Kosong");

      // Reset notifikasi tong sampah penuh
      trashFullNotified = false;
    } else {
      Blynk.virtualWrite(V3, "Tempat Sampah Belum Penuh"); // Mengirimkannya ke Blynk

      // Menampilkan kapasitas sampah di LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Kapasitas: ");
      lcd.print(capacity);
      lcd.print("%");
      lcd.setCursor(0, 1);
      lcd.print("Smart Trash!!");

      // Reset notifikasi tong sampah penuh
      trashFullNotified = false;
    }

    // Kirim data jarak ke Blynk
    Blynk.virtualWrite(V0, jarak1); // Jarak objek sampah untuk membuka tutup sampah
    Blynk.virtualWrite(V1, jarak2); // Jarak sampah untuk mengukur kapasitas
  }
}

void setup() {
  Serial.begin(115200); // Memulai komunikasi serial
  Serial.println("Starting...");
  Blynk.begin(auth, ssid, pass); // Menghubungkan ke Blynk
  servo.attach(SERVO_PIN); // Menghubungkan servo ke pin yang ditentukan
  lcd.begin(16, 2); // Menginisialisasi LCD dengan ukuran 16x2 karakter
  lcd.init(); // Menginisialisasi LCD
  lcd.backlight(); // Mengaktifkan lampu latar LCD

  // Menampilkan pesan "System Loading" pada LCD
  lcd.setCursor(1, 0);
  lcd.print("System Loading");
  for (int a = 0; a <= 15; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(200);
  }
  lcd.clear();

  timer.setInterval(2000L, ukurJarak); // Panggil ukurJarak setiap 2 detik
}

void loop() {
  Blynk.run(); // Menjalankan fungsi Blynk untuk menjaga koneksi dan menerima data
  timer.run(); // Menjalankan fungsi timer untuk memanggil fungsi terjadwal
}

