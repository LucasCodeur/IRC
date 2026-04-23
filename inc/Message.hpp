#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <ostream>
#include <vector>

class Message
{
	private:
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
	int							_clientFd;
	commandType					_commandType;
	std::vector<std::string>	_params;

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