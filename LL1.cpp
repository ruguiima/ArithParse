#include <bits/stdc++.h>
using namespace std;

static string stack_to_string(const vector<char>& st) {
	string s;
	for (char c : st) s.push_back(c);
	return s;
}

static string input_to_string(const deque<char>& inq) {
	string s;
	for (char c : inq) s.push_back(c);
	return s;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	string input;
	if (!getline(cin, input)) {
		// 无输入也按空串处理
		input = "";
	}

	// 规范化：剔除空白，仅保留字符 n 与 $（其余视为错误字符但仍入队，便于显示与报错）
	deque<char> inq;
	for (char ch : input) {
		if (isspace(static_cast<unsigned char>(ch))) continue;
		inq.push_back(ch);
	}
	// 若末尾没有 '$'，自动补上
	if (inq.empty() || inq.back() != '$') {
		inq.push_back('$');
	}

	// 分析栈：栈底在最左、栈顶在最右
	vector<char> st;
	st.push_back('$');
	st.push_back('E');

	auto print_step = [&](const string& action) {
		cout << stack_to_string(st) << "\t" << input_to_string(inq) << "\t" << action << "\n";
	};

	// 预测分析主循环
	while (true) {
		if (st.empty()) {
			// 栈空，若输入还未到$，则错误，否则接受
			if (!inq.empty() && inq.front() == '$') {
				print_step("accept");
			} else {
				print_step("error");
			}
			break;
		}

		char X = st.back();
		char a = inq.empty() ? '\0' : inq.front();

		// 接受条件：栈顶为'$'且输入为'$'
		if (X == '$' && a == '$') {
			print_step("accept");
			break;
		}

		// 终结符匹配
		if (X == 'n' || X == '+' || X == '-' || X == '*' 
			|| X == '/' || X == '(' || X == ')' || X == '$') {
			if (X == a) {
				// match: 弹栈并读入一个符号
				print_step("match");
				st.pop_back();
				inq.pop_front();
				continue;
			} else {
				// 终结符不匹配
				print_step("error");
				break;
			}
		}

		// 非终结符：
		if (X == 'E') {
			int prod = 0;
			if (a == 'n' || a == '(') prod = 1;
			else prod = 0; // 其他符号均为 error

			if (prod == 0) {
				print_step("error");
				break;
			}

			if (prod == 1) {
				// 输出产生式编号
				print_step("1");
				st.pop_back();
				st.push_back('A');
				st.push_back('T');
				continue;
			}
		} else if (X == 'A') {
			int prod = 0;
			if (a == '+') prod = 2;
			else if (a == '-') prod = 3;
			else if (a == ')' || a == '$') prod = 4;
			else prod = 0;

			if (prod == 0) {
				print_step("error");
				break;
			}

			if (prod == 2) {
				print_step("2");
				st.pop_back();
				st.push_back('A');
				st.push_back('T');
				st.push_back('+');
				continue;
			} else if (prod == 3) {
				print_step("3");
				st.pop_back();
				st.push_back('A');
				st.push_back('T');
				st.push_back('-');
				continue;
			} else if (prod == 4) {
				print_step("4");
				st.pop_back();
				// ε产生式，栈不变
				continue;
			}
		} else if (X == 'B') {
			int prod = 0;
			if (a == '*') prod = 6;
			else if (a == '/') prod = 7;
			else if (a == ')' || a == '$' 
					|| a == '+' || a == '-') prod = 8;
			else prod = 0;

			if (prod == 0) {
				print_step("error");
				break;
			}

			if (prod == 6) {
				print_step("6");
				st.pop_back();
				st.push_back('B');
				st.push_back('F');
				st.push_back('*');
				continue;
			} else if (prod == 7) {
				print_step("7");
				st.pop_back();
				st.push_back('B');
				st.push_back('F');
				st.push_back('/');
				continue;
			} else if (prod == 8) {
				print_step("8");
				st.pop_back();
				continue;
			}
		} else if (X == 'T') {
			int prod = 0;
			if (a == 'n' || a == '(') prod = 5;
			else prod = 0; // 其他符号均为 error

			if (prod == 0) {
				print_step("error");
				break;
			}

			if (prod == 5) {
				// 输出产生式编号
				print_step("5");
				st.pop_back();
				st.push_back('B');
				st.push_back('F');
				continue;
			}
		} else if (X == 'F') {
			int prod = 0;
			if (a == '(') prod = 9;
			else if (a == 'n') prod = 10;
			else prod = 0;

			if (prod == 0) {
				print_step("error");
				break;
			}

			if (prod == 9) {
				print_step("9");
				st.pop_back();
				st.push_back(')');
				st.push_back('E');
				st.push_back('(');
				continue;
			} else if (prod == 10) {
				print_step("10");
				st.pop_back();
				st.push_back('n');
				continue;
			}
		}


		// 其他未知栈符号
		print_step("error");
		break;
	}

	return 0;
}

