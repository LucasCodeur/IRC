#ifndef REPLY_BUILDER_HPP
# define REPLY_BUILDER_HPP

#include <iostream>
# define SERVERNAME "irc_server"
# define PREFIX_MARKER ":"
# define SPACE " "

class reply 
{
	public:
		std::string prefixe;
		std::string numeric;
		std::string params;
		std::string message;
		std::string crln;
};

class ReplyBuilder
{
	public: 
		ReplyBuilder();
		ReplyBuilder(std::string params);
		ReplyBuilder&	addPrefixe();
		ReplyBuilder&	addNumeric(std::string numeric);
		ReplyBuilder&	addMessage(std::string message);
		ReplyBuilder&	addParams(std::string params);
		ReplyBuilder&	addCrln();
		std::string		getReply();
	private:
		reply _reply;
};

class Director
{
	public:
		ReplyBuilder*	builder;
		void			setBuilderType(ReplyBuilder* builder);
		std::string		rplWelcome(std::string client);
		std::string		rplYourhost(std::string client);
};

#endif
