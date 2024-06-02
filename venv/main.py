import time  # подключаем библиотеку чтобы задействовать функции задержки в программе
import numpy as np
import serial  # подключаем библиотеку для последовательной связи
import serial.tools.list_ports
import itertools

try:
    #вывод на экран доступных портов
    ports = serial.tools.list_ports.comports()
    print('I found this ports:')
    for port in ports:
        print(port.device)

    #инициализируем СОМ-порт
    ArduinoSerial = serial.Serial('COM3', 9600, timeout=1)  # создаем объект для работы с портом последовательной связи
    time.sleep(2)  # ждем 2 секунды чтобы установилась последовательная связь
    print('Serial connection is open')

    receivedList = np.zeros(7)
    while ArduinoSerial.isOpen():
        for i in range(receivedList.size):
            receivedData = ArduinoSerial.readline().rstrip()
            data = receivedData.decode('utf-8')

            if 'MPU_9150' in data:
               continue
            if ' ' in data or data.count('.') > 1:
                data = 0

            receivedList[i] = data

        dict = {'accX' : receivedList[0],
                'accY' : receivedList[1],
                'accZ' : receivedList[2],
                'temp' : receivedList[3],
                'gyroX' : receivedList[4],
                'gyroY' : receivedList[5],
                'gyroZ' : receivedList[6]}

        if receivedData:
            print(dict)

except serial.SerialException as se:
    print("Serial port error:", str(se))

except KeyboardInterrupt:
    pass

finally:
    if ArduinoSerial.is_open:
        ArduinoSerial.close()
        print("Serial connection is closed")