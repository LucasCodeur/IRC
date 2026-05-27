#include "ReplyBuilder.hpp"
#include "NumericReplies.h"
#include "Channel.hpp"
#include "debug.hpp"

#include <ctime>
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
std::string	Director::rplWelcome(Client client)
{
	ReplyBuilder	builder;
	
	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(RPL_WELCOME)
				.addParams(client.getNickname())
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
std::string	Director::rplYourhost(Client client)
{
	ReplyBuilder	builder;

	std::string trailing = "Your host is ";
	trailing += SERVERNAME;
	trailing += SERVER_VERSION;

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

	return (reply);
}

/**
* @brief creates the numeric reply to indicate when the server was created.
* @param client, the name of the person successfully connect.
* @return the reply in order to send it to the client.
*/
std::string	Director::rplCreated(Client client)
{
	ReplyBuilder	builder;
	std::string		trailing = "This server was created ";

	trailing += "01/01/2026";

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(RPL_CREATED)
				.addParams(client.getNickname())
				.addTrailing(trailing)
				.addCrln()
				.buildReply();

	if (reply.size() > 512) 
		throw std::runtime_error("Reply longer than 512 characters");

	return (reply);
}

std::string	Director::rplMyInfo(Client client)
{
	ReplyBuilder	builder;
	std::string		trailing = "This server was created ";
	std::string		params = client.getNickname();;

	params += SERVERNAME;
	params += SERVER_VERSION;

	std::string reply = builder
				.reset()
				.addPrefix(SERVERNAME)
				.addNumeric(RPL_MYINFO)
				.addParams(params)
				.addTrailing(trailing)
				.addCrln()
				.buildReply();

	if (reply.size() > 512) 
		throw std::runtime_error("Reply longer than 512 characters");

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
