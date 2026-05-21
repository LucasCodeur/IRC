#ifndef REPLY_BUILDER_HPP
# define REPLY_BUILDER_HPP

#include <iostream>

class ReplyBuilder
{
	public:
		ReplyBuilder(std::string numeric, std::string nickname);
		virtual void buildPrefixe() const = 0;
		virtual void buildNumeric() const = 0;
		virtual void buildNickname() const = 0;
};

#endif
