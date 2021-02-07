//
// Created by nikita on 2/7/21.
//

#ifndef DATAARMOR_TEST_EXCEPTION_HPP
#define DATAARMOR_TEST_EXCEPTION_HPP

# include <exception>
# include <string>

class Exception : public std::exception {
	const std::string	error_message;

public:
	explicit Exception(const std::string& error_message) : error_message(error_message) { }
	~Exception() noexcept override = default;

	const char*		what() const noexcept override { return (error_message.c_str()); }
};


#endif //DATAARMOR_TEST_EXCEPTION_HPP
