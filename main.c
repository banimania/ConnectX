#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
* Board state
* EMPTY -> 0
* O -> 1
* X -> 2
*/

int* b;
int n, m, t, s;

int ai_enabled;
int ai_enabled_2;

const int MAX = 1000;
const int MIN = -1000;
const int depth = 64;

void setup_board(int* b) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      *(b + i * n + j) = 0;
    }
  }
}

void print_board(int* b) {
  printf("\n");
  printf("Turn %i:\n", t);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      char c;
      switch (*(b + i * n + j)) {
        case 2:
          c = 'X';
          break;
        case 1:
          c = 'O';
          break;
        default:
          c = '-';
          break;
      }
      printf("%c ", c);
    }
    printf("\n");
  }
  printf("\n");
}

int* move(int* b, int x, int p) {
  if (x < 0 || x >= n) return 0;
  int c = n - 1;

  for (int i = c; i >= -1; i--) {
    if (i == -1) return 0;
    if (*(b + i * n + x) != 0) c--;
    else break;
  }

  int* nb = (int*) malloc(n * n * sizeof(int));
  memcpy(nb, b, n * n * sizeof(int));
  *(nb + c * n + x) = p;
  return nb;
}

int check_winner(int* b, int p) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (*(b + n * i + j) == p) {
        int c;
        //Horizontal
        do {
          if (*(b + n * (c + i) + j) == p) c++;
          else c = 0;

          if (c == m) return 1;
        } while (c);
        
        //Vertical
        do {
          if (*(b + n * i + j + c) == p) c++;
          else c = 0;
          
          if (c == m) return 1;
        } while (c);

        //Diagonal ascending
        do {
          if (*(b + n * (i - c) + j + c) == p) c++;
          else c = 0;

          if (c == m) return 1;
        } while (c);

        //Diagonal descending
        do {
          if (*(b + n * (i + c) + j + c) == p) c++;
          else c = 0;

          if (c == m) return 1;
        } while (c);  
      }
    }
  }
  return 0;
}

int check_draw(int* b) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (*(b + n * i + j) == 0) return 0;
    }
  }
  return 1;
}

int check_state(int* b) {
  if (check_winner(b, 1)) return 1;
  else if (check_winner(b, 2)) return 2;
  else if (check_draw(b)) return 3;
  else return 0;
}

int eval_board(int* b, int p) {
  int s = check_state(b);
  if (s != 0) {
    if (s == p) return MAX;
    else return MIN;
  }

  return 0;
}

int minimax(int* b, int depth, int currentPlayer, int aiPlayer) {
  int state = check_state(b);
  if (depth == 0 || state != 0) {
    return eval_board(b, aiPlayer);
  }

  int bestScore = (currentPlayer == aiPlayer) ? MIN : MAX;

  for (int i = 0; i < n; i++) {
    if (*(b + (n - 1) * n + i) == 0) {
      int* nb = move(b, i, currentPlayer);
      if (nb) {
        int nextPlayer = (currentPlayer == 1) ? 2 : 1;
        int score = minimax(nb, depth - 1, nextPlayer, aiPlayer);
        if ((currentPlayer == aiPlayer && score > bestScore) ||
          (currentPlayer != aiPlayer && score < bestScore)) {
          bestScore = score;
        }
        free(nb);
      }
    }
  }

  return bestScore;
}

int ai_move(int* b, int p) {
  int bestMove = 0;
  int bestScore = MIN;

  for (int i = 0; i < n; i++) {
    if (*(b + (n - 1) * n + i) == 0) {
      int* nb = move(b, i, p);
      if (nb) {
        int score = minimax(nb, depth, p, p);
        if (score > bestScore) {
          bestScore = score;
          bestMove = i;
        }
        free(nb);
      }
    }
  }

  return bestMove;
}


int main(int argc, char* argv[]) {
  char c;

input_ai:
  printf("Do you want to enable AI for the first player? [Y/N]: ");
  scanf(" %c", &c);
  ai_enabled = (c == 'Y' ? 1 : 0);
  printf("Do you want to enable AI for the second player? [Y/N]: ");
  scanf(" %c", &c);
  ai_enabled_2 = (c == 'Y' ? 1 : 0);

input_game:
  printf("Enter the size of the board: ");
  scanf("%i", &n);
  printf("Enter the number of connections in order to win (1 - %i): ", n);
  scanf("%i", &m);

  if (m > n) {
    printf("\nThe required number of connections must be less than the board size!\n");
    goto input_game;
  }

  printf("\nSize of the board: %ix%i\n", n, n);
  printf("Connections needed to win: %i\n", m);
  printf("AI for first player: %i\n", ai_enabled);
  printf("AI for second player: %i\n", ai_enabled_2);

  b = (int*) malloc(n * n * sizeof(int));

  setup_board(b);

  print_board(b);

  while (!s) {
    int x;
    int p = t % 2 + 1;
    printf("Player %i must move\n", p);

    if ((p == 1 && ai_enabled) || (p == 2 && ai_enabled_2)) {
      int ai_m = ai_move(b, p);
      b = move(b, ai_m, p);
      printf("AI %i moved %i\n", p, ai_m);
    } else {
    input_player:
      printf("Column (1 - %i): ", n);
      scanf("%i", &x);

      x--;

      int* nb = move(b, x, p);
      if(!nb) {
        printf("Illegal move!\n");
        goto input_player;
      } else b = nb;
    }
    print_board(b);

    t++;
    s = check_state(b);
  }

  if (s == 3) printf("Game ended in a draw!\n");
  else printf("The game has ended. Winner is player %i (%c).\n", s, s == 2 ? 'X' : 'O');
  print_board(b);

  return 0;
}

