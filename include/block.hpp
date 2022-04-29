#pragma once

#include "transaction.hpp"

class Block
{
public:
    Block(){}
    Block(const string& prev_hash, const TxVector& transactions, const string& hash, int nonce);

public:
    const string&  get_prev_hash()  const { return prev_hash; }
    const string&  get_hash()       const { return hash; }
    const int      get_nonce()      const { return nonce; }

    TxVector get_txvector() const { return transactions; }

public:
    string serialize();
    void   deserialize(const std::string& data);

private:
    string  prev_hash;
    string  hash;

    TxVector transactions;
    int nonce;

private:
    friend class boost::serialization::access;

    template <class Serializer>
    void serialize(Serializer& serializer, const unsigned int version)
    {
        serializer & prev_hash;
        serializer & hash;
        serializer & transactions;
        serializer & nonce;
    }
};