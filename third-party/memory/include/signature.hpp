#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "memory.hpp"

class Signature {
public:
    explicit Signature(const std::string& pattern) {
        parsePattern(pattern);
    }

    uintptr_t find(const Memory& mem, const uintptr_t base, const size_t scanSize = 0x01000000) const {
        std::vector<uint8_t> buffer(scanSize);
        mem.readRaw(base, buffer.data(), scanSize);

        for (size_t i = 0; i <= scanSize - pattern.size(); ++i) {
            bool found = true;
            for (size_t j = 0; j < pattern.size(); ++j) {
                if (mask[j] != '?' && buffer[i + j] != pattern[j]) {
                    found = false;
                    break;
                }
            }
            if (found) return base + i;
        }

        return 0;
    }

private:
    std::vector<uint8_t> pattern;
    std::string mask;

    void parsePattern(const std::string& str) {
        std::istringstream iss(str);
        std::string byte;
        while (iss >> byte) {
            if (byte == "?" || byte == "??") {
                pattern.push_back(0x00);
                mask += '?';
            } else {
                pattern.push_back(static_cast<uint8_t>(std::stoul(byte, nullptr, 16)));
                mask += 'x';
            }
        }
    }
};