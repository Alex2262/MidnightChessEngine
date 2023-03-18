#pragma once
template<Color color>
constexpr Score evaluate_queens(Position& board, Trace& trace) {
	Bitboard queens = board.bitboard_of(color, QUEEN);
	const Bitboard us_pieces = board.all_pieces<color>();
	const Bitboard them_pieces = board.all_pieces<~color>();

	const Bitboard board_open_files = open_files(board);
	const Bitboard board_semi_open_files = semi_open_files<color>(board);

	Score score = Score();

	while (queens) {
		Square queen_square = pop_lsb(&queens);
		const Bitboard queen_square_bb = SQUARE_BB[queen_square];
		score += PIECE_VALUES[QUEEN];
		trace.material[QUEEN][color] += 1;
		score += read_psqt<color>(QUEEN, queen_square);
		Square trace_square = color == WHITE ? ~queen_square : queen_square;
		trace.queen_pst[trace_square][color] += 1;
		Bitboard pseudo_legal_moves = attacks<QUEEN>(queen_square, them_pieces | us_pieces) & ~us_pieces;
		trace.queen_mobility[color] += pop_count(pseudo_legal_moves);
		score += QUEEN_MOBILITY * pop_count(pseudo_legal_moves);

		const bool on_open_file = queen_square_bb & board_open_files;
		const bool on_semi_open_file = queen_square_bb & board_semi_open_files;
		trace.open_files[QUEEN][color] += on_open_file;
		trace.semi_open_files[QUEEN][color] += on_semi_open_file;
		score += (OPEN_FILE_BONUS[QUEEN] * on_open_file) + (SEMI_OPEN_FILE_BONUS[QUEEN] * on_semi_open_file);
	}
	return score;
}