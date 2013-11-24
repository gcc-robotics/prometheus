import BaseHTTPServer
import SimpleHTTPServer
import json
import serial

IP = ""
PORT = 80
SERIAL_PORT = '/dev/ttyUSB0'

global server
global comm

class requestHandler(SimpleHTTPServer.SimpleHTTPRequestHandler):

	def do_POST(self):
		if self.path == "command/setJointAngle/":
			### JSON control command from the control interface

			length = int(self.headers.getheader('content-length'))        
			dataString = self.rfile.read(length)
			jsonData = {}
			
			try:
				jsonData = json.loads(dataString)
			except:
				response = "Failed to parse input json"
			
			if jsonData != {}:
				jointNumber = int(jsonData['jointNumber'])
				setPoint = int(jsonData['setPoint'])
				
				command = "setJointAngle " + str(jointNumber) + " " + str(setPoint)
				response = 	"Sending command: " + command
				
				# Send command to Arduino
				comm.write(command)

		else:
			response = "Unknown command"

		self.wfile.write(response)


def startServer():
	global server
	
	server_address = (IP, PORT)
	server = BaseHTTPServer.HTTPServer(server_address, requestHandler)
	server.timeout = 0.1

def startSerial():
	global comm
	
	comm = serial.Serial(SERIAL_PORT, 115200, timeout=0.01)
	comm.open()

def main():
	# Setup
	startSerial()
	startServer()
	
	global server, comm
	
	# Loop
	try:
		while True:
			# Handle HTTP requests
			server.handle_request()
			
			# Handle serial communication
			response = comm.readline()
			print response + "\n"
			
	except KeyboardInterrupt:
		comm.close()

if __name__ == "__main__":
	main()