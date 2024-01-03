import argparse
from typing import Dict

import serial


def main(flags: Dict[str, str]):
    port_name = flags["port_name"]
    baud_rate = int(flags["baud_rate"])
    byte_size = int(flags["byte_size"])
    timeout = int(flags["timeout"])
    serial_port = serial.Serial(port=port_name, baudrate=baud_rate, bytesize=byte_size, timeout=timeout, stopbits=serial.STOPBITS_ONE)

    print("Connected to: {} {}bps".format(serial_port.portstr, serial_port.baudrate))

    seq = []
    count = 1
    serial_port.write(b"Hello World!")
    while True:
        for c in serial_port.read():
            print(''.join(chr(c)), end='')

    print()
    serial_port.close()


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    print("*** Started ***")

    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--port", help="Port name", type=str, default="/dev/cu.usbmodem11301")
    parser.add_argument("-b", "--baud_rate", help="Baud rate in bps", type=str, default="115200")
    parser.add_argument("-s", "--byte_size", help="Byte size", type=str, default="8")
    parser.add_argument("-t", "--timeout", help="Timeout", type=str, default="20")
    args = parser.parse_args()

    option_flags = {
        "port_name": args.port,
        "baud_rate": args.baud_rate,
        "byte_size": args.byte_size,
        "timeout": args.timeout,
    }
    main(option_flags)

    print("*** Ended ***")
