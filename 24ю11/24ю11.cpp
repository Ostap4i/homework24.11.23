#include <iostream>
#include <string>

class Handler {
public:
    virtual void setNext(Handler* next) = 0;
    virtual void handleRequest(const std::string& request) = 0;
};

class BaseHandler : public Handler {
protected:
    Handler* nextHandler;

public:
    BaseHandler() : nextHandler(nullptr) {}

    void setNext(Handler* next) override {
        nextHandler = next;
    }

    void handleRequest(const std::string& request) override {
        if (nextHandler) {
            nextHandler->handleRequest(request);
        }
        else {
            std::cout << "End of chain. Unable to process the request.\n";
        }
    }
};

class ValidationHandler : public BaseHandler {
public:
    void handleRequest(const std::string& request) override {
        if (request.length() >= 6) {
            std::cout << "Validation passed.\n";
            BaseHandler::handleRequest(request);
        }
        else {
            std::cout << "Validation failed. Password should be at least 6 characters long.\n";
        }
    }
};

class DatabaseHandler : public BaseHandler {
public:
    void handleRequest(const std::string& request) override {
        std::cout << "Saving user to the database: " << request << "\n";
        BaseHandler::handleRequest(request);
    }
};

class LoggingHandler : public BaseHandler {
public:
    void handleRequest(const std::string& request) override {
        std::cout << "Logging registration: " << request << "\n";
        BaseHandler::handleRequest(request);
    }
};

int main() {
    Handler* chain = new ValidationHandler();
    chain->setNext(new DatabaseHandler());
    chain->setNext(new LoggingHandler());

    // Example registration requests
    chain->handleRequest("user123"); // Validation failed
    chain->handleRequest("user123456"); // Validation passed, saved to database, logged

    delete chain;

    return 0;
}