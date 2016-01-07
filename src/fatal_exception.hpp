// The MIT License (MIT)
// 
// Copyright (c) 2015-2016 Sven Osterwalder
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// Header file defining fatal exception class.

#pragma once

#include <exception>

// Provides an interface for throwing fatal exceptions, based on
// std::exception.
//
// Example:
//      throw new FatalException(message);
class FatalException : public std::exception {
public:

    // Class constructor.
    // 
    // Accepts a string as parameter which represents the exceptions
    // message.
    FatalException(std::string str) : exception(), str(str) {}

    // Returns the cause of the exception as C-string object.
    const char *what() const throw()
    {
        return str.c_str();
    }

private:
    // The message of the exception.
    std::string str;
};
