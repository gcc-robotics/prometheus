import json
import serial

class PrometheusSerial:
	portPrefix = '/dev/ttyUSB'
	portNumber = 0

	connected = False
	connection = None

	socket = None

	def __init__(self, socket):
		print "PrometheusSerial: Starting serial communication"

		self.init()
	
	def init(self):
		serialPort = self.portPrefix + str(self.portNumber)

		try:
			print "PrometheusSerial: Attempting to connect on port " + serialPort
			self.connection = serial.Serial(serialPort, 115200, timeout=0.05) # 9600, 115200

		except serial.serialutil.SerialException, e:
			self.portNumber += 1

			if self.portNumber > 9:
				self.portNumber = 0
				self.connected = False

				print "PrometheusSerial: Failed to establish communication"

			else:
				self.init()

		else:
			#self.connection.stopbits = 2
			self.connection.open()
			self.connected = True

			print "PrometheusSerial: Successfully connected on port " + serialPort

			linesToSkip = 2
			currentLine = 0
		
			while self.connection.inWaiting() > 0 or currentLine < linesToSkip:
				self.connection.readline()
				currentLine += 1
	
	def close(self):
		self.connection.close()
		self.connected = False

	def setSocket(self, socket):
		self.socket = socket

	def writeToSocket(self, data):
		if self.socket != None:
			self.socket.writeToAll(data)

	def proccessData(self):
		while self.connected:
			if self.connection.inWaiting() > 0:
				data = self.connection.readline().split()

				print "PrometheusSerial: Data from arduino: " + str(data)

				command = data[0]

				jsonData = {
					"command": command
				}

				if command == "armStatus":
					jsonData["currentAngle"] = {}
					jsonData["currentError"] = {}

					for index in xrange(5):
						jsonData["currentAngle"][index] = data[index + 1]

					for index in xrange(5):
						jsonData["currentError"][index] = data[index + 6]

					self.writeToSocket(json.dumps(jsonData))

				elif command == "jointAngle":
					jsonData["jointNumber"] = data[1]
					jsonData['angle'] = data[2]

					self.writeToSocket(json.dumps(jsonData))

				elif command == "jointLimits":
					jsonData["jointNumber"] = data[1]
					jsonData["min"] = data[2]
					jsonData["max"] = data[3]

					self.writeToSocket(json.dumps(jsonData))

				elif command == "jointGains":
					jsonData["jointNumber"] = data[1]
					jsonData["PGain"] = data[2]
					jsonData["IGain"] = data[3]
					jsonData["DGain"] = data[4]

					self.writeToSocket(json.dumps(jsonData))
				elif command == "jointError":
					jsonData["jointNumber"] = data[1]
					jsonData["error"] = data[2]

					self.writeToSocket(json.dumps(jsonData))

				else:
					print "PrometheusSerial: Unknown command received from Arduino"


	def setJointAngle(self, jointNumber, angle):
		if self.connected:
			self.connection.write("setJointAngle " + str(jointNumber) + " " + str(angle))

	def getJointAngle(self, jointNumber):
		if self.connected:
			self.connection.write("getJointAngle " + str(jointNumber))
	
	def getJointLimits(self, jointNumber):
		if self.connected:
			self.connection.write("getJointLimits  " + str(jointNumber))

	def setJointGains(self, jointNumber, PGain, IGain, DGain):
		if self.connected:
				self.connection.write("setJointGains " + str(jointNumber) \
				                      " " + str(PGain) \
				                      " " + str(IGain) \
				                      " " + str(DGain))

	def getJointGains(self, jointNumber):
		if self.connected:
			self.connection.write("getJointGains  " + str(jointNumber))

	def getJointError(self, jointNumber):
		if self.connected:
			self.connection.write("getJointError  " + str(jointNumber))