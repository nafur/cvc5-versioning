(set-logic ALL)
(set-info :status sat)
(declare-fun v0 () Bool)
(declare-fun v25 () Bool)
(declare-fun v88 () Bool)
(assert (= v25 (or v0 (not v0))))
(assert (= v0 (= v88 (forall ((q131 Bool)) (xor v88 q131 q131 v25)))))
(check-sat)
