(set-logic ALL)
(set-info :status sat)
(declare-fun f (Int) Int)
(declare-fun g1 (Int) Int)
(declare-fun x () Int)
(assert (= ((as f Int) x) (g1 x)))
(check-sat)
