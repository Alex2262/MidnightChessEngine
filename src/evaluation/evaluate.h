#pragma once
#include "move_generation/position.h"
#include "eval_constants.h"
#include "evaluation/pieces/pawn.h"
#include "evaluation/pieces/knight.h"
#include "evaluation/pieces/bishop.h"
#include "evaluation/pieces/rook.h"
#include "evaluation/pieces/queen.h"
#include "evaluation/pieces/king.h"

template<Color color>
constexpr int compute_game_phase(Position& board) {
	int game_phase = 0;
	for (PieceType piece_type : { KNIGHT, BISHOP, ROOK, QUEEN }) {
		Bitboard piece_bitboard_us = board.bitboard_of(color, piece_type);
		Bitboard piece_bitboard_them = board.bitboard_of(~color, piece_type);
		game_phase += GAME_PHASE_BONUS[piece_type] * pop_count(piece_bitboard_us);
		game_phase += GAME_PHASE_BONUS[piece_type] * pop_count(piece_bitboard_them);
	}
	return game_phase;
}

template<Color color>
constexpr Score evaluate_single_side(Position& board, Trace& trace) {
	return 	evaluate_pawn_structure<color>(board, trace) +
			evaluate_knight<color>(board, trace) +
			evaluate_bishops<color>(board, trace) +
			evaluate_rooks<color>(board, trace) +
			evaluate_queens<color>(board, trace) +
			evaluate_king<color>(board, trace);
}

template<Color color>
constexpr int evaluate(Position& board) {
	const int game_phase = compute_game_phase<color>(board);
	struct Trace trace = {};

	Score us = evaluate_single_side<color>(board, trace);
	Score them = evaluate_single_side<~color>(board, trace);

	int mg_phase = std::min(game_phase, 24);
	int eg_phase = 24 - mg_phase;

	const int mg_score = us.middle_game - them.middle_game;
	const int eg_score = us.end_game - them.end_game;
	int final_score = (mg_score * mg_phase + eg_score * eg_phase) / 24;

	trace.score = final_score;
	return final_score;
}

template<Color color>
constexpr void evaluate(Position& board, Trace& trace) {
	const int game_phase = compute_game_phase<color>(board);

	Score us = evaluate_single_side<color>(board, trace);
	Score them = evaluate_single_side<~color>(board, trace);

	int mg_phase = std::min(game_phase, 24);
	int eg_phase = 24 - mg_phase;

	const int mg_score = us.middle_game - them.middle_game;
	const int eg_score = us.end_game - them.end_game;
	int final_score = (mg_score * mg_phase + eg_score * eg_phase) / 24;
	if (color == BLACK) final_score = -final_score;
	trace.score = final_score;
}