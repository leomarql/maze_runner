#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <thread>
#include <chrono>
#include <mutex>

// Representação do labirinto
using Maze = std::vector<std::vector<char>>;

// Estrutura para representar uma posição no labirinto
struct Position {
    int row;
    int col;
};

// Variáveis globais
Maze maze;
int num_rows;
int num_cols;
// std::stack<Position> valid_positions; Removida para a implementação do exercício 2
// Novas variáveis globais para o controle de concorrência
std::mutex mtx; 
bool exit_found_global = false;

// Função para carregar o labirinto de um arquivo
Position load_maze(const std::string& file_name) {
    // TODO: Implemente esta função seguindo estes passos:
    // 1. Abra o arquivo especificado por file_name usando std::ifstream
    // 2. Leia o número de linhas e colunas do labirinto
    // 3. Redimensione a matriz 'maze' de acordo (use maze.resize())
    // 4. Leia o conteúdo do labirinto do arquivo, caractere por caractere
    // 5. Encontre e retorne a posição inicial ('e')
    // 6. Trate possíveis erros (arquivo não encontrado, formato inválido, etc.)
    // 7. Feche o arquivo após a leitura
    //==================================================================================================
    std::ifstream file (file_name);

    // 6. Trate possíveis erros
    if (!file.is_open()) {
        std::cerr << "Erro: Não foi possível abrir o arquivo " << file_name << std::endl;
        return {-1, -1};
    }

    // 2. Leia o número de linhas e colunas
    file >> num_rows >> num_cols;
    
    // 3. Redimensione a matriz
    maze.resize(num_rows, std::vector<char>(num_cols));

    Position start_pos = {-1, -1};

    // 4. Leia o conteúdo do labirinto
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            file >> maze[i][j];
            
            // 5. Encontre a posição inicial
            if (maze[i][j] == 'e') {
                start_pos = {i, j};
            }
        }
    }

    // 7. Feche o arquivo
    file.close();
    
    return start_pos;
    
    return {-1, -1}; // Placeholder - substitua pelo valor correto
}

// Função para imprimir o labirinto
void print_maze() {
    // TODO: Implemente esta função
    // 1. Percorra a matriz 'maze' usando um loop aninhado
    // 2. Imprima cada caractere usando std::cout
    // 3. Adicione uma quebra de linha (std::cout << '\n') ao final de cada linha do labirinto
    //==================================================================================================
    // Limpa o terminal para dar o efeito de animação
    system("clear"); 
    
    // 1 e 2. Percorra e imprima a matriz
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            std::cout << maze[i][j];
        }
        // 3. Quebra de linha ao final de cada linha do labirinto
        std::cout << '\n';
    }
}

// Função para verificar se uma posição é válida
bool is_valid_position(int row, int col) {
    // TODO: Implemente esta função
    // 1. Verifique se a posição está dentro dos limites do labirinto
    //    (row >= 0 && row < num_rows && col >= 0 && col < num_cols)
    // 2. Verifique se a posição é um caminho válido (maze[row][col] == 'x')
    // 3. Retorne true se ambas as condições forem verdadeiras, false caso contrário
    //==================================================================================================
    // 1. Verifica os limites do labirinto
    if (row >= 0 && row < num_rows && col >= 0 && col < num_cols) {
        // 2. Verifica se é caminho livre ('x') ou a saída ('s')
        if (maze[row][col] == 'x' || maze[row][col] == 's') {
            return true;
        }
    }
    // 3. Retorna false caso contrário
    return false;
}

// Função principal para navegar pelo labirinto
void walk(Position pos) {
    // TODO: Implemente a lógica de navegação aqui
    // 1. Marque a posição atual como visitada (maze[pos.row][pos.col] = '.')
    // 2. Chame print_maze() para mostrar o estado atual do labirinto
    // 3. Adicione um pequeno atraso para visualização:
    //    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // 4. Verifique se a posição atual é a saída (maze[pos.row][pos.col] == 's')
    //    Se for, retorne true
    // 5. Verifique as posições adjacentes (cima, baixo, esquerda, direita)
    //    Para cada posição adjacente:
    //    a. Se for uma posição válida (use is_valid_position()), adicione-a à pilha valid_positions
    // 6. Enquanto houver posições válidas na pilha (!valid_positions.empty()):
    //    a. Remova a próxima posição da pilha (valid_positions.top() e valid_positions.pop())
    //    b. Chame walk recursivamente para esta posição
    //    c. Se walk retornar true, propague o retorno (retorne true)
    // 7. Se todas as posições foram exploradas sem encontrar a saída, retorne false
    //=================================================================================================
    // 1. Condição de parada global: se alguma thread já achou a saída, as outras podem parar.
    if (exit_found_global) return;

    // 2. Trava o Mutex para leitura/escrita segura na matriz
    mtx.lock();
    
    // Verifica se outra thread já passou por aqui ou se é a saída
    if (maze[pos.row][pos.col] == '.' || maze[pos.row][pos.col] == 'o') {
        mtx.unlock();
        return;
    }

    if (maze[pos.row][pos.col] == 's') {
        exit_found_global = true; // Avisa todas as threads que acabou
        mtx.unlock();
        return;
    }

    // Marca posição corrente e imprime
    maze[pos.row][pos.col] = 'o';
    print_maze();
    
    // Destrava o Mutex ANTES do sleep!
    // Se você dormir com o mutex travado, as outras threads vão ficar travadas
    // esperando você acordar, e o programa voltará a ser sequencial.
    mtx.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Trava de novo só para atualizar a marcação
    mtx.lock();
    maze[pos.row][pos.col] = '.';
    mtx.unlock();

    // 3. Mapeamento de caminhos adjacentes (bifurcações)
    std::vector<Position> caminhos_possiveis;
    
    mtx.lock(); // Protege a leitura com is_valid_position
    if (is_valid_position(pos.row - 1, pos.col)) caminhos_possiveis.push_back({pos.row - 1, pos.col}); // Cima
    if (is_valid_position(pos.row + 1, pos.col)) caminhos_possiveis.push_back({pos.row + 1, pos.col}); // Baixo
    if (is_valid_position(pos.row, pos.col - 1)) caminhos_possiveis.push_back({pos.row, pos.col - 1}); // Esquerda
    if (is_valid_position(pos.row, pos.col + 1)) caminhos_possiveis.push_back({pos.row, pos.col + 1}); // Direita
    mtx.unlock();

    // 4. A Lógica de Criação de Threads exigida no Exercício 2
    if (!caminhos_possiveis.empty()) {
        std::vector<std::thread> novas_threads;

        // Se houver mais de 1 caminho, cria threads para do caminho 1 em diante
        for (size_t i = 1; i < caminhos_possiveis.size(); ++i) {
            novas_threads.push_back(std::thread(walk, caminhos_possiveis[i]));
        }

        // A thread atual sempre assume o primeiro caminho (índice 0)
        walk(caminhos_possiveis[0]);

        // 5. Sincronização Final (Join)
        // A thread atual deve esperar que as threads filhas que ela criou terminem
        // antes de finalizar a sua própria execução.
        for (auto& t : novas_threads) {
            if (t.joinable()) {
                t.join();
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_labirinto>" << std::endl;
        return 1;
    }

    Position initial_pos = load_maze(argv[1]);
    if (initial_pos.row == -1 || initial_pos.col == -1) {
        std::cerr << "Posição inicial não encontrada no labirinto." << std::endl;
        return 1;
    }

    // Inicia a primeira thread (a main thread) na posição inicial
    walk(initial_pos);

    // Avalia o resultado através da variável global
    if (exit_found_global) {
        std::cout << "\nSaída encontrada com sucesso pelas threads!" << std::endl;
    } else {
        std::cout << "\nNão foi possível encontrar a saída." << std::endl;
    }

    return 0;
}

// Nota sobre o uso de std::this_thread::sleep_for:
// 
// A função std::this_thread::sleep_for é parte da biblioteca <thread> do C++11 e posteriores.
// Ela permite que você pause a execução do thread atual por um período especificado.
// 
// Para usar std::this_thread::sleep_for, você precisa:
// 1. Incluir as bibliotecas <thread> e <chrono>
// 2. Usar o namespace std::chrono para as unidades de tempo
// 
// Exemplo de uso:
// std::this_thread::sleep_for(std::chrono::milliseconds(50));
// 
// Isso pausará a execução por 50 milissegundos.
// 
// Você pode ajustar o tempo de pausa conforme necessário para uma melhor visualização
// do processo de exploração do labirinto.
