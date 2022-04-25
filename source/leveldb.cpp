#include "leveldb.hpp"

LevelDB::LevelDB(const std::string& path)
{
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options, path, &db_ptr);
    if (!status.ok())
        throw std::runtime_error("Failed to open leveldb");
}

void LevelDB::put(const std::string& key, const std::string& value)
{
    leveldb::Status status = db_ptr->Put(leveldb::WriteOptions(), key, value);
    if (!status.ok())
        throw std::runtime_error("Failed to put value");
}

std::string LevelDB::get(const std::string& key)
{
    std::string buffer;
    leveldb::Status status = db_ptr->Get(leveldb::ReadOptions(), key, &buffer);

    return buffer;
}