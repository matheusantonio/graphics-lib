# graphics-lib
*Biblioteca para processamento gráfico para a disciplina de Computação Gráfica (2019.1)*

A biblioteca stb_image_write é necessária para a execução (A  biblioteca stb_image foi usada para adicionar texturas e se encontra no mesmo link):

https://github.com/nothings/stb

versão utilizada: 1.13

- main.cpp renderiza a cena utilizando a biblioteca desenvolvida durante o curso (make glib)
- open.cpp renderiza a cena através do opengl de maneira simplificada (make openglib)
- openplus.cpp renderiza a cena de maneira incrementada, com texturas e efeitos de iluminação (make openplus)

Observações:
* Existe um travamento ocorrendo em algumas execuções de glib. Ela printa na tela quando esse travamento ocorre (quando when drawning mostra um valor aproximadamente bilhões de vezes maior ou menor que o outro), dê cntrl C e execute novamente. Normalmente, ele executa direto mais do que trava. Ela irá salvar 10 frames da cena, que foram usados para gerar o arquivo boat.gif. É importante ter uma pasta chamada "figuras".
* A interação com o usuário feita em plus é alternar entre dia e noite através da barra de espaço. O céu e os objetos irão escurecer conforme a barra é apertada e tornarão a ficar claros após voltar a amanhecer.
