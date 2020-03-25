- Welche Genauigkeit kann mit N=16 Iterationen erreicht werden?

Theoretisch kann eine Genauigkeit von 2^-(N-1) erreicht werden.
In der Praxis kommen jedoch die Fehler durch Quantisierungseffekte (Rounding/Truncation) dazu.


- Ist die Erhöhung der Iterationen mit bestehender Schnittstelle sinnvoll?

Erhöht man die Anzahl der Iterationen auf 17, werden zumindest theoretisch alle 16 Nachkommabit ausgenutzt.
Da wir mit begrenzter Genauigkeit arbeiten, führt erst eine Erhöhung auf 18 Iterationen zu einem maximalen Fehler von
2**-16 (1 LSB).
