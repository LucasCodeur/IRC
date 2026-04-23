#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <ostream>
#include <string>
#include <vector>

class Channel
{
	private:
	std::string				_name;
	std::string				_topic;
	std::string				_password;
	std::vector<int>		_users;
	std::vector<int>		_operators;
	std::vector<int>		_invited;
	int						_mode;

 	public:
  	// CONSTRUCTOR
  	Channel();
	~Channel();
	Channel(Channel const &original);

	// OPERATOR
	Channel &operator=(Channel const &other);
};
std::ostream &operator<<(std::ostream &o, const Channel &obj);
#endif // !CHANNEL_HPP