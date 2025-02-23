//
// Created by Archishmaan Peyyety on 5/17/23.
//
#include "king.h"
#include <iostream>


template<Color color, DoTrace do_trace>
Score evaluate_king(const Position &board, const SharedEvalFeatures &eval_features, Trace &trace) {
	Score score = SCORE_ZERO;
	const Bitboard board_open_files = open_files(board);
	const Bitboard board_semi_open_files = semi_open_files<color>(board);
	const Bitboard pawns = board.occupancy<color, PAWN>();

	Bitboard king = board.occupancy<color, KING>();
	Square king_square = pop_lsb(king);
	File file = file_of(king_square);
	const int king_side = file >> 2;

	const Bitboard king_bb = square_to_bitboard(king_square);
	score += read_pst<color, KING>(king_square);
	if constexpr (do_trace) {
		trace.king_pst[color == WHITE ? flip(king_square) : king_square][color] += 1;
	}

	const bool on_open_file = king_bb & board_open_files;
	const bool on_semi_open_file = king_bb & board_semi_open_files;
	score += (OPEN_FILE_BONUS[KING] * on_open_file) + (SEMI_OPEN_FILE_BONUS[KING] * on_semi_open_file);
	if constexpr (do_trace) {
		trace.open_files[KING][color] += on_open_file;
		trace.semi_open_files[KING][color] += on_semi_open_file;
	}

	const Bitboard queen_attacks = eval_features.king_virtual_mobility[color];
	const Bitboard safe_rank = MASK_RANK[relative_rank<color>(RANK1)];
	score += KING_LINE_SAFETY[pop_count(~safe_rank & queen_attacks)];
	if constexpr (do_trace) {
		trace.king_safe_line[pop_count(~safe_rank & queen_attacks)][color] += 1;
	}

	if (!(board.occupancy<color, PAWN>() & KING_FLANKS[file_of(king_square)])) {
		score += PAWNLESS_KING_FLANK;
		if constexpr (do_trace) trace.pawnless_king_flank[color] += 1;
	}

	if (king_bb & KING_SAFE_AREA[color]) {
		const Bitboard pawn_shield = PAWN_SHIELD[king_side][color];

		score += KING_PAWN_SHIELD[0] * pop_count(pawns & pawn_shield);
		if constexpr (do_trace) trace.king_pawn_shield[0][color] += pop_count(pawns & pawn_shield);

		score += KING_PAWN_SHIELD[1] * pop_count(pawns & shift_relative<color, NORTH>(pawn_shield));
		if constexpr (do_trace) trace.king_pawn_shield[1][color] += pop_count(pawns & shift_relative<color, NORTH>(pawn_shield));
	}
	return score;
}

template Score
evaluate_king<WHITE, TRACE_EVAL>(const Position &board, const SharedEvalFeatures &eval_features, Trace &trace);
template Score
evaluate_king<BLACK, TRACE_EVAL>(const Position &board, const SharedEvalFeatures &eval_features, Trace &trace);
template Score
evaluate_king<WHITE, COMPUTE_EVAL>(const Position &board, const SharedEvalFeatures &eval_features, Trace &trace);
template Score
evaluate_king<BLACK, COMPUTE_EVAL>(const Position &board, const SharedEvalFeatures &eval_features, Trace &trace);