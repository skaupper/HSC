# Elaboration

## Comparing loosely timed (LT) with cycle callable (CC)

|                           |        LT |         CC |
|---------------------------|----------:|-----------:|
| lines of code (code only) |       169 |        185 |
| degree of reality *       |       low |       high |
| delta cycles              | 2,008,192 | 49,218,926 |
| Execution time [s]        |           |            |
| &nbsp; &nbsp; &nbsp; --> write all             |     0.035 |      0.426 |
| &nbsp; &nbsp; &nbsp; --> read all              |     0.037 |      0.425 |
| &nbsp; &nbsp; &nbsp; --> random 10e6 wr+rd       |    17.447 |    211.745 |

\* degree of reality: \
LT does not have a representation of clock cycles. It handles actions
as self-contained abstract transactions. \
LT therefor represents a low level of reality, but is 10x faster than CC.

CC is a more exact representation of hardware reality. It can simulate
clock cycles. \
Because of that it is much slower, since many more context
switches, etc. have to be handled.

## Variants of the LT implementation

- wait a random number of waitstates after each access (single w.)
- wait accumulated waitstates after each test case (accum. w.)

|                           | single w. | accum. w.  |
|---------------------------|----------:|-----------:|
| delta cycles              | 2,008,192 |          3 |
| Execution time [s]        |           |            |
| &nbsp; &nbsp; &nbsp; --> write all             |     0.049 |      0.001 |
| &nbsp; &nbsp; &nbsp; --> read all              |     0.043 |      0.001 |
| &nbsp; &nbsp; &nbsp; --> random 10e6 wr+rd       |    17.447 |      0.639 |

## Analysis of delta cycles

It becomes obvious, that every call of wait() introduces a single delta cycle. \
The specific amount of wait time does not affect that.
Waiting 1 ns or 20 ms both only take 1 delta cycle.

There are 4096 registers in the memory.

Comparing both implementation variants:

|                           | single w. | accum. w.  |
|---------------------------|----------:|-----------:|
| Calls of wait()           |           |            |
| &nbsp; &nbsp; &nbsp; --> write all             |      4096 |      1 |
| &nbsp; &nbsp; &nbsp; --> read all              |      4096 |      1 |
| &nbsp; &nbsp; &nbsp; --> random 10e6 wr+rd       |   2000000 |      1 |
| **Sum**                                        | **2008192**|   **3** |

<style>
table {
  border-collapse: collapse;
}

td, th {
  border: 1px solid #999;
  padding: 0.5rem;
  text-align: left;
}
</style>
