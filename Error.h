#ifndef ERROR
#define ERROR

#include <iostream>
#include <string>

class my_error : public std::exception {
private:
        std::string _message;

public:
        my_error(const std::string& message, const std::string& filename) {
                _message = message;
                _message += " Error in file: ";
                _message += filename;
        }

        const char* what() const noexcept override {
                return _message.c_str();
        }
};

#endif
