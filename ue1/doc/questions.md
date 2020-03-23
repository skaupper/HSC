- Welche Genauigkeit kann mit N=16 Iterationen erreicht werden?

Theoretisch kann eine Genauigkeit von 2^-N erreicht werden.
In der Praxis kommen jedoch die Fehler durch Quantisierungseffekte (Rounding/Truncation) dazu.

- Ist die Erhöhung der Iterationen mit bestehender Schnittstelle sinnvoll?
Nein, weil die Schnittstelle eine Bitbreite von 16 hat. Somit ist ein LSB 2^-16 wert.
Wie oben angegeben, ist der Fehler des Cordics bereits kleiner als die LSB-Auflösung.
