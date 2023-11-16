#include <stdlib.h>
#include <assert.h>

#include "../include/ADTVector.h"

#define VECTOR_MIN_CAPACITY 10


// Δημιουργεί και επιστρέφει ένα νεό vector μεγέθους size, με στοιχεία αρχικοποιημένα σε NULL.
// Αν δεν υπάρχει διαθέσιμη μνήμη επιστρέφει VECTOR_FAIL.
// Αν destroy_value != NULL, τότε καλείται destroy_value(value) κάθε φορά που αφαιρείται (ή αντικαθίσταται) ένα στοιχείο.

Vector vector_create(int size, DestroyFunc destroy_value){
	
	int capacity = VECTOR_MIN_CAPACITY > size ? VECTOR_MIN_CAPACITY : size;

	Vector vec = malloc(sizeof(*vec));
	Pointer* array = calloc(capacity, sizeof(Pointer));
	if(!vec || !array){
		free(vec);
		free(array);
		return VECTOR_FAIL;
	}

	vec->array = array;
	vec->size = size;
	vec->capacity = capacity;
	vec->destroy_value = destroy_value;
	
	return vec;
}

// Επιστρέφει τον αριθμό στοιχείων που περιέχει το vector vec.

int vector_size(Vector vec){
	return vec->size;
}

// Προσθέτει την τιμή value στο _τέλος_ του vector vec. Το μέγεθος του vector μεγαλώνει κατά 1.
// Αν δεν υπάρχει διαθέσιμη μνήμη το vector παραμένει όπως ήταν (αυτό μπορεί να ελεγχθεί με τη vector_size)

void vector_insert_last(Vector vec, Pointer value){
	
	if(vec->capacity == vec->size){
		Pointer* new_array = realloc(vec->array, 2 * vec->capacity * sizeof(Pointer));
		if(!new_array)
			return;

		vec->array = new_array;
		vec->capacity *= 2;
	}

	vec->array[vec->size] = value;
	vec->size++;
}

// Αφαιρεί το τελευταίο στοιχείο του vector. Το μέγεθος του vector μικραίνει κατά 1.
// Αν το vector είναι κενό η συμπεριφορά είναι μη ορισμένη.

void vector_remove_last(Vector vec){
	assert(vec->size);

	if(vec->destroy_value)
		vec->destroy_value(vec->array[vec->size - 1]);
	
	vec->size--;

	if(vec->capacity == 4 * vec->size && vec->capacity >= 2 * VECTOR_MIN_CAPACITY){
		Pointer* new_array = realloc(vec->array, vec->capacity / 2 * sizeof(Pointer));

		if(new_array){
			vec->array = new_array;
			vec->capacity /= 2;
		}
	}
}

// Αφαιρεί το τελευταίο στοιχείο του vector απο την θέση pos. Το μέγεθος του vector μικραίνει κατά 1.
// Αν το vector είναι κενό η συμπεριφορά είναι μη ορισμένη.
void vector_remove_at(Vector vec, int pos){
    assert(vec->size);
    assert(pos >= 0 && pos < vec->size);

    if(vec->destroy_value)
        vec->destroy_value(vec->array[pos]);

    for(int i = pos; i < vec->size - 1; i++)
        vec->array[i] = vec->array[i + 1];

    vec->size--;

    if(vec->capacity == 4 * vec->size && vec->capacity >= 2 * VECTOR_MIN_CAPACITY){
        Pointer* new_array = realloc(vec->array, vec->capacity / 2 * sizeof(Pointer));

        if(new_array){
            vec->array = new_array;
            vec->capacity /= 2;
        }
    }
}

// Επιστρέφει την τιμή στη θέση pos του vector vec (μη ορισμένο αποτέλεσμα αν pos < 0 ή pos >= size)

Pointer vector_get_at(Vector vec, int pos){
	assert(pos >= 0 && pos < vec->size);
	return vec->array[pos];
}

// Αλλάζει την τιμή στη θέση pos του Vector vec σε value.
// ΔΕΝ μεταβάλλει το μέγεθος του vector, αν pos >= size το αποτέλεσμα δεν είναι ορισμένο.

void vector_set_at(Vector vec, int pos, Pointer value){
	assert(pos >= 0 && pos < vec->size);

	if(vec->destroy_value && vec->array[pos] != value)
		vec->destroy_value(vec->array[pos]);

	vec->array[pos] = value;
}

// Βρίσκει και επιστρέφει το πρώτο στοιχείο στο vector που να είναι ίσο με value
// (με βάση τη συνάρτηση compare), ή NULL αν δεν βρεθεί κανένα στοιχείο.

Pointer vector_find(Vector vec, Pointer value, CompareFunc compare){
	for(int i = 0; i < vec->size; i++){
		if(!compare(vec->array[i], value))
			return vec->array[i];
	}
	return NULL;
}

// Αλλάζει τη συνάρτηση που καλείται σε κάθε αφαίρεση/αντικατάσταση στοιχείου σε
// destroy_value. Επιστρέφει την προηγούμενη τιμή της συνάρτησης.

DestroyFunc vector_set_destroy_value(Vector vec, DestroyFunc destroy_value){
	DestroyFunc old = vec->destroy_value;
	vec->destroy_value = destroy_value;
	return old;
}

// Ελευθερώνει όλη τη μνήμη που δεσμεύει το vector vec.
// Οποιαδήποτε λειτουργία πάνω στο vector μετά το destroy είναι μη ορισμένη.

void vector_destroy(Vector vec){
	for(int i = 0; i < vec->size; i++)
		if(vec->destroy_value)
			vec->destroy_value(vec->array[i]);

	free(vec->array);
	free(vec);
}


// Διάσχιση του vector ////////////////////////////////////////////////////////////
//
// Οι παρακάτω συναρτήσεις επιτρέπουν τη διάσχιση του vector μέσω κόμβων.
// Δεν είναι τόσο συχνά χρησιμοποιούμενες όσο για άλλα ADTs, γιατί μπορούμε
// εύκολα να διασχίσουμε το array και μέσω indexes. Παραμένουν πάντως χρήσιμες,
// τόσο για ομοιομορφία με τους άλλους ADTs, αλλά και γιατί για κάποιες υλοποιήσεις
// η διάσχιση μέσω κόμβων μπορεί να είναι πιο γρήγορη.

// Οι σταθερές αυτές συμβολίζουν εικονικούς κόμβους _πριν_ τον πρώτο και _μετά_ τον τελευταίο
#define VECTOR_BOF (VectorNode)0
#define VECTOR_EOF (VectorNode)0

typedef struct vector_node* VectorNode;

// Επιστρέφουν τον πρώτο και τον τελευταίο κομβο του vector, ή VECTOR_BOF / VECTOR_EOF αντίστοιχα αν το vector είναι κενό

VectorNode vector_first(Vector vec){
	if(!vec->size)
		return VECTOR_BOF;

	Pointer* p = &vec->array[0];
	return (VectorNode)p;
}
VectorNode vector_last(Vector vec){
	if(!vec->size)
		return VECTOR_EOF;

	Pointer* p = &vec->array[vec->size - 1];
	return (VectorNode)p;
}

// Επιστρέφουν τον επόμενο και τον προηγούμενο κομβο του node, ή VECTOR_EOF / VECTOR_BOF
// αντίστοιχα αν ο node δεν έχει επόμενο / προηγούμενο.

VectorNode vector_next(Vector vec, VectorNode node){
	Pointer* p = (Pointer*)node;

	if (p == &vec->array[vec->size-1])
		return VECTOR_EOF;
	else
		return (VectorNode)(p + 1);
}
VectorNode vector_previous(Vector vec, VectorNode node){
	Pointer* p = (Pointer*)node;

	if (p == &vec->array[0])
		return VECTOR_EOF;
	else
		return (VectorNode)(p - 1);
}

// Επιστρέφει το περιεχόμενο του κόμβου node

Pointer vector_node_value(Vector vec, VectorNode node){
	Pointer* p = (Pointer*)node;
	return *p;
}

// Βρίσκει το πρώτο στοιχείο στο vector που να είναι ίσο με value (με βάση τη συνάρτηση compare).
// Επιστρέφει τον κόμβο του στοιχείου, ή VECTOR_EOF αν δεν βρεθεί.

VectorNode vector_find_node(Vector vec, Pointer value, CompareFunc compare){
	for (int i = 0; i < vec->size; i++)
		if (compare(vec->array[i], value) == 0)
			return (VectorNode)&vec->array[i];		// βρέθηκε

	return VECTOR_EOF;
}
