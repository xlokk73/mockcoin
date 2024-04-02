//
// Created by manwel on 01/04/2024.
//

#include "Transaction.h"

bool Transaction::is_signature_valid() const {
    std::string hash = Block::sha256(std::to_string(previous_transaction_index) + recipient_public_key);
    return hash == signature;
}

