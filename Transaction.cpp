//
// Created by manwel on 01/04/2024.
//

#include "Transaction.h"

bool Transaction::is_signature_valid() const {
    std::string hash = sha256(std::to_string(previous_transaction_index) + recipient_public_key);
    return hash == signature;
}

std::string Transaction::sha256(const std::string &input) {
    CryptoPP::SHA256 hash;
    std::string digest;
    CryptoPP::StringSource s(input, true,
                             new CryptoPP::HashFilter(hash,
                                                      new CryptoPP::HexEncoder(
                                                              new CryptoPP::StringSink(digest))));
    return digest;
}
