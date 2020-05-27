## Überlegen Sie mit Rücksicht auf eine spätere Implementierung in Hardware, an welcher Stelle – in Software oder in Hardware – der Faktor K multipliziert werden sollte. Begründen Sie Ihre Entscheidung.

Das ist ein Trade-off zwischen Ausführungszeit und Ressourcen (wie immer..).
Wir entscheiden uns für Ausführungszeit, somit wird es in Hardware implementiert. Eine Multiplikation in Hardware ist üblicherweise in einem Takt fertig.

Würde die Multiplikation in Software laufen, müsste man 2 Register lesen, und diese dann erst zu einem Ergebnis multiplizieren. Das wäre in diesem Fall eine Verfehlung der Anwendung. Diese besteht ja nur darin, x und y schnell zu berechnen. Würde man K in Software multiplizieren, könnte man ja gleich sin() und cos() auch verwenden..

## Überlegen Sie sich wo bzw. wie die Synchronisation eingebaut werden kann und begründen Sie Ihre Umsetzung.

Die Synchronisation muss vom TLM Modul übernommen werden. Hierbei ist wichtig, dass beide IPs gleichzeitig gestartet werden, und darauf gewartet wird, bis auch beide fertig sind. Eine IP ist "fertig" sobald am entsprechenden `oRdy` eine steigende Flanke erkannt wird.
