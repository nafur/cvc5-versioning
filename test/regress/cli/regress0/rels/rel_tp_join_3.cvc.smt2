; EXPECT: unsat
(set-option :incremental false)
(set-logic ALL)

(declare-fun w () (Set (Tuple Int Int)))
(declare-fun x () (Set (Tuple Int Int)))
(declare-fun y () (Set (Tuple Int Int)))
(declare-fun z () (Set (Tuple Int Int)))
(declare-fun r () (Set (Tuple Int Int)))
(assert (set.member (tuple 7 1) x))
(assert (set.member (tuple 2 3) x))
(assert (set.member (tuple 7 3) y))
(assert (set.member (tuple 4 7) y))
(assert (set.member (tuple 3 4) z))
(assert (set.member (tuple 3 3) w))
(declare-fun a () (Tuple Int Int))
(assert (= a (tuple 4 1)))
(assert (not (set.member a (rel.transpose r))))
(declare-fun zz () (Set (Tuple Int Int)))
(assert (= zz (rel.join (rel.transpose x) y)))
(assert (not (set.member (tuple 1 3) w)))
(assert (not (set.member (tuple 1 3) (set.union w zz))))
(check-sat)
