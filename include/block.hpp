#pragma once

#include <string>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class Block
{
public:
    Block(){}
    Block(const std::string& prev_hash, const std::string& data, const std::string& hash, int nonce);

public:
    const std::string& get_prev_hash() const { return prev_hash; }
    const std::string& get_hash()      const { return hash; }
    const std::string& get_data()      const { return data; }
    const int          get_nonce()     const { return nonce; }

public:
    std::string serialize();
    void        deserialize(const std::string& data);

private:
    std::string  prev_hash;
    std::string  hash;
    std::string  data;
    int          nonce;

private:
    friend class boost::serialization::access;

    template <class Serializer>
    void serialize(Serializer& serializer, const unsigned int version)
    {
        serializer & prev_hash;
        serializer & hash;
        serializer & data;
        serializer & nonce;
    }
};