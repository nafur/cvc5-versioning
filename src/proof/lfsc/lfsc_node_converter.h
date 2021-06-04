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
 * Implementation of LFSC node conversion
 */
#include "cvc5_private.h"

#ifndef CVC4__PROOF__LFSC__LFSC_NODE_CONVERTER_H
#define CVC4__PROOF__LFSC__LFSC_NODE_CONVERTER_H

#include <iostream>
#include <map>

#include "expr/node.h"
#include "expr/node_converter.h"
#include "expr/type_node.h"

namespace cvc5 {
namespace proof {

class LfscNodeConverter : public NodeConverter
{
 public:
  LfscNodeConverter();
  ~LfscNodeConverter() {}
  /** convert to internal */
  Node postConvert(Node n) override;
  /** convert to internal */
  TypeNode postConvertType(TypeNode tn) override;
  /**
   * Get the null terminator for kind k and type tn. The type tn can be
   * omitted if applications of kind k do not have parametric type.
   */
  Node getNullTerminator(Kind k, TypeNode tn = TypeNode::null());
  /**
   * Return the properly named operator for n of the form (f t1 ... tn), where
   * f could be interpreted or uninterpreted.  This method is used for cases
   * where it is important to get the term corresponding to the operator for
   * a term. An example is for the base REFL step of nested CONG.
   */
  Node getOperatorOfTerm(Node n, bool macroApply = false);
  /** get closure operator */
  Node getOperatorOfClosure(Node q, bool macroApply = false);
  /** get closure operator, cop is return  */
  Node getOperatorOfBoundVar(Node cop, Node v);
  /** get or assign variable index for variable v */
  size_t getOrAssignIndexForVar(Node v);
  /**
   * Make an internal symbol with custom name. This is a BOUND_VARIABLE that
   * has a distinguished status so that it is *not* printed as (bvar ...). The
   * returned variable is always fresh.
   */
  Node mkInternalSymbol(const std::string& name, TypeNode tn);

  /** get name for user name */
  static std::string getNameForUserName(const std::string& name);

 private:
  /** Should we traverse n? */
  bool shouldTraverse(Node n) override;
  /** Make skolem function */
  Node maybeMkSkolemFun(Node k, bool macroApply = false);
  /** Type as node */
  Node typeAsNode(TypeNode tni) const;
  /** Get symbol for term */
  Node getSymbolInternalFor(Node n, const std::string& name);
  /** Get symbol internal, (k,tn,name) are for caching, name is the name */
  Node getSymbolInternal(Kind k, TypeNode tn, const std::string& name);
  /**
   * Get character vector, add internal vector of characters for c.
   */
  void getCharVectorInternal(Node c, std::vector<Node>& chars);
  /** is indexed operator kind */
  static bool isIndexedOperatorKind(Kind k);
  /** get indices, n is the operator */
  static std::vector<Node> getOperatorIndices(Kind k, Node n);
  /** terms with different syntax than smt2 */
  std::map<std::tuple<Kind, TypeNode, std::string>, Node> d_symbolsMap;
  /** the set of all internally generated symbols */
  std::unordered_set<Node> d_symbols;
  /** arrow type constructor */
  TypeNode d_arrow;
  /** the type of LFSC sorts, which can appear in terms */
  TypeNode d_sortType;
  /** Used for getting unique index for variable */
  std::map<Node, size_t> d_varIndex;
  /** Cache for typeAsNode */
  std::map<TypeNode, Node> d_typeAsNode;
  /** Used for interpreted builtin parametric sorts */
  std::map<Kind, Node> d_typeKindToNodeCons;
};

}  // namespace proof
}  // namespace cvc5

#endif