import serial
import keyboard
import re
import pdb
import getopt
import sys
import time
import subprocess
import os

from xmodem import XMODEM

#rexec.py is a server attached to a serial port that responds to commands sent to it by a rexec client on CMM2.
#Author: Epsilon.

REXEC_VERSION = "0.1"
REXEC_KEY = "REXEC07101972 "
REXEC_DONE_KEY = "REXEC07101972_DONE\r\n"
XMODEM_SEND_CMD = "xmodem_send_to_pc"
XMODEM_RECEIVE_CMD = "xmodem_receive_from_pc"
SHELL_CMD = "shell_cmd_to_pc"
SHELL_RET = "shell_ret_from_pc"
SHELL_OUT = "shell_out_from_pc"
SHELL_ERR = "shell_err_from_pc"
QUIT_CMD = "quit"

#Regular expressions used:
rexec_req_pat = re.compile(REXEC_KEY + '(\S+)')
xmodem_send_cmd_pat = re.compile(REXEC_KEY + '(\S+) (\S+) (\S+)')
xmodem_receive_cmd_pat = re.compile(REXEC_KEY + '(\S+) (\S+)')
shell_cmd_pat = re.compile(REXEC_KEY + '(\S+) (.+)')

#globals
ser = None
modem = None
shell_returncode = None
shell_out = None
shell_err = None

#getc and putc are used by the xmodem implementation

def getc(size, timeout=1):
    global ser
    res = ser.read(size)
    return res or None

def putc(data, timeout=1):
    global ser
    return ser.write(data)  # note that this ignores the timeout

def handle_shell_returncode(receivedLine):
    "Send shell return code to client."
    global ser
    global shell_returncode

    if shell_returncode:
        ser.write(shell_returncode)
        shell_returncode = None

    ser.write(REXEC_DONE_KEY.encode())

def handle_shell_out(receivedLine):
    "Send shell output to client."
    global ser
    global shell_out

    if shell_out:
        ser.write(shell_out)
        shell_out = None

    ser.write(REXEC_DONE_KEY.encode())

def handle_shell_err(receivedLine):
    "Send shell stderr to client."
    global ser
    global shell_err

    if shell_err:
        ser.write(shell_err)
        shell_err = None

    ser.write(REXEC_DONE_KEY.encode())

def handle_shell_cmd(receivedLine):
    "Execute shell command given by client and store resulting returncode, output and stderr."
    global shell_returncode, shell_out, shell_err

    match = shell_cmd_pat.match(receivedLine)
    if match:
        cmd = match.group(2)

        result = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
        shell_returncode = "{}\r\n".format(result.returncode).encode()
        shell_out = "{}\r\n".format(result.stdout).encode()
        shell_err = "{}\r\n".format(result.stderr).encode()

        #print("shell command executed.")
        
def handle_xmodem_send(receivedLine):
    "Receive xmodem transmission from client. Correct for filesize."
    global modem
    global ser

    match = xmodem_send_cmd_pat.match(receivedLine)
    if match:
        filename = match.group(2)
        filesize = int(match.group(3))

        #print("{} filename: {} filesize: {}".format(XMODEM_SEND_CMD, filename, filesize))

        try:
            stream = open(filename, 'wb')
            
            #Workaround to make xmodem quiet.
            f = open(os.devnull, 'w')
            old_stderr = sys.stderr
            sys.stderr = f

            modem.recv(stream, retry=8, quiet=1)

            sys.stderr = old_stderr
            f.close()

            stream.truncate(filesize)
            stream.close()
            shell_returncode = "0\r\n".encode()
        except:
            shell_returncode = "-1\r\n".encode()

        #print("xmodem transfer complete")

def handle_xmodem_receive(receivedLine):
    "Send xmodem transmission to client."
    global modem

    match = xmodem_receive_cmd_pat.match(receivedLine)
    if match:
        filename = match.group(2)
        try:
            filesize = os.path.getsize(filename)
        except OSError:
            filesize = -1

        #print("{} filename: {} filesize: {}".format(XMODEM_RECEIVE_CMD, filename, filesize))

        ser.write("{}\r\n".format(filesize).encode())

        if filesize != -1:
            #Workaround to make xmodem quiet.
            f = open(os.devnull, 'w')
            old_stderr = sys.stderr
            sys.stderr = f

            stream = open(filename, 'rb')
            modem.send(stream, retry=8, quiet=1)
            stream.close()

            sys.stderr = old_stderr
            f.close()

            #print("xmodem transfer complete")
        else:
            print("rexec.py: xmodem transfer aborted")

def rexec(port, baudrate):
    "Start rexec server"
    global ser
    global modem

    ser = serial.Serial(port, 
        baudrate=baudrate, 
        bytesize=8, 
        parity='N',
        stopbits=1,
        timeout=1,
        rtscts=1)

    modem = XMODEM(getc, putc)

    while not keyboard.is_pressed('q'):
        rxline = ser.readline()
        if rxline:
            try:
                l = rxline.decode()
            except:
                l=""

            #Print what we receive.
            print(l, end="")

            match = rexec_req_pat.match(l)
            if match:
                cmd = match.group(1)
                #print("Rexec command received: {}".format(cmd))

                if cmd == XMODEM_SEND_CMD:
                    handle_xmodem_send(l)

                if cmd == XMODEM_RECEIVE_CMD:
                    handle_xmodem_receive(l)

                if cmd == SHELL_CMD:
                    handle_shell_cmd(l)

                if cmd == SHELL_RET:
                    handle_shell_returncode(l)

                if cmd == SHELL_OUT:
                    handle_shell_out(l)

                if cmd == SHELL_ERR:
                    handle_shell_err(l)

                if cmd == QUIT_CMD:
                    sys.exit(0)

def usage():
    print("rexec.py is a server attached to a serial port that responds to commands sent to it by a rexec client on CMM2.")
    print()
    print("Usage:")
    print("python rexec.py -p <serial port> -b <baudrate>")
    print("Example:")
    print("python rexec.py -p COM8 -b 115200")
    print()

if __name__ == "__main__":
    print("rexec.py V{} by Epsilon.".format(REXEC_VERSION))

    try:
        opts, args = getopt.getopt(sys.argv[1:], "hp:b:", ["help", "port=", "baudrate="])
    except getopt.GetoptError as err:
        # print help information and exit:
        print(err)  # will print something like "option -a not recognized"
        usage()
        sys.exit(2)

    port = None
    baudrate = None

    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            sys.exit()
        elif o in ("-p", "--port"):
            port = a
        elif o in ("-b", "--baudrate"):
            baudrate = int(a)
        else:
            assert False, "unhandled option"

    if port and baudrate:
        rexec(port, baudrate)
    else:
        usage()
