#define RX_PIN 8     // Подключен к RO MAX485
#define TX_PIN 9     // Подключен к DI MAX485
#define DE_RE_PIN 2  // Управление DE/RE MAX485
#define LED_PIN LED_BUILTIN // Аппаратный LED (Вроде D2)

SoftwareSerial rs485(RX_PIN, TX_PIN);  // Единый порт для приема/передачи

bool ledState = LOW;
unsigned long previousSendTime = 0;
const unsigned long sendInterval = 2000;  // Отправка каждые 2 сек

void setup() {
  Serial.begin(9600);  // Для отладки
  rs485.begin(9600);   // Инициализация RS-485

  pinMode(DE_RE_PIN, OUTPUT); // Устанавливаем режим управления пином контроля приёма/передачи MAX485
  pinMode(LED_PIN, OUTPUT); // Устанавливаем режим управления пином LED
  digitalWrite(DE_RE_PIN, LOW);  // Устанавливаем MAX485 в режим приёма
}

void loop() {
  // Приём команд
  if (rs485.available()) {
    String received = rs485.readStringUntil('\n'); // Ищем команду
    received.trim(); // Избавляемся от мусора

    Serial.print("Received: "); // Отправляем в аппаратный TX/RX дебаг-инфу
    Serial.println(received); // Сама, собственно, инфа

    if (received == "TOGGLE") { // Обрабатываем ток конкретную команду (Я знаю что это не лучший способ фильтровать команды)
      ledState = !ledState; // Переключаем LED
      digitalWrite(LED_PIN, ledState); // Переводим пин в новое состояние
    }
  }

  // Передача времени жизни
  unsigned long currentMillis = millis();
  if (currentMillis - previousSendTime >= sendInterval) {
    previousSendTime = currentMillis;

    digitalWrite(DE_RE_PIN, HIGH);  // Перевожу MAX485 в режим отправки
    rs485.println(currentMillis);        // Отсылаем кол-во мс с момента включения
    delay(10);                      // Ждем завершения передачи
    digitalWrite(DE_RE_PIN, LOW);   // Возвращаемся в прием
  }
}