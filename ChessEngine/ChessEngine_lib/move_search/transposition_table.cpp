//
// Created by Archishmaan Peyyety on 1/15/23.
//
#include "transposition_table.h"
#include "search_params.h"

static int sign(int x) {
	return (x >= 0 ? 1 : -1);
}

TranspositionTable::TranspositionTable(uint64_t size) {
	table_size = size;
	transposition_table = new TranspositionTableEntry[table_size];

	TranspositionTableEntry default_entry = TranspositionTableEntry();
	default_entry.value = 0;
	default_entry.zobrist_hash = 0;
	default_entry.depth = 0;
	default_entry.best_move = 0;
	default_entry.node_type = EXACT;
	for (int i = 0; i < table_size; i++) {
		transposition_table[i] = default_entry;
	}
}

TranspositionTable::~TranspositionTable() {
	delete [] transposition_table;
}

int TranspositionTable::correct_mate_for_retrieval(int score, int ply) {
	if (std::abs(score) > MATE_SCORE - MAX_MATE_DEPTH) {
		return (score * sign(score) + ply) * sign(score);
	}
	return score;
}

int TranspositionTable::correct_mate_for_storage(int score, int ply) {
	if (std::abs(score) > MATE_SCORE - MAX_MATE_DEPTH) {
		return (score * sign(score) - ply) * sign(score);
	}
	return score;
}

uint64_t TranspositionTable::get_index(uint64_t zobrist_hash) {
	return zobrist_hash % table_size;
}

TranspositionTableEntryNodeType
TranspositionTable::get_node_type(const int &alpha_initial, const int &alpha, const int &beta, const int &value) {
	TranspositionTableEntryNodeType node_type;
	if (value >= beta) {
		node_type = LOWER_NODE;
	} else {
		if (alpha != alpha_initial) {
			node_type = EXACT;
		} else {
			node_type = UPPER_NODE;
		}
	}
	return node_type;
	/*
	if (value <= alpha_initial) node_type = UPPER_NODE;
	else if (value >= beta) node_type = LOWER_NODE;
	else node_type = EXACT;
	return node_type;
	 */
}

void TranspositionTable::put(zobrist_hash hash, int depth, int score, int ply, Move best_move, const std::string fen, TranspositionTableEntryNodeType node_type) {
	TranspositionTableEntry entry;
	entry.zobrist_hash = hash;
	entry.depth = depth;
	entry.value = correct_mate_for_storage(score, ply);
	entry.node_type = node_type;
	entry.best_move = best_move;
	entry.fen = fen;
	transposition_table[get_index(hash)] = entry;
}

TranspositionTableSearchResults
TranspositionTable::probe_for_move_ordering(zobrist_hash hash) {
	TranspositionTableEntry entry = transposition_table[get_index(hash)];
	TranspositionTableSearchResults results;
	results.entry_found = false;
	if (entry.zobrist_hash == hash) {
		results.entry_found = true;
		results.entry = entry;
	}
	return results;
}

TranspositionTableSearchResults
TranspositionTable::probe_for_search(zobrist_hash hash, int depth, int ply) {
	TranspositionTableEntry entry = transposition_table[get_index(hash)];
	TranspositionTableSearchResults results;
	results.entry_found = false;
	if (entry.zobrist_hash == hash && entry.depth >= depth && ply != 0) {
		results.entry_found = true;
		results.entry = entry;
		results.entry.value = correct_mate_for_retrieval(results.entry.value, ply);
	}
	return results;
}

bool TranspositionTable::key_in_table(zobrist_hash hash) {
	return transposition_table[get_index(hash)].zobrist_hash == 0;
}

int TranspositionTable::count_entries() {
	int entries = 0;
	for (int i = 0; i < table_size; i++) {
		if (transposition_table[i].zobrist_hash != 0) {
			entries += 1;
		}
	}
	return entries;
}
