#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "poly.h"

// Utility function: Adds a term to a polynomial in increasing order of degree
poly* poly_create(poly* existing_poly, int degree, double coef) {
    if (coef == 0) {
        return existing_poly; // Skip adding zero-coefficient terms
    }

    if (existing_poly == nullptr) {
        existing_poly = malloc(sizeof(poly));
        if (existing_poly == nullptr) {
            fprintf(stderr, "Failed to allocate memory for polynomial.\n");
            return nullptr;
        }
        existing_poly->head = existing_poly->tail = nullptr;
    }

    polycell* new_cell = malloc(sizeof(*new_cell));
    if (new_cell == nullptr) {
        fprintf(stderr, "Failed to allocate memory for polynomial cell.\n");
        return nullptr;
    }

    new_cell->degree = degree;
    new_cell->coef = coef;
    new_cell->next = nullptr;

    // Case 1: If the polynomial is empty, insert the new term as the first term
    if (existing_poly->head == nullptr) {
        existing_poly->head = existing_poly->tail = new_cell;
        return existing_poly;
    }

    // Case 2: If the new term's degree is smaller than the head, insert at the beginning
    if (degree < existing_poly->head->degree) {
        new_cell->next = existing_poly->head;
        existing_poly->head = new_cell;
        return existing_poly;
    }

    // Case 3: Traverse to find the correct position for the new term
    polycell* prev = nullptr;
    polycell* current = existing_poly->head;

    while (current != nullptr && current->degree < degree) {
        prev = current;
        current = current->next;
    }

    // Check if a term with the same degree already exists
    if (current != nullptr && current->degree == degree) {
        current->coef += coef; // Add coefficients for the same degree
        if (current->coef == 0) {
            // If coefficient becomes zero, remove the term
            if (prev != nullptr) {
                prev->next = current->next;
            }
            else {
                existing_poly->head = current->next;
            }
            if (current == existing_poly->tail) {
                existing_poly->tail = prev;
            }
            free(current);
        }
        free(new_cell); // Free the newly created cell since it's not needed
        return existing_poly;
    }

    // Insert the new term at the correct position
    new_cell->next = current;
    if (prev != nullptr) {
        prev->next = new_cell;
    }

    // Update the tail if the new term is inserted at the end
    if (current == nullptr) {
        existing_poly->tail = new_cell;
    }

    return existing_poly;
}


// Utility function: Prints a polynomial
void poly_print(FILE* file, const poly* p) {
    if (poly_is_null(p)) {
        fprintf(file, "0\n");
        return;
    }

    polycell* current = p->head;
    while (current != nullptr) {
        fprintf(file, "%.2fx^%d ", current->coef, current->degree);
        if (current->next != nullptr) {
            fprintf(file, "+ ");
        }
        current = current->next;
    }
    fprintf(file, "\n");
}

// Main testing function
int main() {
    // Test poly_null and poly_is_null
    poly* p_null = poly_null();
    fprintf(stdout, "Testing poly_null and poly_is_null:\n");
    poly_print(stdout, p_null);
    fprintf(stdout, "Is null: %s\n\n", poly_is_null(p_null) ? "true" : "false");

    // Test creating a polynomial with multiple terms
    poly* p1 = nullptr;
    p1 = poly_create(p1, 2, 3.0); // 3x^2
    p1 = poly_create(p1, 1, -4.0); // -4x
    p1 = poly_create(p1, 0, 5.0); // +5
    fprintf(stdout, "Testing poly_create with multiple terms:\n");
    poly_print(stdout, p1);
    fprintf(stdout, "\n");

    // Test poly_degree
    fprintf(stdout, "Testing poly_degree:\n");
    fprintf(stdout, "Degree: %d\n\n", poly_degree(p1));

    // Test poly_is_constant
    poly* p_constant = nullptr;
    p_constant = poly_create(p_constant, 0, 5.0); // 5
    fprintf(stdout, "Testing poly_is_constant:\n");
    poly_print(stdout, p_constant);
    fprintf(stdout, "Is constant: %s\n\n", poly_is_constant(p_constant) ? "true" : "false");

    // Test poly_eval
    fprintf(stdout, "Testing poly_eval:\n");
    fprintf(stdout, "Evaluating at x=2: %.2f\n\n", poly_eval(p1, 2.0)); // Evaluate 3x^2 - 4x + 5 at x=2

    // Test poly_self_mul
    fprintf(stdout, "Testing poly_self_mul:\n");
    poly_self_mul(&p1, 2.0); // Multiply by 2
    poly_print(stdout, p1);
    fprintf(stdout, "\n");

    // Test pole_self_derivate
    fprintf(stdout, "Testing pole_self_derivate:\n");
    pole_self_derivate(&p1); // Derivative: 6x - 4
    poly_print(stdout, p1);
    fprintf(stdout, "\n");

    // Test poly_sum
    poly* p2 = nullptr;
    p2 = poly_create(p2, 1, 4.0); // 4x
    p2 = poly_create(p2, 0, -2.0); // -2
    fprintf(stdout, "Testing poly_sum:\n");
    poly_print(stdout, p1);
    poly_print(stdout, p2);
    poly* p_sum = poly_sum(p1, p2); // Add polynomials
    poly_print(stdout, p_sum);
    fprintf(stdout, "\n");

    // Clean up memory
    poly_dispose(&p_null);
    poly_dispose(&p1);
    poly_dispose(&p2);
    poly_dispose(&p_constant);
    poly_dispose(&p_sum);

    return EXIT_FAILURE;
}
