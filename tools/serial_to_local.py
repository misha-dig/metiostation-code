import serial
import http.server
import socketserver
from http import HTTPStatus
import serial

        # check which port was really used



class Handler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        ser = serial.Serial('/dev/ttyUSB0')  # open serial port
        print(ser.name) 
        self.send_response(HTTPStatus.OK)
        self.end_headers()
        data=ser.readline()
        self.wfile.write(data)


httpd = socketserver.TCPServer(('', 8000), Handler)
httpd.serve_forever()

