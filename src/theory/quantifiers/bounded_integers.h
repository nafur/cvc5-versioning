/*********************                                                        */
/*! \file bounded_integers.h
** \verbatim
** Original author: Andrew Reynolds
** This file is part of the CVC4 project.
** Copyright (c) 2009-2013  New York University and The University of Iowa
** See the file COPYING in the top-level source directory for licensing
** information.\endverbatim
**
** \brief This class manages integer bounds for quantifiers
**/

#include "cvc4_private.h"

#ifndef __CVC4__BOUNDED_INTEGERS_H
#define __CVC4__BOUNDED_INTEGERS_H


#include "theory/quantifiers_engine.h"

#include "context/context.h"
#include "context/context_mm.h"
#include "context/cdchunk_list.h"

namespace CVC4 {
namespace theory {
namespace quantifiers {

class BoundedIntegers : public QuantifiersModule
{
  typedef context::CDHashMap<Node, bool, NodeHashFunction> NodeBoolMap;
  typedef context::CDHashMap<Node, int, NodeHashFunction> NodeIntMap;
  typedef context::CDHashMap<Node, Node, NodeHashFunction> NodeNodeMap;
private:
  //for determining bounds
  bool isBound( Node f, Node v );
  bool hasNonBoundVar( Node f, Node b );
  std::map< Node, std::map< Node, Node > > d_bounds[2];
  std::map< Node, std::vector< Node > > d_set;
  std::map< Node, std::map< Node, Node > > d_range;
  void hasFreeVar( Node f, Node n );
  void process( Node f, Node n, bool pol );
  void processLiteral( Node f, Node lit, bool pol );
  std::vector< Node > d_bound_quants;
private:
  class RangeModel {
  private:
    BoundedIntegers * d_bi;
    void allocateRange();
  public:
    RangeModel(BoundedIntegers * bi, Node r, context::Context* c) : d_bi(bi),
      d_range(r), d_curr_max(-1), d_range_assertions(c), d_has_range(c,false), d_curr_range(c,-1) {}
    Node d_range;
    int d_curr_max;
    std::map< int, Node > d_range_literal;
    std::map< Node, bool > d_lit_to_pol;
    std::map< Node, int > d_lit_to_range;
    NodeBoolMap d_range_assertions;
    context::CDO< bool > d_has_range;
    context::CDO< int > d_curr_range;
    void initialize();
    void assertNode(Node n);
    Node getNextDecisionRequest();
  };
  Node getValueInModel( Node n );
private:
  //information for minimizing ranges
  std::vector< Node > d_ranges;
  //map to range model objects
  std::map< Node, RangeModel * > d_rms;
  //literal to range
  std::map< Node, std::vector< Node > > d_lit_to_ranges;
  //list of currently asserted arithmetic literals
  NodeBoolMap d_assertions;
private:
  void addLiteralFromRange( Node lit, Node r );
public:
  BoundedIntegers( context::Context* c, QuantifiersEngine* qe );

  void check( Theory::Effort e );
  void registerQuantifier( Node f );
  void assertNode( Node n );
  Node getNextDecisionRequest();
  Node getLowerBound( Node f, Node v ){ return d_bounds[0][f][v]; }
  Node getUpperBound( Node f, Node v ){ return d_bounds[1][f][v]; }
  Node getLowerBoundValue( Node f, Node v ){ return getValueInModel( d_bounds[0][f][v] ); }
  Node getUpperBoundValue( Node f, Node v ){ return getValueInModel( d_bounds[1][f][v] ); }
};

}
}
}

#endif