#ifndef EXCEPTIONS_HPP
 # define EXCEPTIONS_HPP

#include <stdexcept>

class FatalError : public std::runtime_error
{
	public:
		FatalError() throw();
		FatalError(std::string msg) throw();
		~FatalError() throw();
};

class BadPort : public std::runtime_error
{
	public:
		BadPort() throw ();
		BadPort(std::string msg) throw();
		~BadPort() throw();
};

#endif
