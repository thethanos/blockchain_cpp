#pragma once

#include <string>

#include "leveldb/db.h"

class LevelDB
{
public:
    LevelDB(const std::string& path);
    ~LevelDB() { delete db_ptr; }

public:
    void        put(const std::string& key, const std::string& value);
    std::string get(const std::string& key);

private:
    leveldb::DB* db_ptr = nullptr;
};