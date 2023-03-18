//
// Created by Archishmaan Peyyety on 3/2/23.
//

#include "string"
#include "sstream"
#include "cmath"
#include "midnight.h"
#include "move_generation/position.h"
#include "evaluation/evaluate.h"

void add_param(parameters_t& params, const Score score) {
	pair_t pair = { (double) score.middle_game, (double) score.end_game };
	params.push_back(pair);
}

void add_params(parameters_t& params, const Score scores[], int size) {
	for (int i = 0; i < size; i++) {
		add_param(params, scores[i]);
	}
}

parameters_t Midnight::MidnightEval::get_initial_parameters() {
	parameters_t parameters;

	add_params(parameters, PIECE_VALUES, NPIECE_TYPES);

	add_params(parameters, PAWN_TABLE, NSQUARES);
	add_params(parameters, KNIGHT_TABLE, NSQUARES);
	add_params(parameters, BISHOP_TABLE, NSQUARES);
	add_params(parameters, ROOK_TABLE, NSQUARES);
	add_params(parameters, QUEEN_TABLE, NSQUARES);
	add_params(parameters, KING_TABLE, NSQUARES);

	add_params(parameters, OPEN_FILE_BONUS, NPIECE_TYPES);
	add_params(parameters, SEMI_OPEN_FILE_BONUS, NPIECE_TYPES);
	add_params(parameters, PAWN_PROTECTION, NPIECE_TYPES);
	add_params(parameters, KING_PAWN_SHIELD, 2);

	add_param(parameters, PASSED_PAWN_BONUS);
	add_param(parameters, BLOCKED_PASSED_PAWN_PENALTY);
	add_param(parameters, ISOLATED_PAWN_PENALTY);
	add_param(parameters, DOUBLED_PAWN_PENALTY);
	add_param(parameters, BISHOP_PAIR_BONUS);

	add_param(parameters, KNIGHT_MOBILITY);
	add_param(parameters, BISHOP_MOBILITY);
	add_param(parameters, ROOK_MOBILITY);
	add_param(parameters, QUEEN_MOBILITY);


	return parameters;
}

EvalResult Midnight::MidnightEval::get_fen_eval_result(const std::string &fen) {
	Position p;
	Position::set(fen, p);
	Trace trace = {};

	if (p.turn() == BLACK) evaluate<BLACK>(p, trace);
	else evaluate<WHITE>(p, trace);

	coefficients_t coefficients;
	get_coefficient_array(coefficients, trace.material, NPIECE_TYPES);

	get_coefficient_array(coefficients, trace.pawn_pst, NSQUARES);
	get_coefficient_array(coefficients, trace.knight_pst, NSQUARES);
	get_coefficient_array(coefficients, trace.bishop_pst, NSQUARES);
	get_coefficient_array(coefficients, trace.rook_pst, NSQUARES);
	get_coefficient_array(coefficients, trace.queen_pst, NSQUARES);
	get_coefficient_array(coefficients, trace.king_pst, NSQUARES);

	get_coefficient_array(coefficients, trace.open_files, NPIECE_TYPES);
	get_coefficient_array(coefficients, trace.semi_open_files, NPIECE_TYPES);
	get_coefficient_array(coefficients, trace.pawn_protection, NPIECE_TYPES);
	get_coefficient_array(coefficients, trace.king_pawn_shield, 2);

	get_coefficient_single(coefficients, trace.passed_pawns);
	get_coefficient_single(coefficients, trace.blocked_passed_pawns);
	get_coefficient_single(coefficients, trace.isolated_pawns);
	get_coefficient_single(coefficients, trace.doubled_pawns);
	get_coefficient_single(coefficients, trace.bishop_bonus);

	get_coefficient_single(coefficients, trace.knight_mobility);
	get_coefficient_single(coefficients, trace.bishop_mobility);
	get_coefficient_single(coefficients, trace.rook_mobility);
	get_coefficient_single(coefficients, trace.queen_mobility);

	EvalResult result;
	result.score = trace.score;
	result.coefficients = coefficients;
	return result;
}
static int32_t round_value(tune_t value) {
	return static_cast<int32_t>(round(value));
}
static void print_parameter(std::stringstream& ss, const pair_t parameter) {
	const auto mg = round(parameter[static_cast<int32_t>(PhaseStages::Midgame)]);
	const auto eg = round(parameter[static_cast<int32_t>(PhaseStages::Endgame)]);
	ss << "S(" << mg << ", " << eg << ")";
}
static void print_parameter(std::stringstream& ss, const tune_t parameter) {
	ss << round_value(std::round(parameter));
}

static void print_single(std::stringstream& ss, const parameters_t& parameters, int& index, const std::string& name) {
	ss << "constexpr Score " << name << " = ";
	print_parameter(ss, parameters[index]);
	index++;

	ss << ";" << std::endl;
}

static void print_array(std::stringstream& ss, const parameters_t& parameters, int& index, const std::string& name, int count) {
	ss << "constexpr Score " << name << "[] = {";
	for (auto i = 0; i < count; i++)
	{
		print_parameter(ss, parameters[index]);
		index++;

		if (i != count - 1)
		{
			ss << ", ";
		}
	}
	ss << "};" << std::endl;
}

static void print_max_material(std::stringstream& ss, const parameters_t& parameters) {
	ss << "const int max_material[] = {";
	for (auto i = 0; i < 6; i++)
	{
		const auto mg = parameters[i][static_cast<int>(PhaseStages::Midgame)];
		const auto eg = parameters[i][static_cast<int>(PhaseStages::Endgame)];
		const auto max_material = round_value(std::max(mg, eg));
		ss << max_material << ", ";
	}
	ss << "0};" << std::endl;
}

void Midnight::MidnightEval::print_parameters(const parameters_t &parameters) {
	parameters_t parameters_copy = parameters;

	int index = 0;
	std::stringstream ss;
	print_array(ss, parameters_copy, index, "PIECE_VALUES", NPIECE_TYPES);

	print_array(ss, parameters_copy, index, "PAWN_TABLE", NSQUARES);
	print_array(ss, parameters_copy, index, "KNIGHT_TABLE", NSQUARES);
	print_array(ss, parameters_copy, index, "BISHOP_TABLE", NSQUARES);
	print_array(ss, parameters_copy, index, "ROOK_TABLE", NSQUARES);
	print_array(ss, parameters_copy, index, "QUEEN_TABLE", NSQUARES);
	print_array(ss, parameters_copy, index, "KING_TABLE", NSQUARES);

	print_array(ss, parameters_copy, index, "OPEN_FILE_BONUS", NPIECE_TYPES);
	print_array(ss, parameters_copy, index, "SEMI_OPEN_FILE_BONUS", NPIECE_TYPES);
	print_array(ss, parameters_copy, index, "PAWN_PROTECTION", NPIECE_TYPES);
	print_array(ss, parameters_copy, index, "KING_PAWN_SHIELD", 2);

	print_single(ss, parameters_copy, index, "PASSED_PAWN_BONUS");
	print_single(ss, parameters_copy, index, "BLOCKED_PASSED_PAWN_PENALTY");
	print_single(ss, parameters_copy, index, "ISOLATED_PAWN_PENALTY");
	print_single(ss, parameters_copy, index, "DOUBLED_PAWN_PENALTY");
	print_single(ss, parameters_copy, index, "BISHOP_PAIR_BONUS");

	print_single(ss, parameters_copy, index, "KNIGHT_MOBILITY");
	print_single(ss, parameters_copy, index, "BISHOP_MOBILITY");
	print_single(ss, parameters_copy, index, "ROOK_MOBILITY");
	print_single(ss, parameters_copy, index, "QUEEN_MOBILITY");

	std::cout << ss.str() << "\n";
}
