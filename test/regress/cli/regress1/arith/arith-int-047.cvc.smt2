; EXPECT: sat
(set-logic ALL)
(set-option :incremental false)
(declare-fun x0 () Int)
(declare-fun x1 () Int)
(declare-fun x2 () Int)
(declare-fun x3 () Int)
(assert (= (+ (+ (+ (* (- 14) x0) (* 27 x1)) (* 10 x2)) (* 1 x3)) 10))
(assert (>= (+ (+ (+ (* (- 29) x0) (* (- 26) x1)) (* (- 16) x2)) (* 17 x3)) 16))
(assert (< (+ (+ (+ (* (- 3) x0) (* (- 2) x1)) (* 26 x2)) (* 30 x3)) (- 27)))
(check-sat-assuming ( (not false) ))
