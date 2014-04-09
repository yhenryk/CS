from twisted.protocols import basic
from time import time
import httplib
import re
import logging

PORT1 = 12782
LOC_PATTERN = re.compile(r'([\+\-]\d+\.\d+)([\+\-]\d+\.\d+)')

class MessageReceiver(basic.LineReceiver):
	name = 'Bryant' #server Bryant
	database = {} #Database of client info 
	
	#Log connection made
	def connectionMade(self):
		print "Connection made. "
		logging.info("Connection made. ")
		
	#log connection dropped
	def connectionLost(self, reason):
		print "Connection dropped: " + str(reason)
		logging.info("Connection dropped: " + str(reason))
	
	#Process incoming client message
	def lineReceived(self, line):
		logging.info("Input message: "+ line)
		try:
			msg = line.split()
			if msg[0] =="IAMAT":
				self.processIAMAT(msg)
			elif msg[0] =="WHATSAT":
				self.processWHATSAT(msg)
			else:
				outmsg = "?" + msg[0] + "\n"
				self.transport.write(outmsg)
				logging.info("Command unrecognized. OUTPUT MSG: " +outmsg)
		except IndexError:
			self.transport.write("?\n")
			logging.info("Empty message. OUTPUT MSG: ?\n")
			
	#Process 'AT' message
	def processIAMAT(self, data):
		logging.info("Processing IAMAT message. ")
		
		#get message components :: should check if it exists?
		try: 
			clientID = data[1]
			print "ClientID: " +str(data[1])
			location = data[2]
			time_stamp = data[3]
		except IndexError:
			outmsg = "Error: Syntax is wrong."
			self.transport.write(outmsg)
			logging.warning("Index Error. OUTPUT MSG: " +outmsg)
			return
			
		#compute time difference
		try:
			timediff = time() - float(time_stamp)
			print "time diff: " +str(timediff)
			print "location: " +str(location)
		except:
			outmsg = "Error computing time difference. Check that your command is in the proper format. \n"
			self.transport.write(outmsg)
			return
			
		#update database with client info
		outmsg = outmsg = "AT " "%+f" %(timediff) + " " + self.name  + " " + clientID + " " + str(location)+ " " + str(time_stamp) + "\n"
		client_dict = {'Server_ID': self.name, 'time_in': time_stamp, 'loc_': location ,'at_msg': outmsg}
		self.database[client_ID] = client_dict

		#output response message
		self.transport.write(outmsg)
		
		#send to other servers somehow
		
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
			
			#get latitude and longitude
			location = self.database[clientID]['loc_']
			try:
				m = re.search(LOC_PATTERN, location)
				latitude = m.group(1)
				latitude = m.group(2)
				print 'latitude: ' +str(lat)
				print 'longitude: ' +str(long)
			except:
				self.transport.write("Error parsing location coordinates")
				logging.warning("OUTPUT MSG: Error parsing location coordinates")
				return
			
			#lat, long = self._getLatLong(location)
			
			#dfrd = self._queryTwitter(clientID, radius, max_tweets)
			#dfrd.addCallback(self._handleQueryResult)
			#dfrd.addErrBack(self._handlQueryFailure)
		
		#client doesn't exist in database
		else:
			outmsg ="No information found for client $s\n" %clientID
			self.transport.write(outmsg)
	def _getLatLong(self, location):
		try:
			m = re.search(LOC_PATTERN, location)
			latitude = m.group(1)
			longitude = m.group(2)
			return latitude, longitude
		except:
			self.transport.write("Error parsing location coordinates")	

#$$ END class Message Receiver

class ClientProtocol(basic.LineReceiver):
	def connectionMade(self):
		self.factory.originator.forwardLine(self)
		self.transport.loseConnection()
		
from twisted.internet import protocol
from twisted.application import service, internet 


factory = protocol.ServerFactory()
factory.protocol = MessageReceiver
factory.clients = []

application = service.Application("Server")

internet.TCPServer(PORT1, factory).setServiceParent(application)