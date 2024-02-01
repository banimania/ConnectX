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

void move(int* b, int x, int p) {
  int c = n - 1;

  for (int i = c; i > 0; i--) {
    if (*(b + i * n + x) != 0) c--;
    else break;
  }

  *(b + c * n + x) = p;
}

int check_winner(int* b, int p) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (*(b + n * i + j) == p) {
        int c;
        //Horizontal
        do {
          if (n * (c + i) + j > n * n) c = 0; // definitely not 3 am bugfixing, i totally know why this is needed only in this check and not the other ones
          else if (*(b + n * (c + i) + j) == p) c++;
          else c = 0;

          if (c == m) return 1;
        } while (c);
        
        //Vertical
        do {
          if (*(b + n * i + (c + j)) == p) c++;
          else c = 0;
          
          if (c == m) return 1;
        } while (c);

        //Diagonal ascending
        do {
          if (*(b + n * (i + c) + (j + c)) == p) c++;
          else c = 0;

          if (c == m) return 1;
        } while (c);

        //Diagonal descending
        do {
          if (*(b + n * (i - c) + (j + c)) == p) c++;
          else c = 0;

          if (c == m) return 1;
        } while (c);  
      }
    }
  }
  return 0;
}

int check_state(int* b) {
  if (check_winner(b, 1)) return 1;
  else if (check_winner(b, 2)) return 2;
  else return 0;
}

int main(int argc, char* argv[]) {
  printf("Enter the size of the board (3 - 30): ");
  scanf("%i", &n);
  printf("Enter the number of connections in order to win (1 - %i): ", n);
  scanf("%i", &m);
  printf("Size of the board: %ix%i\n", n, n);
  printf("Connections needed to win: %i\n", m);

  b = (int*) malloc(n * n * sizeof(int));

  setup_board(b);

  print_board(b);

  while (!s) {
    int x;
    int p = t % 2 + 1;
    printf("Player %i must move\n", p);
    printf("Column (1 - %i): ", n);
    scanf("%i", &x);

    x--;

    move(b, x, p);

    print_board(b);

    t++;
    s = check_state(b);
  }

  printf("The game has ended. Winner is player %i.", s);
  print_board(b);

  return 0;
}

