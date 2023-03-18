#pragma once

constexpr int NKING_SIDES = 2;

constexpr Bitboard KING_SAFE_AREA[NCOLORS] = {
		0xC3D7, 0xd7c3000000000000
};

constexpr Bitboard PAWN_SHIELD[NKING_SIDES][NCOLORS] = {
		{0x700, 0x7000000000000},
		{0xe000, 0xe0000000000000}
};

template<Color color>
constexpr Score evaluate_king(Position& board, Trace& trace) {
	Score score = Score();
	const Bitboard board_open_files = open_files(board);
	const Bitboard board_semi_open_files = semi_open_files<color>(board);
	const Bitboard pawns = board.bitboard_of(color, PAWN);

	Bitboard king = board.bitboard_of(color, KING);
	Square king_square = pop_lsb(&king);
	File file = file_of(king_square);
	const int king_side = file >> 2;

	const Bitboard king_bb = SQUARE_BB[king_square];
	score += PIECE_VALUES[KING];
	trace.material[KING][color] += 1;
	score += read_psqt<color>(KING, king_square);
	if (color == WHITE) king_square = ~king_square;
	trace.king_pst[king_square][color] += 1;
	const bool on_open_file = king_bb & board_open_files;
	const bool on_semi_open_file = king_bb & board_semi_open_files;
	trace.open_files[KING][color] += on_open_file;
	trace.semi_open_files[KING][color] += on_semi_open_file;
	score += (OPEN_FILE_BONUS[KING] * on_open_file) + (SEMI_OPEN_FILE_BONUS[KING] * on_semi_open_file);

	if (king_bb & KING_SAFE_AREA[color]) {
		const Bitboard pawn_shield = PAWN_SHIELD[king_side][color];
		trace.king_pawn_shield[0][color] += pop_count(pawns & pawn_shield);
		score += KING_PAWN_SHIELD[0] * pop_count(pawns & pawn_shield);
		trace.king_pawn_shield[1][color] += pop_count(pawns & shift<relative_dir<color>(NORTH)>(pawn_shield));
		score += KING_PAWN_SHIELD[1] * pop_count(pawns & shift<relative_dir<color>(NORTH)>(pawn_shield));
	}
	return score;
}