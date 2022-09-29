#include <stdio.h>
char cell[10];
int csv_read_cell(FILE *db)
{
  int c, i = 0;
  while ((c = fgetc(db)) != EOF && c != ',' && c != '\n')
    cell[i++] = c;
  cell[i] = '\n';
  return i;
}
 
int main(int argc, char *argv[]) {
  if (argc < 2)
      return 1;
  FILE *csv = fopen(argv[1], "r");
  int len = csv_read_cell(csv);
  while (len > 0) {
    printf(cell);
    len = csv_read_cell(csv);
  }
  return 0;
}
