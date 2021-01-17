/*
**    Stash, a UCI chess playing engine developed from scratch
**    Copyright (C) 2019-2021 Morgan Houppin
**
**    Stash is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    Stash is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "board.h"

bitboard_t  slider_blockers(const board_t *board, bitboard_t sliders,
            square_t square, bitboard_t *pinners)
{
    bitboard_t  blockers = *pinners = 0;
    bitboard_t  snipers =
        ((PseudoMoves[ROOK][square] & piecetypes_bb(board, QUEEN, ROOK))
        | (PseudoMoves[BISHOP][square] & piecetypes_bb(board, QUEEN, BISHOP)))
        & sliders;
    bitboard_t  occupied = occupancy_bb(board) ^ snipers;

    while (snipers)
    {
        square_t    sniper_square = bb_pop_first_sq(&snipers);
        bitboard_t  between = between_bb(square, sniper_square) & occupied;

        if (between && !more_than_one(between))
        {
            blockers |= between;
            if (between & color_bb(board, piece_color(piece_on(board, square))))
                *pinners |= square_bb(sniper_square);
        }
    }

    return (blockers);
}
