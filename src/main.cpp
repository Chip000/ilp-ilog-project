#include <iostream>
#include <sstream>
#include <ctime>
#include <cstdio>
#include "../include/ilp.h"

using namespace std;

#define TRA "trans"
#define REV "rev"
#define T_R "trans_rev"

/*
 * usage: prints the usage message
 */
void usage(const char name[])
{
	cout << "USAGE:\n";
	cout << "======\n\n";

	cout << name << " <m> <b> <p>\n\n";

	cout << "- m is the model for the problem.\n";
	cout << "* " << TRA << "     - sorting by transposition.\n";
	cout << "* " << REV << "       - sorting by reversal.\n";
	cout << "* " << T_R << " - sorting by transpositions+reversals.\n";
	cout << "\n";

	cout << "- b is the type of bounds for the problem.\n";
	cout << "* " << DEF << "    - default bound.\n";
	cout << "* " << TRA_BR << " - transposition breakpoint bound.\n";
	cout << "* " << REV_BR << " - reversal breakpoint bound.\n";
	cout << "* " << T_R_BR << " - transposition+reversal breakpoint bound.\n";
	cout << "\n";

	cout << "- p is the given permutation.\n";
	cout << "* Ex: \"1 2 3 4\".\n";
	cout << "\n";


	return;

} /* usage */


/*
 * perm2int: Returns an array of int representing the permutation and
 * its length
 */
int *perm2int(const char perm[], int &n)
{
	char *ptr;
	int tmp;
	int *ret;

	ptr = (char *) perm;
	n = 0;

	istringstream tmp_iss(ptr);
	while (tmp_iss >> tmp) {
		n++;
	}

	ptr = (char *) perm;
	ret = new int[n+1];
	tmp = 0;
	istringstream iss(ptr);
	while (iss >> ret[tmp]) {
		tmp ++;
	}

	return ret;

} /* perm2int */

int main(int argc, const char *argv[])
{

	const char *model = argv[1];
	const char *btype = argv[2];
	const char *char_perm = argv[3];
	int *perm;
	int perm_size;
	int ret;

	clock_t c_begin;
	clock_t c_end;

	c_begin = clock();

	if (argc < 4) {
		usage(argv[0]);
		return 1;
	}

	cout << ">> Model: " << model << endl;
	cout << ">> Bound type: " << btype << endl;
	cout << ">> Permutation: " << char_perm << endl;

	perm = perm2int(char_perm, perm_size);

	/* Creating the object problem */
	ILP prob(perm, perm_size, btype);

	cout << endl;

	ret = -1;

	if (strcmp(TRA, model) == 0) {
		ret = prob.trans_dist();
	}
	else if (strcmp(REV, model) == 0) {
		ret = prob.rev_dist();
	}
	else if (strcmp(T_R, model) == 0) {
		ret = prob.trans_rev_dist();
	}
	else {
		cerr << ">> ERROR: Model not found" << endl << endl;
		usage(argv[0]);
	}

	if (ret == 0) {
		cout << "Optimal value: 0" << endl;
	}

	c_end = clock();

	fprintf(stdout, "Total Time: %.4f\n",
		(double) (c_end - c_begin) / CLOCKS_PER_SEC);

	cout << endl;

	/* desallocate the memory */
	delete[] perm;

	return 0;

}

/* EOF */
