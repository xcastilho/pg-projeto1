Projeto 1 de PG.
===========
Como compilar:

É necessário ter a biblioteca Allegro com versão maior ou igual a 5.0 instalada
para compilar o código-fonte. As bibliotecas que devem ser linkadas na
compilação são: 

liballegro.a (no Linux) ou liballegro.dll.a (no Windows)
liballegro_dialog.a (no Linux) ou liballegro_dialog.dll.a (no Windows)
liballegro_primitives.a (no Linux) ou liballegro_primitives.dll.a (no Windows)

===========
Como instalar o Allegro:

Baixe o Allegro. (alleg.sourceforge.com) A versão sendo utilizada é a 5.0.9.
Vá no diretório em que o Allegro foi descompactado, crie um diretório chamado build.
Baixe o cmake. (procurar cmake no google e baixar uma versão apropriada a 
seu sistema operacional)
no diretório build, rode o seguinte comando: No Linux, "cmake ..", no Windows, "cmake .. -G "MinGW Makefiles""
ainda no diretório build, rode os seguintes comandos:

make
make install

===========
Instruções de uso:

Clicar com o botão esquerdo do mouse adiciona pontos de controle.
Clicar com o botão direito sobre um ponto de controle o remove.
Clicar e arrastar pontos de controle edita a curva.
Tab: desabilita ou habilita a visualização dos pontos de controle
P: desabilita ou habilita a visualização da poligonal de controle
C: desabilita ou habilita a visualização da curva
Q: aumenta o número de avaliações (segurar aumenta rapidamente)
W: reduz o número de avaliações (segurar reduz rapidamente)
Quando o programa é inicializado, o número de avaliações é 10.
A: aumenta o valor de t para o qual as derivadas serão avaliadas
S: diminui o valor de t para o qual as derivadas serão avaliadas
Quando o programa é inicializado, as derivadas são calculadas para t = 0.5
(Tanto para o A quanto para o S, segurar o botão altera o valor rapidamente.)

===========

Legenda:

Linha vermelha: Curva de Bézier
Linha azul-clara: Poligonal de controle
Linha azul: Vetor da primeira derivada
Linha verde: Vetor da segunda derivada
Pontos brancos: Pontos de controle
