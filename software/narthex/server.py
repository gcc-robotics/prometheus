from twisted.internet import reactor, task
from txws import WebSocketFactory

import BaseHTTPServer, SimpleHTTPServer, os

from PrometheusSerial import PrometheusSerial
from PrometheusSocket import PrometheusSocketFactory, PrometheusSocket

# Serial
serial = PrometheusSerial(reactor)

serialTask = task.LoopingCall(serial.proccessData)
serialTask.start(0.1)

# HTTP Server
IP = ""
PORT = 80
server_address = (IP, PORT)

os.chdir("/home/prometheus/prometheus/narthex/")

server = BaseHTTPServer.HTTPServer(server_address, SimpleHTTPServer.SimpleHTTPRequestHandler)
server.timeout = 0.1

webServerTask = task.LoopingCall(server.handle_request)
webServerTask.start(0.1)

print "PrometheusWeb: Web Server Started"

# WebSocket Server
socketFactory = PrometheusSocketFactory(serial)
serial.setSocket(socketFactory)

reactor.listenTCP(8888, WebSocketFactory(socketFactory))

# Testing

# def sendSocketData():
# 	global socketFactory
# 	import time, json

# 	time.sleep(1)
# 	socketFactory.writeToAll(json.dumps({"command": "Hello!"}))
# 	print "LoopingCall called me!"

# test = task.LoopingCall(sendSocketData)
# test.start(1)

reactor.run()