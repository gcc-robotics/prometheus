from twisted.internet import reactor
from txws import WebSocketFactory

import BaseHTTPServer
import SimpleHTTPServer

from PrometheusSerial import PrometheusSerial
from PrometheusSocket import PrometheusSocketFactory, PrometheusSocket

# Serial
serial = PrometheusSerial()

reactor.callInThread(serial.proccessData)

# HTTP Server
IP = ""
PORT = 80
server_address = (IP, PORT)
server = BaseHTTPServer.HTTPServer(server_address, SimpleHTTPServer.SimpleHTTPRequestHandler)
server.timeout = 0.1

reactor.callInThread(server.handle_request)

print "PrometheusWeb: Web Server Started"

# WebSocket Server
socketFactory = PrometheusSocketFactory(serial)
serial.setSocket(socketFactory)

reactor.listenTCP(8888, WebSocketFactory(socketFactory))

reactor.run()