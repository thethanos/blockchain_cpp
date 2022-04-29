#pragma once

#include <string>

#include "leveldb/db.h"

using std::string;

class LevelDB
{
public:
    LevelDB(const string& path);
    ~LevelDB() { delete db_ptr; }

public:
    void   put(const string& key, const string& value);
    string get(const string& key);

private:
    leveldb::DB* db_ptr = nullptr;
};