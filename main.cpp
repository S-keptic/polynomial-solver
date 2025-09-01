#include <bits/stdc++.h>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

struct BigInt {
    string digits;
    BigInt(string s="0") {
        int i = 0; while (i+1 < (int)s.size() && s[i] == '0') i++;
        digits = s.substr(i);
    }
    BigInt(long long x) { digits = to_string(x); }

    static BigInt add(const BigInt &a, const BigInt &b) {
        string A = a.digits, B = b.digits;
        if (A.size() < B.size()) swap(A, B);
        int carry = 0; string res = "";
        int i = A.size()-1, j = B.size()-1;
        while (i >= 0 || j >= 0 || carry) {
            int x = (i >= 0 ? A[i]-'0' : 0);
            int y = (j >= 0 ? B[j]-'0' : 0);
            int sum = x + y + carry;
            res.push_back(char('0' + (sum % 10)));
            carry = sum / 10;
            i--; j--;
        }
        reverse(res.begin(), res.end());
        return BigInt(res);
    }

    static BigInt sub(const BigInt &a, const BigInt &b) { 
        string A = a.digits, B = b.digits;
        while (B.size() < A.size()) B = "0" + B;
        int borrow = 0; string res="";
        for (int i = A.size()-1; i >= 0; i--) {
            int x = (A[i]-'0') - borrow;
            int y = (i >= 0 ? B[i]-'0' : 0);
            if (x < y) { x += 10; borrow = 1; }
            else borrow = 0;
            res.push_back(char('0' + (x-y)));
        }
        while (res.size() > 1 && res.back()=='0') res.pop_back();
        reverse(res.begin(), res.end());
        return BigInt(res);
    }

    static BigInt mul(const BigInt &a, const BigInt &b) {
        vector<int> res(a.digits.size()+b.digits.size(), 0);
        for (int i = a.digits.size()-1; i >= 0; i--) {
            for (int j = b.digits.size()-1; j >= 0; j--) {
                int p = (a.digits[i]-'0') * (b.digits[j]-'0');
                int pos1 = i+j, pos2 = i+j+1;
                int sum = p + res[pos2];
                res[pos1] += sum/10;
                res[pos2] = sum%10;
            }
        }
        string s=""; for (int x: res) s.push_back(char('0'+x));
        int k=0; while (k+1<s.size() && s[k]=='0') k++;
        return BigInt(s.substr(k));
    }

    friend ostream& operator<<(ostream &os, const BigInt &x) {
        os << x.digits; return os;
    }
};

BigInt convertToDecimal(const string &value, int base) {
    BigInt result("0");
    for (char c : value) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else digit = tolower(c) - 'a' + 10;
        result = BigInt::mul(result, BigInt(base));
        result = BigInt::add(result, BigInt(digit));
    }
    return result;
}

vector<BigInt> multiplyPoly(const vector<BigInt> &coeffs, BigInt root) {
    vector<BigInt> newCoeffs(coeffs.size() + 1, BigInt("0"));
    for (size_t i = 0; i < coeffs.size(); i++) {
        newCoeffs[i] = BigInt::add(newCoeffs[i], coeffs[i]);
        BigInt prod = BigInt::mul(coeffs[i], root);
        newCoeffs[i+1] = BigInt::sub(newCoeffs[i+1], prod);
    }
    return newCoeffs;
}

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    ifstream file(argv[1]);
    if (!file.is_open()) return 1;

    json input;
    file >> input;
    int n = input["keys"]["n"];
    int k = input["keys"]["k"];
    int degree = k - 1;

    vector<BigInt> roots;
    for (auto it = input.begin(); it != input.end(); ++it) {
        if (it.key() == "keys") continue;
        string baseStr = it.value()["base"];
        string value = it.value()["value"];
        int base = stoi(baseStr);
        BigInt decimal = convertToDecimal(value, base);
        roots.push_back(decimal);
    }

    vector<BigInt> coeffs; coeffs.push_back(BigInt("1"));
    for (int i = 0; i < degree && i < (int)roots.size(); i++) {
        coeffs = multiplyPoly(coeffs, roots[i]);
    }

    for (int i = 0; i < (int)coeffs.size(); i++) {
        cout << coeffs[i];
        if (i < (int)coeffs.size()-1) cout << " ";
    }
    cout << endl;
}
