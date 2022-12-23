#pragma once

struct Ratio {
	int playoffs {0};
	int wins {0};
	int loses {0};

	Ratio() {}

	Ratio(Ratio const&) = default;

	Ratio(int playoffs_, int wins_, int loses_)
		: playoffs{playoffs_}
		, wins{wins_}
		, loses{loses_}
		{}

	Ratio& operator += (Ratio const& o) {
		playoffs += o.playoffs;
		wins += o.wins;
		loses += o.loses;
		return *this;
	}

	Ratio operator + (Ratio const& o) const {
		Ratio result = *this;
		result += o;
		return result;
	};

	double as_double() const {
		int ties = playoffs - wins - loses;
		return (double)wins / playoffs;
	}

	Ratio complement() const {
		return {playoffs, loses, wins};
	}
};
