<!-- Logo do jogo -->
![image](https://github.com/user-attachments/assets/c771257a-ea26-4466-976f-64b4b926992b)

# PaCroissant-Lógico — Projeto de Programação Imperativa e Funcional

## História

Em um mundo de padarias assombradas, um corajoso croissant mágico embarca em uma aventura lógica: ele deve capturar tabelas-verdade de expressões lógicas enquanto foge de fantasmas famintos para se manter vivo.

## Informações Acadêmicas

- **Disciplina:** Programação Imperativa e Funcional — 2025.1  
- **Instituição:** CESAR School

## Integrantes

- Gustavo Queque ([github.com/gqueque](https://github.com/gqueque))  
- Maria Júlia ([github.com/mmajumagalhaes](https://github.com/mmajumagalhaes))  
- Luiz Felipe ([github.com/LuizFelipexDD](https://github.com/LuizFelipexDD))  

---

## Sobre o Jogo

**PaCroissant-Lógico** é um jogo inspirado no clássico Pac-Man, com um objetivo educacional.  
O jogador deve resolver expressões lógicas com base na tabela-verdade, coletando os itens **V (verdadeiro)** e **F (falso)** na ordem correta, enquanto foge de fantasmas famintos.

### Objetivo

- Coletar os valores da tabela-verdade da expressão exibida no topo da tela.  
- Coletar um item fora da ordem resulta na perda de 1 vida e reinício da sequência.  
- Colisões com fantasmas também causam perda de vida.  
- O jogador começa com 3 vidas.

---

## Controles

| Tecla | Ação         |
|-------|--------------|
| W     | Cima         |
| A     | Esquerda     |
| S     | Baixo        |
| D     | Direita      |
| Q     | Sair do jogo |

---

## Regras

- Cada expressão lógica possui 4 resultados (para as 4 combinações possíveis de A e B).  
- Os itens V e F são espalhados no mapa.  
- O jogador deve coletar os valores na ordem correta.  
- A cada acerto:  
  - Ganha 100 pontos × multiplicador atual.  
  - O multiplicador aumenta a cada acerto consecutivo.  
- O multiplicador zera ao errar a ordem ou ser pego por um fantasma.

---

## Fantasmas

- Os fantasmas perseguem o jogador.  
- Ao colidir com um fantasma:  
  - O jogador perde 1 vida e é reposicionado.  
  - O fantasma retorna à sua posição inicial.

---

## Power-ups Especiais

### Item de Congelamento

- Um item de gelo aparece no mapa como 🥶.  
- Quando coletado, congela todos os fantasmas por 5 segundos, permitindo que o jogador pensar com calma.

### Xícara de Café

- Um item de café aparece no mapa como ☕.  
- Quando coletado, concede **+1 vida** ao jogador.  
- Aparece uma vez por fase.

---

## Fases

O jogo possui 3 fases de dificuldade:

1. Fácil  
2. Médio  
3. Difícil  

O jogador avança de fase ao completar a expressão atual corretamente.  
Ao finalizar todas as fases, o jogo se encerra.

---

## Ranking

- Antes de iniciar, o jogador insere seu nome.  
- Ao perder todas as vidas ou vencer o jogo:  
  - A pontuação é salva no arquivo `ranking.txt`.  
  - O ranking com os melhores resultados é exibido ao final da partida, em ordem decrescente.

---

## Gameplay (Screenshot)

![Gameplay Screenshot](assets/screenshot.png)

---

## Como Rodar o Projeto

### Pré-requisitos

- Linux ou macOS (ou WSL no Windows)  
- `gcc` instalado  
- Terminal compatível com ANSI (bash, zsh etc.)

### Passo a Passo

1. **Abra o terminal**  
   - No Windows, abra o WSL.  
   - No Linux/macOS, abra o shell de sua preferência.

2. **Clone o repositório e rode o jogo**  
   ```bash
   git clone https://github.com/gqueque/Projeto-PIF.git
   cd Projeto-PIF
   make run
