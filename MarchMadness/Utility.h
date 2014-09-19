#ifndef UTILITIES_H
#define UTILITIES_H
#include <exception>

// Error class used when throwing general errors during the simulation
class Error : public std::exception {
public:
	Error(const char* msg_) : msg(msg_) {}
    const char* what() const noexcept override
    {return msg;}
private:
	const char* msg;
};

#endif
