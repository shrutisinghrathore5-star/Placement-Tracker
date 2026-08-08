#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

// Base exception for the whole application
class PlacementException : public std::runtime_error {
public:
    explicit PlacementException(const std::string& msg) : std::runtime_error(msg) {}
};

class InvalidLoginException : public PlacementException {
public:
    explicit InvalidLoginException(const std::string& msg = "Invalid username or password.")
        : PlacementException(msg) {}
};

class RecordNotFoundException : public PlacementException {
public:
    explicit RecordNotFoundException(const std::string& msg = "Record not found.")
        : PlacementException(msg) {}
};

class DuplicateRecordException : public PlacementException {
public:
    explicit DuplicateRecordException(const std::string& msg = "Record already exists.")
        : PlacementException(msg) {}
};

class InvalidInputException : public PlacementException {
public:
    explicit InvalidInputException(const std::string& msg = "Invalid input provided.")
        : PlacementException(msg) {}
};

class FileIOException : public PlacementException {
public:
    explicit FileIOException(const std::string& msg = "File could not be opened/read/written.")
        : PlacementException(msg) {}
};

#endif // EXCEPTIONS_HPP
