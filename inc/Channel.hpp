#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <ostream>
#include <string>
#include <vector>
#include "Client.hpp"

class Channel
{
	private:
	std::string 			_name;
	std::string 			_topic;
	std::string 			_password;
	std::vector<Client> 	_user;
	std::vector<Client> 	_operator;
	std::vector<Client> 	_invited;
	bool 					_invOnly;
	bool					_topicRestric;
	int						_maxUser;


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