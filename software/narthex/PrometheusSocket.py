from twisted.internet import protocol
import json

class PrometheusSocket(protocol.Protocol):

	def __init__(self, clients, serial):
		self.clients = clients
		self.serial = serial

	def connectionMade(self):
		self.clients.append(self)

		print "PrometheusSocket: Client Connected, Total clients: " + str(len(self.clients))

	def connectionLost(self, reason):
		self.clients.remove(self)

		print "PrometheusSocket: Client Disconnected, Total clients: " + str(len(self.clients))

	def dataReceived(self, data):

		print "PrometheusSocket: Received data: " + data

		jsonData = {}
		
		try:
			jsonData = json.loads(data)
			
		except:
			# The JSON Parser threw an exception
			self.transport.write(json.dumps({'error': 'Failed to parse JSON data.'}))
			
		else:
			# We successfully parsed the JSON data
			
			if jsonData != {}:
				try:
					jointNumber = int(jsonData['jointNumber'])

					# Check if we received a known command
					if jsonData['command'] == "setJointAngle":
						angle = int(jsonData['angle'])

						self.serial.setJointAngle(jointNumber, angle)
					
					elif jsonData['command'] == "getJointAngle":
						self.serial.getJointAngle(jointNumber)
					
					elif jsonData['command'] == "getJointLimits":
						self.serial.getJointLimits(jointNumber)

					elif jsonData['command'] == "setJointGains":
						PGain = float(jsonData['PGain'])
						IGain = float(jsonData['IGain'])
						DGain = float(jsonData['DGain'])

						self.serial.setJointGains(jointNumber, PGain, IGain, DGain)

					elif jsonData['command'] == "getJointGains":
						self.serial.getJointGains(jointNumber)

					elif jsonData['command'] == "getjointError":
						self.serial.getjointError(jointNumber)

					else:
						self.transport.write(json.dumps({'error': 'Unknown command.'}))
				except:
					self.transport.write(json.dumps({'error': 'Invalid command data.'}))
			else:
				self.transport.write(json.dumps({'error': 'No data received. Please send data as JSON.'}))

class PrometheusSocketFactory(protocol.Factory):

	def __init__(self, serial):
		self.clients = list()
		self.serial = serial
		
		print "PrometheusSocket: WebSocket Server Started"

	def buildProtocol(self, addr):
		return PrometheusSocket(self.clients, self.serial)

	def writeToAll(self, data):
		for client in self.clients:
			client.transport.write(data)