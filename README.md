# Chat
A basic chat program. The Server in written in Python and the client is wirtten in C.

Compile the client:
	1) run `make`

Start the server:
	1) In one terminal run `python chatserve.py`. 
	2) The script witll print out the hostname of the machine you are currently on. 
	3) Please choose which port you would like to chat on.

Start the client:
	1) In a second terminal run `./chatclient [hostname] [port]`


Order of messages:
	1) The server will send a "Thank you for connecting" message to the client
	2) The client should enter their username
	3) The server writes the first message
	4) Messages must be sent in order starting with the server, then client, server, etc.

To exit the chat:
	Either the client or the server can end the chat by entering `\quit`
	At least one series of messages must be passed before you can quit

	If the server ends the chat both parites will be kicked out
	If the client ends the chat the server will wait for the chat to start again