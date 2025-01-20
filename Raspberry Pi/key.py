from pynput.keyboard import Key, Controller
import os
import serial
from subprocess import run

keyboard = Controller()

def main():
    ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
    ser.reset_input_buffer()
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            print(line)
            if line=='EVENT_SHUTDOWN':
              os.system('shutdown -h now')
            if line=='UP':
              keyboard.press(Key.up)
              keyboard.release(Key.up)
            if line=='DOWN':
              keyboard.press(Key.down)
              keyboard.release(Key.down)
            if line=='LEFT':
              keyboard.press('1')
              keyboard.release('1')
            if line=='RIGHT':
              keyboard.press('2')
              keyboard.release('2')
            if line=='ENTER':
              keyboard.press(Key.enter)
              keyboard.release(Key.enter)
            if line=='ESC':
              keyboard.press(Key.esc)
              keyboard.release(Key.esc)
            if line=='HOME':
              keyboard.press('h')
              keyboard.release('h')
            if line=='NEXT':
              keyboard.press('n')
              keyboard.release('n')
            if line=='PREV':
              keyboard.press('v')
              keyboard.release('v')
            if line=='YES':
              keyboard.press('m')
              keyboard.release('m')
            if line=='NO':
              keyboard.press('b')
              keyboard.release('b')
            if line=='CALL':
              run('vcgencmd display_power 1', shell=True)
            if line=='DENY':
              run('vcgencmd display_power 0', shell=True)
    ser.close()
    
if __name__ == '__main__':
    main()
