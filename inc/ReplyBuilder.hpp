/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReplyBuilder.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enchevri <enchevri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 18:26:21 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/29 14:20:06 by kbarru           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_BUILDER_HPP
# define REPLY_BUILDER_HPP

#include "Client.hpp"
#include "Channel.hpp"

# define SERVERNAME "irc_server"
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

		std::string		rplJoin(Client const &client, Channel const &channel);
		std::string		rplWelcome(Client client) const;
		std::string		rplChannelModeIs(std::string channelName, std::string modes, std::string password) const;
		std::string		rplYourhost(Client client) const;
		std::string		errNeedMoreParams(std::string command) const;
		std::string		errNoSuchChannel(std::string channelName) const;
		std::string		errUnknownMode(std::string modeChar) const;
		std::string		rplTopic(std::string channelName, std::string topic) const;
		std::string		rplNoTopic(std::string channelName) const;
		std::string		errNotOnChannel(std::string channelName) const;
		std::string		errChanOPrivsNeeded(std::string channelName) const;
		std::string		errBadChannelKey(std::string channelName) const;
		std::string		errNoSuchNick(std::string channelName) const;
		std::string		errUnknownCommand(std::string cmdKeyword) const;
};
#endif
