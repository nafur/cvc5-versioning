(set-logic ALL)
(set-info :status sat)
(declare-const x Bool)
(declare-const x9 (Bag String))
(check-sat-assuming (((_ divisible 861286585) (bag.card (ite false (bag.difference_remove x9 x9) x9))) (or (bag.subbag x9 (bag.inter_min x9 (ite x x9 (bag.difference_remove x9 x9)))))))
