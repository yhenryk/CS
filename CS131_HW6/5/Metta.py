from twisted.protocols import basic
from time import time
import httplib
import re
import logging
from twisted.internet import protocol, reactor, defer
from twisted.application import service, internet


PORT = 12784
PATTERN = re.compile('([\+\-]\d+\.\d+)([\+\-]\d+\.\d+)')

# Set up logging
logging.basicConfig(filename='hw6log.log', level=logging.DEBUG)
class MessageReceiver(basic.LineReceiver):
	name = 'Gasol' #server Gasol
	database = {} #Database of client info 
	
	#Log connection made
	def connectionMade(self):
		logging.info("Connection made. ")
		
	#log connection dropped
	def connectionLost(self, reason):
		logging.info("Connection dropped: " + str(reason))
	
	#Process incoming client message
	def lineReceived(self, line):
		logging.info("Input message: "+ line)
		try:
			msg = line.split()
			if msg[0] =="IAMAT":
				self.processIAMAT(msg)
			elif msg[0] == "AT":
				self.processAT(msg)
			elif msg[0] =="WHATSAT":
				self.processWHATSAT(msg)
			else:
				outmsg = "?" + " " + msg[0] + "\n"
				self.transport.write(outmsg)
				logging.info("Command unrecognized. OUTPUT MSG: " +outmsg)
		except IndexError:
			self.transport.write("?\n")
			logging.info("Empty message. OUTPUT MSG: ?\n")
	
	#Process 'AT' message
	def processAT(self, data):
		try:
			timediff = data[1]
			server = data[2]
			clientID = data[3]
			location = data[4]
			time_stamp = data[5]
			#check if data exists and is the same
			if self.database.has_key(clientID):
				if time_stamp == self.database[clientID]['time_in']:
					if location == self.database[clientID]['loc_']:
						return
			#update because data is not the same
			outmsg = 'AT ' + timediff + ' ' + server + ' ' + clientID + ' ' + location + ' ' + time_stamp + '\n'
			lat, long = self._getLatLong(location)
			client_dict = {'Server_ID': server, 'time_in': time_stamp, 'loc_': location ,'at_msg': outmsg, '_lat': lat, '_long': long}
			self.database[clientID] = client_dict
			
			#send to other servers 
			self._received = outmsg
			factory = protocol.ClientFactory()
			factory.protocol = ClientProtocol
			factory.originator = self
			reactor.connectTCP('localhost', 12781, factory)
			
		except IndexError:
			outmsg = "?" + " " + data[0] + "\n"
			#self.transport.write(outmsg)
			logging.info("Command unrecognized. OUTPUT MSG: " +outmsg)
	#Process 'IAMAT' message
	def processIAMAT(self, data):
		logging.info("Processing IAMAT message. ")
		
		#get message components :: should check if it exists?
		try: 
			clientID = data[1]
			location = data[2]
			time_stamp = data[3]
		except IndexError:
			outmsg = "Error: Syntax is wrong."
			self.transport.write(outmsg)
			logging.warning("Index Error. OUTPUT MSG: " +outmsg)
			return
		
		#check if it exists
		if self.database.has_key(clientID):
			if time_stamp == self.database[clientID]['time_in']:
				if location == self.database[clientID]['loc_']:
					return
		#compute time difference
		try:
			timediff = time() - float(time_stamp)
			lat, long = self._getLatLong(location)
		except:
			outmsg = "Error computing time difference. Check that your command is in the proper format. \n"
			self.transport.write(outmsg)
			return
			
		#update database with client info
		outmsg = "AT " "%+f" %(timediff) + " " + self.name  + " " + clientID + " " + str(location)+ " " + str(time_stamp) + "\n"
		client_dict = {'Server_ID': self.name, 'time_in': time_stamp, 'loc_': location ,'at_msg': outmsg, '_lat': lat, '_long': long}
		self.database[clientID] = client_dict
		
		#send to other servers where the number is the port of what is needed to be sent
		self._received = outmsg
		factory = protocol.ClientFactory()
		factory.protocol = ClientProtocol
		factory.originator = self
		reactor.connectTCP('localhost', 12781, factory)
	
		#output response message
		self.transport.write(outmsg)
		
		
	def forwardLine(self, recipient):
		recipient.sendLine(self._received)
		
	#Process 'WHATSAT' message
	def processWHATSAT(self, data):

		#get message components
		try:
			clientID = data[1]
			radius = data[2]
			max_tweets = data[3]
			if int(max_tweets) > 100:
				max_tweets = str(100)
				self.transport.write("Max results = 100, setting max_tweets to 100. \n")			 
		except IndexError:
			self.transport.write("Error: Syntax incorrect for WHATSAT")
			return
			
		#if client exists, query twitter
		if self.database.has_key(clientID):
			self.transport.write(self.database[clientID]['at_msg'])
			dfrd = self._queryTwitter(clientID, radius, max_tweets)
			dfrd.addCallback(self._handleQueryResult)
			dfrd.addErrback(self._handleQueryFailure)
		
		#client doesn't exist in database
		else:
			outmsg ="No information found for client %s\n" %clientID
			self.transport.write(outmsg)
		return
	def _queryTwitter(self, clientID, radius, max_tweets):
		d = defer.Deferred()
		
		lat = str(float(self.database[clientID]['_lat']))
		long = str(float(self.database[clientID]['_long']))
		query = "http://search.twitter.com/search.json?q=&geocode=" + lat + "," + long + "," + radius + "km&rpp=" + max_tweets +"&page=1"
		conn = httplib.HTTPConnection("www.twitter.com")
		conn.request("GET", query)
		response = conn.getresponse()
		query_result = response.read()
		
		d.callback(query_result)
		return d
	def _handleQueryResult(self, query_result):
		self.transport.write(query_result)
		logging.info("Query successful. OUTPUT: " +query_result)

	def _handleQueryFailure(self, query_failure):
		self.transport.write("Error in Twitter query.\n")
		logging.error("Errback. OUTPUT: Error in Twitter query.\n")
		query_failure.trap(RuntimeError)
	
	def _getLatLong(self, location):
		try:
			m = PATTERN.match(location)
			latitude = m.group(1)
			longitude = m.group(2)
			return latitude, longitude 	
		except:
			self.transport("Error parsing location coordinates")
			logging.warning("OUTPUT MSG: Error parsing location coordinates")
#$$ END class Message Receiver

class ClientProtocol(basic.LineReceiver):
	def connectionMade(self):
		self.factory.originator.forwardLine(self)
		self.transport.loseConnection()	
		
#$$ End class CLientProtocol

factory = protocol.ServerFactory()
factory.protocol = MessageReceiver
factory.clients = []

application = service.Application("Server")

internet.TCPServer(PORT, factory).setServiceParent(application)
