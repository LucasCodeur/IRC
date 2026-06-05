#ifndef REPLY_BUILDER_HPP
# define REPLY_BUILDER_HPP

#include "Client.hpp"
#include "Channel.hpp"

# define SERVERNAME "irc_server"
# define SERVER_VERSION "running version 1.0"
# define CHANNEL_MODES  "+i, +t, +k, +o, +l"
# define PREFIX_MARKER ":"
# define SPACE " "

class reply 
{
	public:
		std::string getPrefix();
		std::string getNumeric();
		std::string getParams();
		std::string getTrailing();
		std::string getCrln();

		void setPrefix(std::string prefix);
		void setNumeric(std::string numeric);
		void setParams(std::string params);
		void setTrailing(std::string trailing);
		void setCrln(std::string crln);
	private:
		std::string _prefix;
		std::string _numeric;
		std::string _params;
		std::string _trailing;
		std::string _crln;
};

class IReplyBuilder
{
	public: 
		virtual	IReplyBuilder&	reset() = 0;
		virtual IReplyBuilder&	addPrefix(std::string const &prefix) = 0;
		virtual IReplyBuilder&	addNumeric(std::string numeric) = 0;
		virtual IReplyBuilder&	addTrailing(std::string trailing) = 0;
		virtual IReplyBuilder&	addParams(std::string params) = 0;
		virtual IReplyBuilder&	addCrln() = 0;
		virtual std::string		buildReply() = 0;
};

class ReplyBuilder : IReplyBuilder
{
	public: 
		ReplyBuilder();
		ReplyBuilder(std::string params);
		ReplyBuilder&	reset();
		ReplyBuilder&	addPrefix(std::string const &string);
		ReplyBuilder&	addNumeric(std::string numeric);
		ReplyBuilder&	addTrailing(std::string trailing);
		ReplyBuilder&	addParams(std::string params);
		ReplyBuilder&	addCrln();
		std::string		buildReply();
	private:
		reply _reply;
};

class Director
{
	public:
		ReplyBuilder*	builder;
		void			setBuilderType(ReplyBuilder* builder);
		std::string		rplCreated(Client client) const;
		std::string		rplMyInfo(Client client) const;
		std::string		rplJoin(Client const &client, Channel const &channel);
		std::string		rplWelcome(Client client) const;
		std::string		rplChannelModeIs(std::string channelName, std::string modes, std::string password) const;
		std::string		rplJoin(Client const &client, Channel const &channel) const;
		std::string		rplPart(Client const &client, Channel const &channel, std::string const &reason) const;
		std::string		rplTopic(Client const &client, Channel const &channel) const;
		std::string		rplNoTopic(Client const &client, Channel const &channel) const;
		std::string		rplNameReply(Client const &client, Channel const &channel, std::string const &namesList) const;
		std::string		rplEndOfNames(Client const &client, Channel const &channel) const;
		std::string		rplWhoReply(Client const &requester, std::string const &channel, Client const &target, bool isOp) const;
		std::string		rplEndOfWho(Client const &requester, std::string const &mask) const;
		std::string		rplError(std::string const &numeric, Client const &client, Channel const &channel);
		std::string		rplYourhost(Client client) const;
		std::string		errNeedMoreParams(std::string clientNick, std::string command) const;
		std::string		errNoSuchChannel(std::string clientNick, std::string channelName) const;
		std::string		errUnknownMode(std::string clientNick, std::string modeChar) const;
		std::string		rplTopic(std::string clientNick, std::string channelName, std::string topic) const;
		std::string		rplNoTopic(std::string clientNick, std::string channelName) const;
		std::string		errNotOnChannel(std::string clientNick, std::string channelName) const;
		std::string		errUserOnChannel(std::string clientNick, std::string invitedNick, std::string channelName) const;
		std::string		errChanOPrivsNeeded(std::string clientNick, std::string channelName) const;
		std::string		errBadChannelKey(std::string clientNick, std::string channelName) const;
		std::string		errNoSuchNick(std::string clientNick, std::string channelName) const;
		std::string		errUnknownCommand(std::string clientNick, std::string cmdKeyword) const;
		std::string		rplNameReply(std::string clientNick, Channel &channel) const;
		std::string		rplEndofNames(std::string clientNick, Channel &channel) const;
		std::string		rplInviting(std::string clientNick, std::string invitedNick, std::string channelName) const;
		std::string		errAlreadyRegistred(void) const;
};
#endif
