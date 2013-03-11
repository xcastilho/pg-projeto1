Projeto 1 de PG.
===========
Como compilar:

� necess�rio ter a biblioteca Allegro com vers�o maior ou igual a 5.0 instalada
para compilar o c�digo-fonte. As bibliotecas que devem ser linkadas na
compila��o s�o: 

liballegro.a (no Linux) ou liballegro.dll.a (no Windows)
liballegro_dialog.a (no Linux) ou liballegro_dialog.dll.a (no Windows)
liballegro_primitives.a (no Linux) ou liballegro_primitives.dll.a (no Windows)

===========
Como instalar o Allegro:

Baixe o Allegro. (alleg.sourceforge.com) A vers�o sendo utilizada � a 5.0.9.
V� no diret�rio em que o Allegro foi descompactado, crie um diret�rio chamado build.
Baixe o cmake. (procurar cmake no google e baixar uma vers�o apropriada a 
seu sistema operacional)
no diret�rio build, rode o seguinte comando: No Linux, "cmake ..", no Windows, "cmake .. -G "MinGW Makefiles""
ainda no diret�rio build, rode os seguintes comandos:

make
make install

===========
Instru��es de uso:

Clicar com o bot�o esquerdo do mouse adiciona pontos de controle.
Clicar com o bot�o direito sobre um ponto de controle o remove.
Clicar e arrastar pontos de controle edita a curva.
Tab: desabilita ou habilita a visualiza��o dos pontos de controle
P: desabilita ou habilita a visualiza��o da poligonal de controle
C: desabilita ou habilita a visualiza��o da curva
Q: aumenta o n�mero de avalia��es (segurar aumenta rapidamente)
W: reduz o n�mero de avalia��es (segurar reduz rapidamente)
Quando o programa � inicializado, o n�mero de avalia��es � 10.
A: aumenta o valor de t para o qual as derivadas ser�o avaliadas
S: diminui o valor de t para o qual as derivadas ser�o avaliadas
Quando o programa � inicializado, as derivadas s�o calculadas para t = 0.5
(Tanto para o A quanto para o S, segurar o bot�o altera o valor rapidamente.)

===========

Legenda:

Linha vermelha: Curva de B�zier
Linha azul-clara: Poligonal de controle
Linha azul: Vetor da primeira derivada
Linha verde: Vetor da segunda derivada
Pontos brancos: Pontos de controle
