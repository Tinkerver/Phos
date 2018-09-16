#pragma once

#ifndef POLY_H
#define POLY_H
#define _PNI_R polynomial_N_index&
#include <map>
#include <string>
#include <iostream>
#include <memory>
#include <initializer_list>
namespace _201700800404
{
	class polynomial_N_index final
	{
		friend	polynomial_N_index operator+(const _PNI_R, const _PNI_R);
		friend	polynomial_N_index operator-(const _PNI_R, const _PNI_R);
		friend  polynomial_N_index operator*(const _PNI_R, const _PNI_R);
	public://forward declaration
		using allocator_type = std::allocator<char>;
	public:
		polynomial_N_index();

		//construct a polynomial by a list
		polynomial_N_index(std::initializer_list<std::pair<std::string, std::size_t>>);

		//move constructor
		polynomial_N_index(polynomial_N_index&&);
		polynomial_N_index(const polynomial_N_index &);

		//move assignment
		_PNI_R operator=(polynomial_N_index&&);

		//cpoy assignment
		_PNI_R operator=(const _PNI_R);
		_PNI_R operator=(std::initializer_list<std::pair<std::string, std::size_t>>);

		//trivial dtor
		~polynomial_N_index();
	public:
		std::size_t terms() const;

		//return string representing *this. Just like to_String in Java
		std::string format() const;

		bool empty() const;

		//for some index, return the coefficient. return string{} if the index doesn't exist 
		std::string coefficient_at(const std::size_t&);

		// append n terms (override every term that exists) 
		_PNI_R append(std::initializer_list<std::pair<std::string, std::size_t>>);
		_PNI_R append(const std::string&, const std::size_t&);
		_PNI_R append(const std::size_t&, const std::size_t&); //传递整系数

		// remove n terms (ignore the terms that don't occur)
		void remove(std::initializer_list<std::size_t>);
		bool remove(const std::size_t&); //remove appointed term. Return true iif the term exists.

		_PNI_R operator+=(const _PNI_R);
		_PNI_R operator+=(std::initializer_list<std::pair<std::string, std::size_t>>);
		_PNI_R operator-=(const _PNI_R);
		_PNI_R operator-=(std::initializer_list<std::pair<std::string, std::size_t>>);
		_PNI_R operator*=(const _PNI_R);
		_PNI_R operator*=(std::initializer_list<std::pair<std::string, std::size_t>>);

		//return f(x)
		std::string value_at(const std::string&);

		//return f(x)^(n)
		polynomial_N_index derivative(std::size_t=1);

		//return definite integral
		std::string integral_between(std::string lower, std::string upper);

		//integrate. We can define what C is.
		polynomial_N_index integral_with_constant(std::string);
		polynomial_N_index integral_with_constant(const std::size_t&);

	private:
		void assert_legal_and_make_deno(std::string&);
		std::string string_left(const std::string&) const; //get numerator
		std::string string_right(const std::string&) const; //get denominator
		std::string simplification(const std::string&);

		std::string add(const std::string&, const std::string&); 
		std::string minus(const std::string&, const std::string&);
		std::string mult(const std::string&, const std::string&);
		std::string div(const std::string&, const std::string&);

		std::string gcd(const std::string&, const std::string&);
		std::string value_at_internal(const std::string&); 
		std::string powInt(const std::string&, const std::size_t&); 
		void delete_tail(std::string&);
	private:
		std::map<std::size_t, std::string> inner_terms;  //the only data member
	};

	std::ostream& operator<<(std::ostream &os, const polynomial_N_index &obj);
}

#endif