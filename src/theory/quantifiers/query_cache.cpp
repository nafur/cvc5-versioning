/******************************************************************************
 * Top contributors (to current version):
 *   Andrew Reynolds
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Implementation of query cache
 */

#include "theory/quantifiers/query_cache.h"

using namespace std;
using namespace cvc5::kind;
using namespace cvc5::context;

namespace cvc5 {
namespace theory {
namespace quantifiers {

QueryCache::QueryCache(Options* optr)
{
  d_true = NodeManager::currentNM()->mkConst(true);
  if (optr!=nullptr)
  {
    d_subOptions.copyValues(optr);
  }
}

void QueryCache::initialize(const std::vector<Node>& vars, SygusSampler* ss)
{
  NodeManager * nm = NodeManager::currentNM();
  TypeNode boolType = nm->booleanType();
  d_sampler.initialize(boolType, vars, 0);
}

bool QueryCache::addTerm(Node sol, std::ostream& out)
{
  sol = d_extr.extendedRewrite(sol);
  if (sol.isConst())
  {
    return !sol.getConst<bool>();
  }
  size_t npoints = d_sampler.getNumSamplePoints();
  for (size_t i=0; i<npoints; i++)
  {
    Node ev = d_sampler.evaluate(sol, i);
    if (ev==d_true)
    {
      // already satisfied by a point
      return false;
    }
  }
  sol = convertToSkolem(sol);
  std::vector<Node> modelVals;
  Result r = checkWithSubsolver(sol, d_skolems, modelVals, &d_subOptions);
  if (r.asSatisfiabilityResult().isSat() == Result::UNSAT)
  {
    return true;
  }
  d_sampler.addPoint(modelVals);
  return false;
}

}  // namespace quantifiers
}  // namespace theory
}  // namespace cvc5
