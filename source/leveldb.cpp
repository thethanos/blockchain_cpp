#include "leveldb.hpp"

LevelDB::LevelDB(const string& path)
{
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options, path, &db_ptr);
    if (!status.ok())
        throw std::runtime_error("Failed to open leveldb");
}

void LevelDB::put(const string& key, const string& value)
{
    leveldb::Status status = db_ptr->Put(leveldb::WriteOptions(), key, value);
    if (!status.ok())
        throw std::runtime_error("Failed to put value");
}

string LevelDB::get(const string& key)
{
    string buffer;
    leveldb::Status status = db_ptr->Get(leveldb::ReadOptions(), key, &buffer);

    return buffer;
}