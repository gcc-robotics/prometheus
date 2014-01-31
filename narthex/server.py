import BaseHTTPServer
import SimpleHTTPServer
import json
import serial

IP = ""
PORT = 80

global server

class PrometheusSerial:
	__serialPort = '/dev/ttyUSB0'
	#__connection
	
	def init(self):
		self.__connection = serial.Serial(self.__serialPort, 9600, timeout=0.01) # 115200
		#self.__connection.stopbits = 2
		self.__connection.open()
	
	def close(self):
		self.__connection.close()
	
	def getResponse(self):
		response = ""
		
		while response == "":
			response = self.__connection.readline()
		
		return str(response)
	
	def setJointAngle(self, jointNumber, angle):
		self.__connection.write("setJointAngle " + str(jointNumber) + " " + str(angle))

		return json.dumps({'msg': 'Joint ' + str(jointNumber) + " set to " + str(angle)})
		
	def getJointAngle(self, jointNumber):
		self.__connection.write("getJointAngle " + str(jointNumber))
		
		data = self.getResponse().split()

		print data
		
		if data[0] == 'jointAngle' and data[1] == str(jointNumber):
			response = json.dumps({'jointNumber': jointNumber, 'angle': str(data[2])})
		else:
			response = json.dumps({'error': 'Bad data received from arm'})
		
		return response
	
	def getJointLimits(self, jointNumber):
		self.__connection.write("getJointLimits  " + str(jointNumber))
		
		response = self.getResponse()

		print "Data from Arduino: " + response

		data = response.split()
		
		if data[0] == 'jointLimits' and data[1] == str(jointNumber):
			response = json.dumps({'jointNumber': jointNumber, 'min': str(data[2]), 'max': str(data[3])})
		else:
			response = json.dumps({'error': 'Bad data received from arm'})
		
		return response
		

class requestHandler(SimpleHTTPServer.SimpleHTTPRequestHandler):

	def do_POST(self):
		global comm

		# Get the data
		length = int(self.headers.getheader('content-length'))        
		dataString = self.rfile.read(length)
		jsonData = {}

		print "POST request: " + self.path
		print "Request Data: " + dataString
		
		try:
			jsonData = json.loads(dataString)
			
		except:
			# The JSON Parser threw an exception
			response = json.dumps({'error': 'Failed to parse JSON data.'})
			
		else:
			# We successfully parsed the JSON data
			
			if jsonData != {}:
				# Check if we received a known command
				if self.path == "/command/setJointAngle/":
					jointNumber = int(jsonData['jointNumber'])
					angle = int(jsonData['angle'])

					response = comm.setJointAngle(jointNumber, angle)
				
				elif self.path == "/command/getJointAngle/":
					jointNumber = int(jsonData['jointNumber'])

					response = comm.getJointAngle(jointNumber)
				
				elif self.path == "/command/getJointLimits/":
					jointNumber = int(jsonData['jointNumber'])

					response = comm.getJointLimits(jointNumber)

				else:
					response = json.dumps({'error': 'Unknown command.'})
			else:
				response = json.dumps({'error': 'No data received. Please send data as JSON.'})

		print "Response: " + response

		# Send response back to the web control interface
		self.wfile.write(response)


def startServer():
	global server
	
	server_address = (IP, PORT)
	server = BaseHTTPServer.HTTPServer(server_address, requestHandler)
	server.timeout = 0.1



def main():
	global server, comm

	# Setup
	startServer()

	comm = PrometheusSerial()
	comm.init();

	print "Prometheus Remote Control Server Started!"
	
	# Loop
	try:
		while True:
			# Handle HTTP requests
			server.handle_request()
			
			# Handle serial communication
			#response = comm.readline()
			#print response + "\n"
			
	except KeyboardInterrupt:
		comm.close()

if __name__ == "__main__":
	main()