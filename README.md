*This project has been created as part of the 42 curriculum by lud-adam, enchevri, kbarru.*

# ft_irc

**An IRC server in C++98**

## Description

The goal of this project is to create a minimal IRC server that can handle communication between clients connected to it.
The server must be developed using the C++98 standard.

### About IRC

IRC (Internet Relay Chat) is a text-based communication protocol for instant messaging.
IRC is designed for group communication in discussion forums, called channels,
but also allows one-on-one communication via private messages as well as chat and data transfer, including file sharing.
Users can create and subscribe to *channels* in order to set the topic of a conversation between multiple clients.
The protocol also allows for *private messages* to be sent to other clients.

### Features

+ Handling of multiple simultaneous clients without hanging, using a single `epoll` event loop
+ Client/Server communication via the TCP/IP protocol
+ Password-protected connection (`PASS`)
+ Client authentication with nickname and username (`NICK`, `USER`)
+ Join and leave channels as a client (`JOIN`, `PART`)
+ Send and receive private messages between clients (`PRIVMSG`)
+ Forwarding of all messages sent to a channel to every other client that joined the channel
+ `WHO` and `QUIT` support
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

The server can then be run with the command :

`./ircserv <port> <password>`

`port` : the port number the server will listen to for incoming IRC connections.

`password` : The connection password. It will be needed by any IRC client that tries to connect to the server.

### Other Makefile targets

+ `make run` : build and launch the server (override defaults with `make run PORT=6667 PASSWORD=pass`)
+ `make debug` : build `ircserv_debug`, a build of the server with debug output enabled
+ `make bot` : build the bonus IRC bot (see below)
+ `make runbot` : build and launch the bot (override defaults with `make runbot BOT_PORT=6667 BOT_PASSWORD=password BOT_CHANNEL='#bot'`)

## Bonus : IRC bot

The `IRC_BOT/` directory contains a bot that connects to the server as a regular client and lets users play **2048** over IRC.

Build it with `make bot` (or `make` inside `IRC_BOT/`), then, with the server already running :

`./IRC_BOT/ircbot <port> <password> <channel>`

or simply `make runbot`.

Once connected, send `!2048` to the bot in a private message to start a game, then play by sending a single character per move.

## Resources

[Original IRC specification : RFC 1459](https://www.rfc-editor.org/rfc/rfc1459)

### AI usage

Ai was used to fix the typos in this README.
