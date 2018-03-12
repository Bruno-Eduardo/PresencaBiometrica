import serial  # pyserial
import serial.tools.list_ports

a = serial.Serial(serial.tools.list_ports.comports()[0], 9600, timeout=2)
