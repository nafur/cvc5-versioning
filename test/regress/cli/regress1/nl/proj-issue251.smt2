(set-logic QF_UFNIA)
(set-info :status sat)
(declare-fun _substvar_205_ () Bool)
(declare-fun _substvar_231_ () Bool)
(declare-fun _substvar_248_ () Int)
(declare-fun i0 () Int)
(declare-fun i6 () Int)
(declare-fun i8 () Int)
(declare-fun i13 () Int)
(declare-fun i19 () Int)
(declare-fun v55 () Bool)
(assert (>= 0 (* 53 i19 _substvar_248_)))
(assert (or (>= 0 0 (* (abs i19) i13 (- i8 i0)) 591) _substvar_231_ v55))
(assert (or _substvar_205_ (> (abs (- i19 i6)) 94)))
(check-sat)
