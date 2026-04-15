*This project has been created as part of the 42 curriculum by lud-adam, enchevri, kbarru.*

# ft_irc

**An IRC server in C++98**

## Description

The goal of this project is to create a minimal IRC server that can handle communication between clients connected to it.
The server must be developped using the C++98 standard.

### About IRC

IRC (Internet Relay Chat) is a text-based communication protocol for instant messaging.
IRC is designed for group communication in discussion forums, called channels, 
but also allows one-on-one communication via private messages as well as chat and data transfer, including file sharing.
Users can create and subscribe to *channels* in order to set the topic of a conversation between multiple clients.
The protocol also allows for *private messages* to be sent to other clients.

### Features

+ Handling of multiple simulaneous client without hunging
+ Client/Server communication via the TCP/IP protocol
+ Client authentication with nickname and username
+ Join a channel as client
+ Send and recieve private messages between clients
+ Forwarding of all messages from a client to a channel to every other client that joined the channel
+ *Operators* support

#### Commands available to operators

+ `KICK` - Eject a client from the channel
+ `INVITE` - Invite a client to a channel
+ `TOPIC` - Change or view the channel topic
+ `MODE` - Change the channel mode :
    + `i` : Toggle Invite-only channel
    + `t` : Set or remove restrictions of the `TOPIC` command to channel operators
    + `k` : Set or remove the channel key
    + `o` : Give or take channel operator privilege
    + `l` : Set or remove user limit to channel

### Limitations

+ No server to server communication


## Instructions

`clone` the repository, then run `make` to compile it.

the server can then be run with the command :

`./ircserv <port> <password>`

`port` : the port numer the server will listen to for incoming IRC connections.

`password` : The connection password. It will be needed by any IRC client that tries to connect to the server.

## Resources

[Original IRC specification : RFC 1459](https://www.rfc-editor.org/rfc/rfc1459)

### AI usage
