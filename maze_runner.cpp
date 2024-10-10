#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <thread>
#include <chrono>

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
std::stack<Position> valid_positions;

// Função para imprimir o labirinto após carregar
void debug_print_maze() {
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            std::cout << maze[i][j];
        }
        std::cout << '\n';
    }
}

// Função para carregar o labirinto de um arquivo
Position load_maze(const std::string& file_name) {
    std::ifstream arquivoE(file_name);
    if (!arquivoE.is_open()) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return {0, 0};
    }

    // Leia a primeira linha para obter o número de linhas e colunas
    arquivoE >> num_rows >> num_cols;
    maze.resize(num_rows);
    for (int i = 0; i < num_rows; ++i) {
        maze[i].resize(num_cols);
    }

    arquivoE.ignore();

    char caractere;
    Position posicao_inicial = {-1, -1};  
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            arquivoE.get(caractere);
            maze[i][j] = caractere;
            if (caractere == 'e') {
                posicao_inicial = {i, j};  
            }
        }
        arquivoE.ignore();  
    }

    if (posicao_inicial.row == -1) {
        std::cerr << "Erro: Posição inicial 'e' não encontrada no labirinto." << std::endl;
        return {0, 0}; 
    }

    arquivoE.close();
    return posicao_inicial;
}

// Função para imprimir o labirinto
void print_maze() {
    for (int i = 0; i < num_rows; i++){
        for(int j = 0; j < num_cols; j++){
           std::cout << maze[i][j];
        }
        std::cout << '\n';
    } 
}

// Função para verificar se uma posição é válida
bool is_valid_position(int row, int col) {
    if (row >= 0 && row < num_rows && col >= 0 && col < num_cols) {
        if (maze[row][col] == 'x' || maze[row][col] == 's') {
            return true;
        }
    }
    return false;
}

// Função principal para navegar pelo labirinto
bool walk(Position pos) {
    if (maze[pos.row][pos.col] == 's') {
        return true; // Encontrou a saída!
    }

    maze[pos.row][pos.col] = '.'; 
    print_maze();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    Position direcoes[] = {
        {pos.row - 1, pos.col},  // Cima
        {pos.row + 1, pos.col},  // Baixo
        {pos.row, pos.col - 1},  // Esquerda
        {pos.row, pos.col + 1}   // Direita
    };

    std::vector<std::thread> threads;

    for (const Position& nova_pos : direcoes) {
        if (is_valid_position(nova_pos.row, nova_pos.col)) {
            if (valid_positions.empty()) {
                valid_positions.push(nova_pos);
            } else {
                // Crie uma nova thread para explorar o caminho adicional
                threads.emplace_back(walk, nova_pos);
            }
        }
    }

    while (!valid_positions.empty()) {
        Position prox_pos = valid_positions.top();
        valid_positions.pop();
        if (walk(prox_pos)) {
            for (auto& th : threads) {
                if (th.joinable()) {
                    th.join();
                }
            }
            return true;
        }
    }

    for (auto& th : threads) {
        if (th.joinable()) {
            th.join();
        }
    }

    return false;
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

    bool exit_found = walk(initial_pos);

    if (exit_found) {
        std::cout << "Saída encontrada!" << std::endl;
    } else {
        std::cout << "Não foi possível encontrar a saída." << std::endl;
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
