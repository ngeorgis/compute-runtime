/*
 * Copyright (c) 2017 - 2018, Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "test_files.h"

#if defined(__LP64__) || (_WIN64)
std::string testFiles("test_files/x64/");
#else
std::string testFiles("test_files/x86/");
#endif
std::string clFiles("test_files/");
std::string binaryNameSuffix("");

void retrieveBinaryKernelFilename(std::string &outputFilename, const std::string &kernelName, const std::string &extension, const std::string &options) {
    if (outputFilename.length() > 0) {
        outputFilename.clear();
    }
    outputFilename.reserve(2 * testFiles.length());
    outputFilename.append(testFiles);
    outputFilename.append(kernelName);
    outputFilename.append(binaryNameSuffix);
    outputFilename.append(extension);
    outputFilename.append(options);
}