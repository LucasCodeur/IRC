#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <ostream>
#include <string>
#include <vector>
#include <bitset>

class Channel
{
	private:
	std::string			_name;
	std::string			_topic;
	std::string			_password;
	std::vector<int>	_users;
	std::vector<int>	_operators;
	std::vector<int>	_invited; // FIXME: what happens if I want to invite someone not connected to the server yet?
	std::bitset<4>		_mode; // itkl = 0100

	public:
	enum inviteMode {i = 0, t, k, l};
	// CONSTRUCTOR
	Channel();
	Channel(std::string const &name, std::string const &password);
	Channel(std::string const &name);
	~Channel();
	Channel(Channel const &original);

	// OPERATOR
	Channel &operator=(Channel const &other);

	// GETTERS
	std::string const &getName() const;
	std::string const &getTopic() const;
	std::string const &getPassword() const;
	std::vector<int> const &getUsers() const;
	std::vector<int> const &getOperators() const;
	std::vector<int> const &getInvited() const;
	int						getMode(int i) const;
	int						isInviteOnly() const;
	int						hasPassword() const;
	int						isTopicRestricted() const;
	int						hasUserLimit() const;
	int						isChanOp(std::string userName) const;

	// SETTERS
	void setTopic(std::string const &topic);
	void setPassword(std::string const &password);
	void setMode(int mode);
	void setModeItem(unsigned int item, bool value);

	// METHODS
	bool addUser(int clientFd);
	bool removeUser(int clientFd);
	bool setOperator(int clientFd);
	bool removeOperator(int clientFd);
	bool isOp(int clientFd) const;
	bool isOnChan(int clientFd);
	void sendMessageToAll(const std::string &message) const;
	void sendMessageToAllOther(const std::string &message, int senderFd) const;

};
std::ostream &operator<<(std::ostream &o, const Channel &obj);

#endif // !CHANNEL_HPP
