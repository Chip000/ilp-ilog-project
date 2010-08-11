/*
 * ilp.h: definitions for integer linear programming functions
 */

#ifndef HAVE_ILP_H
#define HAVE_ILP_H

#include <ilcplex/ilocplex.h>
#include "./prob.h"

class ILP: public Prob {
 public:
	/* Constructor */
	ILP(int *, int, const char *);
	/* Destructor */
	~ILP();

	/* trans_dist */
	int trans_dist();
	int trans_dist(int P[], int n, const char *bt);

	/* rev_dist */
	int rev_dist();
	int rev_dist(int P[], int n, const char *bt);

	/* trans_rev_dist */
	int trans_rev_dist();
	int trans_rev_dist(int P[], int n, const char *bt);
};

#endif
/* ! HAVE_ILP_H */
