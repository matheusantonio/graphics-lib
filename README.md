# graphics-lib

Biblioteca para processamento gráfico para a disciplina de Computação Gráfica (2019.1)

A biblioteca stb_image_write é necessária para a execução:
https://github.com/nothings/stb

versão utilizada: 1.13

Por fazeres que foram pulados:
- Recorte em 3D (até que apareça algum problema, pronto)
- Verificação do zbuffer antes de desenhar pixel em 3d (funções draw line e draw triangle refeitas para vec3)
- Corrigir a função de ler de um arquivo obj (falta testar)

Tarefas que faltam:

- triangulos 3d usando recorte (slide 10)
- desenhar wireframe (slide 11)
- poligonização de superfície implícita (slide 11)
- desenhar modelo a partir de um arquivo obj (slide 12)