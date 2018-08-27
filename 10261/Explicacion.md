f(k1, k2, i) =  { si l[i] + k1 > K y l[i] + k2 > K entonces i - 1
				{ si l[i] + k1 <= K y l[i]+ k2 > K entonces f(k1 + l[i], 	k2, i+1)
				{ si l[i] + k2 <= K y l[i] + k1 > K entonces f(k1, k2 + l[i], i+1)
				{ sino max(f(k1 + l[i], k2, i+1), f(k1, k2 + l[i], i+1))

f(k1, k2, i) = la maxima cant de autos que puedo poner, si ya puse i-1 y tengo ocupado k1 del primer carril, y k2 del segundo

Como las capacidades las mido en cm, 0 <= k1/k2 <= 30000, ademas, trivialmente se ve que a lo sumo puedo usar 30000\*2/100 autos 0 <= i <= 600

Luego, puedo acotar la cant de instancias por 30000\*30000\*600 = 54\*10^10

Este numero es demasiado grande como para calcular todas las subinstancias del problema, es decir que incluso usando PD y calculando cada instancia una sola vez, tardaria demasiado.

Pero notemos que si f(k1, k2, i) es una instancia a la que puedo llegar a traves de la recurrencia, entonces k1 + k2 = sum l[j], j \in [1..i), entonces la instancia (k1, k2, i) la puedo codificar como (k1, i) (y calcular k2 a partir de estos), luego, la cant de subinstancias (validas) son 30000\*600 = 1,8\*10^7, un numero manejable.





[1] Cabe destacar que si utilizamos la primera recursion, la que toma instancias de la pinta (k1, k2, i) y la hubiesemos implementado con la tecnica top down, utilizando algun diccionario bueno para claves ralas (map) la cantidad de subinstancias que hubiese calculado (y guardado) hubiesen sido acotadas por 1,8\*10^7.