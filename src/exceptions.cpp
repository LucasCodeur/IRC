#include <exceptions.hpp>

FatalError::FatalError() throw() : std::runtime_error("Fatal error") {}
FatalError::FatalError(std::string msg) throw() : std::runtime_error(msg) {}
FatalError::~FatalError() throw() {}

BadPort::BadPort() throw() : std::runtime_error("Bad Port") {}
BadPort::BadPort(std::string msg) throw() : std::runtime_error(msg) {}
BadPort::~BadPort() throw() {}
