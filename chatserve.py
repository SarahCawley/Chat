#!/usr/bin/python           # This is server.py file

import socket               # Import socket module


s = socket.socket()         # Create a socket object
host = socket.gethostname() # Get local machine name
#port = 12345                # Reserve a port for your service.
print '\nhostname = "', host, '". Please use this host name in the chat client'

# Get port number from user
port = int(input("Please enter the port number to use: "))
# Bind to the port
s.bind((host, port))        

s.listen(5)                 # Now wait for client connection.
c, addr = s.accept()     # Establish connection with client.

while True:
	
	print 'Got connection from', addr
	c.send('Thank you for connecting')

	userName = c.recv(1024)
	userName = userName.rstrip()
	print "hello ", userName
	message = raw_input('server send first message> ')
	c.send(message)


	while True:
		messageReceived = c.recv(1024)
		messageReceived = messageReceived.rstrip()
		#if the client ends the chat
		if messageReceived == "quit":
			print "The client has ended the chat. Have a nice day\n"
			print "Waiting for a new connection"
			s.listen(5)                 # Now wait for client connection.
			c, addr = s.accept()     # Establish connection with client.
			print 'Got connection from', addr
			c.send('Thank you for connecting')

			userName = c.recv(1024)
			userName = userName.rstrip()
			print "hello ", userName
			message = raw_input('server send first message> ')
			c.send(message)
			messageReceived = c.recv(1024)
			messageReceived = messageReceived.rstrip()
			print userName, "> ", messageReceived
			
		else:
			print userName, "> ", messageReceived

		message = raw_input("server> ")
		#if server wants to end chat
		if message == '\quit':
			print "Thank you for chatting today"
			c.send("quit")
			c.close() 
			quit()
		else:
			c.send(message)
   
c.close()                # Close the connection