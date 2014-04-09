from twisted.protocols import basic
from time import time
import httplib
import re

PORT1 = 12780
PORT2 = 12781
LOC_PATTERN = re.compile(r'([\+\-]\d+\.\d+)([\+\-]\d+\.\d+)')
class MessageReceiver(basic.LineReceiver):
	name = 'Blake'
	database = {} #Database of client info 
	def connectionMade(self):
		print "Connection made. "

	def connectionLost(self, reason):
		print "Connection dropped: " + str(reason)

	def lineReceived(self, line):
		try:
			msg = line.split()
			if msg[0] =="IAMAT":
				self.processIAMAT(msg)
			elif msg[0] =="WHATSAT":
				self.processWHATSAT(msg)
			else:
				outmsg = "?" + msg[0] + "\n"
				self.transport.write(outmsg)
		except IndexError:
			self.transport.write("?\n")
	#Process 'AT' message
	def processIAMAT(self, data):
		#get message components
		try: 

			clientID = data[1]
			print "ClientID: " +str(data[1])
			location = data[2]
			time_stamp = data[3]
		except IndexError:
			return
		#compute time difference, latitude/longtitude
		try:
			timediff = time() - float(time_stamp)
			print "time diff: " +str(timediff)
			print "location: " +str(location)
		except:
			outmsg = "Error computing time difference or latitude/longitude coordinates. Check that your command is in the proper format. \n"
			self.transport.write(outmsg)
			return
		#update database with client info
		outmsg = outmsg = "AT " + self.name + " " + "%+f" %(timediff) + " " + clientID + " " + str(time_stamp)+ " " + str(location) + "\n"
		client_dict = {'time_in': time_stamp, 'loc_': location ,'at_msg': outmsg}
		client_ID = data[1]
		self.database[client_ID] = client_dict

		#output response message
		self.transport.write(outmsg)
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
			return
		#if client exists, query twitter
		if self.database.has_key(clientID):
			self.transport.write(self.database[clientID]['at_msg'])
			location = self.database[clientID]['loc_']
			m = re.search(LOC_PATTERN, location)
			latitude = m.group(1)
			latitude = m.group(2)
			
			#lat, long = self._getLatLong(location)
			print 'latitude: ' +str(lat)
			print 'longitude: ' +str(long)
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

from twisted.internet import protocol
from twisted.application import service, internet 

factory2 = protocol.ServerFactory()
factory2.protocol = MessageReceiver
factory = protocol.ServerFactory()
factory.protocol = MessageReceiver
factory.clients = []

application = service.Application("Server")

internet.TCPServer(PORT1, factory).setServiceParent(application)
internet.TCPServer(PORT2, factory2).setServiceParent(application)
