import json
import serial

class PrometheusSerial:
	portPrefix = '/dev/ttyUSB'
	portNumber = 0

	connected = False
	connection = None

	def __init__(self):
		print "PrometheusSerial: Starting serial communication"

		self.init()
	
	def init(self):
		serialPort = self.portPrefix + str(self.portNumber)

		try:
			print "PrometheusSerial: Attempting to connect on port " + serialPort
			self.connection = serial.Serial(serialPort, 115200, timeout=0.01) # 9600, 115200

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
		
			while self.connection.inWaitting() > 0 or currentLine < linesToSkip:
				self.connection.readline()
				currentLine += 1
	
	def close(self):
		self.connection.close()
		self.connected = False

	def proccessData(self):
		if self.connected:
			if self.connection.inWaitting() > 0:
				data = self.__connection.readline().split()

				print "Data from arduino: "
				print data

		else:
			# Try reconnecting?
			pass

	def setJointAngle(self, jointNumber, angle):
		if self.connected:
			self.connection.write("setJointAngle " + str(jointNumber) + " " + str(angle))

	def getJointAngle(self, jointNumber):
		if self.connected:
			self.connection.write("getJointAngle " + str(jointNumber))
	
	def getJointLimits(self, jointNumber):
		if self.connected:
			self.connection.write("getJointLimits  " + str(jointNumber))