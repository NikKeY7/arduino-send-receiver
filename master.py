import serial
import threading
import time


PORT = 'COM5'       # Последовательный порт, в который подключен PL2303 (Да, я на винде, не бейте)
BAUDRATE = 9600     # Скорость обмена (должна совпадать с Arduino)
TIMEOUT = 1         # Таймаут чтения (секунды)

def read_from_serial(ser):
    """Функция для чтения данных из порта в отдельном потоке."""
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            print(f"\n[Получено] {line}")
        time.sleep(0.1)  # Небольшая задержка, чтобы не нагружать CPU

def main():
    try:
        # Открываем порт
        ser = serial.Serial(PORT, BAUDRATE, timeout=TIMEOUT)
        print(f"Подключено к {ser.name}. Ожидание данных...")
        print("Введите команду для отправки (или 'exit' для выхода):")

        # Запускаем поток для чтения
        thread = threading.Thread(target=read_from_serial, args=(ser,), daemon=True)
        thread.start()

        # Основной цикл для отправки команд
        while True:
            user_input = input("> ").strip()
            if user_input.lower() == 'exit':
                break
            if user_input:
                ser.write((user_input + '\n').encode('utf-8'))
                print(f"[Отправлено] {user_input}")

    except serial.SerialException as e:
        print(f"Ошибка порта: {e}")
    except KeyboardInterrupt:
        print("\nПрограмма остановлена пользователем")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
        print("Порт закрыт")

if __name__ == "__main__":
    main()