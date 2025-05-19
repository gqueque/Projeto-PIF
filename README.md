# 🕹️ Pac-Lógica — Projeto de Programação Imperativa e Funcional

## 🎓 Informações Acadêmicas

- **Disciplina:** Programação Imperativa e Funcional - 2025.1  
- **Instituição:** CESAR School

## 👥 Integrantes

- Gustavo Queque ([@gqueque](https://github.com/gqueque))
- Maria Júlia ([@mmajumagalhaes](https://github.com/mmajumagalhaes))
- Luiz Felipe ([@LuizFelipexDD](https://github.com/LuizFelipexDD))

---

## 🧠 Sobre o Jogo

O **Pac-Lógica** é um jogo inspirado no clássico Pac-Man, mas com uma pegada educativa:  
O jogador deve **resolver expressões lógicas** usando a **tabela verdade**, coletando os itens `V` (verdadeiro) e `F` (falso) **na ordem correta**.

### 💡 Objetivo:
- Coletar os valores da **tabela verdade** da expressão exibida na parte superior da tela.
- Se pegar fora de ordem, perde 1 vida e reinicia a sequência daquela expressão.
- Se um **fantasma** encostar no jogador, também perde 1 vida.
- O jogador começa com **3 vidas**.

---

## 🎮 Como Funciona

### ▶️ Controles

| Tecla | Ação        |
|-------|-------------|
| `W`   | Cima        |
| `A`   | Esquerda    |
| `S`   | Baixo       |
| `D`   | Direita     |
| `Q`   | Sair do jogo|

---

### 🧾 Regras

- Cada expressão lógica possui **4 valores** (para as 4 combinações possíveis de A e B).
- Esses valores são espalhados como itens (`V` ou `F`) no mapa.
- O jogador deve coletar **na ordem correta da tabela verdade**.
- Ao acertar, ganha **100 pontos x multiplicador**.
- O **multiplicador** aumenta a cada acerto consecutivo, e **zera** se errar a ordem ou for pego por um fantasma.

---

### 👻 Fantasmas

- Existem fantasmas no mapa que perseguem o jogador.
- Ao colidir com um fantasma, o jogador perde 1 vida e é reposicionado.
- Fantasmas voltam para sua posição de origem após colisão.

---

## 🏁 Níveis

O jogo possui 3 níveis de dificuldade:
1. **Fácil**
2. **Médio**
3. **Difícil**

A cada expressão resolvida, o jogador avança para o próximo nível.  
Ao completar todos os níveis: **O jogo se encerra!**

---

## 🏆 Ranking
- O jogador insere seu nome e starta o jogo.
- Ao perder todas as vidas ou zerar o jogo.
- A pontuação final é salva em um arquivo `ranking.txt`
- O ranking é exibido ao final da partida.

---

## ⚙️ Como Rodar o Projeto

### 🖥️ Pré-requisitos

- Sistema **Linux ou macOS** (ou usar WSL no Windows)
- `gcc` instalado
- Terminal compatível com ANSI (bash, zsh etc.)

---

### 🚀 Passo a passo

1. Clone o projeto:

```bash
git clone https://github.com/gqueque/Projeto-PIF.git
cd Projeto-PIF
make run
