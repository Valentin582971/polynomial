#include <stdbool.h>

typedef struct polycell polycell;
typedef struct poly poly;
typedef struct polycell polycell;

struct polycell {
	int degree;
	double coef;
	polycell* next;
};

struct poly {
	polycell* head;
	polycell* tail;
};

// Frees the memory allocated for a polynomial and sets the pointer to NULL.
void poly_dispose(poly** pp);

// Creates and returns a new null (zero) polynomial.
poly* poly_null();

// Checks if a given polynomial is null (zero). Returns true if it is, otherwise false.
bool poly_is_null(const poly* p);

// Returns the degree of the polynomial. If the polynomial is null, the degree is typically defined as -1.
int poly_degree(const poly* p);

// Checks if the polynomial is constant (degree 0 or null). Returns true if constant, otherwise false.
bool poly_is_constant(const poly* p);

// Evaluates the polynomial at a given value of x and returns the result.
double poly_eval(const poly* p, double x);

// Multiplies all coefficients of the polynomial by a scalar a, modifying the polynomial in place.
void poly_self_mul(poly** pp, double a);

// Computes the derivative of the polynomial in place, modifying the current polynomial.
void pole_self_derivate(poly** pp);

// Adds two polynomials and returns a new polynomial representing their sum.
poly* poly_sum(const poly* p1, const poly* p2);
