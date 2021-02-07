//
// Created by nikita on 2/6/21.
//

#ifndef DATAARMOR_TEST_BYTES_HPP
#define DATAARMOR_TEST_BYTES_HPP


#include <cstddef>
#include <cstring>
#include <string>

class bytes {
private:
	size_t			size_;
	size_t			capacity;
	char*			buffer;

	inline void		byte_init(const char* data) {
		capacity = (capacity) ? capacity : 10;
		buffer = new char[capacity];
		memcpy(buffer, data, size_);
		buffer[size_] = 0;
	}

public:
	bytes() : size_(0), capacity(10) { buffer = new char[capacity]; }
	~bytes() { delete []buffer; }

	bytes(const char* data, size_t size) : size_(size), capacity(size * 2) { byte_init(data); }
	bytes(const bytes& string) : size_(string.size_), capacity(string.capacity) { byte_init(string.c_str()); }
	bytes(const std::string& string) : size_(string.size()), capacity(string.size() * 2) { byte_init(string.c_str()); }

	bytes&			operator=(const bytes& string);

	inline void				add(const std::string& string) { add(string.c_str(), string.size()); }
	void					add(const char* string, size_t i);
	inline void				add(const bytes& string) { add(string.c_str(), string.size()); }
	void					erase(size_t n);
	void					rtrim(size_t n);

	void					clear();
	size_t					find(const char* needle) const ;
	size_t					rfind(const char* needle) const;

	inline bytes			substr(size_t i) { return (bytes(buffer, (i < size_) ? i : size_)); }
	inline size_t			size() const { return (size_); }
	inline bool				empty() const { return (!size_); }
	inline const char*		c_str() const { return (buffer); }

};


#endif //DATAARMOR_TEST_BYTES_HPP
