#include <bits/stdc++.h>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

long long toDecimal(string s, int base) {
    long long val = 0;
    for (char c : s) {
        int d;
        if (isdigit(c)) d = c - '0';
        else d = 10 + (tolower(c) - 'a');
        val = val * base + d;
    }
    return val;
}

int main() {
    ifstream f("input.json");
    json j;
    f >> j;

    int n = j["keys"]["n"];
    int k = j["keys"]["k"];

    vector<long long> roots;
    for (auto& [key, val] : j.items()) {
        if (key == "keys") continue;
        int base;
        if (val["base"].is_string()) base = stoi(val["base"].get<string>());
        else base = val["base"].get<int>();
        string value = val["value"];
        roots.push_back(toDecimal(value, base));
    }

    vector<long long> selected(roots.begin(), roots.begin() + k);

    vector<long long> coeff = {1};
    for (auto r : selected) {
        vector<long long> next(coeff.size() + 1, 0);
        for (int i = 0; i < coeff.size(); i++) {
            next[i] -= coeff[i] * r;
            next[i + 1] += coeff[i];
        }
        coeff = next;
    }

    for (int i = coeff.size() - 1; i >= 0; i--) {
        long long c = coeff[i];
        if (c == 0) continue;
        if (i != coeff.size() - 1 && c > 0) cout << "+";
        if (i == 0) cout << c;
        else {
            if (c == -1) cout << "-";
            else if (c != 1) cout << c;
            cout << "x";
            if (i > 1) cout << "^" << i;
        }
    }
    cout << endl;
}
