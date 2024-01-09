import argparse
from enum import Enum

import serial
import time
from typing import Dict
import numpy as np
import cv2
from serial.serialutil import SerialException


def show_temperature(serial_port, timeout):
    serial_data = serial_port.read(24 * 32)
    if len(serial_data) > 0:
        print("Serial data received, {} bytes".format(len(serial_data)))
        frame_linear = np.frombuffer(serial_data, dtype=np.uint8)
        frame = np.rot90(frame_linear.reshape((24, 32)))

        scale_percent = 10  # scale factor
        dim = (int(frame.shape[1] * scale_percent), int(frame.shape[0] * scale_percent))
        frame_resized = cv2.resize(frame, dim, interpolation=cv2.INTER_AREA)

        cv2.imshow("Thermal Camera", frame_resized)
        # waits for user to press any key (this is necessary to avoid Python kernel form crashing)
        cv2.waitKey(100)
    else:
        # cv2.destroyAllWindows()
        print("Timeout {} sec occurred during read".format(timeout))


def show_distance(serial_port, timeout):
    serial_data = serial_port.read(8 * 8)
    if len(serial_data) > 0:
        print("Serial data received, {} bytes".format(len(serial_data)))
        frame_linear = np.frombuffer(serial_data, dtype=np.uint8)
        frame = frame_linear.reshape((8, 8)).transpose()

        scale_percent = 3000  # percent of original size
        dim = (int(frame.shape[1] * scale_percent / 100), int(frame.shape[0] * scale_percent / 100))
        frame_resized = cv2.resize(frame, dim, interpolation=cv2.INTER_AREA)

        cv2.imshow("Distance", frame_resized)
        # waits for user to press any key (this is necessary to avoid Python kernel form crashing)
        cv2.waitKey(100)
    else:
        # cv2.destroyAllWindows()
        print("Timeout {} sec occurred during read".format(timeout))


def save_frame(filename: str, img: np.array) -> None:
    cv2.imwrite(filename, img)


class SerialStates(Enum):
    HANDSHAKE_SEND_WELCOME_MSG = 0
    HANDSHAKE_WAIT_WELCOME_MSG = 1
    HANDSHAKE_COMPLETED = 2


def main(flags: Dict[str, str]) -> bool:
    port_name = flags["port_name"]
    baud_rate = int(flags["baud_rate"])
    byte_size = int(flags["byte_size"])
    timeout = int(flags["timeout"])
    serial_port = serial.Serial(port=port_name, baudrate=baud_rate, bytesize=byte_size, timeout=timeout, stopbits=serial.STOPBITS_ONE)

    print("Open: {} @ {}bps".format(serial_port.portstr, serial_port.baudrate))
    serial_state = SerialStates.HANDSHAKE_SEND_WELCOME_MSG

    terminate = False
    while not terminate:
        # for c in serial_port.read():
        #     print(''.join(chr(c)), end='')

        if serial_state == SerialStates.HANDSHAKE_SEND_WELCOME_MSG:
            print("Handshake, send welcome message")
            serial_port.write(b"Hello!\r\n")
            serial_state = SerialStates.HANDSHAKE_WAIT_WELCOME_MSG

        elif serial_state == SerialStates.HANDSHAKE_WAIT_WELCOME_MSG:
            print("Handshake, wait for welcome message")
            welcome_msg = serial_port.readline()
            if welcome_msg == b"Hello!\r\n":
                serial_state = SerialStates.HANDSHAKE_COMPLETED
                print("Handshake, welcome received!")
        else:
            # Temperature measurement
            show_temperature(serial_port, timeout)

            # Distance measurement
            # show_distance(serial_port, timeout)

    print("Closing connection with {}".format(serial_port.portstr))
    serial_port.close()

    # closing all open windows
    cv2.destroyAllWindows()

    return True


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    print("*** Started ***")

    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--port", help="Port name", type=str, default="/dev/cu.usbmodem1101")
    parser.add_argument("-b", "--baud_rate", help="Baud rate in bps", type=str, default="115200")
    parser.add_argument("-s", "--byte_size", help="Byte size", type=str, default="8")
    parser.add_argument("-t", "--timeout", help="Timeout", type=str, default="10")
    args = parser.parse_args()

    option_flags = {
        "port_name": args.port,
        "baud_rate": args.baud_rate,
        "byte_size": args.byte_size,
        "timeout": args.timeout,
    }

    close = False
    while not close:
        try:
            close = main(option_flags)
        except SerialException as exception:
            print("{} closed unexpected, try again in {} sec".format(option_flags["port_name"], option_flags["timeout"]))
            cv2.destroyAllWindows()
            time.sleep(int(option_flags["timeout"]))

    print("*** Ended ***")
