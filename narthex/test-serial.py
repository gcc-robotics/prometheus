import serial


class PrometheusSerial:
	__serialPort = '/dev/ttyUSB0'
	dataSent = False
	#__connection
	
	def init(self):
		self.__connection = serial.Serial(self.__serialPort, 9600, timeout=0.01) # 115200
		#self.__connection.stopbits = 2
		self.__connection.open()

		# Throw away first data
		while inWaiting() > 0:
			self.__connection.readLine();
	
	def close(self):
		self.__connection.close()

	def iterate(self):
		if inWaiting() > 0:
			# We have data
			response = self.__connection.readLine();

			print "Data from Arduino: " + response
			dataSent = False

		elif dataSent == True:
			# No data get user input and not waiting for a response
			userInput = raw_input("Command: ")
			self.__connection.write(userInput)
			dataSent = True

	
	def getResponse(self):
		response = ""
		
		while response == "":
			response = self.__connection.readLine()
		
		return str(response)


def main():
	comm = PrometheusSerial()
	comm.init();

	print "Serial port opened"

	try:
		while True:
			comm.iterate()
			
	except KeyboardInterrupt:
		comm.close()

if __name__ == "__main__":
	main()