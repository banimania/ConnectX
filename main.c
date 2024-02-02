#include <stdio.h>
#include <stdlib.h>

/*
* Board state
* EMPTY -> 0
* O -> 1
* X -> 2
*/

int* b;
int n, m, t, s;

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

int move(int* b, int x, int p) {
  if (x < 0 || x >= n) return 0;
  int c = n - 1;

  for (int i = c; i >= -1; i--) {
    if (i == -1) return 0;
    if (*(b + i * n + x) != 0) c--;
    else break;
  }

  *(b + c * n + x) = p;
  return 1;
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

int main(int argc, char* argv[]) {
  input_game:
  printf("Enter the size of the board: ");
  scanf("%i", &n);
  printf("Enter the number of connections in order to win (1 - %i): ", n);
  scanf("%i", &m);

  if (m > n) {
    printf("\nThe required number of connections must be less than the board size!\n");
    goto input_game;
  }

  printf("Size of the board: %ix%i\n", n, n);
  printf("Connections needed to win: %i\n", m);

  b = (int*) malloc(n * n * sizeof(int));

  setup_board(b);

  print_board(b);

  while (!s) {
    int x;
    int p = t % 2 + 1;
    printf("Player %i must move\n", p);
    
    input_player:
    printf("Column (1 - %i): ", n);
    scanf("%i", &x);

    x--;

    if(!move(b, x, p)) {
      printf("Illegal move!\n");
      goto input_player;
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

