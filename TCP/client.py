import socket
import struct
import sys

def main():
    if len(sys.argv) != 3:
        print("Usage: python client.py <server_ip> <server_port>")
        return

    server_ip = sys.argv[1]
    server_port = int(sys.argv[2])

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((server_ip, server_port))

    message = input("Enter a string: ")
    client_socket.send(message.encode('utf-8'))

    space_count = client_socket.recv(4)  # Assuming space_count is a 4-byte integer
    space_count = struct.unpack('i', space_count)[0]

    print(f"Number of spaces in the string: {space_count}")

    client_socket.close()

if __name__ == "__main__":
    main()
