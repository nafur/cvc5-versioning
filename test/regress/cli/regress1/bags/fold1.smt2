(set-logic HO_ALL)
(set-info :status sat)
(set-option :fmf-bound true)
(set-option :uf-lazy-ll true)
(define-fun plus ((x Int) (y Int)) Int (+ x y))
(declare-fun A () (Bag Int))
(declare-fun sum () Int)
(assert (= sum (bag.fold plus 1 A)))
(assert (= sum 10))
(check-sat)
