//
// Created by Archishmaan Peyyety on 2/25/23.
//
#pragma once

constexpr int NGAME_PHASES = 2;
enum GamePhase {
	MIDDLE,
	END
};

struct Score {
	short middle_game;
	short end_game;
	constexpr Score (short m, short e) : middle_game(m), end_game(e) {}
	constexpr Score () : middle_game(0), end_game(0) {}
	Score& operator = (const Score& s) = default;
	constexpr Score operator + (const Score& s) const {
		return {
			(short) (middle_game + s.middle_game),
			(short) (end_game + s.end_game)
		};
	}
	constexpr Score operator * (const short& s) const {
		return {
			(short) (middle_game * s),
			(short) (end_game * s)
		};
	}
	constexpr Score operator - (const Score& s) const {
		return {
			(short) (middle_game - s.middle_game),
			(short) (end_game - s.end_game)
		};
	}
	inline constexpr bool operator == (const Score& s) const {
		return middle_game == s.middle_game && end_game == s.end_game;
	}
	inline Score& operator += (const Score& s) {
		middle_game = short(middle_game + s.middle_game);
		end_game = short(end_game + s.end_game);
		return *this;
	}
	inline Score& operator -= (const Score& s) {
		middle_game = short(middle_game - s.middle_game);
		end_game = short(end_game - s.end_game);
		return *this;
	}
	inline Score& operator *= (const short& s) {
		middle_game = short(middle_game * s);
		end_game = short(end_game * s);
		return *this;
	}
	friend inline std::ostream& operator<<(std::ostream& os, const Score& s) {
		return os << "midgame: " << s.middle_game << " endgame: " << s.end_game;
	}
};

typedef Score S;

struct Trace {
	int score={};

	short material[NPIECE_TYPES][NCOLORS]{};

	short pawn_pst[NSQUARES][NCOLORS]{};
	short knight_pst[NSQUARES][NCOLORS]{};
	short bishop_pst[NSQUARES][NCOLORS]{};
	short rook_pst[NSQUARES][NCOLORS]{};
	short queen_pst[NSQUARES][NCOLORS]{};
	short king_pst[NSQUARES][NCOLORS]{};

	short open_files[NPIECE_TYPES][NCOLORS]{};
	short semi_open_files[NPIECE_TYPES][NCOLORS]{};
	short pawn_protection[NPIECE_TYPES][NCOLORS]{};

	short king_pawn_shield[2][NCOLORS]{};
	short passed_pawns[NCOLORS]{};
	short blocked_passed_pawns[NCOLORS]{};
	short isolated_pawns[NCOLORS]{};
	short doubled_pawns[NCOLORS]{};
	short bishop_bonus[NCOLORS]{};

	short knight_mobility[NCOLORS]{};
	short bishop_mobility[NCOLORS]{};
	short rook_mobility[NCOLORS]{};
	short queen_mobility[NCOLORS]{};
};

std::ostream& operator<<(std::ostream& os, const Trace& t) {
	os << "score: " << t.score << std::endl;
	os << "pawn pst: ";
	for (int j = 0; j < NCOLORS; j++) {
		for (int i = 0; i < NSQUARES; i++) os << " " << t.pawn_pst[i][j];
	}
	os << std::endl;

	os << "knight pst: ";
	for (int j = 0; j < NCOLORS; j++) {
		for (int i = 0; i < NSQUARES; i++) os << " " << t.knight_pst[i][j];
	}
	os << std::endl;
	return os;
}
