#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"

// defined std::cout
#include <iostream>

using namespace std;

int main(void) {
    // -- allocate on the heap so that we can reset later if needed
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());

    Minisat::Lit p,q,r;

    // create 4 positive literals over 4 new variables
    p= Minisat::mkLit(solver->newVar());
    q = Minisat::mkLit(solver->newVar());
    r = Minisat::mkLit(solver->newVar());
    //l4 = Minisat::mkLit(solver->newVar());

    // (l1 || b0)
    solver->addClause(p,q,r);
    // (!b0 || l2 || b1)
    solver->addClause(~q, r);
    // (!b1 || l3 || b2)
    solver->addClause(q,~r);
    // (!b2|| l4)
    solver->addClause(~p,~q,~r);
    solver->addClause(~q);
    solver->addClause(~p,q,r);

    //solver->addClause(~b2, l4);

    bool res = solver->solve();
    std::cout << "The result is: " << res << "\n";

    std::cout << "satisfying assignment is: "
              << "p=" << Minisat::toInt(solver->modelValue(p)) << " "
              << "q=" << Minisat::toInt(solver->modelValue(q)) << " "
              << "r=" << Minisat::toInt(solver->modelValue(r)) << " " << std::endl;

    // more clauses can be added to the solver

    // the next line de-allocates existing solver and allocates a new
    // one in its place.
    solver.reset (new Minisat::Solver());

    // at this point the solver is ready. You must create new
    // variable and new clauses
    return 0;
}