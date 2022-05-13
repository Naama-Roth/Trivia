import socket
import json

SERVER_IP = "127.0.0.1"
SERVER_PORT = 8826
DATA_SIZE = 1024
ERROR_MESSAGE = "ERROR"
HELLO_MESSAGE = "Hello"
LOGIN_CODE = 1
SIGNUP_CODE = 2
INT = 4
START_DATA = 5


def create_msg(code, data):
    """
    create the msg
    :param code: code for msg
    :param data: json
    :return: the binary request
    """
    json_str = json.dumps(data)

    request = bytearray()
    request.append(code)
    data_size = len(json_str).to_bytes(INT, byteorder = "little")

    for byte in data_size:
        request.append(byte)

    for byte in json_str:
        request.append(ord(byte))

    return request


def main():
    #  Create a TCP/IP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    #  Connecting to remote computer 8820
    server_address = (SERVER_IP, SERVER_PORT)
    sock.connect(server_address)

    while True:
        choice = int(input("Enter choice: 1 - login, 2 - signUp, else - exit: "))

        try:
            if choice == LOGIN_CODE:
                msg = {"userName": "user1", "password": "1234"}
                sock.sendall(create_msg(LOGIN_CODE, msg))
            elif choice == SIGNUP_CODE:
                msg = {"userName":"user1", "password":"1234", "email":"e@s.com"}
                sock.sendall(create_msg(SIGNUP_CODE, msg))
            else:
                break

            server_msg = sock.recv(DATA_SIZE)
            print("response code: " + str(server_msg[0]))
            print("data size: " + str(int.from_bytes(server_msg[1 : START_DATA], "little")))
            print("data: " + server_msg[START_DATA : ].decode())
        except:
            print("There was an error with the connection")
            break

    sock.close()

if __name__ == "__main__":
    main()
