#include <bits/stdc++.h>
using namespace std;

enum class ActType { SHIFT, REDUCE, ACCEPT, ERROR_ };
struct Action { ActType type; int val; }; // SHIFT: next state; REDUCE: prod no.

// 产生式信息：编号 -> (lhs, rhsLen)
static pair<char,int> production(int p) {
    switch (p) {
        case 1: case 2: return {'E', 3};
        case 3:         return {'E', 1};
        case 4: case 5: return {'T', 3};
        case 6:         return {'T', 1};
        case 7:         return {'F', 3};
        case 8:         return {'F', 1};
        default: return {'?', 0};
    }
}

// Action 查表：state × terminal -> 动作
static Action getAction(int state, char a) {
    switch (state) {
        case 0: // I0
        case 4: // I4
        case 6: // I6
        case 7: // I7
        case 8: // I8
        case 9: // I9
            if (a == '(') return {ActType::SHIFT, 4};
            else if (a == 'n') return {ActType::SHIFT, 5};
            return {ActType::ERROR_, -1};
        case 1: // I1
            if (a == '+') return {ActType::SHIFT, 6};
            else if (a == '-') return {ActType::SHIFT, 7};
            else if (a == '$') return {ActType::ACCEPT, 0};
            return {ActType::ERROR_, -1};
        case 2: // I2
            if (a == '*') return {ActType::SHIFT, 8};
            else if (a == '/') return {ActType::SHIFT, 9};
            else if (a=='$'||a ==')'||a=='+'||a=='-') return {ActType::REDUCE, 3};
            return {ActType::ERROR_, -1};
        case 3: // I3
            if (a=='+'||a=='-'||a=='*'||a=='/'||a==')'||a=='$') 
                return {ActType::REDUCE, 6};
            return {ActType::ERROR_, -1};
        case 5: // I5
            if (a=='+'||a=='-'||a=='*'||a=='/'||a==')'||a=='$')
                return {ActType::REDUCE, 8};
            return {ActType::ERROR_, -1};
        case 10: // I10
            if (a == '+') return {ActType::SHIFT, 6};
            else if (a == '-') return {ActType::SHIFT, 7};
            else if (a == '$' || a == ')') return {ActType::SHIFT, 15};
            return {ActType::ERROR_, -1};
        case 11: // I11
            if (a == '*') return {ActType::SHIFT, 8};
            else if (a == '/') return {ActType::SHIFT, 9};
            else if (a=='+'||a=='-'||a=='$'||a ==')') return {ActType::REDUCE, 1};
            return {ActType::ERROR_, -1};
        case 12: // I12
            if (a == '*') return {ActType::SHIFT, 8};
            else if (a == '/') return {ActType::SHIFT, 9};
            else if (a=='+'||a=='-'||a=='$'||a ==')') return {ActType::REDUCE, 2};
            return {ActType::ERROR_, -1};
        case 13: // I13
            if (a=='+'||a=='-'||a=='*'||a=='/'||a==')'||a=='$') 
                return {ActType::REDUCE, 4};
            return {ActType::ERROR_, -1};
        case 14: // I14
            if (a=='+'||a=='-'||a=='*'||a=='/'||a==')'||a=='$') 
                return {ActType::REDUCE, 5};
            return {ActType::ERROR_, -1};
        case 15: // I15
            if (a=='+'||a=='-'||a=='*'||a=='/'||a==')'||a=='$') 
                return {ActType::REDUCE, 7};
            return {ActType::ERROR_, -1};
        default:
            return {ActType::ERROR_, -1};
    }
}

// Goto 查表：state × NonTerminal -> nextState
static int getGoto(int state, char A) {
    switch (state) {
        case 0: 
            if (A == 'E') return 1;
            else if (A == 'T') return 2;
            else if (A == 'F') return 3;
            return -1;
        case 4:
            if (A == 'E') return 10;
            else if (A == 'T') return 2;
            else if (A == 'F') return 3;
            return -1;
        case 6:
            if (A == 'T') return 11;
            else if (A == 'F') return 3;
            return -1;
        case 7:
            if (A == 'T') return 12;
            else if (A == 'F') return 3;
            return -1; 
        case 8:
            if (A == 'F') return 13;
            return -1;
        case 9:
            if (A == 'F') return 14;
        default: return -1;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string input;
    if (!getline(cin, input)) input = "";

    // 规范化：去空白，只保留字符
    deque<char> inq;
    for (char ch : input) {
        if (isspace(static_cast<unsigned char>(ch))) continue;
        inq.push_back(ch);
    }
    // 末尾补 '$'
    if (inq.empty() || inq.back() != '$') inq.push_back('$');

    // LR 状态栈（仅存状态号），初始状态 0
    vector<int> st;
    st.push_back(0);

    while (true) {
        int s = st.back();
        char a = inq.empty() ? '\0' : inq.front();

        Action act = getAction(s, a);
        if (act.type == ActType::SHIFT) {
            cout << "shift\n";
            st.push_back(act.val); // 移进到新状态
            if (!inq.empty()) inq.pop_front(); // 消耗一个输入符号
        } else if (act.type == ActType::REDUCE) {
            // 输出产生式编号
            cout << act.val << "\n";
            auto [lhs, rhsLen] = production(act.val);
            if (lhs == '?' || (int)st.size() < rhsLen + 1) { // 栈不足，错误保护
                cout << "error\n";
                break;
            }
            // 弹出 |rhs| 个状态
            for (int i = 0; i < rhsLen; ++i) st.pop_back();
            int t = st.back();
            int g = getGoto(t, lhs);
            if (g < 0) {
                cout << "error\n";
                break;
            }
            st.push_back(g);
        } else if (act.type == ActType::ACCEPT) {
            cout << "accept\n";
            break;
        } else {
            cout << "error\n";
            break;
        }
    }

    return 0;
}
