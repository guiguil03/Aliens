#include "aliens.h"

// Manage ship
void init_ship()
{
  ship = COLS / 2;
}

void print_ship()
{
  attron(COLOR_SHIP);

  move(LINES - 1, ship);
  addch(ACS_BLOCK);

  move(LINES - 1, ship - 1);
  addch(ACS_BLOCK);

  move(LINES - 1, ship + 1);
  addch(ACS_BLOCK);

  move(LINES - 2, ship);
  addch(ACS_BLOCK);
}

// Manage bombs
void init_bombs()
{
  // Allocate matrix of LINES x COLS
  bombs = malloc(LINES * sizeof(int *));
  for (int i = 0; i < LINES; i++)
  {
    bombs[i] = malloc(COLS * sizeof(int));
  }

  // Initialize matrix with 0
  for (int row = 0; row < LINES; row++)
  {
    for (int col = 0; col < COLS; col++)
    {
      bombs[row][col] = 0;
    }
  }
}

void print_bombs()
{
  attron(COLOR_BOMBS);

  for (int row = 0; row < LINES; row++)
  {
    for (int col = 0; col < COLS; col++)
    {
      if (bombs[row][col])
      {
        move(row, col);
        addch(ACS_DIAMOND);
      }
    }
  }
}

// Manage shots
void init_shots()
{
  // Allocate matrix of LINES x COLS
  shots = malloc(LINES * sizeof(int *));
  for (int i = 0; i < LINES; i++)
  {
    shots[i] = malloc(COLS * sizeof(int));
  }

  // Initialize matrix with 0
  for (int row = 0; row < LINES; row++)
  {
    for (int col = 0; col < COLS; col++)
    {
      shots[row][col] = 0;
    }
  }
}

void print_shots()
{
  attron(COLOR_SHOTS);

  for (int row = 0; row < LINES; row++)
  {
    for (int col = 0; col < COLS; col++)
    {
      if (shots[row][col])
      {
        move(row, col);
        addch(ACS_DEGREE);
      }
    }
  }
}

// Manage shields
void init_shields(int count, int width)
{
  // Allocate matrix of 2 x COLS
  shields = malloc(2 * sizeof(int *));
  shields[0] = malloc(COLS * sizeof(int));
  shields[1] = malloc(COLS * sizeof(int));

  // Initialize matrix
  int totalMargin = COLS - count * width;
  int margin = totalMargin / (count + 1);

  for (int row = 0; row < 2; row++)
  {
    for (int col = 0; col < COLS; col++)
    {
      shields[row][col] = 0;
    }
  }

  for (int i = 0; i < count; i++)
  {
    for (int j = 0; j < width; j++)
    {
      shields[0][margin * (i + 1) + i * width + j] = 1;
      shields[1][margin * (i + 1) + i * width + j] = 1;
    }
  }
}

void print_shields()
{
  attron(COLOR_SHIELDS);

  // Write the 1st line
  for (int col = 0; col < COLS; col++)
  {
    if (shields[0][col])
    {
      move(LINES - 5, col);
      addch(ACS_BLOCK);
    }
  }

  // Write the 2nd line

  for (int col = 0; col < COLS; col++)
  {
    if (shields[1][col])
    {
      move(LINES - 4, col);
      addch(ACS_BLOCK);
    }
  }
}

// Manage aliens
void init_aliens(char *path)
{
  // Open file
  FILE *file = fopen(path, "r");
  if (!file)
  {
    perror("Can't open file");
    exit(1);
  }

  // Count the number of chars in the file
  int size = 0;
  while (fgetc(file) != EOF)
  {
    size++;
  }

  // Initialize frame_data
  frames_data = malloc(size * sizeof(char));

  // Read file and store chars in frame_data
  fseek(file, 0, SEEK_SET);
  for (int i = 0; i < size; i++)
  {
    frames_data[i] = fgetc(file);
  }

  // Close file
  fclose(file);

  // Initialize frames_count
  frames_count = 0;
  for (int i = 0; i < size - 1; i++)
  {
    if (frames_data[i] == '\n' && frames_data[i + 1] == '\n')
    {
      frames_count++;
    }
  }

  // Initialize frames_height
  frames_height = 0;
  for (int i = 0; i < size - 1; i++)
  {
    if (frames_data[i] == '\n')
    {
      frames_height++;
    }

    if (frames_data[i] == '\n' && frames_data[i + 1] == '\n')
    {
      break;
    }
  }

  // Initialize frames_width
  frames_width = 0;
  int width = 0;
  for (int i = 0; i < size - 1; i++)
  {
    if (frames_data[i] != '\n')
    {
      width++;
    }
    else
    {
      frames_width = frames_width > width ? frames_width : width;
    }

    if (frames_data[i] == '\n' && frames_data[i + 1] == '\n')
    {
      break;
    }
  }
}

// Main program
int main()
{
  // Initialize nCurses
  INIT();

  init_ship();
  print_ship();

  init_bombs();
  print_bombs();

  init_shields(10, 5);
  print_shields();

  wait_key();

  // Remove nCurses
  DONE();

  return EXIT_SUCCESS;
}