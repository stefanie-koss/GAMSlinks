$setglobal PDFLINK coin
$eolcom //
set g OsiGlpk Option Groups /
        general        General Options
      /
    e / '-1', 0*100, 1000, 10000,
        primal, dual,
        off, equilibrium, mean, meanequilibrium,
        textbook, steepestedge,
        forresttomlin, bartelsgolub, givens,
        standard, advanced, bixby, user,
        depthfirst, breadthfirst, bestprojection /
    f / def Default, lo Lower Bound, up Upper Bound, ref Reference /
    t / I Integer, R Real, S String, B Binary /
    o Options /
      startalg               LP solver for root node
      scaling                scaling method
      pricing                pricing method
      factorization          basis factorization method
      initbasis              method for initial basis
      tol_dual               dual feasibility tolerance
      tol_primal             primal feasibility tolerance
      tol_integer            integer feasibility tolerance
      backtracking           backtracking heuristic
      presolve               LP presolver
      cuts                   turning on or off all cut generators
      covercuts              cover cuts
      cliquecuts             clique cuts
      gomorycuts             gomorys mixed-integer cuts
      mircuts                mixed-integer rounding cuts
* GAMS options
      reslim                 resource limit
      iterlim                iteration limit
      optcr                  relative stopping tolerance on MIP gap
*      cutoff                 Cutoff for objective function value
* immediates
      nobounds               ignores bounds on options
      readfile               read secondary option file
 /

$onembedded
optdata(g,o,t,f) /
general.(
  startalg        .s.(def primal)
  scaling         .s.(def meanequilibrium)
  pricing         .s.(def steepestedge)
  factorization   .s.(def givens)
  initbasis       .s.(def advanced)
  tol_dual        .r.(def 1e-7)
  tol_primal      .r.(def 1e-7)
  tol_integer     .r.(def 1e-5)
  backtracking    .s.(def bestprojection)
  presolve        .b.(def 1)
  cuts            .i.(def 0, lo '-1', up 1)
  covercuts       .b.(def 1)
  cliquecuts      .b.(def 1)
  gomorycuts      .b.(def 1)
  mircuts         .b.(def 0)
* GAMS options
  reslim          .r.(def 1000)
  iterlim         .i.(def 10000)
  optcr           .r.(def 0.1)
*  cutoff          .r.(def 0, lo mindouble)
* immediates
  nobounds        .b.(def 0)
  readfile        .s.(def '')
) /
$onempty
  oe(o,e) /
  startalg.(      primal          
                  dual       )
  scaling.(       off              
                  equilibrium      
                  mean             
                  meanequilibrium  )
  pricing.(       textbook         
                  steepestedge     )
  factorization.( forresttomlin
                  bartelsgolub
                  givens           )
  initbasis.(     standard
                  advanced
                  bixby            
                  user             )
  backtracking.(  depthfirst       
                  breadthfirst     
                  bestprojection   )
  cuts.(          '-1', 0, 1)
  cliquecuts.(    0, 1)
  covercuts.(     0, 1)
  gomorycuts.(    0, 1)
  mircuts.(       0, 1)
  presolve.(      0, 1)
 /
$offempty
 im  immediates recognized  / EolFlag , ReadFile, Message, NoBounds /
 immediate(o,im)   / NoBounds.NoBounds, ReadFile.ReadFile /
 hidden(o)         / NoBounds, ReadFile /
 odefault(o)       / reslim     'GAMS reslim'
                     iterlim    'GAMS iterlim' 
                     optcr      'GAMS optcr'
*                     cutoff     'GAMS cutoff'
                   /
$onempty
 oep(o) enum options for documentation only / presolve, cuts, cliquecuts, covercuts, gomorycuts, mircuts /;
$offempty