# Water Ripple Effect (WRE)

### WHAT IS THIS?
----
Esta é uma biblioteca feita em C++ 20 que simula ondulações de água em uma superfície, você pode tocar a água em qualquer ponto, e ver as ondas se espalharem e refletirem nos limites da superficie.

### HOW DOES IT WORK?
----
A ideia básica é você possuir dois arrays para manter o estado da água, um para o estado atual e o outro para manter o estado do frame anterior.

Aqui está o pseudo-código de como o algoritmo funciona:

```
damping = some non-integer between 0 and 1

begin loop 
    for every non-edge element:
    loop
        Buffer2(x, y) = (Buffer1(x-1,y)
                         Buffer1(x+1,y)
                         Buffer1(x,y+1)
                         Buffer1(x,y-1)) / 2 - Buffer2(x,y)

        Buffer2(x,y) = Buffer2(x,y) * damping
    end loop

    Display Buffer2
    Swap the buffers 

end loop
```

Observe que neste caso o Buffer1 contém o estado da agua "atual", e o
Buffer2 contém o estado da àgua do frame anterior.

Seguindo esse fluxo:
```
Buffer 2 -> Buffer1 -> Buffer2 -> ...
```

Dessa forma, o Buffer2 possui informações sobre a velocidade vertical da onda.
```
Velocity(x, y) = -Buffer2(x, y)
```

Para que as ondas se propaguem é necessário que os buffers sejam "suavizados"
a cada frame.

Seja SmoothFn(x,y) nossa função de suavização, temos que:

```
SmoothFn(x,y) = (Buffer1(x-1, y) +
                 Buffer1(x+1, y) +
                 Buffer1(x, y-1) +
                 Buffer1(x, y+1)) / 4
```

Isso é basicamente a média dos 4 pixels vizinho do ponto (x, y), também chamado de ```4-neighborhood```.

```
4-neighborhood: {(x − 1, y), (x, y + 1), (x + 1, y), (x, y − 1)}
```

Nessa biblioteca utilizamos ```8-neighborhood``` pois resulta em uma suavização melhor da propagação da onda na superficie.

![neighborhood](https://www.cs.auckland.ac.nz/courses/compsci773s1c/lectures/ImageProcessing-html/neighbourhoods.gif).

Agora precisamos calcular a nova altura da onda, a multiplicação por 2 reduz o efeito da velocidade:

```
new_height = SmoothFn(x,y)*2 + Velocity(x,y)
```

Por último, as ondulações devem perder energia, por isso são amortecidas por um fator de amortecimento que chamamos de ```damping```:
```
new_height *= damping
```

Isso pode ser otimizado para:
```
new_height = new_height - (new_height >> N)
```
No qual N é o $\log_2(2^N)$, ou seja, é o logaritmo base 2 de alguma potencia de 2.

```ex: 32 == 2⁵, portanto o N = 5;```

Veja a seção de referências, ela contém os links para os sites que explicam como todo esse algoritmo funciona em detalhes.

### REFERENCES
---
- https://web.archive.org/web/20160418004149/http://freespace.virgin.net/hugo.elias/graphics/x_water.htm
- https://web.archive.org/web/20080112065719/http://www.gamedev.net/reference/programming/features/water/page2.asp