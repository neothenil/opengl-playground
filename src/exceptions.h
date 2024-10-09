#include <stdexcept>

class OpenGLException: public std::runtime_error
{
public:
    explicit OpenGLException(const std::string& msg): std::runtime_error(msg) {}
    explicit OpenGLException(const char* msg): std::runtime_error(msg) {}
};
