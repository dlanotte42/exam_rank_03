#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

typedef struct s_draw
{
	int width;
	int height;
	char bg;
	char *matrice;
} t_draw;

typedef struct s_circle
{
	char type;
	float x;
	float y;
	float r;
	char color;
} t_circle;

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

int is_in_circle(t_circle *circle, int x, int y)
{
	float dist;
	
	dist = sqrtf(powf((circle->x - x), 2) + powf((circle->y - y), 2));
	if (dist > circle->r)
		return (0);
	if (circle->r - dist <= 1)
		return (1);
	return (2);
}

int operation(t_draw *tela, t_circle *circle)
{
	int x, y;
	int pixel;

	x = -1;
	if (circle->r <= 0)
		return 0;
	if (circle->type != 'c' && circle->type != 'C')
		return 0;
	while (++x < tela->width)
	{
		y = -1;
		while(++y < tela->height)
		{
			pixel = is_in_circle(circle, x, y);
			if (pixel == 1 || (pixel == 2 && circle->type == 'C'))
				tela->matrice[(y * tela->width) + x] = circle->color;
		}
	}
	return 1;
}

int just_do_it(FILE *file, t_draw *tela)
{
	int error;
	int args;
	t_circle circle;

	args = fscanf(file, "%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.r, &circle.color);
	while (args == 5)
	{
		if (!operation(tela, &circle))
			return 0;
		args = fscanf(file, "%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.r, &circle.color);
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