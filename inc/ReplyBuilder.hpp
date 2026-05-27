/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReplyBuilder.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 18:26:21 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/27 10:02:35 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_BUILDER_HPP
# define REPLY_BUILDER_HPP

#include <iostream>
#include "Client.hpp"

# define SERVERNAME "irc_server"
# define PREFIX_MARKER ":"
# define SPACE " "

class reply 
{
	public:
		std::string getPrefixe();
		std::string getNumeric();
		std::string getParams();
		std::string getTrailing();
		std::string getCrln();

		void setPrefixe(std::string prefixe);
		void setNumeric(std::string numeric);
		void setParams(std::string params);
		void setTrailing(std::string trailing);
		void setCrln(std::string crln);
	private:
		std::string _prefixe;
		std::string _numeric;
		std::string _params;
		std::string _trailing;
		std::string _crln;
};

class IReplyBuilder
{
	public: 
		virtual	IReplyBuilder&	reset() = 0;
		virtual IReplyBuilder&	addPrefixe() = 0;
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
		ReplyBuilder&	addPrefixe();
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
		std::string		rplWelcome(Client client);
		std::string		rplYourhost(Client client);
};

#endif
