#include "ModeCommand.hpp"
#include "NumericReplies.h"
#include <sstream>

ModeCommand::ModeCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params)
{
	std::string possibleModes = "itkol";
	std::string possibleOperations = "+-";

	std::ostringstream response;

	std::string	targetChannel = params.front().front();
	std::string modeEdition = params.front()[1];
	if (params.size() < ModeCommand::min_params)
		throw Command::IncorrectParametersException("Not enough parameters");
	else if (params.size() > ModeCommand::max_params)
		throw Command::IncorrectParametersException("Too much parameters");
	else if (type != MODE)
		throw UnknownCommandException(); //FIXME: use appropriate exception for this
	else if (modeEdition.length() < 2 || modeEdition.length() > 2 || possibleOperations.find(modeEdition[0]) == possibleOperations.npos)
		throw Command::IncorrectParametersException(); //TODO: same
	else if (possibleModes.find(modeEdition[0]) == possibleModes.npos)
	{
			response << ERR_UNKNOWNMODE << " " << modeEdition[0] << ":is unknown mode char to me" << LF CR;
			throw Command::IncorrectParametersException();
	}

}

ModeCommand::~ModeCommand() {}

void ModeCommand::execute(Server &server) const
{

}
