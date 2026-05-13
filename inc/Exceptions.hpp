#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

#include <exception>
#include <string>

class socketFailed : public std::exception
{
    public:
        const char* what() const throw ();
};

class setSocketOptionFailed : public std::exception
{
    public:
        const char* what() const throw ();
};

class bindFailed: public std::exception
{
    public:
        const char* what() const throw ();
};

class listenSocketFailed: public std::exception
{
    public:
        const char* what() const throw ();
};

class epollCreateFailed: public std::exception
{
    public:
        const char* what() const throw ();
};

class controlEpollFailed: public std::exception
{
    public:
        const char* what() const throw ();
};

class acceptFailed: public std::exception
{
    public:
        const char* what() const throw ();
};


class epollWaitFailed: public std::exception
{
    public:
        const char* what() const throw ();
};

class sendFailed: public std::exception
{
    public:
        const char* what() const throw ();
};


class receiveDataFailed: public std::exception
{
    public:
        const char* what() const throw ();
};

class setnonblockingFailed: public std::exception
{
    public:
        const char* what() const throw ();
};

class passwordNotCorrect: public std::exception
{
    public:
        const char* what() const throw();
};

class badCharactersInsidePort: public std::exception
{
    public:
        const char* what() const throw();
};

class badRangePort: public std::exception
{
    public:
        const char* what() const throw();
};

#endif
