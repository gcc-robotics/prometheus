import BaseHTTPServer
import SimpleHTTPServer
import json
#import serial

IP = ""
PORT = 80

global server
global comm

class requestHandler(SimpleHTTPServer.SimpleHTTPRequestHandler):

	def do_POST(self):
		length = int(self.headers.getheader('content-length'))        
		dataString = self.rfile.read(length)
		jsonData = {}
		
		try:
			jsonData = json.loads(dataString)
		except:
			result = "Failed to parse input json"
		
		if jsonData != {}:
			jointNumber = int(jsonData['jointNumber'])
			setPoint = int(jsonData['setPoint'])
			
			result = "Setting joint " + str(jointNumber) + " to " + str(setPoint) + " degrees!"	
			
			# Send command to arduino
			comm.write("setJointSetPoint " + str(jointNumber) + " " + str(setPoint))
			
		self.wfile.write(result)

def startServer():
	global server
	
	server_address = (IP, PORT)
	server = BaseHTTPServer.HTTPServer(server_address, requestHandler)

def startSerial():
	global comm
	
	comm = serial.Serial('/dev/ttyUSB0', 115200, timeout=0.01)
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
			#comm.write("testing")
			#response = comm.readline()
			#print response
			
	except KeyboardInterrupt:
		comm.close()

if __name__ == "__main__":
	main()