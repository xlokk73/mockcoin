#include <iostream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

class Transaction {
public:
    std::string recipient_public_key;
    uint previous_transaction_index; // Index of the previous transaction in the block
    std::string signature; // Hash of previous transaction + recipient public key
};

class Block {
public:
    uint nonce;

    // List of transactions
    std::vector<Transaction> transactions;

    // Serialize the block
    std::string to_bytes() {
        std::stringstream ss;
        ss << nonce;
        for (Transaction &transaction : transactions) {
            ss << transaction.recipient_public_key;
            ss << transaction.previous_transaction_index;
            ss << transaction.signature;
        }
        return ss.str();
    }

    // Deserialize the block
    void from_bytes(std::string bytes) {
        std::stringstream ss(bytes);
        ss >> nonce;
        while (!ss.eof()) {
            Transaction transaction;
            ss >> transaction.recipient_public_key;
            ss >> transaction.previous_transaction_index;
            ss >> transaction.signature;
            transactions.push_back(transaction);
        }
    }

    void increment_nonce() {
        this->nonce++;
    }
};

// Hashing function
std::string sha256(const std::string &input) {
    CryptoPP::SHA256 hash;
    std::string digest;
    CryptoPP::StringSource s(input, true,
                             new CryptoPP::HashFilter(hash,
                                                      new CryptoPP::HexEncoder(
                                                              new CryptoPP::StringSink(digest))));
    return digest;
}

// Check if the hash is valid
bool is_valid(std::string hash) {
    // Check if hash starts with 5 zeros
    return hash.substr(0, 5) == "00000";
}

int main() {
    Block block = Block();

    std::cout << "Hello, World!" << std::endl;

    // Start the proof-of-work
    std::string hash = sha256(block.to_bytes());
    while (!is_valid(hash)) {
        block.increment_nonce();
        hash = sha256(block.to_bytes());
    }

    std::cout << "Proof of work hash: " << hash << std::endl;
    std::cout << "Nonce: " << block.nonce << std::endl;


    return 0;
}