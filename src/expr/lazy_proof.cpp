/*********************                                                        */
/*! \file lazy_proof.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Lazy proof utility
 **/

#include "expr/lazy_proof.h"

using namespace CVC4::kind;

namespace CVC4 {

LazyCDProof::LazyCDProof(ProofNodeManager* pnm,
                         ProofGenerator* dpg,
                         context::Context* c)
    : CDProof(pnm, c), d_gens(c ? c : &d_context), d_defaultGen(dpg)
{
}

LazyCDProof::~LazyCDProof() {}

std::shared_ptr<ProofNode> LazyCDProof::mkProof(Node fact)
{
  Trace("lazy-cdproof") << "LazyCDProof::mkLazyProof " << fact << std::endl;
  // make the proof, which should always be non-null, since we construct an
  // assumption in the worst case.
  std::shared_ptr<ProofNode> opf = CDProof::mkProof(fact);
  Assert(opf != nullptr);
  if (!hasGenerators())
  {
    Trace("lazy-cdproof") << "...no generators, finished" << std::endl;
    // optimization: no generators, we are done
    return opf;
  }
  // otherwise, we traverse the proof opf and fill in the ASSUME leafs that
  // have generators
  std::unordered_set<ProofNode*> visited;
  std::unordered_set<ProofNode*>::iterator it;
  std::vector<ProofNode*> visit;
  ProofNode* cur;
  visit.push_back(opf.get());
  do
  {
    cur = visit.back();
    visit.pop_back();
    it = visited.find(cur);

    if (it == visited.end())
    {
      visited.insert(cur);
      if (cur->getRule() == PfRule::ASSUME)
      {
        Node afact = cur->getResult();
        bool isSym = false;
        ProofGenerator* pg = getGeneratorFor(afact, isSym);
        if (pg != nullptr)
        {
          Trace("lazy-cdproof")
              << "LazyCDProof: Call generator for assumption " << afact << std::endl;
          Assert(!isSym || afact.getKind() == EQUAL);
          Node afactGen = isSym ? afact[1].eqNode(afact[0]) : afact;
          // use the addProofTo interface
          if (!pg->addProofTo(afactGen, this))
          {
            Trace("lazy-cdproof") << "LazyCDProof: Failed added fact for " << afactGen << std::endl;
            Assert(false) << "Proof generator " << pg->identify()
                          << " could not add proof for fact " << afactGen
                          << std::endl;
          }
          else
          {
            Trace("lazy-cdproof") << "LazyCDProof: Successfully added fact for " << afactGen << std::endl;
          }
        }
        else
        {
          Trace("lazy-cdproof") << "LazyCDProof: No generator for " << afact << std::endl;
        }
        // Notice that we do not traverse the proofs that have been generated
        // lazily by the proof generators here.  In other words, we assume that
        // the proofs from provided proof generators are "complete" and need
        // no further modification by this class.
      }
      else
      {
        const std::vector<std::shared_ptr<ProofNode>>& cc = cur->getChildren();
        for (const std::shared_ptr<ProofNode>& cp : cc)
        {
          visit.push_back(cp.get());
        }
      }
    }
  } while (!visit.empty());
  // we have now updated the ASSUME leafs of opf, return it
  Trace("lazy-cdproof") << "...finished" << std::endl;
  return opf;
}

void LazyCDProof::addLazyStep(Node expected,
                              ProofGenerator* pg,
                              bool forceOverwrite)
{
  Assert(pg != nullptr);
  NodeProofGeneratorMap::const_iterator it = d_gens.find(expected);
  if (it != d_gens.end() && !forceOverwrite)
  {
    // don't overwrite something that is already there
    return;
  }
  // just store now
  d_gens.insert(expected, pg);

  if (forceOverwrite)
  {
    // TODO: if we stored expected via a normal call to CDProof::addStep, then
    // we should overwrite the step in d_nodes with an ASSUME?
  }
}

ProofGenerator* LazyCDProof::getGeneratorFor(Node fact, bool& isSym)
{
  isSym = false;
  NodeProofGeneratorMap::const_iterator it = d_gens.find(fact);
  if (it != d_gens.end())
  {
    return (*it).second;
  }
  // could be symmetry
  if (fact.getKind() != EQUAL || fact[0] == fact[1])
  {
    // can't be symmetry, return the default generator
    return d_defaultGen;
  }
  Node factSym = fact[1].eqNode(fact[0]);
  it = d_gens.find(factSym);
  if (it != d_gens.end())
  {
    isSym = true;
    return (*it).second;
  }
  // return the default generator
  return d_defaultGen;
}

bool LazyCDProof::hasGenerators() const
{
  return !d_gens.empty() || d_defaultGen != nullptr;
}

}  // namespace CVC4
