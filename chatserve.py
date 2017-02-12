#!/usr/bin/python           
# Sarah Cawley 2/12/2017, Intro to computer networks
# project 1 "create a client/ server chat program using sockets"
# python server architecture borrowed from https://www.tutorialspoint.com/python/python_networking.htm

import socket               # Import socket module
import sys					# Import system to read from command line

#gets user name. returns user Name
def getUserName():
	userName = c.recv(1024)
	userName = userName.rstrip()
	print "hello ", userName
	return userName

#reads in and prints message from client, Takes user name
#returns 1 if client quit chat, 0 if not
def clientMessage(userName):
	messageReceived = c.recv(1024)
	messageReceived = messageReceived.rstrip()
	if messageReceived == "quit":
		print "The client has ended the chat. Have a nice day\n"
		print "Waiting for a new connection"
		return 1
	else:
		print userName, "> ", messageReceived
		return 0

#starts the server. Returns the hostname, port number and socket
def startServer():
	s = socket.socket()         # Create a socket object
	host = socket.gethostname() # Get local machine name
	print '\nhostname = "', host, '". Please use this host name in the chat client'
	print 'port number: ',  sys.argv[1]
	port = int(sys.argv[1])
	return s, host, port

# binds the socket, returns client connection, sends thank you message
def listen(s, host, port):
	# s.bind((host, port))        
	s.listen(5)              # waitfor client connection.
	c, addr = s.accept()     # Establish connection with client.
	c.send('Thank you for connecting')
	print 'Got connection from', addr
	return c

# gets server message, sends message from the server
def serverMessage(c):
	message = raw_input("server> ")
	
	if message == '\quit':
		print "Thank you for chatting today"
		c.send("quit")
		return 1
	else:
		c.send(message)
		return 0

#check port number was send in
if( len(sys.argv) < 2):
	print "usage: python chatserve.py [port number]\n"
	quit()

# Start the server
s, host, port = startServer()

# Bind to the port
s.bind((host, port)) 
c = listen(s, host, port)

while True:
	
	userName = getUserName()
	#serverMessage(c)

	while True:
		clientQuit = clientMessage(userName)
		
		#if the client has quit, wait for new connection
		if clientQuit == 1:
			c = listen(s, host, port)
			userName = getUserName()
			clientMessage(userName)

		# if the server quits end program
		serverQuit = serverMessage(c)
		if (serverQuit == 1):
			c.close() 
			quit()
		
   
c.close()                # Close the connection