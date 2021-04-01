/*********************                                                        */
/*! \file alpha_equivalence.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Alpha equivalence checking
 **
 **/

#include "theory/quantifiers/alpha_equivalence.h"

#include "expr/proof.h"

using namespace cvc5::kind;

namespace cvc5 {
namespace theory {
namespace quantifiers {

struct sortTypeOrder {
  expr::TermCanonize* d_tu;
  bool operator() (TypeNode i, TypeNode j) {
    return d_tu->getIdForType( i )<d_tu->getIdForType( j );
  }
};

Node AlphaEquivalenceTypeNode::registerNode(
    Node q,
    Node t,
    std::vector<TypeNode>& typs,
    std::map<TypeNode, size_t>& typCount)
{
  AlphaEquivalenceTypeNode* aetn = this;
  size_t index = 0;
  while (index < typs.size())
  {
    TypeNode curr = typs[index];
    Assert(typCount.find(curr) != typCount.end());
    Trace("aeq-debug") << "[" << curr << " " << typCount[curr] << "] ";
    std::pair<TypeNode, size_t> key(curr, typCount[curr]);
    aetn = &(aetn->d_children[key]);
    index = index + 1;
  }
  Trace("aeq-debug") << " : ";
  std::map<Node, Node>::iterator it = aetn->d_quant.find(t);
  if (it != aetn->d_quant.end())
  {
    return it->second;
  }
  aetn->d_quant[t] = q;
  return q;
}

Node AlphaEquivalenceDb::addTerm(Node q)
{
  Assert(q.getKind() == FORALL);
  Trace("aeq") << "Alpha equivalence : register " << q << std::endl;
  //construct canonical quantified formula
  Node t = d_tc->getCanonicalTerm(q[1], d_sortCommutativeOpChildren);
  Trace("aeq") << "  canonical form: " << t << std::endl;
  //compute variable type counts
  std::map<TypeNode, size_t> typCount;
  std::vector< TypeNode > typs;
  for( unsigned i=0; i<q[0].getNumChildren(); i++ ){
    TypeNode tn = q[0][i].getType();
    typCount[tn]++;
    if( std::find( typs.begin(), typs.end(), tn )==typs.end() ){
      typs.push_back( tn );
    }
  }
  sortTypeOrder sto;
  sto.d_tu = d_tc;
  std::sort( typs.begin(), typs.end(), sto );
  Trace("aeq-debug") << "  ";
  Node ret = d_ae_typ_trie.registerNode(q, t, typs, typCount);
  Trace("aeq") << "  ...result : " << ret << std::endl;
  return ret;
}

AlphaEquivalence::AlphaEquivalence(ProofNodeManager* pnm)
    : d_termCanon(),
      d_aedb(&d_termCanon, !pnm),
      d_pnm(pnm),
      d_pfAlpha(pnm ? new CDProof(pnm) : nullptr)
{
}

TrustNode AlphaEquivalence::reduceQuantifier(Node q)
{
  Assert(q.getKind() == FORALL);
  Node ret = d_aedb.addTerm(q);
  if (ret == q)
  {
    return TrustNode::null();
  }
  Node lem;
  ProofGenerator* pg = nullptr;
  // lemma ( q <=> d_quant )
  // Notice that we infer this equivalence regardless of whether q or ret
  // have annotations (e.g. user patterns, names, etc.).
  Trace("alpha-eq") << "Alpha equivalent : " << std::endl;
  Trace("alpha-eq") << "  " << q << std::endl;
  Trace("alpha-eq") << "  " << ret << std::endl;
  lem = q.eqNode(ret);
  if (q.getNumChildren() == 3)
  {
    Notice() << "Ignoring annotated quantified formula based on alpha "
                "equivalence: "
             << q << std::endl;
  }
  // disabling temporarily
  if (false && isProofEnabled())
  {
    // arguments are the original formula and the renaming
    std::vector<Node> args{q};
    args.insert(args.end(), ret[0].begin(), ret[0].end());
    d_pfAlpha->addStep(lem, PfRule::ALPHA_EQUIV, {}, args);
    pg = d_pfAlpha.get();
  }
  return TrustNode::mkTrustLemma(lem, pg);
}

bool AlphaEquivalence::isProofEnabled() const { return d_pfAlpha != nullptr; }

}  // namespace quantifiers
}  // namespace theory
}  // namespace cvc5
