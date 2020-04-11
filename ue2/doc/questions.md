# Ausarbeitung

## Gegenüberstellung von loosely timed (LT) und cycle callable (CC)

|                           |        LT |         CC |
|---------------------------|----------:|-----------:|
| lines of code (code only) |       169 |        185 |
| degree of reality *       |       low |       high |
| delta cycles              | 2,008,192 | 49,218,926 |
| Execution time [s]        |           |            |
| &nbsp; &nbsp; &nbsp; --> write all             |     0.035 |      0.426 |
| &nbsp; &nbsp; &nbsp; --> read all              |     0.037 |      0.425 |
| &nbsp; &nbsp; &nbsp; --> random 10e6 w/r       |    17.447 |    211.745 |

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

\* degree of reality: \
LT does not have a representation of clock cycles. It handles actions
as self-contained abstract transactions. \
LT therefor represents a low level of reality, but is 10x faster than CC.

CC is a more exact representation of hardware reality. It can simulate
clock cycles. \
Because of that it is much slower, since many more context
switches, etc. have to be handled.
