#pragma once
template<Color color>
constexpr Score evaluate_rooks(Position& board, Trace& trace) {
	Score score = Score();
	Bitboard rooks = board.bitboard_of(color, ROOK);
	Bitboard us_pieces = board.all_pieces<color>();
	Bitboard them_pieces = board.all_pieces<~color>();
	const Bitboard board_open_files = open_files(board);
	const Bitboard board_semi_open_files = semi_open_files<color>(board);
	while (rooks) {
		Square rook_square = pop_lsb(&rooks);
		const Bitboard rook_square_bb = SQUARE_BB[rook_square];
		score += PIECE_VALUES[ROOK];
		trace.material[ROOK][color] += 1;
		score += read_psqt<color>(ROOK, rook_square);
		Square trace_square = color == WHITE ? ~rook_square : rook_square;
		trace.rook_pst[trace_square][color] += 1;
		Bitboard pseudo_legal_moves = attacks<ROOK>(rook_square, them_pieces | us_pieces) & ~us_pieces;
		trace.rook_mobility[color] += pop_count(pseudo_legal_moves);
		score += ROOK_MOBILITY * pop_count(pseudo_legal_moves);
		const bool on_open_file = rook_square_bb & board_open_files;
		const bool on_semi_open_file = rook_square_bb & board_semi_open_files;
		trace.open_files[ROOK][color] += on_open_file;
		trace.semi_open_files[ROOK][color] += on_semi_open_file;
		score += (OPEN_FILE_BONUS[ROOK] * on_open_file) + (SEMI_OPEN_FILE_BONUS[ROOK] * on_semi_open_file);
	}
	return score;
}