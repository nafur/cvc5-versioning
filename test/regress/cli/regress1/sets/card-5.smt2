(set-logic QF_UFLIAFS)
(set-info :status unsat)
(declare-sort E 0)
(declare-fun s () (Set E))
(declare-fun t () (Set E))
(declare-fun u () (Set E))
(assert (>= (set.card (set.union s t)) 8))
(assert (>= (set.card (set.union s u)) 8))
;(assert (<= (set.card (set.union t u)) 5))
(assert (<= (set.card s) 5))
(assert (= (as set.empty (Set E)) (set.inter t u)))
(declare-fun x1 () E)
(declare-fun x2 () E)
(declare-fun x3 () E)
(declare-fun x4 () E)
(declare-fun x5 () E)
(declare-fun x6 () E)
(assert (set.member x1 s))
(assert (set.member x2 s))
(assert (set.member x3 s))
(assert (set.member x4 s))
(assert (set.member x5 s))
(assert (set.member x6 s))
(assert (distinct x1 x2 x3 x4 x5 x6))
(check-sat)
