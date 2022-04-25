#include "block.hpp"

#include <sstream>

Block::Block(const std::string& prev_hash, const std::string& data, const std::string& hash, int nonce)
{
    this->prev_hash = prev_hash;
    this->data      = data;
    this->hash      = hash;
    this->nonce     = nonce;
}

std::string Block::serialize()
{
    std::stringstream ostream;
    boost::archive::text_oarchive serializer(ostream);

    serializer << *this;

    return ostream.str();
}

void Block::deserialize(const std::string& data)
{
    std::stringstream istream(data);
    boost::archive::text_iarchive deserializer(istream);

    deserializer >> *this;
}