#ifndef OPERATIONSTRUCTURE_H
#define OPERATIONSTRUCTURE_H

#include "boost/filesystem.hpp"

enum Operation {
    COPY = 0,
    CUT,
    INVALID
};

struct FileOperation {
    Operation operation;
    boost::filesystem::path sourcePath;

    FileOperation()
    {
        operation = INVALID;
    }

    ~FileOperation()
    {
        operation = INVALID;
        sourcePath.clear();
    }

    bool isInvalid()
    {
        return operation == INVALID || sourcePath.empty();
    }

    bool isValid()
    {
        return operation != INVALID && !sourcePath.empty();
    }
};

typedef FileOperation *PFileOperation;

#endif // OPERATIONSTRUCTURE_H
