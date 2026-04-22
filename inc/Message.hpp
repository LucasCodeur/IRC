#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "Client.hpp"
#include <ostream>
#include <vector>

class Message
{
	enum commandType
	{
		PRIVMSG,
		JOIN,
		KICK,
		INVITE,
		TOPIC,
		MODE,
		WHO,
		PASS,
		NICK,
		USER,
		PART
	};
  private:
	Client 						&client;
	commandType					commandeType;
	std::vector<std::string>	params;

  public:
	// CONSTRUCTOR
	Message();
	~Message();
	Message(Message const &original);

	// OPERATOR
	Message &operator=(Message const &other);
};
std::ostream &operator<<(std::ostream &o, const Message &obj);

#endif // !MESSAGE_HPP