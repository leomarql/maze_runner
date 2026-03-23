## Exercícios

### Exercício Computacional 1

Neste exercício você deve desenvolver um programa para encontrar a saída de um labirinto.

### Especificações

- O labirinto é representado por uma matriz em um arquivo de texto.
- A primeira linha do arquivo contém o número de linhas e colunas do labirinto.
- As linhas seguintes representam o conteúdo do labirinto:
  - 'x': caminho válido
  - '#': parede
  - 'e': entrada do labirinto
  - 's': saída do labirinto
- Para cada nova posição explorada, o programa deve imprimir o labirinto atualizado:
  - '.': posições já exploradas
  - 'o': posição corrente

### Condições de Término
O programa deve terminar quando:
- A saída for encontrada
- Não existirem mais posições a serem exploradas

### Implementação
Um esqueleto do código com as principais funções está disponível no repositório. Você pode modificá-lo conforme necessário.

### Exercício Computacional 2

Neste exercício voces irao modificar o código gerado no exercício anterior de forma que, durante a exploração do labirinto, toda vez que seja encontrado mais de um possível caminho, a thread que está realizando a exploração, crie uma thread adicional para explorar cada um dos demais caminhos adicionais e  continue a explorar um destes.

Ou seja, considere um cenário em que, durante a exploração existam dois caminhos possíveis a serem explorados, um dos caminhos deve ser explorado pela thread que está executando a função de exploração e uma thread adicional deve ser criada para explorar o outro caminho. 

Vocês devem usar a biblioteca de threads padrão de C++ vista em sala de aula.

---

## Codespaces

Vocês podem usar o [Github Codespaces](https://github.com/features/codespaces) como IDE.

O Codespaces é um recurso oferecido pelo GitHub que permite aos desenvolvedores desenvolver código na nuvem usando o Visual Studio Code ou navegadores da Web modernos, sem necessidade de instalar nada no seu computador. O github da direito 60 horas/mensais de uso gratuito do Codespaces para desenvolvedores individuais no GitHub com uma conta gratuita.

Aqui está um passo a passo de como usar o Codespaces para desenvolver a partir de um repositório:

1. Faça login na sua conta do GitHub. Caso não tenha uma conta, crie uma gratuitamente acessando este [link](https://github.com/join). 

2. Navegue até o repositório que deseja usar e clique no botão “Fork” no canto superior direito da página para criar uma cópia do repositório na sua conta do GitHub.

3. Clique no botão “Code” e selecione “Create codespace on main”.

4. Uma nova aba será aberta com a versão web do visual studio code contendo o seu projeto. Ao clicar no arquivo `maze_runner.cpp` um popup será aberto perguntando se voce deseja instalar as extensões para C/C++, responda não.

5. Após esse passo, outro popup aparecerá para você escolher o compilador a ser usado (escolha o GCC 11).

5. Quando o ambiente estiver pronto, você poderá começar a editar o código diretamente no navegador.

6. Para compilar o código, clique no botão "Build" da barra inferior. Essa barra também tem ícones para debugar (ícone "bug") e executar (ícone "play") o projeto.

Crédito: atividade preparada pelo prof. André Paim Lemos.
