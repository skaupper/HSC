## Überlegen Sie ob für das Interrupt-Handling im Target und der CPU `SC_METHOD` oder `SC_THREAD` verwenden.

Der entsprechende Interrupt-Handler soll immer dann ausgeführt werden, wenn das Interrupt-Signal eine steigende Flanke aufweist. Damit nicht mit `SC_THREAD` ein Busy-Waiting implementiert werden muss, wurde `SC_METHOD` verwendet, da dadurch die Verwendung einer Sensitivity-List möglich ist.

`SC_METHOD` kann durch eine Sensitivity-List immer wieder aufgerufen werden. Ein `SC_THREAD` kann hingegen nur einmal durchlaufen.
