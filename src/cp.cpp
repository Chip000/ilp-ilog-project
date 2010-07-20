/*
 * cp.cpp
 */

#include "../include/cp.h"

/* Constructor */
CP::CP(int *P, int n, const char *bt)
{
	perm = new int[n];
	perm_size = n;

	for (int i = 0; i < perm_size; i++) {
		perm[i] = P[i];
	}

	btype = new char[strlen(bt)+1];
	strcpy((char *) btype, bt);

	return;
}

/* Destructor */
CP::~CP()
{
	delete[] perm;
	delete[] btype;
}

/* 
 * get_bound: Returns the model for the model bt.
 */
void CP::get_bound(int P[], int n, const char *bt, 
		   int &lb, int &ub)
{
	if (strcmp(bt, DEF) == 0) {
		lb = 0;
		ub = n;
	}
	else if (strcmp(bt, REV_BR) == 0) {
		/*TODO*/
		lb = 0;
		ub = n;
	}
	else if (strcmp(bt, TRA_BR) == 0) {
		/*TODO*/
		lb = 0;
		ub = n;
	}
	else if (strcmp(bt, T_R_BR) == 0) {
		/*TODO*/
		lb = 0;
		ub = n;
	}

	return;
} /* get_bound */

/*
 * permutation: Verify if the given array is a permutation.
 */
int CP::permutation(int P[], int n)
{
	int *num = new int[n];

	memset(num, 0, n*sizeof(int));

	for (int i = 0; i < n; i++) {
		if (num[P[i]] != 0) {
			return FAILURE;
		}
		num[P[i]] ++;
	}

	delete[] num;

	return SUCCESS;
} /* permutation */

/*
 * is_identity: Verify if the given array is the identity 
 * permutation.
 */
int CP::is_identity(int P[], int n)
{
	for (int i = 0; i < n; i ++) {
		if (P[i] != i+1) {
			return FAILURE;
		}
	}

	return SUCCESS;
} /* is_identity */

/*
 * rev_dist
 */
int CP::rev_dist(int P[], int n, const char *bt)
{
	IloEnv env;
	int dist;
	int lb, ub;
	IloIntArray Pi(env, n+1);
	IloIntArray Sigma(env, n+1);

	int i, j, k;
	int count;

	for (count = 0; count < n; count ++) {
		Pi[count+1] = P[count];
		Sigma[count+1] = count + 1;
	}

	if (is_identity(P, n) == SUCCESS) {
		return 0;
	}

	dist = FAILURE;

	try {
		get_bound(P, n, bt, lb, ub);

#if DEBUG == 1
		std::cerr << ">> DEBUG: creating vars" << std::endl;
#endif

		/* var B_{ijk} */
		IloArray<IloArray<IloBoolVarArray> > B(env, n+1);
		for (i = 0; i < n+1; i++) {
			B[i] = IloArray<IloBoolVarArray> (env, n+1);
			for (int j = 0; j < n+1; j++) {
				B[i][j] = IloBoolVarArray (env, n);
			}
		}

		/* var R_{abk} */
		IloArray<IloArray<IloBoolVarArray> > R(env, n+1);
		for (i = 0; i < n+1; i++) {
			R[i] = IloArray<IloBoolVarArray> (env, n+1);
			for (int j = 0; j < n+1; j++) {
				R[i][j] = IloBoolVarArray (env, n);
			}
		}

		/* var RD_{k} */
		IloBoolVarArray RD(env, n);

#if DEBUG == 1
		std::cerr << ">> DEBUG: vars created" << std::endl;
#endif

		/* Constraints */ 
#if DEBUG == 1
		std::cerr << ">> DEBUG: creating constraint";
		std::cerr << std::endl;
#endif
		IloModel model(env);

#if DEBUG == 1
		std::cerr << ">> DEBUG: creating common constraint";
		std::cerr << std::endl;
#endif

		/* Common Constraints*/
		/* Initial and final permutation are correct */
		for (i = 1; i < n+1; i++) {
			model.add(B[i][Pi[i]][0] == 1);
			model.add(B[i][Sigma[i]][n-1] == 1);
		}

		/* any position of permutation has exactly one 
		   value associated to it */
		for (i = 1; i < n+1; i++) {
			for (k = 0; k < n; k++) {
				IloExpr sum(env);

				for (j = 1; j < n+1; j++) {
					sum += B[i][j][k];
				}

				model.add(sum == 1);
				/* destroy the sum object */
				sum.end();
			}
		}

		/* every value is assigned to one position of 
		   each permutation */
		for (j = 1; j < n+1; j++) {
			for (k = 0; k < n; k++) {
				IloExpr sum(env);

				for (i = 1; i < n+1; i++) {
					sum += B[i][j][k];
				}

				model.add(sum == 1);
				/* destroy the sum object */
				sum.end();
			}
		}

#if DEBUG == 1
		std::cerr << ">> DEBUG: common constraint created";
		std::cerr << std::endl;
#endif
		/* End Common Constraints */

		/* Reversal Constraints */
#if DEBUG == 1
		std::cerr << ">> DEBUG: reversal constraint created";
		std::cerr << std::endl;
#endif

		/* if kth permutation does not alter the permutation,
		   none of subsequent will do so */
		model.add(RD[0] == 1);
		for (k = 1; k < n; k++) {
			model.add(RD[k] <= RD[k-1]);
		}

		/* at most one reversal is done at each step */
		for (k = 0; k < n; k++) {
			IloExpr sum(env);

			for (int a = 1; a < n; a++) {
				for (int b = a+1; b < n+1; b++) {
					sum += R[a][b][k];
				}
			}

			model.add(sum == RD[k]);
			/* destroy the sum object */
			sum.end();
		}

		/*  changes in permutation by reversal */
		/* i < a or i > b */
		for (i = 1; i < n+1; i++) {
			for (j = 1; j < n+1; j++) {
				for (k = 1; k < n; k++) {

					IloExpr sum(env);
			    
					for (int a = i+1; a < n; a++) {
						for (int b = a+1; b < n+1; b++) {
							sum += R[a][b][k];
						}
					}
					
					for (int a = 1; a < n; a++) {
						for (int b = a+1; b < i; b++) {
							sum += R[a][b][k];
						}
					}

					sum += B[i][j][k-1] + (1 - RD[k]) - B[i][j][k];

					model.add(sum <= 1);
					/* destroy the sum object */
					sum.end();

				}
			}
		}

		/* a <= i <= b */
		for (int a = 1; a < n; a++) {
			for (int b = a+1; b < n+1; b++) {
				for (i = a; i <= b; i++) {
					for (j = 1; j < n+1; j++) {
						for (k = 1; k < n; k++) {
							model.add(R[a][b][k] + B[b+a-i][j][k-1] - B[i][j][k] <= 1);
						}
					}
				}
			}
		}

#if DEBUG == 1
		std::cerr << ">> DEBUG: reversal constraint created";
		std::cerr << std::endl;
#endif
		/* End Reversal Constraints */

		/* Bounds */
		for (k = 1; k < n; k++) {
			model.add(RD[k] * k <= ub);
			model.add(RD[k] * n + k - 1 >= lb);
		}

#if DEBUG == 1
		std::cerr << ">> DEBUG: constraint created";
		std::cerr << std::endl;
#endif

		/* objective function */
		IloExpr obj(env);

		for (k = 1; k < n; k++) {
			obj += RD[k];
		}

		model.add(IloMinimize(env, obj));
		obj.end();

		/* Solving the problem */
		IloCplex cplex(env);
		cplex.extract(model);
		if (cplex.solve()) {
			cplex.out() << std::endl ;
			cplex.out() << "Optimal value: ";
			dist = cplex.getObjValue();
			cplex.out() <<  dist << std::endl;
		}
	}
	catch (IloException &ex) {
		env.out() << "Error: " << ex.getMessage() << std::endl;
		return FAILURE;
	}

	return dist;

} /* rev_dist */

/* EOF */
