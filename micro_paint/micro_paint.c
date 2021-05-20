#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct s_draw
{
	int width;
	int height;
	char bg;
	char *matrice;
} t_draw;

typedef struct s_rect
{
	char type;
	float x;
	float y;
	float w;
	float h;
	char color;
} t_rect;

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int init(FILE *file, t_draw *tela)
{
	int i;
	int args;

	args = fscanf(file, "%d %d %c\n", &tela->width, &tela->height, &tela->bg);

	if (tela->width <= 0 || tela->width > 300 || tela->height <= 0 || tela->height > 300)
		return (0);
	if (args == 3)
	{
		tela->matrice = malloc(tela->width * tela->height);
		i = 0;
		while (i < tela->width * tela->height)
		{
			tela->matrice[i] = tela->bg;
			i++;
		}
		return (1);
	}
	return (0);
}

void print(t_draw *tela)
{
	int x;
	int y;

	y = -1;

	while (++y < tela->height)
	{
		x = -1;
		while(++x < tela->width)
		{
			printf("%c", tela->matrice[(y * tela->width) + x]);
		}
		printf("\n");
	}
}

int is_in_recto(t_rect *recto, int x, int y)
{
	if (x < recto->x || x > recto->x + recto->w || y < recto->y || y > recto->y + recto->h)
		return 0;
	if (x - recto->x < 1 || recto->x + recto->w - x < 1 || y - recto->y < 1 || recto->y + recto->h - y < 1)
		return 1;
	return 2;
}

int operation(t_draw *tela, t_rect *recto)
{
	int x, y;
	int dito;

	x = -1;
	if (recto->w <= 0 || recto->h <= 0)
		return 0;
	if (recto->type != 'r' && recto->type != 'R')
		return 0;
	while (++x < tela->width)
	{
		y = -1;
		while(++y < tela->height)
		{
			dito = is_in_recto(recto, x, y);
			if (dito == 1 || (dito == 2 && recto->type == 'R'))
				tela->matrice[(y * tela->width) + x] = recto->color;
		}
	}
	return 1;
}

int just_do_it(FILE *file, t_draw *tela)
{
	int error;
	int args;
	t_rect recto;

	args = fscanf(file, "%c %f %f %f %f %c\n", &recto.type, &recto.x, &recto.y, &recto.w, &recto.h, &recto.color);
	while (args == 6)
	{
		if (!operation(tela, &recto))
			return 0;
		args = fscanf(file, "%c %f %f %f %f %c\n", &recto.type, &recto.x, &recto.y, &recto.w, &recto.h, &recto.color);
	}
	return (args == -1);
}

int main(int ac, char **av)
{
	FILE *file;
	t_draw tela;
	int i;

	if (ac == 2)
	{
		file = fopen(av[1], "r");
		if (file != NULL && init(file, &tela) && just_do_it(file, &tela))
		{
			print(&tela);
			return (0);
		}
		else
		{
			i = ft_strlen("Error: Operation file corrupted\n");
			write(1, "Error: Operation file corrupted\n", i);
		}
	}
	else
	{ 
		i = ft_strlen("Error: Argument\n");
		write(1, "Error: Argument\n", i);
	}
	return (1);
}