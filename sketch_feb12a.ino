#include <EEPROM.h>

/* Програма вмикає/вимикає світлодіод командами, надісланими через BlueTooth,
а також змінює параметри роботи BlueTooth, які котролюються з Serial Monitor
Скетч модіфікований для роботи з бібліотекою SoftwareSerial */

#include <SoftwareSerial.h>
#include <TimerOne.h>


const int rxPin = 3; // SoftwareSerial RX pin, під’єднаний до виводу TX HC-06
const int txPin = 4; // SoftwareSerial TX pin, під’єднаний до виводу RX HC-06

SoftwareSerial mySerial(rxPin, txPin); // RX, TX - створення програмного послідовного порта mySerial

const int ledPin = 10; // вивід 13, під’єднаний до вбудованого світлодіода
boolean impulse = false;
boolean long_cycle = false;
byte short_period;
long long_period; 
void setup()
{
  Serial.begin(9600);  // встановлення швидвості передавання Serial 9600 бод
  mySerial.begin(9600); // встановлення швидкості передавання mySerial також 9600 бод
  
  // встановлення виводу як вихідного:
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // світлодіод ініціалізуємо як вимкнений
  mySerial.println("Starting config"); // передача через Serial
  delay(1000);  // затримка 1 сек

  // встановлення швидкості передачі 9600 на BlueTooth
  Serial.print("AT+BAUD4");
  waitForResponse();

  // повинна бути надіслана відповідь з OK
  Serial.print("AT");
  waitForResponse();

  // повинна бути надіслана відповідь з версією
  Serial.print("AT+VERSION");
  waitForResponse();

  // встановлення пін-коду в 1234 (тобто, не змінюємо код за замовчуванням)
  Serial.print("AT+PIN1234");
  waitForResponse();

  // встановлення імені HC-06
  Serial.print("AT+NAMEMITIN-TEST");
  waitForResponse();

  mySerial.println("Done!"); // передача через Serial
  // Загрузка начальных значений из EEPROM
  short_period = EEPROM.read(0);
  long_period = 2000000 / EEPROM.read(1);
  Timer1.initialize(long_period);
  Timer1.attachInterrupt(Timer1_action);
  Timer1.start();
  //Timer1.pwm(ledPin, init_duty * 10.23, period/init_freq);
  
//  Timer1.pwm(10,512);
  pinMode(ledPin,OUTPUT);
}

void waitForResponse()
{

    // міст між mySerial і Serial для відображення на Serial Monitor
    delay(1000);

    while (Serial.available())
    {
      mySerial.write(Serial.read());
    }
    mySerial.write("\r\n");
}

void loop() {
    if(Serial.available() > 0) { // тест з ПК, закоментувати для роботи зі смартфона через BlueTooth
      int command = Serial.parseInt();
      //Timer1.stop();      
      if( 0 < command) {
                // state = mySerial.read(); // розкоментувати для роботи зі смартфона через BlueTooth

        // тест з ПК, закоментувати для роботи зі смартфона через BlueTooth
       
       byte freq = lowByte(command);
       short_period = highByte(command);
       // Запись значений частоты и duty в EEPROM для последующего извлечения при старте контроллера.
       EEPROM.write(1, freq);
       EEPROM.write(0, highByte(command));
       
       long_period = 2000000 / freq;
       Timer1.setPeriod(long_period);
       //mySerial.println(freq);
       //mySerial.println(duty);
       
       //Timer1.pwm(ledPin, duty, period);
       //Timer1.start();
      } else {
        Timer1.stop();
        digitalWrite(ledPin, LOW);
      }
    }
}

    
void Timer1_action()
{
  int i = short_period * 14; // Устанавливаем 1 ждём интервал кратный 14 тактов (Предположительно 16 тактов микросекунда минус 2 на комманды)
  // в теории получаем в цикле while задержку кратную микросекунде 
  digitalWrite(ledPin, HIGH);
  while (i > 0) {
    i--;
  }
  digitalWrite(ledPin, LOW);  
}
