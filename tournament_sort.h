#include <utility>
#include <vector>

using namespace std;

struct TournamentTree {
    string value;
    vector<TournamentTree*> children;

    explicit TournamentTree(string value) : value(move(value)) {}
};

class tournament_sort {
    vector<TournamentTree*> items;

    static TournamentTree* Round(TournamentTree* l, TournamentTree* r) {
        if (l == nullptr) return r;
        if (r == nullptr) return l;
        if (l->value < r->value) {
            l->children.push_back(r);
            return l;
        } else {
            r->children.push_back(l);
            return r;
        }
    }

    void play_round() {
        while (items.size() > 1) {
            vector<TournamentTree*> winners;
            if (items.size() % 2 == 1) {
                items.push_back(nullptr);
            }
            for (int i = 0; i < items.size(); i += 2) {
                winners.push_back(Round(items[i], items[i + 1]));
            }
            items = winners;
        }
    }

    string get_round_winner() {
        TournamentTree* round_winner = items[0];
        for (int i = 1; i < items.size(); i++) {
            round_winner = Round(round_winner, items[i]);
        }
        items = round_winner->children;
        string res = round_winner->value;
        delete round_winner;
        return res;
    }

public:
    explicit tournament_sort(const vector<string>& values) {
        for (const auto& value : values) {
            items.push_back(new TournamentTree(value));
        }
        play_round();
    }

    vector<string> sorted() {
        vector<string> result;
        while (!items.empty()) {
            result.push_back(get_round_winner());
        }
        return result;
    }
};
