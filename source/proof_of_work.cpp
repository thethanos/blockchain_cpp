#include "proof_of_work.hpp"

#include "sha256.hpp"
#include "util.hpp"

std::pair<std::string, int> prove(std::string prev_hash, std::string data, uint difficulty)
{  
    uint256_t uint_hash(0);
    uint256_t target(1);

    target = target << (256 - difficulty);

    std::string hash;
    uint        nonce(0); 
    while(nonce < INT32_MAX)
    {
        auto new_data = std::string{prev_hash + data + to_hex_string(nonce) + to_hex_string(difficulty)};

        hash = sha256sum(new_data);
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
    auto test_data = std::string{block->get_prev_hash() + block->get_data() + to_hex_string(block->get_nonce()) + to_hex_string(difficulty)};

    uint256_t uint_hash(0);
    uint_hash.assign("0x" + sha256sum(test_data));

    uint256_t target(1);
    target = target << (256 - difficulty);

    return uint_hash < target;
}