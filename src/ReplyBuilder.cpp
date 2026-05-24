#include "ReplyBuilder.hpp"

ReplyBuilder::ReplyBuilder()
{
	std::cout << "Default constructor Reply_builder called" << std::endl;
}

ReplyBuilder::ReplyBuilder(std::string params)
{
	std::cout << "Params constructor Reply_builder called" << std::endl;
	this->addParams(params);
}

/**
 * @brief allows to setting the params.
 * @param string params to add inside the reply.
 * @return an object of the replyBuilder in order to do the chaining of differents adds.
 */
ReplyBuilder&	ReplyBuilder::addParams(std::string params)
{	
	_reply.params = params;
	return (*this);
}

/**
 * @brief allows to setting the prefixe.
 * @return an object of the replyBuilder in order to do the chaining of differents adds.
 */
ReplyBuilder&	ReplyBuilder::addPrefixe()
{
	_reply.prefixe = PREFIX_MARKER;
	_reply.prefixe += SERVERNAME;
	return (*this);
}

/**
 * @brief allows to setting the numeric.
 * @param string numeric to add inside the reply.
 * @return an object of the replyBuilder in order to do the chaining of differents adds.
 */
ReplyBuilder&	ReplyBuilder::addNumeric(std::string numeric)
{
	_reply.numeric = numeric;
	return (*this);
}

/**
 * @brief allows to setting the message.
 * @param string message to add inside the reply.
 * @return an object of the replyBuilder in order to do the chaining of differents adds.
 */
ReplyBuilder&	ReplyBuilder::addMessage(std::string message)
{
	_reply.message = PREFIX_MARKER;
	_reply.message += message;
	return (*this);
}

/**
 * @brief allows to setting the crln.
 * @return an object of the replyBuilder in order to do the chaining of differents adds.
 */
ReplyBuilder&	ReplyBuilder::addCrln()
{
	_reply.crln = "\r\n";
	return (*this);
}

/**
 * @brief allows concatenating all the strings.
 * @return a string numeric reply to be sent.
 */
std::string	ReplyBuilder::getReply()
{
	std::string reply;

	reply += this->_reply.prefixe;
	reply += SPACE;
	reply += this->_reply.numeric;
	reply += SPACE;
	reply += this->_reply.params;
	reply += SPACE;
	reply += this->_reply.message;
	reply += this->_reply.crln;

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
std::string	Director::rplWelcome(std::string client)
{
	ReplyBuilder	builder;
	
	std::string reply = builder
						.addPrefixe()
						.addNumeric("001")
						.addParams(client)
						.addMessage("Welcome to the IRC Network")
						.addCrln()
						.getReply();
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
std::string		Director::rplYourhost(std::string client)
{
	ReplyBuilder	builder;

	std::string message = "Your host is ";
	message += SERVERNAME;
	message += ", running version 1.0";

	std::string reply = builder
						.addPrefixe()
						.addNumeric("002")
						.addParams(client)
						.addMessage(message)
						.addCrln()
						.getReply();
	if (reply.size() > 512) 
        throw std::runtime_error("Reply longer than 512 characters");

	return (reply);
}

