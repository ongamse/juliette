#pragma once

#include <vector>
#include "Piece.h"
#include "Board.h"

class Bishop : public Piece {

public:

    Bishop(Board::Color c, int8_t file, int8_t rank, Board *parent);

    void add_moves(std::vector<uint32_t> *move_list) override;

    static bool can_attack(int8_t from_file, int8_t from_rank, int8_t to_file, int8_t to_rank, Board *parent);

    bool can_attack(int8_t file, int8_t rank) override;

    char get_piece_char() override;

    uint8_t get_type() const override;
};