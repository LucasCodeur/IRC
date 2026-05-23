#ifndef REPLY_BUILDER_HPP
# define REPLY_BUILDER_HPP

#include <iostream>

class reply 
{
	std::string prefixe;
	std::string numeric;
	std::string params;
	std::string message;
	std::string crln;
};

class replyBuilder
{
	public: 
		replyBuilder&			AddPrefixe();
		replyBuilder&			AddNumeric();
		virtual replyBuilder&	AddMessage() = 0;
		virtual replyBuilder&	AddParams() = 0;
		virtual replyBuilder&	AddCrln();
		reply product;
};

#endif
