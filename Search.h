#pragma once

#include <unordered_set>
#include <stack>
#include <vector>
#include "Board.h"

class BoardHasher {

    size_t operator() (const uint8_t *&board) const {
        size_t hash = 0;
        for (uint8_t i = 0; i < 64; ++i) {
            hash += board[i];
        }
        return hash;
    }

};

class BoardComparator {
    bool operator() (const uint8_t *&board1, const uint8_t *&board2) const {
        for (uint8_t i = 0; i < 64; ++i) {
            if (board1[i] != board2[i]) {
                return false;
            }
        }
        return true;
    }
};

std::unordered_set<uint8_t *, BoardHasher, BoardComparator> positions_seen;

Board *subject;

uint32_t search(Board *board, uint16_t depth);