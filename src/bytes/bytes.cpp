//
// Created by nikita on 2/6/21.
//

#include "bytes.hpp"

void		bytes::add(const char* string, size_t i) {
	char*		temp_buff = buffer;

	if (size_ + i >= capacity) {
		capacity = (size_ + i) * 2;
		buffer = new char[capacity];
		memcpy(buffer, temp_buff, size_);
		delete []temp_buff;
	}
	memcpy(buffer + size_, string, i);
	size_ += i;
	buffer[size_] = 0;
}

void		bytes::clear() {
	delete []buffer;
	size_ = 0;
	capacity = 10;
	buffer = new char[capacity];
}

void		bytes::erase(size_t n) {
	char*	temp_buff = buffer;

	if (n >= size_)
		clear();
	else {
		capacity = capacity - n + 10;
		size_ -= n;
		buffer = new char[capacity];
		memcpy(buffer, temp_buff + n, size_);
		buffer[size_] = 0;
		delete[] temp_buff;
	}
}

bytes&		bytes::operator=(const bytes& string) {
	delete []buffer;
	capacity = string.capacity;
	size_ = string.size();
	buffer = new char[capacity];
	memcpy(buffer, string.c_str(), size_);
	buffer[size_] = 0;
	return (*this);
}

size_t		bytes::find(const char* needle) const {
	for (size_t i = 0; i < size_; ++i) {
		if (buffer[i] == *needle) {
			int 	j;
			for (j = 0; needle[j]; ++j)
				if (buffer[i + j] != needle[j]) break;
			if (!needle[j])	return (i);
		}
	}
	return (-1);
}

size_t		bytes::rfind(const char* needle) const {
	int i = size_;

	i--;
	for (; i >= 0; --i) {
		if (buffer[i] == *needle) {
			int 	j;
			for (j = 0; needle[j]; ++j)
				if (buffer[i + j] != needle[j]) break;
			if (!needle[j])	return (i);
		}
	}
	return (-1);
}

void		bytes::rtrim(size_t n) {
	size_ -= n;
	buffer[size_] = 0;
}
