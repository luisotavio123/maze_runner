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
// Função para carregar o labirinto de um arquivo
Position load_maze(const std::string& file_name) {
    std::ifstream arquivoE(file_name);
    if (!arquivoE.is_open()) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return {0, 0};
    }

    // Leia a primeira linha para obter o número de linhas e colunas
    arquivoE >> num_rows >> num_cols;
    // Redimensione a matriz 'maze' de acordo
    maze.resize(num_rows);
    for (int i = 0; i < num_rows; ++i) {
        maze[i].resize(num_cols);
    }

    // Limpar o caractere de nova linha após a leitura das dimensões
    arquivoE.ignore();

    // Leitura do conteúdo do labirinto, caractere por caractere
    char caractere;
    Position posicao_inicial = {-1, -1};  
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            arquivoE.get(caractere);
            maze[i][j] = caractere;

            // Encontre e retorne a posição inicial 'e'
            if (caractere == 'e') {
                posicao_inicial = {i, j};  
            }
        }
        arquivoE.ignore();  // Ignora a quebra de linha após cada linha do labirinto
    }

    // Trate possíveis erros
    if (posicao_inicial.row == -1) {
        std::cerr << "Erro: Posição inicial 'e' não encontrada no labirinto." << std::endl;
        return {0, 0}; 
    }

    // Feche o arquivo após a leitura
    arquivoE.close();
    return posicao_inicial;
}


// Função para imprimir o labirinto
void print_maze() {
    // TODO: Implemente esta função
    // 1. Percorra a matriz 'maze' usando um loop aninhado
    // 2. Imprima cada caractere usando std::cout
    // 3. Adicione uma quebra de linha (std::cout << '\n') ao final de cada linha do labirinto
    for (int i = 0; i <num_rows; i++){
        for(int j = 0; j < num_cols; j++){
           std:: cout << maze [i][j];
        }
        std:: cout << '\n';
    } 
}

// Função para verificar se uma posição é válida
bool is_valid_position(int row, int col) {
    // TODO: Implemente esta função
    // 1. Verifique se a posição está dentro dos limites do labirinto
    if (row >= 0 && row < num_rows && col >= 0 && col < num_cols){
        if(maze[row][col] == 'x' || maze[row][col] == 's' ){
            return true;
        }
    }
    //    (row >= 0 && row < num_rows && col >= 0 && col < num_cols)
    // 2. Verifique se a posição é um caminho válido (maze[row][col] == 'x')
    // 3. Retorne true se ambas as condições forem verdadeiras, false caso contrário

    return false; // Placeholder - substitua pela lógica correta
}

// Função principal para navegar pelo labirinto
bool walk(Position pos) {
    // Verifique se a posição atual é válida
    

    // Verifique se a posição atual é a saída
    if (maze[pos.row][pos.col] == 's') {
        
        return true; // Encontrou a saída!
    }

    // Marque a posição atual como visitada
    maze[pos.row][pos.col] = '.'; 

    // Chame print_maze() para mostrar o estado atual do labirinto
    print_maze();
    
    // Adicione um pequeno atraso para visualização
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    
    Position direcoes[] = {
        {pos.row - 1, pos.col},  // Cima
        {pos.row + 1, pos.col},  // Baixo
        {pos.row, pos.col - 1},  // Esquerda
        {pos.row, pos.col + 1}   // Direita
    };

    for (const Position& nova_pos : direcoes) {
        if (is_valid_position(nova_pos.row, nova_pos.col)) {
            // Coloque a nova posição na pilha de posições válidas
            valid_positions.push(nova_pos);
        }
    }

    
    while (!valid_positions.empty()) {
        // Pegue a próxima posição válida
        Position prox_pos = valid_positions.top();
        valid_positions.pop();

       
        if (walk(prox_pos)) {
            return true;  
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
