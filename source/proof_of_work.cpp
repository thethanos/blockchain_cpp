#include "proof_of_work.hpp"

#include "util.hpp"

std::pair<string, int> prove(string prev_hash, string data, uint difficulty)
{  
    uint256_t uint_hash(0);
    uint256_t target(1);

    target = target << (256 - difficulty);

    string hash;
    uint   nonce(0); 
    while(nonce < INT32_MAX)
    {
        auto new_data = string{prev_hash + data + to_hex_string(nonce) + to_hex_string(difficulty)};

        hash = sha::sum256(new_data);
        std::cout << hash << '\r';

        uint_hash.assign("0x" + hash);
        if (uint_hash < target)
            break;
        
        nonce++;
    }

    std::cout << '\n';
    return std::make_pair(hash, nonce);
}

bool validate(const Block* block, int difficulty)
{
    auto data = string{block->get_prev_hash() + block->get_txvector().get_txhash() + to_hex_string(block->get_nonce()) + to_hex_string(difficulty)};

    uint256_t uint_hash(0);
    uint_hash.assign("0x" + sha::sum256(data));

    uint256_t target(1);
    target = target << (256 - difficulty);

    return uint_hash < target;
}