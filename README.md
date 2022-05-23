# IKS solver/learner
##### (Incomplete Knowledge Semantics)

This repository contains the compiled executable for the implementation of an IKS solver and learner. IKS solver/learner is a solver for probabilistic ASP (PASP) interpreted under the Incomplete Knowledge Semantics that has been updated to allow the use. Under this semantics, the logic program need not be stratified: they can accept multiple worlds (called answer sets). 

The semantics is described in our paper (Link to come soon)

This solver is based on the PASOCS solver for the Credal Semantic, and retains all of its features. On can find the original PASOCS solver in the pasocs branch, however the IKS solver offer new features

## Used third party tools

PASOCS makes uses at the moment of the following third party tools:

- [Clingo](https://github.com/potassco/clingo) ([License](https://github.com/potassco/clingo/blob/master/LICENSE.md))

## Prediction under the IKS

The IKS is implemented only for the full inference and not for sampling. (For details of sampling under the CS, see the pasocs branch readme). 

## Installation

### Executable 
We distribute the compiled version of the software, so to run it on your computer select the appropriate folder depending on your OS and run the command line `pasocsbin` in the folder which contains the executable with the libraries.
It is important to keep the dynamic libraries (.dll or .so) in the folder with the PASOCS executable.

At the moment, CUDA (because of Pytorch) needs to be installed on the machine to run. We will soon provide a SINGULARITY container to run seemlessly the solver.

### Library
To use the solver/learner as a C++ library, one needs to use the content of the library folder and add it to the project. Also, one needs to use in the same project Pytorch, as the libpasocs requires pytorch linking. If an example environment is needed, please contact us directly.

The C++ header file is given in the library folder.

## How do I use it ?

### Running IKS

To use PASOCS, run the command line: 

```
pasocsbin <pasocs_program_path> [--options...]
```
for linux

```
pasocsbin.exe <pasocs_program_path> [--options...]
``` 
for windows

`<pasp_program_path>` is the path to the PASOCS program, which can be followed by a list of options [--options...] that are described bellow. 

If you have cloned this repository, from the beginning the following command should work (placing you shell in the appropriate folder)

```
pasocsbin ../examples/simple.pasocs
```
for linux

```
pasocsbin.exe ../examples/simple.pasocs
``` 
for windows

(The pasocs programs can have any extensions, they are in essence text files.)

Here is an example of command line with options (detailed below)
```
pasocsbin test_lp_files/test.pasocs --n_cpus 4 -q wind(d1) -m 2 -n_min 10000 -ut 0.1 -hm 0.3
```

### How do I write a PASOCS program

A PASOCS program is a set of probabilistic facts, a logic program and a set of queries. All together they form all the information the PASOCS needs. Here is an example of simple PASOCS program:
```
0.4::sunshine(X) :- var1(X).
0.8::warm(X) :- var1(X).
0.3::tired(X) :- var1(X).
0.6::wind(X) :- var1(X).
0.4::wake_up.


% This is a comment
var1(d1).
var1(d2).
run(X) :- var1(X), wake_up, not walk(X), sunshine(X), not wind(X).
walk(X) :- var1(X), wake_up, not run(X).
sick(X) :- var1(X), run(X), tired(X).
sick(X) :- var1(X), walk(X), not warm(X).

#query(sick(d1)). [optim=1]
#query(sick(d2)). [optim=0.4]
```

It has one ground probabilistic fact `0.4::wake_up` and a 4 non-ground probabilistic facts (line 1-4). Then the logic program is defined and finally the queries. 
A commented line starts with a `%`, **Do not put comments on the same line as probabilistic facts or queries !**

#### Probabilistic facts

Probabilistic facts (PFs) can either be ground or non-ground. With non-ground PFs you are obliged to provide a body which gives to PASOCS instructions on how to ground it. Here are examples of declaration of probabilistic facts:

```
0.4::p.
0.4::p :- b1, not b2, b3.
0.4::p;0.6::q.
0.4::p;0.5::q.
0.4::p;0.6::q :- b1, not b2, b3.
0.4::p;0.5::q :- b1, not b2, b3, b4.
0.4::p;0.3::q;0.1::r.
0.4::p(X) :- r(X), q.
0.4::p(X);0.6::q(X) :- r(X).
0.4::p(X);0.6::q(Y) :- r(X), s(Y).
```

I have included here multiple examples to highlight different behaviors:
    
   - `0.4::p.` is the simplest way to declare a probabilistic fact, this means that `p` is true with probability 0.4 and false with probability 0.6.
   - `0.4::p :- b1, not b2, b3` is a simple probabilistic fact with a body. This means that when the condition `b1, not b2, b3` is met, `p` is true with probability 0.4. In practice, this is replaced by a clause `p :- b1, not b2, b3, pfi` and the PF `0.4::pfi` is added to the program.
   - `0.4::p;0.6::q` is an annotated disjunction. This means that this PF can take two values `p` or `q`, and takes value `p` with probability 0.4 and value `q` with probability 0.6. It is different than from writing `0.4::p. 0.6::q` as this would mean that `p` is true with probability 0.4 and `q` is true with probability 0.6 and it is possible for `p` and `q` to both be true at the same time. Whereas with an annotated disjunction it is either one or the other. In practice this is a syntactic sugar and is transformed into a set of probabilistic facts by PASOCS. 
   - `0.4::p;0.5::q.` is the same as above, but shows that in an annotated disjunction the probabilities need not sum to 1 (though they need to sum to a value lower than 1). Here with probability 0.1 neither `p` and `q` are true. 
   - `0.4::p;0.6::q :- b1, not b2, b3.` and `0.4::p;0.5::q :- b1, not b2, b3, b4.` are ground annotated disjunction with bodies
   - `0.4::p;0.3::q;0.1::r.` is an annotated disjunction with three elements in the disjunction.
   - `0.4::p(X) :- r(X), q.` is a non-ground probabilistic fact. This will be replaced by all possible groundings of p(X) and a independent probabilistic fact will be created for each grounding. Note that the ground body is kept when the grounding is done, so `q` will appear in each ground clause. 
   - `0.4::p(X);0.6::q(X) :- r(X).` is a non-ground annotated disjunction, where both `p` and `q` have the same variable.
   - `0.4::p(X);0.6::q(Y) :- r(X), s(Y).` is a non-ground annotated disjunction which shows that we can have multiple variables in the disjunction.

**Note that in these examples I have only used predicates of arity 1 or 0 but any predicate can be used.**

#### Logic Program

The logic program of the pasocs program is written in ASP and accepts all of Clingo's syntax. We can for example use choice rules `1{p(1), p(2)}2.` or aggregation. 

For a full documentation of Clingo's syntax please see [https://github.com/potassco/guide/releases](https://github.com/potassco/guide/releases).

#### Queries

Queries are what instructs PASOCS to count the probabilities. They are written as part of the program and multiple ones can be run at the same time. They accept evidence and disjunction queries. To declare a query, use write it as follows:

```
#query(my_query_here).
```

(Do not forget the `.` at the end).
Here are some examples of queries

```
#query(a, not b, c).
#query(not c).
#query(a|b:true).
#query(a|b:true, c:false).
#query(a, not b, not c|d:true).
#query(a;b).
#query(a;b|d:true).
#query(a;b, not c|d:true).
```

As shown, queries are form of the queries atoms and of evidence. Moreover, we can have disjunctive queries:

- `#query(a, not b, c).` asks what is the probability for `a` to be true, `b` to be false and `c` to the true at the same time.
- `#query(not c).` asks what is the probability for `c` to be false.
- `#query(a|b:true).` is a conditional query: what is the probability of `a` to be true given that `b` is true. 
- `#query(a|b:true, c:false).` asks what is the probability of `a` is to be true given that `b` is true and `c` is false.
- `#query(a, not b, not c|d:true).` asks what is the probability of `a` to be true, `b` to be false and `c` false (all three at the same time) given that `d` is true.
- `#query(a;b).` is a disjunctive query. It asks what is the probability that either `a` or `b` are true.
- `#query(a;b|d:true).` asks what is the probability of either `a` or `b` to be true given that `d` is true.
- `#query(a;b, not c|d:true).` asks what is the probability that either `a` is true or `b` is true and `c` is false given that `d` is true.

### Optimism and OWA operator

The IKS is based on the use of OWA operator and optimism. To set the optism, add the following syntax after a query: "[optim=i]". Examples:

```
#query(a, not b, c). [optim=1]
#query(not c). [optim=0]
#query(a|b:true). [optim=2]
#query(a|b:true). [optim=0.4]
```

Where `i` is either an optimism value in [0,1] or the keyword '2' that signals the solver to assign the same probability to all answer set (Weq in the paper).

The solver offers two different ways to generate the OWA operators:
 - Solving an optimization problem
 - Using matrices (used in the paper, Gaussian shaped OWA)

To set the method to use, use the following command line option: `--owa_method <INT>`, with <INT>={0,1}. 0=matrix, 1=optimisation

### Saving and loading the OWA operators 

It is possible to save/load the owa operators to avoid having to re-compute them each time. It will be save as a JSON format. To do so, use the following command line options:
  
  - `--save_owa <PATH>`: where <PATH> is a file path to save the file into.
  - `--load_owa <PATH>`: where <PATH> is a file path to load the file from.

### Command line options

When calling PASOCS, we would more often than not define a series of options. Here is an example:
```
pasocs test_lp_files/test.pasocs --n_cpus 4 -q wind(d1) -m 2 -n_min 10000 -ut 0.1 -hm 0.3
```
Most option require a user set value, which is given after the option name with a space separating it. Each option usually has a long name `--long_name` and a short one `-short`, both can be used interchangeably.

##### Algorithm Choice
- `--solving_mode`, `-sm` followed by a number `(0|1|2|3)`. It selects the solving algorithm to use: 0:Exact Inference, 1:Naive Sampling, 2:MH Sampling, 3: Gibbs Sampling. Default is 0.

##### Sampling stopping criterium
- - `--n_min_samples`, `-n_min` followed by a `int`, is the way to set the `min_samples` sampling stopping criteria. If the threshold of uncertainty is not reached, we sample at least `min_samples` for each query (queries may have different count due to evidence rejection). Set to -1 to deactivate this stopping criteria. Default is -1.
- `--n_max_samples`, `-n_max` followed by a `int`, is the way to set the `max_samples` sampling stopping criteria. Hard stop on the total number of samples taken. In case the evidence has a low probability, to avoid sampling forever. Can be set to -1 to deactivate it. Default is 50000.
- `--uncertainty_threshold`, `-ut` followed by a `double` \<1, is the way to set the `threshold` stopping criteria. Set to -1 to deactivate this stopping criteria. Default is 0.05. PASOCS will sample until each query has an uncertainty under this threshold (or some other stopping criteria is met).
- `--percentile`, `-p` followed by a `double` is the way to set the `percentile` parameter. As we estimate the uncertainty using a gaussian, we have to give the percentile we would like to use. For example, using the 97.5th percentile, we mean '95% of the possible values for the probability need to be under the uncertainty threshold'. Usual values are :  1. =>68%, 2=>95%, 3=>99.7%. Default is 1.96.

##### MCMC specific parameters
- `--burn`, `-b` followed by a `int` is the way to set the `burn` parameter. Number of samples to use at the burning step (for MCMC samplings). Default is 100.
- `--mh_prob`, `-mh` followed by a `double` is the way to set the `mh_prob` parameter. For MH MCMC sampling. It is the probability for each of the random variable to switch value at each sample. Prefer a lower value for stability. Default is 0.3.
- `--gibbs_block`, `-gb` followed by an `int` is the way to set the `gibbs_block` parameter. Block size for gibbs sampling. Default is 1.

##### Parallelization parameters
- `--n_cpus` followed by an `int`, number of threads/CPUs we would like the solver to use. Default is 1.
- `--sample_batch`, `-sb` followed by an `int`, Number of samples to run at once when doing sampling before computing the uncertainty and verifying the stopping criteria. Default is 100.

##### Hand Feeding
- `--query`, `-q` followed by a query string without `#query().` (for example just `not a, b|c:true`). It is in theory possible to do this but is not recommended and hasn't been tested much.

##### Others
- `--verbose`, `-v` followed by a `int`. Prints details of the computation. (1: Parsed program, ground pfs; 2: Detail of answer sets for each total choice when doing exact inference(to use with --n_cpus=1)
- `--help`, `-h`: displays the help page.

##### Output
- `output_path`, `-op` followed by a `path`. If specified, fill an output file with the results in CSV format. It will create the file if it doesn't exist.
 
### Documentation

A full documentation of PASOCS is in the works and will be release at a later date. 


## Examples

##### Simple example with non-ground probabilistic facts
The file `examples/simple.pasocs` is an example of simple PASOCS program with a negative loop and non-ground PFs.

##### Probability range
The file `examples/probability_range.pasocs` is an example of how to model knowledge of the type "I know that the probability  that there is traffic is between 0.6 and 0.8", by using an annotated disjunction and a choice rule

##### Dempster Schaffer Combination Rule
The file `examples/dempster_schaffer.pasocs` is an example of how to use PASOCS to compute the belief/plausibility functions (which are the lower and upper probabilities in PASOCS with this encoding). The evidence in the query is to account for the denominator in the combination rule of the DS.


## Contact me / Report a bug

Please do not hesitate to contact me directly for any inquiry. I am happy to discuss about improvements to the software and make changes accordingly. To report a bug, please open an issue in this repository or contact me directly by email (provided in the executable).

## How to cite PASOCS

If you publish a paper using PASOCS, you can cite this pre-print:

 - [PASOCS: A Parallel Approximate Solver for Probabilistic Logic Programs under the Credal Semantics](https://arxiv.org/abs/2105.10908)


## License
<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.


