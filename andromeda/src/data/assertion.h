#pragma once
#define ANDROMEDA_ASSERT(cond, message) if (!###cond) throw "ERROR: message";

template<class T, class B> struct Derived_from {
	static void constraints(T* p) { B* pb = p; }
	Derived_from() { void(*p)(T*) = constraints; }
};

template<class T1, class T2> struct Can_copy {
	static void constraints(T1 a, T2 b) { T2 c = a; b = a; }
	Can_copy() { void(*p)(T1, T2) = constraints; }
};

template<class T1, class T2 = T1> struct Can_compare {
	static void constraints(T1 a, T2 b) { a == b; a != b; a < b; }
	Can_compare() { void(*p)(T1, T2) = constraints; }
};

template<class T1> struct Has_AritmeticOps {
	static void constraints(T1 a) { T1 c = (a * a + a)/a-a; }
	Has_AritmeticOps() { void(*p)(T1) = constraints; }
};
