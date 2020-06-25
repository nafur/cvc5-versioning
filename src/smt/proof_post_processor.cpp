/*********************                                                        */
/*! \file proof_post_processor.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Morgan Deters, Andrew Reynolds, Tim King
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of module for processing proof nodes
 **/

#include "smt/proof_post_processor.h"

#include "smt/smt_engine.h"
#include "theory/builtin/proof_checker.h"
#include "theory/rewriter.h"

using namespace CVC4::kind;
using namespace CVC4::theory;

namespace CVC4 {
namespace smt {

ProofPostprocessCallback::ProofPostprocessCallback(ProofNodeManager* pnm,
                                                   SmtEngine* smte)
    : d_pnm(pnm), d_smte(smte), d_pchecker(pnm ? pnm->getChecker() : nullptr)
{
  d_true = NodeManager::currentNM()->mkConst(true);
}

void ProofPostprocessCallback::setEliminateRule(PfRule rule)
{
  d_elimRules.insert(rule);
}

bool ProofPostprocessCallback::shouldUpdate(ProofNode* pn)
{
  return d_elimRules.find(pn->getRule()) != d_elimRules.end();
}

bool ProofPostprocessCallback::update(PfRule id,
                                      const std::vector<Node>& children,
                                      const std::vector<Node>& args,
                                      CDProof* cdp)
{
  ProofRuleChecker* prc = d_pchecker->getCheckerFor(id);
  if (prc != nullptr)
  {
    return prc->expand(id, children, args, cdp);
  }
  Node ret = updateInternal(id, children, args, cdp);
  return !ret.isNull();
}

Node ProofPostprocessCallback::updateInternal(PfRule id,
                                              const std::vector<Node>& children,
                                              const std::vector<Node>& args,
                                              CDProof* cdp)
{
  if (d_elimRules.find(id) == d_elimRules.end())
  {
    // not eliminated
    return Node::null();
  }
  // macro elimination
  if (id == PfRule::MACRO_SR_EQ_INTRO)
  {
    // (TRANS
    //   (SUBS <children> :args args[0:1])
    //   (REWRITE :args <t.substitute(x1,t1). ... .substitute(xn,tn)> args[2]))
    Node t = args[0];
    Node ts;
    if (!children.empty())
    {
      std::vector<Node> sargs;
      sargs.push_back(t);
      MethodId sid = MethodId::SB_DEFAULT;
      if (args.size() >= 2)
      {
        if (builtin::BuiltinProofRuleChecker::getMethodId(args[1], sid))
        {
          sargs.push_back(args[1]);
        }
      }
      ts =
          builtin::BuiltinProofRuleChecker::applySubstitution(t, children, sid);
      if (ts != t)
      {
        // apply SUBS proof rule if necessary
        update(PfRule::SUBS, children, sargs, cdp);
      }
    }
    else
    {
      // no substitute
      ts = t;
    }
    std::vector<Node> rargs;
    rargs.push_back(ts);
    MethodId rid = MethodId::RW_REWRITE;
    if (args.size() >= 3)
    {
      if (builtin::BuiltinProofRuleChecker::getMethodId(args[2], rid))
      {
        rargs.push_back(args[2]);
      }
    }
    Node tr = builtin::BuiltinProofRuleChecker::applyRewrite(ts, rid);
    if (ts != tr)
    {
      // apply REWRITE proof rule
      update(PfRule::REWRITE, children, rargs, cdp);
      // did substitute and rewrite, must add TRANS
      if (ts != t)
      {
        Node eq1 = t.eqNode(ts);
        Node eq2 = ts.eqNode(tr);
        cdp->addStep(t.eqNode(tr), PfRule::TRANS, {eq1, eq2}, {});
      }
    }
    if (t == tr)
    {
      // typically not necessary, but done to be robust
      cdp->addStep(t.eqNode(tr), PfRule::REFL, {}, {t});
    }
    return t.eqNode(tr);
  }
  else if (id == PfRule::MACRO_SR_PRED_INTRO)
  {
    // (TRUE_ELIM
    //   (MACRO_SR_EQ_INTRO <children> :args <args>))
    // We call the updateInternal method on MACRO_SR_EQ_INTRO, where notice
    // that this rule application is immediately expanded in the recursive
    // call and not added to the proof.
    Node conc = updateInternal(PfRule::MACRO_SR_EQ_INTRO, children, args, cdp);
    Assert(!conc.isNull() && conc.getKind() == EQUAL && conc[1] == d_true);
    cdp->addStep(conc[0], PfRule::TRUE_ELIM, {conc}, {});
    return args[0];
  }
  else if (id == PfRule::MACRO_SR_PRED_ELIM)
  {
    // (TRUE_ELIM
    //   (TRANS
    //     (SYMM (MACRO_SR_EQ_INTRO children[1:] :args children[0] ++ args))
    //     (TRUE_INTRO children[0])))
    std::vector<Node> schildren(children.begin() + 1, children.end());
    std::vector<Node> srargs;
    srargs.push_back(children[0]);
    srargs.insert(srargs.end(), args.begin(), args.end());
    Node conc =
        updateInternal(PfRule::MACRO_SR_EQ_INTRO, schildren, srargs, cdp);
    Assert(!conc.isNull() && conc.getKind() == EQUAL && conc[0] == children[0]);

    Node eq1 = children[0].eqNode(d_true);
    cdp->addStep(eq1, PfRule::TRUE_INTRO, {children[0]}, {});

    Node concSym = conc[1].eqNode(conc[0]);
    Node eq2 = conc[1].eqNode(d_true);
    cdp->addStep(eq2, PfRule::TRANS, {concSym, eq1}, {});

    cdp->addStep(conc[1], PfRule::TRUE_ELIM, {eq2}, {});
    return conc[1];
  }
  else if (id == PfRule::MACRO_SR_PRED_TRANSFORM)
  {
    // (TRUE_ELIM
    //    (TRANS
    //       (MACRO_SR_EQ_INTRO children[1:] :args <args>)
    //       (SYMM (MACRO_SR_EQ_INTRO children[1:] :args children[0] args[1:]))
    //       (TRUE_INTRO children[0])))
    std::vector<Node> schildren(children.begin() + 1, children.end());
    Node eq1 = updateInternal(PfRule::MACRO_SR_EQ_INTRO, schildren, args, cdp);
    Assert(!eq1.isNull() && eq1.getKind() == EQUAL && eq1[0] == args[0]);

    std::vector<Node> args2;
    args2.push_back(children[0]);
    args2.insert(args2.end(), args.begin(), args.end());
    Node eq2 = updateInternal(PfRule::MACRO_SR_EQ_INTRO, schildren, args2, cdp);
    Assert(!eq2.isNull() && eq2.getKind() == EQUAL && eq2[0] == children[0]);

    Node eq3 = children[0].eqNode(d_true);
    cdp->addStep(eq3, PfRule::TRUE_INTRO, {children[0]}, {});

    Node eq = args[0].eqNode(d_true);
    std::vector<Node> tchildren;
    if (eq1[0] != eq1[1])
    {
      tchildren.push_back(eq1);
    }
    if (eq2[0] != eq2[1])
    {
      Node symEq2 = eq2[1].eqNode(eq2[0]);
      tchildren.push_back(symEq2);
    }
    tchildren.push_back(eq3);
    cdp->addStep(eq, PfRule::TRANS, tchildren, {});

    cdp->addStep(args[0], PfRule::TRUE_ELIM, {eq}, {});
    return args[0];
  }
  else if (id == PfRule::SUBS)
  {
    // This does a term conversion proof *for each* substitution.
    // The proof of f(a) * { a -> g(b) } * { b -> c } = f(g(c)) is:
    //   TRANS( CONG{f}( a=g(b) ), CONG{f}( CONG{g}( b = c ) ) )
    // Notice that more optimal proofs are possible that do a single traversal:
    //   CONG{f}( TRANS( a=g(b), CONG{g}( b=c ) ) )
    // This is significantly more challenging and is not done here.
    Node t = args[0];
    // get the kind of substitution
    MethodId ids = MethodId::SB_DEFAULT;
    if (args.size() >= 2)
    {
      builtin::BuiltinProofRuleChecker::getMethodId(args[1], ids);
    }
    std::vector<Node> tchildren;
    for (const Node& c : children)
    {
      TNode var, subs;
      builtin::BuiltinProofRuleChecker::getSubstitution(c, var, subs, ids);
      // apply the substitution
      Node ts = t.substitute(var, subs);
      if (ts != t)
      {
        TConvProofGenerator tcpg(d_pnm, nullptr, TConvPolicy::ONCE);
        // ensure cdp has proof of var = subs
        Node veqs = var.eqNode(subs);
        if (veqs != c)
        {
          // should be true intro or false intro
          Assert(subs.isConst());
          cdp->addStep(
              veqs,
              subs.getConst<bool>() ? PfRule::TRUE_INTRO : PfRule::FALSE_INTRO,
              {c},
              {});
        }
        tcpg.addRewriteStep(var, subs, cdp);
        Node eq = t.eqNode(ts);
        // add the proof constructed by the term conversion utility
        std::shared_ptr<ProofNode> pfn = tcpg.getProofFor(eq);
        // should give a proof, if not, then tcpg does not agree with the
        // substitution.
        Assert(pfn != nullptr);
        if (pfn != nullptr)
        {
          // add to the children of the transitivity step
          tchildren.push_back(eq);
          cdp->addProof(pfn);
        }
        // update term
        t = ts;
      }
    }
    Node finalEq = args[0].eqNode(t);
    if (tchildren.size() > 1)
    {
      cdp->addStep(finalEq, PfRule::TRANS, tchildren, {});
    }
    else if (tchildren.empty())
    {
      // should not be necessary typically
      cdp->addStep(finalEq, PfRule::REFL, {}, {t});
    }
    return finalEq;
  }
  else if (id == PfRule::REWRITE)
  {
    // get the kind of rewrite
    MethodId idr = MethodId::RW_REWRITE;
    if (args.size() >= 2)
    {
      builtin::BuiltinProofRuleChecker::getMethodId(args[1], idr);
    }
    Node ret = builtin::BuiltinProofRuleChecker::applyRewrite(args[0], idr);
    Node eq = args[0].eqNode(ret);
    if (idr == MethodId::RW_REWRITE || idr == MethodId::RW_REWRITE_EQ_EXT)
    {
      // rewrites from theory::Rewriter
      // automatically expand THEORY_REWRITE as well here if set
      bool elimTR =
          (d_elimRules.find(PfRule::THEORY_REWRITE) != d_elimRules.end());
      bool isExtEq = (idr == MethodId::RW_REWRITE_EQ_EXT);
      // use rewrite with proof interface
      Rewriter* rr = d_smte->getRewriter();
      TrustNode trn = rr->rewriteWithProof(args[0], elimTR, isExtEq);
      std::shared_ptr<ProofNode> pfn =
          trn.getGenerator()->getProofFor(trn.getProven());
      cdp->addProof(pfn);
      Assert(trn.getNode() == ret);
    }
    else if (idr == MethodId::RW_EVALUATE)
    {
      // change to evaluate, which is never eliminated
      cdp->addStep(eq, PfRule::EVALUATE, {}, {args[0]});
    }
    else
    {
      // don't know how to eliminate
      return Node::null();
    }
    if (args[0] == ret)
    {
      // should not be necessary typically
      cdp->addStep(eq, PfRule::REFL, {}, {args[0]});
    }
    return eq;
  }

  // THEORY_LEMMA, THEORY_PREPROCESS?

  return Node::null();
}

ProofPostproccess::ProofPostproccess(ProofNodeManager* pnm, SmtEngine* smte)
    : d_cb(pnm, smte), d_updater(pnm, d_cb)
{
}

ProofPostproccess::~ProofPostproccess() {}

void ProofPostproccess::process(std::shared_ptr<ProofNode> pf)
{
  d_updater.process(pf);
}

}  // namespace smt
}  // namespace CVC4
