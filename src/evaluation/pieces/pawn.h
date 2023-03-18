//
// Created by Archishmaan Peyyety on 2/24/23.
//
#pragma once
#include "move_generation/types.h"
#include "evaluation/bitboards.h"

template<Color c>
constexpr Score evaluate_passed_pawns(Position& board, Trace& trace) {
	Score passed_pawn_score = Score(0, 0);
	Bitboard passed_pawns_us = passed_pawns<c>(board);
	while (passed_pawns_us) {
		const Square pawn = pop_lsb(&passed_pawns_us);
		const Rank rank = relative_rank<c>(rank_of(pawn));
		trace.passed_pawns[c] += rank;
		passed_pawn_score += PASSED_PAWN_BONUS * rank;
		const bool blocked = shift<relative_dir<c>(NORTH)>(SQUARE_BB[pawn]) & board.all_pieces<~c>();
		trace.blocked_passed_pawns[c] += rank * blocked;
		passed_pawn_score += BLOCKED_PASSED_PAWN_PENALTY * rank * blocked;
	}
	return passed_pawn_score;
}

template<Color c>
inline constexpr Score evaluate_doubled_pawns(Position& board, Trace& trace) {
	trace.doubled_pawns[c] += pop_count(doubled_pawns<c>(board));
	return DOUBLED_PAWN_PENALTY * pop_count(doubled_pawns<c>(board));
}

template<Color c>
inline constexpr Score evaluate_isolated_pawns(Position& board, Trace& trace) {
	const int isolated_pawn_count = pop_count(isolated_pawns<c>(board));
	trace.isolated_pawns[c] += isolated_pawn_count;
	return ISOLATED_PAWN_PENALTY * isolated_pawn_count;
}

template<Color c>
inline constexpr Score evaluate_pawn_locations(Position& board, Trace& trace) {
	Score score = Score();
	Bitboard pawns = board.bitboard_of(c, PAWN);
	const Bitboard all_pawns = pawns;
	while (pawns) {
		Square pawn_square = pop_lsb(&pawns);
		const Rank rank = relative_rank<c>(rank_of(pawn_square));
		score += PIECE_VALUES[PAWN];
		trace.material[PAWN][c] += 1;
		score += read_psqt<c>(PAWN, pawn_square);
		if (c == WHITE) pawn_square = ~pawn_square;
		trace.pawn_pst[pawn_square][c] += 1;
		const Bitboard supporting_pawns = all_pawns & pawn_attacks<~c>(pawn_square);
		trace.pawn_protection[PAWN][c] += pop_count(supporting_pawns) * rank;
		score += PAWN_PROTECTION[PAWN] * pop_count(supporting_pawns) * rank;
	}
	return score;
}

template<Color c>
constexpr Score evaluate_pawn_structure(Position& board, Trace& trace) {
	const Score pawn_location_eval = evaluate_pawn_locations<c>(board, trace);
	const Score passed_pawn_eval = evaluate_passed_pawns<c>(board, trace);
	const Score isolated_pawn_eval = evaluate_isolated_pawns<c>(board, trace);
	const Score doubled_pawn_eval = evaluate_doubled_pawns<c>(board, trace);
	return pawn_location_eval + passed_pawn_eval + isolated_pawn_eval + doubled_pawn_eval;
}