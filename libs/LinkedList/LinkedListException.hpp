#ifndef _LinkedListException_H
#define _LinkedListException_H

class LinkedListException: public std::exception {
private:
    std::string message_;
public:
    explicit LinkedListException(const std::string& message) : message_(message) {}
    const char* what() const noexcept override {
        return message_.c_str();
    }   
};

#endif