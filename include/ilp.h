/*
 * ilp.h: definitions for integer linear programming functions
 */

#ifndef HAVE_ILP_H
#define HAVE_ILP_H

#include <ilcplex/ilocplex.h>
#include <iostream>
#include <cstring>

#define DEF "def"
#define REV_BR "rev_br"
#define TRA_BR "tra_br"
#define T_R_BR "t_r_br"
#define DEBUG 1

#define SUCCESS 0
#define FAILURE -1

class ILP {
	int *perm;
	int perm_size;
	const char *btype;
 public:
	/* Constructor */
	ILP(int *P, int n, const char *bt);
	/* Destructor */
	~ILP();

	/* get_bound: Returns the model for the model bt. */
	void get_bound(int P[], int n, const char *bt, 
			   int &lb, int &ub);

	/* permutation: Verify if the given array is a permutation. */
	int permutation(int P[], int n);

	/* is_identity: Verify if the given array is the identity 
	 * permutation. */
	int is_identity(int P[], int n);

	/* trans_dist */
	int trans_dist();
	int trans_dist(int P[], int n, const char *bt);

	/* rev_dist */
	int rev_dist();
	int rev_dist(int P[], int n, const char *bt);

};

#endif
/* ! HAVE_ILP_H */
