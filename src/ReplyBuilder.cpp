#include "ReplyBuilder.hpp"
#include "Channel.hpp"
#include "NumericReplies.h"
#include "debug.hpp"

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
	this->_reply.setCrln("\r\n");
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
	reply += SPACE;
	PRINT("Build reply", RED, "\n");
	PRINT(this->_reply.getNumeric(), BLUE, "\n");
	reply += this->_reply.getNumeric();
	reply += SPACE;
	reply += this->_reply.getParams();
	reply += SPACE;
	reply += this->_reply.getTrailing();
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
std::string	Director::rplWelcome(Client client) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric("001")
				.addParams(client.getNickname())
				.addTrailing("Welcome to the IRC Network")
				.addCrln()
				.buildReply();
	//WARN: Maybe change the content of the runtime or even the runtime
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
std::string	Director::rplChannelModeIs(Client client, std::string channelName, std::string modes, std::string password) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(RPL_CHANNELMODEIS)
				.addParams(channelName)
				.addParams(modes)
				.addParams(password)
				.addCrln()
				.buildReply();
	//WARN: Maybe change the content of the runtime or even the runtime
	if (reply.size() > 512) 
		throw std::runtime_error("Reply longer than 512 characters");
	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string Director::errNeedMoreParams(Client client, std::string command) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(ERR_NEEDMOREPARAMS)
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
std::string	Director::errNoSuchChannel(Client client, std::string channelName) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(ERR_NOSUCHCHANNEL)
				.addParams(channelName)
				.addTrailing("No such channel")
				.addCrln()
				.buildReply();
	//WARN: Maybe change the content of the runtime or even the runtime
	if (reply.size() > 512) 
		throw std::runtime_error("Reply longer than 512 characters");
	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string		Director::errNoSuchNick(Client client, std::string channelName) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(client.getNickname())
				.addNumeric(ERR_NOSUCHNICK)
				.addParams(channelName)
				.addTrailing("There was no such nickname")
				.addCrln()
				.buildReply();
	//WARN: Maybe change the content of the runtime or even the runtime
	if (reply.size() > 512) 
		throw std::runtime_error("Reply longer than 512 characters");
	PRINT(reply, YELLOW, "\n");
	return (reply);
}
std::string		Director::errUnknownMode(Client client, std::string modeChar) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(client.getNickname())
				.addNumeric(ERR_UNKNOWNMODE)
				.addParams(modeChar)
				.addTrailing("is unknown mode char to me")
				.addCrln()
				.buildReply();
	//WARN: Maybe change the content of the runtime or even the runtime
	if (reply.size() > 512) 
		throw std::runtime_error("Reply longer than 512 characters");
	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string		Director::rplTopic(Client client, std::string channelName, std::string topic) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(client.getNickname())
				.addNumeric(RPL_NOTOPIC)
				.addParams(channelName)
				.addTrailing(":")
				.addParams(topic)
				.addCrln()
				.buildReply();
	//WARN: Maybe change the content of the runtime or even the runtime
	if (reply.size() > 512) 
		throw std::runtime_error("Reply longer than 512 characters");
	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string		Director::rplNoTopic(Client client, std::string channelName) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(client.getNickname())
				.addNumeric(RPL_NOTOPIC)
				.addParams(channelName)
				.addTrailing("No topic is set")
				.addCrln()
				.buildReply();
	//WARN: Maybe change the content of the runtime or even the runtime
	if (reply.size() > 512) 
		throw std::runtime_error("Reply longer than 512 characters");
	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string		Director::errNotOnChannel(Client client, std::string channelName) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(client.getNickname())
				.addNumeric(ERR_NOTONCHANNEL)
				.addParams(channelName)
				.addTrailing("You're not on that channel")
				.addCrln()
				.buildReply();
	//WARN: Maybe change the content of the runtime or even the runtime
	if (reply.size() > 512) 
		throw std::runtime_error("Reply longer than 512 characters");
	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string		Director::errChanOPrivsNeeded(Client client, std::string channelName) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(client.getNickname())
				.addNumeric(ERR_NOTONCHANNEL)
				.addParams(channelName)
				.addTrailing("You're not channel operator")
				.addCrln()
				.buildReply();
	//WARN: Maybe change the content of the runtime or even the runtime
	if (reply.size() > 512) 
		throw std::runtime_error("Reply longer than 512 characters");
	PRINT(reply, YELLOW, "\n");
	return (reply);
}


std::string		Director::errBadChannelKey(Client client, std::string channelName) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(client.getNickname())
				.addNumeric(ERR_BADCHANNELKEY)
				.addParams(channelName)
				.addTrailing("Cannot join channel (+k)")
				.addCrln()
				.buildReply();
	//WARN: Maybe change the content of the runtime or even the runtime
	if (reply.size() > 512) 
		throw std::runtime_error("Reply longer than 512 characters");
	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string		Director::errUnknownCommand(Client client, std::string cmdKeyword) const
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(client.getNickname())
				.addNumeric(ERR_UNKNOWNCOMMAND)
				.addParams(cmdKeyword)
				.addTrailing("Unknown command")
				.addCrln()
				.buildReply();
	//WARN: Maybe change the content of the runtime or even the runtime
	if (reply.size() > 512) 
		throw std::runtime_error("Reply longer than 512 characters");
	PRINT(reply, YELLOW, "\n");
	return (reply);
}
/**
* @brief creates the numeric reply to indicate the servername and the version, when a client successfully connects to a irc server.
* @param client, the name of the person successfully connect.
* @return the reply in order to send it to the client.
*/
std::string	Director::rplYourhost(Client client) const
{
	ReplyBuilder	builder;

	std::string trailing = "Your host is ";
	trailing += SERVERNAME;
	trailing += ", running version 1.0";

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(RPL_YOURHOST)
				.addParams(client.getNickname())
				.addTrailing(trailing)
				.addCrln()
				.buildReply();
	if (reply.size() > 512) 
		throw std::runtime_error("Reply longer than 512 characters");

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

std::string Director::rplJoin(Client const &client, Channel const &channel)
{
	ReplyBuilder builder;

	std::string reply = builder
				.reset()
				.addPrefix(client.getNickname())
				.addParams("JOIN")
				.addParams(channel.getName())
				.addCrln()
				.buildReply();

	PRINT(reply, YELLOW, "\n");
	return (reply);
}

ReplyBuilder::ReplyBuilder()
{
	std::cout << "Default constructor Reply_builder called" << std::endl;
}

ReplyBuilder::ReplyBuilder(std::string params)
{
	std::cout << "Params constructor Reply_builder called" << std::endl;
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
	PRINT("Set numeric: ", BLUE, "\n");
	PRINT(numeric, WHITE, "\n");
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
