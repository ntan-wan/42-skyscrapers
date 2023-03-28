/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:24:05 by ntan-wan          #+#    #+#             */
/*   Updated: 2023/03/29 06:15:34 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "skyscrapers.h"
#include <stdio.h>

int	ft_strlen(char *str)
{
	int	len;

	if (!str)
		return (0);
	len = 0;
	while (*str++)
		len++;
	return (len);
}

char	*ft_strdup(char *str)
{
	int		i;
	char	*str_dup;

	if (!str)
		return (NULL);
	i = -1;
	str_dup = malloc((ft_strlen(str) + 1) * sizeof(char));
	while (str[++i])
		str_dup[i] = str[i];
	str_dup[i] = '\0';
	return (str_dup);
}

void	ft_putstr_fd(char *str, int fd)
{
	if (!str)
		return ;
	write(fd, str, ft_strlen(str));
}

void	store_first_column(char *arr[N], char *data)
{
	arr[0] = ft_strdup(data);
}

void	store_mid_column(char *arr[N], char *data)
{
	int	i;

	i = 0;
	while (++i >= 1 && i <= N - 2)
	{
		if (arr[i] == NULL)
		{
			arr[i] = ft_strdup(data);
			break ;
		}
	}
}

void	store_last_column(char *arr[N], char *data)
{
	arr[N - 1] = ft_strdup(data);
}

void	store_clues_to_grid(char **av, char *grid[N][N])
{
	int	i;

	i = 0;
	while (av[++i])
	{
		if (i <= N - 2)
			store_mid_column(grid[0], av[i]);
		else if (i <= 2 * (N - 2))
			store_mid_column(grid[N - 1], av[i]);
		else if (i <= 3 * (N - 2))
			store_first_column(grid[(i + 2) % (N - 1)], av[i]);
		else if (i <= 4 * (N - 2))
			store_last_column(grid[(i + 3) % (N - 1)], av[i]);
	}
}

void	debug_print_spaces(int cell_value_len)
{
	int	i;
	int	len;

	i = -1;
	len = 1;
	if (cell_value_len)
		len = cell_value_len;
	while (++i < ((N + 1) - len) / 2)
		ft_putstr_fd(" ", STDOUT_FILENO);
}

void	debug_print_grid_cell_value(char *cell_value)
{
	debug_print_spaces(ft_strlen(cell_value));
	if (cell_value)
		ft_putstr_fd(cell_value, STDOUT_FILENO);
	else
		ft_putstr_fd("0", STDOUT_FILENO);
	debug_print_spaces(ft_strlen(cell_value));
}

void	debug_print_grid(char *grid[N][N])
{
	int	row;
	int	col;

	row = -1;	
	while (++row < N)
	{
		col = -1;
		while (++col < N)
			debug_print_grid_cell_value(grid[row][col]);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
}

void	grid_cell_free(char **cell_value)
{
	if (*cell_value)
		free(*cell_value);
	*cell_value = NULL;
}

void	grid_free(char *grid[N][N])
{
	int	row;
	int	col;

	row = -1;
	while (++row < N)
	{
		col = -1;
		while (++col < N)
			grid_cell_free(&grid[row][col]);
	}
}

void	iterate_col_top_down(int col, void (*f)(int, int, char *[N][N]), char *grid[N][N])
{
	int	row;

	row = 1;
	while (row >= 1 && row <= N - 2)
		f(row++, col, grid);
}

void	iterate_col_down_top(int col, void (*f)(int, int, char *[N][N]), char *grid[N][N])
{
	int	row;

	row = 1;
	while (row >= 1 && row <= N - 2)
		f(row++, col, grid);
}

void	iterate_row_left_right(int row, void (*f)(int, int, char *[N][N]), char *grid[N][N])
{
	int	col;

	col = 1;
	while (col >= 1 && col <= N - 2)
	 	f(row, col++, grid);
}

void	iterate_row_right_left(int row, void (*f)(int, int, char *[N][N]), char *grid[N][N])
{
	int	col;

	col = 1;
	while (col >= 1 && col <= N - 2)
		f(row, col++, grid);
}

int	count_digits_len(int num)
{
	int	digit_count;

	digit_count = 1;
	while (num / 10 > 0)
	{
		digit_count++;
		num /= 10;
	}
	return (digit_count);
}

char	single_digit_num_to_c(int num)
{
	char	c;

	c = '0' + num;
	return (c);
}

/* 
	@brief Convert integer to char.
	@note Does not handle overflow.
	@note Does not handle negative integer.
 */
char	*ft_simple_itoa(int num)
{
	int		digits_len;
	char	*num_str;
	
	digits_len = count_digits_len(num);
	num_str = malloc((digits_len + 1) * sizeof(char));
	num_str[digits_len] = '\0';
	while (digits_len--)
	{
		num_str[digits_len] = single_digit_num_to_c(num % 10);
		num /= 10;
	}
	return (num_str);
}

/* 
	@brief Convert char to integer.
	@note Does not handle weird char.
	@note Does not handle negative integer.
 */
int	ft_simple_atoi(char *num_str)
{
	int	i;
	int	num;

	i = -1;
	num = 0;
	while (*num_str)
	{
		num = (num * 10) + (*num_str - '0');
		num_str++;
	}
	return (num);
}

void	put_num_ascending(int row, int col, char *grid[N][N])
{
	static int i = 0;

	if (grid[row][col] == NULL)
	{
		grid[row][col] = ft_simple_itoa(i + 1);
	}
	i = (i + 1) % (N - 2);
}

void	put_num_descending(int row, int col, char *grid[N][N])
{
	static int i = N - 2;

	if (grid[row][col] == NULL)
	{
		grid[row][col] = ft_simple_itoa(i);
	}
	if (i == 1)
		i = N - 2;
	else
		i--;
}

void	edge_constraint()
{
	
}

void	process_edge_clue(int row, int col, void (*f)(int, int, char *grid[N][N]), char *grid[N][N])
{
	if (row == 0)
		iterate_col_top_down(col, f, grid);
	else if (row == N - 1)
		iterate_col_down_top(col, f, grid);
	else if (col == 0)
		iterate_row_left_right(row, f, grid);
	else if (col == N - 1)
		iterate_row_right_left(row, f, grid);
}

void	solve_edge_clue(int row, int col, char *grid[N][N])	
{
	int	min;
	int	max;

	min = 1;
	max = N - 2;
	if (ft_simple_atoi(grid[row][col]) == max)
		process_edge_clue(row, col, put_num_ascending, grid);
	else if (ft_simple_atoi(grid[row][col]) == min)
		process_edge_clue(row, col, put_num_descending, grid);
}

void	iterate_edge_clues(void (*f)(int, int, char *[N][N]), char *grid[N][N])
{
	iterate_row_left_right(0, f, grid);
	iterate_row_left_right(N - 1, f, grid);
	iterate_col_top_down(0, f, grid);
	iterate_col_top_down(N - 1, f, grid);
}

int main(int ac, char **av)
{
	static char	*grid[N][N];

	store_clues_to_grid(av, grid);
	iterate_edge_clues(solve_edge_clue, grid);
	// edge_clue_top_solve(grid);
	// process_edge_clues(grid);
	// edge_constraint();
	debug_print_grid(grid);
	grid_free(grid);
	return (0);
}