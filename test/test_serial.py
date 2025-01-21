import serial
import pytest
import time
import sliplib

@pytest.fixture()
def arduino():
    ser = serial.Serial('/dev/cu.usbmodem11201', 115200, timeout=.1)
    time.sleep(2) #give the connection a second to settle
    yield ser
    ser.close()

def test_reqinfo(arduino):
    driver = sliplib.Driver()
    data = driver.send(bytes([0x03]))
    arduino.write(data)
    time.sleep(.1)
    read_data = arduino.readline()
    driver.receive(read_data)
    read_data = driver.get()
    assert read_data[0]==0xc3
    assert read_data[1]==0x06
    assert read_data[2]==0x00
    assert read_data[3]==0x00
    assert read_data[4]==0x00

def test_reqInit(arduino):
    driver = sliplib.Driver()
    data = bytearray()
    data.append(0x05)
    data.append(0)
    hash = 0
    hash = add_crc(hash, data)
    data.append(hash)
    data = driver.send(bytes(data))
    arduino.write(data)
    time.sleep(.1)
    read_data = arduino.read(4)
    assert read_data==b"\xc0\xc5\x00\xc0"
    time.sleep(1)
    read_data = arduino.read(25)
    print(read_data)
    assert read_data>b""


# CRC algorithm after Maxim/Dallas
def add_crc(crc: int, data: bytearray) -> int:
    for i in range(len(data)):
        n = data[i]
        for _j in range(8):
            f = (crc ^ n) & 1
            crc >>= 1
            if f:
                crc ^= 0x8C
            n >>= 1
    return crc & 0xFF
