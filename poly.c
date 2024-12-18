#include "poly.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

poly* poly_null() {
	poly* p = malloc(sizeof(*p));
	if (p == nullptr) {
		fprintf(stderr, "Error: malloc failed\n");
		return nullptr;
	}
	p->head = nullptr;
	return p;
}

void poly_dispose(poly** pp) {
	if (*pp == nullptr) {
		return;
	}
	polycell* p = (*pp)->head;
	while (p != nullptr) {
		polycell* q = p;
		p = p->next;
		free(q);
	}
	*pp = nullptr;
}

bool poly_is_null(const poly* p) {
	return p->head == nullptr;
}

int poly_degree(const poly* p) {
	if (poly_is_null(p)) {
		return -1;
	}
	return p->tail->degree;
}

bool poly_is_constant(const poly* p) {
	if (poly_is_null(p)) {
		return true;
	}
	return p->tail->degree == 0;
}

double poly_eval(const poly* p, double x) {
	double r = 0;
	polycell* q = p->head;
	while (q != nullptr) {
		double t = 1;
		for (int i = 0; i < q->degree; ++i) {
			t *= x;
		}
		r += t * q->coef;
		q = q->next;
	}
	return r;
}

void poly_self_mul(poly** pp, double a) {
	polycell* q = (*pp)->head;
	polycell* prev = nullptr;
	while (q != nullptr) {
		q->coef *= a;
		if (q->coef == 0) {
			if (prev == nullptr) {
				(*pp)->head = q->next;
			}
			else {
				prev->next = q->next;
			}
			free(q);
			q = prev->next;
		}
		else {
			prev = q;
			q = q->next;
		}
	}
	(*pp)->tail = prev;
}

void pole_self_derivate(poly** pp) {
	polycell* q = (*pp)->head;
	polycell* prev = nullptr;
	while (q != nullptr) {
		q->coef *= (q->degree - 1);
		--q->degree;
		if (q->coef == 0 || q->degree < 0) {
			if (prev == nullptr) {
				(*pp)->head = q->next;
			}
			else {
				prev->next = q->next;
			}
			free(q);
			if (prev == nullptr) {
				q = (*pp)->head;
			}
			else {
				q = prev->next;
			}
		}
		else {
			prev = q;
			q = q->next;
		}
	}
	(*pp)->tail = prev;
}

poly* poly_sum(const poly* p1, const poly* p2) {
	poly* p = poly_null();
	if (p == nullptr) {
		return nullptr;
	}
	polycell* prev = nullptr;
	polycell* q1 = p1->head;
	polycell* q2 = p2->head;
	while (q1 != nullptr || q2 != nullptr) {
		if (q1 != nullptr) {
			if (q2 != nullptr) {
				if (q1->degree < q2->degree) {
					polycell* q = malloc(sizeof(*q));
					if (q == nullptr) {
						poly_dispose(&p);
						return nullptr;
					}
					q->degree = q1->degree;
					q->coef = q1->coef;
					q->next = nullptr;
					if (prev == nullptr) {
						p->head = q;
					}
					else {
						prev->next = q;
					}
					q1 = q1->next;
					prev = q;
				}
				else if (q1->degree > q2->degree) {
					polycell* q = malloc(sizeof(*q));
					if (q == nullptr) {
						poly_dispose(&p);
						return nullptr;
					}
					q->degree = q2->degree;
					q->coef = q2->coef;
					q->next = nullptr;
					if (prev == nullptr) {
						p->head = q;
					}
					else {
						prev->next = q;
					}
					q2 = q2->next;
					prev = q;
				}
				else {
					polycell* q = malloc(sizeof(*q));
					if (q == nullptr) {
						poly_dispose(&p);
						return nullptr;
					}
					q->degree = q1->degree;
					q->coef = q1->coef + q2->coef;
					q->next = nullptr;
					if (q->coef == 0) {
						free(q);
					}
					else {
						if (prev == nullptr) {
							p->head = q;
						}
						else {
							prev->next = q;
						}
						prev = q;
					}
					q1 = q1->next;
					q2 = q2->next;
				}
			}
			else {
				polycell* q = malloc(sizeof(*q));
				if (q == nullptr) {
					poly_dispose(&p);
					return nullptr;
				}
				q->degree = q1->degree;
				q->coef = q1->coef;
				q->next = nullptr;
				if (prev == nullptr) {
					p->head = q;
				}
				else {
					prev->next = q;
				}
				q1 = q1->next;
				prev = q;
			}
		}
		else if (q2 != nullptr) {
			polycell* q = malloc(sizeof(*q));
			if (q == nullptr) {
				poly_dispose(&p);
				return nullptr;
			}
			q->degree = q2->degree;
			q->coef = q2->coef;
			q->next = nullptr;
			if (prev == nullptr) {
				p->head = q;
			}
			else {
				prev->next = q;
			}
			q2 = q2->next;
			prev = q;
		}
	}
	p->tail = prev;
	return p;
}