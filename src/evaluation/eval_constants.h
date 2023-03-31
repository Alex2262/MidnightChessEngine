#pragma once
#include "move_generation/types.h"
#include "types.h"

//
// Created by Archishmaan Peyyety on 1/10/23.
//

// If this is updated be sure compute_game_phase loop is also updated. Currently only checks non-zeros.
constexpr int GAME_PHASE_BONUS[NPIECE_TYPES] = {
		0, 1, 1, 2, 4, 0
};

constexpr Score PIECE_VALUES[] = {S(106, 92), S(416, 342), S(406, 339), S(542, 649), S(1292, 1153), S(0, 0)};
constexpr Score PAWN_TABLE[] = {S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(190, 166), S(182, 156), S(114, 149), S(211, 91), S(153, 106), S(216, 94), S(18, 162), S(-10, 194), S(-17, 105), S(8, 97), S(50, 68), S(55, 45), S(79, 27), S(88, 31), S(11, 68), S(-1, 80), S(-14, 33), S(3, 17), S(8, 8), S(39, -21), S(32, -9), S(41, -6), S(14, 7), S(-9, 17), S(-19, 14), S(-15, 6), S(4, -10), S(23, -18), S(19, -11), S(24, -16), S(5, -8), S(-11, -2), S(-17, 3), S(-23, 5), S(-1, -6), S(4, -8), S(10, 3), S(-6, 4), S(-6, -3), S(-25, -1), S(-25, 22), S(-21, 15), S(-22, 24), S(-13, 15), S(-15, 27), S(0, 21), S(-4, 8), S(-41, 6), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0)};
constexpr Score KNIGHT_TABLE[] = {S(-216, -30), S(-119, 3), S(-48, 28), S(-27, 8), S(110, -11), S(-133, 19), S(-4, -43), S(-114, -85), S(-43, 3), S(-44, 25), S(81, 10), S(41, 45), S(61, 26), S(89, 0), S(0, 7), S(-9, -12), S(-32, 4), S(49, 19), S(63, 44), S(90, 49), S(123, 24), S(163, 24), S(102, 5), S(64, -25), S(19, 9), S(34, 36), S(48, 58), S(73, 64), S(56, 64), S(80, 57), S(44, 38), S(44, 9), S(3, 10), S(10, 28), S(37, 52), S(39, 58), S(54, 54), S(44, 46), S(53, 30), S(11, 15), S(-13, -23), S(9, 21), S(31, 19), S(35, 48), S(49, 31), S(42, 11), S(39, -2), S(-3, -3), S(-24, -22), S(-20, 2), S(8, 13), S(23, 28), S(25, 34), S(30, 8), S(10, 12), S(17, -28), S(-112, -10), S(-7, -27), S(-38, 6), S(-7, 16), S(4, 7), S(2, 8), S(-4, -16), S(-45, -46)};
constexpr Score BISHOP_TABLE[] = {S(-20, 7), S(3, 12), S(-131, 38), S(-67, 29), S(-53, 34), S(-63, 28), S(-20, 25), S(-13, 11), S(-36, 32), S(5, 15), S(-12, 28), S(-42, 25), S(19, 25), S(58, 12), S(8, 19), S(-33, 21), S(-11, 25), S(23, 18), S(46, 11), S(26, 16), S(44, 7), S(56, 19), S(52, 21), S(13, 35), S(-7, 22), S(13, 24), S(9, 27), S(55, 13), S(43, 16), S(33, 13), S(20, 23), S(9, 29), S(5, 10), S(13, 17), S(13, 22), S(37, 21), S(43, 9), S(12, 16), S(8, 9), S(33, 4), S(14, 4), S(33, 14), S(23, 16), S(19, 17), S(22, 23), S(36, 7), S(30, 2), S(31, -4), S(24, 1), S(32, -2), S(30, 6), S(12, 14), S(23, 11), S(16, 4), S(52, 10), S(14, -9), S(19, -10), S(21, 7), S(8, 23), S(3, 18), S(-5, 22), S(4, 25), S(-16, 17), S(4, -3)};
constexpr Score ROOK_TABLE[] = {S(21, 46), S(25, 44), S(11, 51), S(37, 39), S(61, 38), S(18, 45), S(54, 36), S(63, 38), S(10, 48), S(12, 49), S(36, 42), S(50, 41), S(64, 28), S(101, 20), S(27, 42), S(62, 31), S(-19, 43), S(8, 40), S(15, 39), S(15, 39), S(32, 29), S(72, 23), S(121, 4), S(31, 20), S(-25, 38), S(-19, 37), S(-1, 43), S(17, 33), S(4, 39), S(33, 32), S(6, 28), S(-5, 33), S(-38, 36), S(-39, 43), S(-26, 42), S(-21, 39), S(-20, 36), S(5, 21), S(29, 17), S(-10, 18), S(-35, 22), S(-22, 28), S(-21, 20), S(-13, 23), S(-12, 23), S(0, 18), S(21, 14), S(-7, 7), S(-36, 19), S(-19, 21), S(-25, 29), S(-15, 24), S(1, 18), S(20, 9), S(15, 3), S(-54, 24), S(-1, 24), S(-2, 26), S(5, 25), S(16, 18), S(12, 18), S(20, 20), S(-10, 25), S(7, 6)};
constexpr Score QUEEN_TABLE[] = {S(-23, 14), S(-16, 57), S(-1, 52), S(24, 33), S(109, 18), S(122, 10), S(71, 24), S(68, 33), S(-17, 15), S(-55, 40), S(-15, 50), S(-25, 68), S(-36, 95), S(73, 49), S(18, 74), S(57, 36), S(-16, 0), S(-18, 2), S(8, -8), S(-27, 75), S(22, 74), S(113, 34), S(96, 43), S(53, 84), S(-20, -1), S(-25, 26), S(-17, 32), S(-17, 42), S(-8, 75), S(12, 77), S(15, 96), S(4, 105), S(-3, -31), S(-12, 26), S(1, 0), S(-10, 45), S(5, 20), S(10, 48), S(12, 56), S(9, 44), S(-14, -1), S(18, -57), S(11, -5), S(15, -28), S(12, -6), S(15, 3), S(21, 16), S(5, 49), S(-17, -18), S(16, -48), S(28, -67), S(20, -29), S(29, -52), S(27, -36), S(31, -60), S(1, -24), S(5, -23), S(1, -35), S(22, -51), S(28, 5), S(14, -38), S(5, -54), S(11, -52), S(-33, -43)};
constexpr Score KING_TABLE[] = {S(-116, -93), S(223, -81), S(187, -62), S(162, -58), S(-136, 5), S(-82, 24), S(98, -19), S(73, -36), S(246, -72), S(79, 12), S(72, 11), S(100, 2), S(51, 14), S(68, 29), S(-3, 33), S(-140, 25), S(76, -4), S(106, 11), S(114, 19), S(39, 19), S(69, 12), S(124, 38), S(162, 30), S(-15, 8), S(23, -17), S(31, 22), S(14, 36), S(-25, 48), S(-35, 42), S(-15, 40), S(-6, 32), S(-96, 12), S(-64, -22), S(48, -11), S(-36, 31), S(-79, 49), S(-92, 53), S(-65, 38), S(-61, 21), S(-88, -6), S(-13, -29), S(-12, 1), S(-43, 20), S(-67, 38), S(-57, 35), S(-53, 24), S(-27, 7), S(-32, -18), S(-16, -33), S(-29, -11), S(-16, 7), S(-74, 21), S(-45, 18), S(-38, 10), S(-35, -0), S(-21, -26), S(-37, -75), S(-14, -43), S(-44, -16), S(-55, -10), S(-66, -18), S(-30, -21), S(-19, -30), S(-17, -73)};
constexpr Score OPEN_FILE_BONUS[] = {S(0, 0), S(0, 0), S(0, 0), S(48, 0), S(-2, 23), S(-60, -1)};
constexpr Score SEMI_OPEN_FILE_BONUS[] = {S(0, 0), S(0, 0), S(0, 0), S(18, 7), S(1, 21), S(-25, 14)};
constexpr Score PAWN_PROTECTION[] = {S(-2, 11), S(-1, 21), S(-4, 21), S(0, 0), S(0, 0), S(0, 0)};
constexpr Score KING_PAWN_SHIELD[] = {S(40, -13), S(34, -10)};

constexpr Score KING_RING_PENALTY[NPIECE_TYPES] = {S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0)};
constexpr Score CHECK_PENALTY[NPIECE_TYPES] = {S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0)};

constexpr Score PASSED_PAWN_BONUS = S(-0, 22);
constexpr Score BLOCKED_PASSED_PAWN_PENALTY = S(5, -20);
constexpr Score ISOLATED_PAWN_PENALTY = S(-19, -8);
constexpr Score DOUBLED_PAWN_PENALTY = S(-16, -19);
constexpr Score BISHOP_PAIR_BONUS = S(28, 67);
constexpr Score KNIGHT_MOBILITY = S(0, 1);
constexpr Score BISHOP_MOBILITY = S(6, 6);
constexpr Score ROOK_MOBILITY = S(4, 3);
constexpr Score QUEEN_MOBILITY = S(1, 6);

// TODO: This kinda slow. Like 8 conditionals when it could just be PST[COLOR][TYPE][SQUARE]
template<Color color>
constexpr Score read_psqt(PieceType piece_type, Square square) {
	if (color == WHITE) square = ~square;
	switch (piece_type) {
		case PAWN: return PAWN_TABLE[square];
		case KNIGHT: return KNIGHT_TABLE[square];
		case BISHOP: return BISHOP_TABLE[square];
		case ROOK: return ROOK_TABLE[square];
		case QUEEN: return QUEEN_TABLE[square];
		case KING: return KING_TABLE[square];
		default: return {};
	}
}
