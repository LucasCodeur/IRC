#include "NickCommand.hpp"
#include "Client.hpp"
#include <cctype>

NickCommand::NickCommand(Server *server, const int clientFd, t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	size_t sizeParams = params.size();
	std::string reply;
	if (sizeParams < NickCommand::min_params)
	{
		reply = this->_director.errNonicknamegiven();
		this->getClient()->addToBuffer(reply);
		throw Command::NotEnoughParametersException("Not enough parameters");
	}
	else if (sizeParams > NickCommand::max_params)
		throw Command::NotEnoughParametersException("Too much parameters");
}

NickCommand::~NickCommand() {}

static bool	check_nickname(std::string& nickname);
static bool	checkCollisionNickname(std::map<int, Client*>& map,  std::string nickname);

void	NickCommand::execute() const
{
	std::map<int, Client*>					map = _server->getClientmap();
	std::map<int, Client*>::const_iterator	it = map.find(this->getClientFd());
	Client*									client = it->second;
	std::string								nickname = this->_params[0][0];
	std::string								reply;
	Authstate&								authstate = client->getAuthstate();

	if (checkCollisionNickname(map, nickname) == true)
	{
		if (authstate.getFullyRegistered() == true)
			reply = this->_director.errNicknameinuse(nickname);
		else
			reply = this->_director.errNickcollision(nickname);
		client->addToBuffer(reply);
	}
	else if (check_nickname(nickname) == false)
	{
		reply = this->_director.errErroneusnickname(nickname);
		this->_client->addToBuffer(reply);
		return ;
	}
	client->setNickname(nickname);
	authstate.setNickReceived(true);
}

static bool	checkCollisionNickname(std::map<int, Client*>& map,  std::string nickname)
{
	for (std::map<int, Client*>::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		if (nickname == it->second->getNickname())
			return (true);
	}
	return (false);
}

static bool	special_charset(char c);

static bool	check_nickname(std::string& nickname)
{
	if (nickname.size() > 9)
		return (false);
	if (std::isalpha(nickname[0]) == false)
		return (false);
	for (int i = 1; nickname[i]; i++)
	{
		if (std::isalpha(nickname[i]) == false && std::isdigit(nickname[i]) == false && special_charset(nickname[i]) == false)
			return (false);
	}
	return (true);
}

static bool	special_charset(char c)
{
	if (c == '-' || c == '[' || c ==']' || c == '\\' || c =='`' || c== '^' || c == '{' || c == '}')
		return (true);
	return (false);
}
