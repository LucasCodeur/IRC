#include "ReplyBuilder.hpp"
#include "Channel.hpp"
#include "NumericReplies.h"
#include "debug.hpp"
#include <ctime>
#include <string>
/**
 * @brief allows to setting the params.
 * @param string params to add inside the reply.
 * @return an object of the replyBuilder in order to do the chaining of differents adds.
 */
ReplyBuilder&	ReplyBuilder::addParams(std::string params)
{	
	this->_reply.setParams(params);
	return (*this);
}

/**
 * @brief allows to setting the prefix.
 * @return an object of the replyBuilder in order to do the chaining of differents adds.
 */
ReplyBuilder&	ReplyBuilder::addPrefix(std::string const &prefix)
{
	this->_reply.setPrefix(prefix);

	return (*this);
}

/**
 * @brief allows to setting the numeric.
 * @param string numeric to add inside the reply.
 * @return an object of the replyBuilder in order to do the chaining of differents adds.
 */
ReplyBuilder&	ReplyBuilder::addNumeric(std::string numeric)
{
	this->_reply.setNumeric(numeric);
	return (*this);
}

/**
 * @brief allows to setting the trailing.
 * @param string trailing to add inside the reply.
 * @return an object of the replyBuilder in order to do the chaining of differents adds.
 */
ReplyBuilder&	ReplyBuilder::addTrailing(std::string trailing)
{
	std::string temp;

	temp = PREFIX_MARKER;
	temp += trailing;
	this->_reply.setTrailing(temp);

	return (*this);
}

/**
 * @brief allows to setting the crln.
 * @return an object of the replyBuilder in order to do the chaining of differents adds.
 */
ReplyBuilder&	ReplyBuilder::addCrln()
{
	this->_reply.setCrln(CRLN);
	return (*this);
}

/**
 * @brief allows to reset all the information inside reply.
 * @return an object of the replyBuilder in order to do the chaining of differents adds.
 */
ReplyBuilder&	ReplyBuilder::reset()
{
	this->_reply.setNumeric("");
	this->_reply.setParams("");
	this->_reply.setTrailing("");
	this->_reply.setPrefix(":");
	this->_reply.setCrln("");

	return (*this);
}

/**
 * @brief allows concatenating all the strings.
 * @return a string numeric reply to be sent.
 */
std::string	ReplyBuilder::buildReply()
{
	std::string reply;

	reply += this->_reply.getPrefix();
	if (!this->_reply.getNumeric().empty())
		reply += SPACE + this->_reply.getNumeric();
	if (!this->_reply.getParams().empty())
		reply += SPACE + this->_reply.getParams();
	if (!this->_reply.getTrailing().empty())
		reply += SPACE + this->_reply.getTrailing();
	reply += this->_reply.getCrln();

	return (reply);
}

/**
 * @brief setter of the type of builder.
 * @param pointer ReplyBuilder to be set.
 * @return
 */
void Director::setBuilderType(ReplyBuilder* builder)
{
	this->builder = builder;
}

/**
* @brief creates the numeric reply of welcome when a client successfully connects to a irc server.
* @param client, the name of the person successfully connect.
* @return the reply in order to send it to the client.
*/
std::string	Director::rplWelcome(const std::string& nickname) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(RPL_WELCOME)
				.addParams(nickname)
				.addTrailing("Welcome to the IRC Network")
				.addCrln()
				.buildReply();

	//WARN: Maybe change the content of the runtime or even the runtime

	if (reply.size() > 512) 
		throw std::runtime_error("Reply longer than 512 characters");

	return (reply);
}

/**
* @brief creates the numeric reply to indicate the servername and the version, when a client successfully connects to a irc server.
* @param client, the name of the person successfully connect.
* @return the reply in order to send it to the client.
*/
std::string	Director::rplYourhost(const std::string& nickname) const
{
	ReplyBuilder	builder;

	std::string trailing = "Your host is ";
	trailing += SERVERNAME;
	trailing += ", ";
	trailing += SERVER_VERSION;

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(RPL_YOURHOST)
				.addParams(nickname)
				.addTrailing(trailing)
				.addCrln()
				.buildReply();

	if (reply.size() > 512) 
		throw std::runtime_error("Reply longer than 512 characters");
	return (reply);
}

/**
* @brief creates the numeric reply to indicate when the server was created.
* @param client, the name of the person successfully connect.
* @return the reply in order to send it to the client.
*/
std::string	Director::rplCreated(const std::string& nickname) const
{
	ReplyBuilder	builder;
	std::string		trailing = "This server was created ";

	trailing += "01/01/2026";

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(RPL_CREATED)
				.addParams(nickname)
				.addTrailing(trailing)
				.addCrln()
				.buildReply();

	if (reply.size() > 512) 
		throw std::runtime_error("Reply longer than 512 characters");
	return (reply);
}

std::string	Director::rplMyInfo(const std::string& nickname) const
{
	ReplyBuilder	builder;
	std::string		params = nickname;

	params += " ";
	params += SERVERNAME;
	params += " ";
	params += SERVER_VERSION;
	params += " ";
	params += "none user modes";
	params += " ";
	params += CHANNEL_MODES;

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(RPL_MYINFO)
				.addParams(params)
				.addCrln()
				.buildReply();

	if (reply.size() > 512) 
		throw std::runtime_error("Reply longer than 512 characters");
	return (reply);
}

std::string Director::rplJoin(Client const &client, Channel const &channel) const
{
	ReplyBuilder builder;

	std::string reply = builder
				.reset()
				.addPrefix(client.getNickname())
				.addParams("JOIN")
				.addParams(channel.getName())
				.addCrln()
				.buildReply();

	if (reply.size() > 512) 
		throw std::runtime_error("Reply longer than 512 characters");
	return (reply);
}

/**
* @brief creates the numeric reply of welcome when a client successfully connects to a irc server.
* @param client, the name of the person successfully connect.
* @return the reply in order to send it to the client.
*/
std::string	Director::rplChannelModeIs(std::string clientNick, std::string channelName, std::string modes, std::string password) const
{
	ReplyBuilder	builder;
	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(RPL_CHANNELMODEIS)
				.addParams(clientNick)
				.addParams(channelName)
				.addParams(modes)
				.addParams(password)
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string Director::errNeedMoreParams(std::string clientNick, std::string command) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(ERR_NEEDMOREPARAMS)
				.addParams(clientNick)
				.addParams(command)
				.addTrailing("Not enough parameters")
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}
/**
* @brief creates the numeric reply of welcome when a client successfully connects to a irc server.
* @param client, the name of the person successfully connect.
* @return the reply in order to send it to the client.
*/
std::string	Director::errNoSuchChannel(std::string clientNick, std::string channelName) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(ERR_NOSUCHCHANNEL)
				.addParams(clientNick)
				.addParams(channelName)
				.addTrailing("No such channel")
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string		Director::errNoSuchNick(std::string clientNick, std::string channelName) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(ERR_NOSUCHNICK)
				.addParams(clientNick)
				.addParams(channelName)
				.addTrailing("There was no such nickname")
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}
std::string		Director::errUnknownMode(std::string clientNick, std::string modeChar) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(ERR_UNKNOWNMODE)
				.addParams(clientNick)
				.addParams(modeChar)
				.addTrailing("is unknown mode char to me")
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string		Director::rplTopic(std::string clientNick, std::string channelName, std::string topic) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(RPL_TOPIC)
				.addParams(clientNick)
				.addParams(channelName)
				.addTrailing(topic)
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string	Director::rplPrivmsg(Client const &client, Channel const &channel, std::string const &msgContent) const
{
	ReplyBuilder builder;

	std::string reply = builder
				.reset()
				.addPrefix(client.getNickname())
				.addParams("PRIVMSG")
				.addParams(channel.getName())
				.addTrailing(msgContent)
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string	Director::rplPrivmsg(Client const &client, Client const &target, std::string const &msgContent) const
{
	ReplyBuilder builder;

	std::string reply = builder
				.reset()
				.addPrefix(client.getNickname())
				.addParams("PRIVMSG")
				.addParams(target.getNickname())
				.addTrailing(msgContent)
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string		Director::rplNoTopic(std::string clientNick, std::string channelName) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(RPL_NOTOPIC)
				.addParams(clientNick)
				.addParams(channelName)
				.addTrailing("No topic is set")
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string		Director::errNotOnChannel(std::string clientNick, std::string channelName) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(ERR_NOTONCHANNEL)
				.addParams(clientNick)
				.addParams(channelName)
				.addTrailing("You're not on that channel")
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string		Director::errChanOPrivsNeeded(std::string clientNick, std::string channelName) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(ERR_CHANOPRIVSNEEDED)
				.addParams(clientNick)
				.addParams(channelName)
				.addTrailing("You're not channel operator")
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}


std::string		Director::errBadChannelKey(std::string clientNick, std::string channelName) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(ERR_BADCHANNELKEY)
				.addParams(clientNick)
				.addParams(channelName)
				.addTrailing("Cannot join channel (+k)")
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string		Director::errUnknownCommand(std::string clientNick, std::string cmdKeyword) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(ERR_UNKNOWNCOMMAND)
				.addParams(clientNick)
				.addParams(cmdKeyword)
				.addTrailing("Unknown command")
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string Director::rplPart(Client const &client, Channel const &channel, std::string const &reason) const
{
	ReplyBuilder builder;

	std::string reply = builder
				.reset()
				.addPrefix(client.getNickname())
				.addParams("PART")
				.addParams(channel.getName())
				.addTrailing(reason)
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string Director::rplTopic(Client const &client, Channel const &channel) const
{
	ReplyBuilder builder;

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric("332")
				.addParams(client.getNickname() + " " + channel.getName())
				.addTrailing(channel.getTopic())
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string Director::rplNoTopic(Client const &client, Channel const &channel) const
{
	ReplyBuilder builder;

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric("331")
				.addParams(client.getNickname() + " " + channel.getName())
				.addTrailing("No topic is set")
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string Director::rplNameReply(Client const &client, Channel const &channel, std::string const &namesList) const
{
	ReplyBuilder builder;

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(RPL_NAMREPLY)
				.addParams(client.getNickname() + " = " + channel.getName())
				.addTrailing(namesList)
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string Director::rplEndOfNames(Client const &client, Channel const &channel) const
{
	ReplyBuilder builder;

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(RPL_ENDOFNAMES)
				.addParams(client.getNickname() + " " + channel.getName())
				.addTrailing("End of /NAMES list")
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string Director::rplWhoReply(Client const &requester, std::string const &channel, Client const &target, bool isOp) const
{
	ReplyBuilder builder;

	std::string flags = isOp ? "H@" : "H";
	std::string params = requester.getNickname() + " " + channel + " "
		+ target.getUsername() + " " + target.getHostname() + " "
		+ SERVERNAME + " " + target.getNickname() + " " + flags;

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(RPL_WHOREPLY)
				.addParams(params)
				.addTrailing("0 " + target.getRealname())
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string Director::rplEndOfWho(Client const &requester, std::string const &mask) const
{
	ReplyBuilder builder;

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(RPL_ENDOFWHO)
				.addParams(requester.getNickname() + " " + mask)
				.addTrailing("End of WHO list")
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string Director::rplNameReply(std::string clientNick, Channel &channel) const
{
	ReplyBuilder builder;
	std::vector<std::string> nicknamesVector = channel.listNames();
	std::string nicknames = "";

	for (std::vector<std::string>::iterator it = nicknamesVector.begin(); it != nicknamesVector.end(); ++it)
	{
		nicknames += (*it);	
		if (it + 1 == nicknamesVector.end())
			nicknames += " ";
	}
	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(RPL_NAMREPLY)
				.addParams(clientNick)
				.addParams(channel.getName())
				.addTrailing(nicknames)
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string Director::rplEndofNames(std::string clientNick, Channel &channel) const
{
	ReplyBuilder builder;

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(RPL_ENDOFNAMES)
				.addParams(clientNick)
				.addParams(channel.getName())
				.addTrailing("End of /NAMES list")
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string Director::rplInviting(std::string clientNick, std::string invitedNick, std::string channelName) const
{
	ReplyBuilder builder;

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(RPL_INVITING)
				.addParams(clientNick)
				.addParams(channelName)
				.addParams(invitedNick)
				.addCrln()
				.buildReply();
	PRINT(reply, YELLOW, "\n")
	return (reply);
}

/**
 * @brief Error reply Returned when a nickname parameter expected for a
 * command and isn't found.
 * @return the reply in order to send it to the client.
 */
std::string Director::errNonicknamegiven() const
{
	ReplyBuilder builder;

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(ERR_NONICKNAMEGIVEN)
				.addTrailing("No nickname given")
				.addCrln()
				.buildReply();
	PRINT(reply, YELLOW, "\n")
	return (reply);
}

std::string Director::errUserOnChannel(std::string clientNick, std::string invitedNick, std::string channelName) const
{
	ReplyBuilder builder;

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(ERR_USERONCHANNEL)
				.addParams(clientNick)
				.addParams(invitedNick)
				.addParams(channelName)
				.addTrailing("is already on channel")
				.addCrln()
				.buildReply();

	if (reply.size() > 512)
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

/**
* @brief Error reply returned by a server to a client when it detects a
* nickname collision.
* @param nickname string Nickname is already in use
* @return the reply in order to send it to the client.
*/
std::string Director::errNickcollision(const std::string& nickname) const
{
	ReplyBuilder builder;

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(ERR_NICKCOLLISION)
				.addTrailing(":")
				.addParams(nickname)
				.addTrailing("Nickname collision KILL")
				.addCrln()
				.buildReply();
	PRINT(reply, YELLOW, "\n")
	return (reply);
}

/** @brief Error reply after receiving a NICK message which contains characters which* do not fall in the defined set. 
* @param nickname string Nickname is already in use
* @return the reply in order to send it to the client.
*/
std::string Director::errErroneusnickname(const std::string& nickname) const
{
	ReplyBuilder builder;

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(ERR_ERRONEUSNICKNAME)
				.addTrailing(":")
				.addParams(nickname)
				.addTrailing("Erroneus nickname")
				.addCrln()
				.buildReply();
	PRINT(reply, YELLOW, "\n")
	return (reply);
}

/**
* @brief Error reply when a NICK message is processed that results 
* in an attempt to change to a currently existing nickname.
* @param nickname string Nickname is already in use
* @return the reply in order to send it to the client.
*/
std::string Director::errNicknameinuse(const std::string& nickname) const
{
	ReplyBuilder builder;

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(ERR_NICKNAMEINUSE)
				.addTrailing(":")
				.addParams(nickname)
				.addTrailing("Nickname is already in use")
				.addCrln()
				.buildReply();
	PRINT(reply, YELLOW, "\n")
	return (reply);
}

/**
* @brief error reply returned by the server to any link which tries to
* change part of the registered details (such as
* password or user details from second USER message).
* @return the reply in order to send it to the client.
*/
std::string		Director::errAlreadyRegistred(void) const
{
	ReplyBuilder builder;

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(ERR_ALREADYREGISTRED)
				.addTrailing("You may not reregister")
				.addCrln()
				.buildReply();
	PRINT(reply, YELLOW, "\n")
	return (reply);
}

ReplyBuilder::ReplyBuilder()
{
}

ReplyBuilder::ReplyBuilder(std::string params)
{
	this->addParams(params);
}

std::string reply::getPrefix()
{
	return (this->_prefix);
}

std::string reply::getNumeric()
{
	return (this->_numeric);
}

std::string reply::getParams()
{
	return (this->_params);
}

std::string reply::getTrailing()
{
	return (this->_trailing);
}

std::string reply::getCrln()
{
	return (this->_crln);
}

void reply::setPrefix(std::string prefix)
{
	this->_prefix += prefix;
}

void reply::setNumeric(std::string numeric)
{
	this->_numeric = numeric;
}

void reply::setParams(std::string params)
{
	if (!this->_params.empty())
		this->_params += ' ';
	this->_params += params;
}

void reply::setTrailing(std::string trailing)
{
	this->_trailing = trailing;
}

void reply::setCrln(std::string crln)
{
	this->_crln = crln;
}
