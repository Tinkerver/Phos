#include "poly.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <vector>
#include <initializer_list>
#include <string>
#include <numeric>
#include <cmath>
#include <stack>
#define _SCOPE_ polynomial_N_index::

using namespace std;
using namespace _201700800404;

void _SCOPE_ assert_legal_and_make_deno(string& str)
{
	if (find(str.begin(), str.end(), '/') == str.end())
		str.append("/1");
	try
	{
		size_t pos, *p = &pos;
		stoi(str, p); /* 
						make p points to the first char that doesn't make sense for an int.
						If the first non-space char is not digit or minus, throw exception directly.                                               
				      */
		if (str[pos] == '/')
		{
			++pos;
			string deno = str.substr(pos);
			if (stoi(deno, p) > 0 && pos == deno.size()) //deno must be greater than 0, and there are no other char remaining.
				return;
		}
		throw invalid_argument({});
	}
	catch (invalid_argument)
	{
		cerr << "invalid format of string" << endl;
		exit(1700800404);
	}
}

string _SCOPE_ string_left(const string &s) const
{//return numerator including possible minus
	return string(s.begin(), find(s.begin(), s.end(), '/'));
}

string _SCOPE_ string_right(const string &s) const
{//return denominator
	return string(++find(s.begin(), s.end(), '/'), s.end());
}


inline string _SCOPE_ gcd(const string& lhs, const string& rhs)
{
	return to_string(std::gcd(abs(stoi(lhs)), abs(stoi(rhs))));
}


string _SCOPE_ simplification(const string& s)
{
	const string left = string_left(s), right = string_right(s);
	const string gcd = _SCOPE_ gcd(left, right);
	const int gcdINT = stoi(gcd);
	if (gcdINT == 1)
		return s;
	return to_string(stoi(left) / gcdINT) + "/" + to_string(stoi(right) / gcdINT);
}

string _SCOPE_ add(const string& lhs, const string &rhs)
{
	if (lhs.empty() && !rhs.empty())return rhs;
	if (!lhs.empty() && rhs.empty())return lhs;
	if (lhs.empty() && rhs.empty())return string("0/1");

	int LhsLeftInt = stoi(string_left(lhs)), LhsRightInt = stoi((string_right(lhs)));
	int RhsLeftInt = stoi(string_left(rhs)), RhsRightInt = stoi((string_right(rhs)));
	string Raw = to_string(LhsLeftInt*RhsRightInt + LhsRightInt * RhsLeftInt) + '/' +
		to_string(LhsRightInt*RhsRightInt);
	return simplification(Raw);
}

string _SCOPE_ minus(const string& lhs, const string &rhs)
{
	if (lhs.empty() && !rhs.empty())return mult(rhs, string("-1/1"));
	if (!lhs.empty() && rhs.empty())return lhs;
	if (lhs.empty() && rhs.empty())return string("0/1");
	int LhsLeftInt = stoi(string_left(lhs)), LhsRightInt = stoi((string_right(lhs)));
	int RhsLeftInt = stoi(string_left(rhs)), RhsRightInt = stoi((string_right(rhs)));
	string Raw = to_string(LhsLeftInt*RhsRightInt - LhsRightInt * RhsLeftInt) + '/' +
		to_string(LhsRightInt*RhsRightInt);
	return simplification(Raw);
}

string _SCOPE_ mult(const string& lhs, const string &rhs)
{
	int LhsLeftInt = stoi(string_left(lhs)), LhsRightInt = stoi((string_right(lhs)));
	int RhsLeftInt = stoi(string_left(rhs)), RhsRightInt = stoi((string_right(rhs)));
	string Raw = to_string(LhsLeftInt*RhsLeftInt) + '/' +
		to_string(LhsRightInt*RhsRightInt);
	return simplification(Raw);
}

string _SCOPE_ div(const string& lhs, const string& rhs)
{
	try //if deno=0
	{
		int RhsLeftInt = stoi(string_left(rhs));
		if (RhsLeftInt == 0)
			throw domain_error("unexpected divisor 0");
		int LhsLeftInt = stoi(string_left(lhs)), LhsRightInt = stoi((string_right(lhs)));
		int RhsRightInt = stoi((string_right(rhs)));

		string Raw = to_string(LhsLeftInt*RhsRightInt) + '/' +
			to_string(LhsRightInt*RhsLeftInt);
		return simplification(Raw);
	}
	catch (domain_error &msg)
	{
		cerr << msg.what() << endl;
		exit(1700800404);
	}
}

_SCOPE_ polynomial_N_index() = default;

_SCOPE_ polynomial_N_index(initializer_list<pair<string, size_t>> il)
{
	//记得开始除掉系数0 erase
	for (pair<string, size_t> obj : il)
	{
		assert_legal_and_make_deno(obj.first);
		obj.first = simplification(obj.first);
		if (inner_terms.find(obj.second) != inner_terms.end())
			inner_terms.erase(obj.second);

		if (obj.first != "0/1")
			inner_terms[obj.second] = obj.first;
	}
}

_SCOPE_ polynomial_N_index(const polynomial_N_index &rhs) :inner_terms{ rhs.inner_terms }
{
}

_SCOPE_ polynomial_N_index(polynomial_N_index&& rhs) :inner_terms{ move(rhs.inner_terms) }
{
}

_PNI_R _SCOPE_ operator=(const _PNI_R rhs)
{
	inner_terms = rhs.inner_terms;
	return *this;
}

_PNI_R _SCOPE_ operator=(initializer_list<pair<string, size_t>> il)
{
	this->inner_terms.clear();
	for (auto obj : il)
	{
		assert_legal_and_make_deno(obj.first);
		obj.first = simplification(obj.first);
		if (inner_terms.find(obj.second) != inner_terms.end())
			inner_terms.erase(obj.second);

		if (obj.first != "0/1")
			inner_terms[obj.second] = obj.first;
	}
	return *this;
}

_PNI_R _SCOPE_ operator=(polynomial_N_index&& rhs)
{
	if (this != &rhs)
		inner_terms = move(rhs.inner_terms);
	return *this;
}

_SCOPE_ ~polynomial_N_index(){}

inline size_t _SCOPE_ terms() const
{
	return inner_terms.size();
}

inline bool _SCOPE_ empty() const
{
	return inner_terms.empty();
}

string _SCOPE_ format() const
{

	string terms, term;
	auto b = inner_terms.cbegin();
	if (b == inner_terms.cend())
		return string("0");

	if (b->second == "1/1")
	{
		if (b->first == 0)
			term = "1";
		else if (b->first == 1)
			term = "x";
		else
			term = "x^" + to_string(b->first);
	}
	else if (b->second == "-1/1")
	{
		if (b->first == 0)
			term = "-1";
		else if (b->first == 1)
			term = "-x";
		else
			term = "-x^" + to_string(b->first);
	}
	else if (string_right(b->second) == "1")
	{
		term = string_left(b->second);
		if (b->first == 1)
			term.push_back('x');
		else if (b->first > 1)
			term.append("x^" + to_string(b->first));
	}
	else if (b->first == 0)
		term = b->second;
	terms.append(term);

	string prefix;
	for (++b;b != inner_terms.cend();++b)
	{
		bool is_positive;
		prefix = (b->second[0] == '-' ? 
			(is_positive = false, " - ") : (is_positive = true, " + "));

		if (b->second == "1/1"||b->second=="-1/1")
		{
			if (b->first == 0)
				term = "1";
			else if (b->first == 1)
				term = "x";
			else
				term = "x^" + to_string(b->first);
		}
		else if (string_right(b->second) == "1")
		{
			term = (is_positive ? string_left(b->second) : string_left(b->second).substr(1));
			if (b->first == 1)
				term.push_back('x');
			else if (b->first > 1)
				term.append("x^" + to_string(b->first));
		}
		else
			term = "(" + (is_positive ? b->second : b->second.substr(1)) 
				+ ")x^" + to_string(b->first);
		terms.append(prefix + term);
	}
	return terms;
}

string _SCOPE_ coefficient_at(const size_t &cu)
{
	string coefficient;
	if (inner_terms.find(cu) == inner_terms.end())
		return coefficient;
	string ret = inner_terms[cu];
	delete_tail(ret);
	return ret;
}

_PNI_R _SCOPE_ append(initializer_list<pair<string, size_t>> il)
{
	for (auto obj : il)
	{
		assert_legal_and_make_deno(obj.first);
		obj.first = simplification(obj.first);

		if (inner_terms.find(obj.second) == inner_terms.end())
			inner_terms.erase(obj.second);
		inner_terms[obj.second] = obj.first;
	}
	return *this;
}

_PNI_R _SCOPE_ append(const string &s, const size_t &u)
{
	string str(s);
	assert_legal_and_make_deno(str);
	if (inner_terms.find(u) != inner_terms.end())
		inner_terms.erase(u);
	inner_terms[u] = simplification(str);
	return *this;
}

_PNI_R _SCOPE_ append(const size_t &lhs, const size_t &rhs)
{
	append(to_string(lhs), rhs);
	return *this;
}

void _SCOPE_ remove(initializer_list<size_t> il)
{
	for (auto obj : il)
	{
		if (inner_terms.find(obj) != inner_terms.end())
			inner_terms.erase(obj);
	}
}

bool _SCOPE_ remove(const size_t &u)
{
	if (inner_terms.find(u) != inner_terms.end())
	{
		inner_terms.erase(u);
		return true;
	}
	return false;
}

_PNI_R _SCOPE_ operator+=(const polynomial_N_index &rhs)
{
	for (const auto &t : rhs.inner_terms)
		inner_terms[t.first] = add(inner_terms[t.first], t.second);
	return *this;
}

_PNI_R _SCOPE_ operator+= (initializer_list<pair<string, size_t>> il)
{
	for (auto obj : il)
	{
		assert_legal_and_make_deno(obj.first);
		obj.first = simplification(obj.first);

		inner_terms[obj.second] = add(inner_terms[obj.second], obj.first);
	}
	return *this;
}

_PNI_R _SCOPE_ operator-=(const polynomial_N_index &rhs)
{
	for (const auto &t : rhs.inner_terms)
		inner_terms[t.first] = minus(inner_terms[t.first], t.second);
	return *this;
}

_PNI_R _SCOPE_ operator-=(initializer_list<pair<string, size_t>> il)
{
	for (auto obj : il)
	{
		assert_legal_and_make_deno(obj.first);
		obj.first = simplification(obj.first);

		inner_terms[obj.second] = minus(inner_terms[obj.second], obj.first);
	}
	return *this;
}

_PNI_R _SCOPE_ operator*=(const polynomial_N_index &obj)
{
	vector<polynomial_N_index> vec;
	for (const auto &thisT : inner_terms)
	{
		polynomial_N_index pni;
		for (const auto &t : obj.inner_terms)
			pni.append({ { mult(thisT.second,t.second),thisT.first + t.first } });
		vec.push_back(std::move(pni));
	}

	polynomial_N_index rec;
	for (const auto& p : vec)
		rec += p;
	*this = rec;
	return *this;
}

_PNI_R _SCOPE_ operator*=(initializer_list<pair<string, size_t>> il)
{
	vector<polynomial_N_index> vec;
	for (const auto &thisT : inner_terms)
	{
		polynomial_N_index pni;
		for (auto t : il)
		{
			assert_legal_and_make_deno(t.first);
			t.first = simplification(t.first);

			pni.append({ { mult(thisT.second,t.first),thisT.first + t.second } });
		}
		vec.push_back(std::move(pni));
		pni = {};
	}

	polynomial_N_index rec;
	for (const auto& p : vec)
		rec += p;
	*this = rec;
	return *this;
}

string _SCOPE_ powInt(const string &base, const size_t &index)
{
	int numerator = static_cast<int>(pow(stoi(string_left(base)), index));
	int denominator = static_cast<int>(pow(stoi(string_right(base)), index));
	string Raw = to_string(numerator) + '/' + to_string(denominator);
	return simplification(Raw);
}

string _SCOPE_ value_at_internal(const string &s)
{

	string cast_s = s;
	assert_legal_and_make_deno(cast_s);
	const string sim_s = simplification(cast_s);

	string ret;
	for (const auto& a : inner_terms)
		ret = add(ret, mult(a.second, powInt(sim_s, a.first)));

	return ret;
}

void _SCOPE_ delete_tail(string &s)
{
	if (s.find('/') == s.size() - 2 && s.back() == '1')
		s.erase(s.size() - 2);
}

string _SCOPE_ value_at(const string &s)
{
	string ret = value_at_internal(s);
	delete_tail(ret);
	return ret;
}

polynomial_N_index _SCOPE_ derivative(size_t degrees)
{
	if (degrees == 0)
		return polynomial_N_index(*this);
	stack<polynomial_N_index> st;
	st.push(*this);
	while (degrees--)
	{
		polynomial_N_index raw;
		if (st.top().inner_terms.empty())
			return polynomial_N_index({ {"0",0} });
		for (const auto &a : st.top().inner_terms)
		{
			if (a.first == 0)
				continue;
			size_t temp = a.first;
			string temp_s = to_string(temp);
			assert_legal_and_make_deno(temp_s);

			raw.inner_terms[a.first - 1] = mult(a.second, temp_s);
		}
		st.push(raw);
	}
	return st.top();
}

string _SCOPE_ integral_between(string lower, string upper)
{
	polynomial_N_index prim;
	for (const auto& a : inner_terms)
	{
		size_t temp = a.first + 1;
		string temp_s = to_string(temp);
		assert_legal_and_make_deno(temp_s);
		prim.inner_terms[temp] = div(a.second, temp_s);
	}
	assert_legal_and_make_deno(lower);
	assert_legal_and_make_deno(upper);
	return minus(prim.value_at_internal(upper), prim.value_at_internal(lower));
}

polynomial_N_index _SCOPE_ integral_with_constant(string c)
{
	polynomial_N_index ret;
	assert_legal_and_make_deno(c);

	for (const auto &a : inner_terms)
	{
		string aux = to_string(a.first + 1);
		assert_legal_and_make_deno(aux);
		ret.inner_terms[a.first + 1] = div(a.second, aux);
	}
	ret.inner_terms[0] = simplification(c);
	return ret;
}

inline polynomial_N_index _SCOPE_ integral_with_constant(const size_t& u)
{
	return integral_with_constant(to_string(u));
}


polynomial_N_index _201700800404::operator+(const _PNI_R lhs, const _PNI_R rhs)
{
	polynomial_N_index ret = lhs;
	ret += rhs;
	return ret;
}

polynomial_N_index _201700800404::operator-(const _PNI_R lhs, const _PNI_R rhs)
{
	polynomial_N_index ret = lhs;
	ret -= rhs;
	return ret;
}

polynomial_N_index _201700800404::operator*(const _PNI_R lhs, const _PNI_R rhs)
{
	polynomial_N_index ret = lhs;
	ret *= rhs;
	return ret;
}

ostream& _201700800404::operator<<(std::ostream &os, const polynomial_N_index &obj)
{
	os << obj.format();
	return os;
}