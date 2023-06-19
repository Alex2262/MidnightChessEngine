#pragma once
#include "search_params.h"
#include "search_constants.h"
#include "../move_gen/types/move.h"
#include <array>
#include "tables/history_table.h"

constexpr array<array<int32_t, 2>, MAX_DEPTH + 1> generate_quiet_lmp_table() {
    array<array<int32_t, 2>, MAX_DEPTH + 1> lmp_table{};
    for (int depth = 0; depth < MAX_DEPTH + 1; depth++) {
        lmp_table[depth][0] = (3 + depth * depth) / 2;
        lmp_table[depth][1] = 3 + depth * depth;
    }
    return lmp_table;
}

constexpr int lmp_table[5] = {0, 8, 16, 26, 45};
constexpr auto quiet_lmp_table = generate_quiet_lmp_table();

inline bool late_move_prune(bool pv_node, int move_idx, int depth, bool improving) {
    return !pv_node && depth <= LMP_MIN_DEPTH && move_idx >= lmp_table[depth] - 2 * !improving;
}

inline bool late_move_prune_quiet(bool pv_node, int move_idx, Move legal_move, int depth, bool improving) {
    return !pv_node && depth <= 12 && legal_move.is_quiet() && move_idx >= quiet_lmp_table[depth][improving];
}

inline bool futility_prune(int static_eval, int alpha, int value, int depth) {
    int fp_margin = depth * FP_COEFFICIENT + FP_MARGIN;
    return value > -MATE_BOUND && depth < FP_DEPTH && static_eval + fp_margin <= alpha;
}

template<Color color>
inline bool history_prune(bool pv_node, int value, int depth, Move legal_move) {
    return !pv_node && value > -MATE_BOUND && depth < 3 && history[color][legal_move.from()][legal_move.to()] < -1024 * depth;
}

template<Color color>
inline bool see_prune_pvs(Position& board, bool pv_node, int depth, int value, Move legal_move) {
    return !pv_node && depth < SEE_PVS_MIN_DEPTH && value > -MATE_BOUND &&
           !static_exchange_eval<color>(board, legal_move, legal_move.is_quiet() ? SEE_PVS_QUIET_MARGIN * depth : SEE_PVS_TACTICAL_MARGIN * depth);
}