(set-logic ALL)
(set-info :status unsat)
(declare-sort T 0)
(declare-fun S (T Int) Int)
(declare-fun R (T Int Int) T)
(assert (forall ((z T) (x Int)) (= x (S (R z 0 0) 0))))
(check-sat)
