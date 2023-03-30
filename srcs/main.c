/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:24:05 by ntan-wan          #+#    #+#             */
/*   Updated: 2023/03/30 16:05:17 by ntan-wan         ###   ########.fr       */
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

void	debug_simple_printf(int width, char *value)
{
	int	i;	

	if (value)
		ft_putstr_fd(value, STDOUT_FILENO);
	i = -1;
	while (++i < (width - ft_strlen(value)))
		ft_putstr_fd(" ", STDOUT_FILENO);
}

void	debug_print_grid_cell_value(char *cell_value)
{
	if (cell_value)
		debug_simple_printf(N * 2, cell_value);
	else
		debug_simple_printf(N * 2, "0");
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

void	iterate_row_left_right(void (*f)(int, int, char *[N][N]), int row, int col, char *grid[N][N])
{
	while (col >= 0 && col <= N - 1)
	 	f(row, col++, grid);
}

void	iterate_row_right_left(void (*f)(int, int, char *[N][N]), int row, int col, char *grid[N][N])
{
	while (col >= 1 && col <= N - 2)
		f(row, col--, grid);
}

void	iterate_col_top_down(void (*f)(int, int, char *[N][N]), int row, int col, char *grid[N][N])
{
	while (row >= 0 && row <= N - 1)
		f(row++, col, grid);
}

void	iterate_col_down_top(void (*f)(int, int, char *[N][N]), int row, int col, char *grid[N][N])
{
	while (row >= 0 && row <= N - 1)
		f(row--, col, grid);
}

void	iterate_top_clues(void (*f)(int, int, char *[N][N]), char *grid[N][N])
{
	int	row;
	int	col;

	row = 0;
	col = 1;
	iterate_row_left_right(f, row, col, grid);
}

void	iterate_bottom_clues(void (*f)(int, int, char *[N][N]), char *grid[N][N])
{
	int	row;
	int	col;

	row = N - 1;
	col = 1;
	iterate_row_left_right(f, row, col, grid);
}

void	iterate_left_clues(void (*f)(int, int, char *[N][N]), char *grid[N][N])
{
	int	row;
	int	col;

	row = 1;
	col = 0;
	iterate_col_top_down(f, row, col, grid);
}

void	iterate_right_clues(void (*f)(int, int, char *[N][N]), char *grid[N][N])
{
	int	row;	
	int	col;

	row = 0;
	col = N - 1;
	iterate_col_top_down(f, row, col, grid);
}

void	process_edge_clue(void (*f)(int, int, char *[N][N]), int row, int col, char *grid[N][N])
{
	if (row == 0)
		iterate_col_top_down(f, row, col, grid);
	else if (row == N - 1)
		iterate_col_down_top(f, row, col, grid);
	else if (col == 0)
		iterate_row_left_right(f, row, col, grid);
	else if (col == N - 1)
		iterate_row_right_left(f, row, col, grid);
}

bool	is_edge_clue(int row, int col)
{
	return (row == 0 || row == N - 1 || col == 0 || col == N - 1);
}

void	put_num_ascending(int row, int col, char *grid[N][N])
{
	static int i = 0;

	if (is_edge_clue(row, col))
		return ;
	else if (ft_strlen(grid[row][col]) != 1)
	{
		if (grid[row][col])
			free(grid[row][col]);
		grid[row][col] = ft_simple_itoa(i + 1);
	}
	i = (i + 1) % (N - 2);
}

void	put_num_descending(int row, int col, char *grid[N][N])
{
	static int i = N - 2;

	if (is_edge_clue(row, col))
		return ;
	else if (ft_strlen(grid[row][col]) != 1)
	{
		if (grid[row][col])
			free(grid[row][col]);
		grid[row][col] = ft_simple_itoa(i);
	}
	if (i == 1)
		i = N - 2;
	else
		i--;
}

/* 
	@brief Calculate the distance from edge to the cell.
	@param clue_pos[2] Row and col index of the clue.
	@param row The cell's row index.
	@param col The cell's col index.
 */
int	calculate_distance(int clue_pos[2], int row, int col)
{
	if (clue_pos[0] == 0)
		return (row);
	else if (clue_pos[0] == N - 1)
		return (N - 1 - row);
	else if (clue_pos[1] == 0)
		return (col);
	else if (clue_pos[1] == N - 1)
		return (N - 1 - col);
	else
		return (0);
}

int	calculate_edge_constraint(int clue, int distance)
{
	return ((N - 2) - clue + 2 + distance);
}

char	*ft_strjoin(char *str1, char *str2)
{
	int		i;
	int		j;
	int		str_len;
	char	*strjoin;

	i = -1;
	j = -1;
	if (!str1 || !str2)
		return (NULL);
	str_len = ft_strlen(str1) + ft_strlen(str2);
	strjoin = malloc((str_len + 1) * sizeof(char));
	while (++i < str_len)
	{
		if (i < ft_strlen(str1))
			strjoin[i] = str1[i];
		else if (++j < ft_strlen(str2))
			strjoin[i] = str2[j];
	}
	strjoin[i] = '\0';
	return (strjoin);
}

void	ft_bzero(void *p, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	i = 0;
	ptr = p;
	while (i < n)
	{
		*ptr++ = 0;
		i++;
	}
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;
	if (count == SIZE_MAX || size == SIZE_MAX)
		return (NULL);
	ptr = malloc(count * size);
	if (ptr)
		ft_bzero(ptr, count *size);
	return (ptr);
}

void	ft_simple_strcat(char *dst, char *src, size_t dst_size)
{
	size_t	i;
	size_t	dst_content_len;

	i = 0;
	dst_content_len = ft_strlen(dst);
	while (src[i] && dst_content_len + 1 < dst_size)
	{
		dst[dst_content_len++]	 = src[i];
		i++;
	}
	dst[dst_content_len] = '\0';
}

char	*get_num_range(int edge_constraint)
{
	int		i;
	char	*num;
	char	*num_range;
	size_t	num_range_size;

	i = 0;
	num_range_size = edge_constraint * 2 + 1;
	while (++i < edge_constraint && i <= N - 2)
	{
		if (i == 1)
			num_range = ft_calloc(num_range_size, sizeof(char));
		num = ft_simple_itoa(i);
		ft_simple_strcat(num_range, num, num_range_size);
		ft_simple_strcat(num_range, ",", num_range_size);
		free(num);
	}
	return (num_range);
}

void	put_num_range(int row, int col, char *grid[N][N])
{
	static int	clue;
	static int	clue_pos[2];
	int			distance;
	int			edge_constraint;

	distance = 0;
	if (is_edge_clue(row, col))
	{
		clue = ft_simple_atoi(grid[row][col]);
		clue_pos[0] = row;
		clue_pos[1] = col;
	}
	else
	{
		distance = calculate_distance(clue_pos, row, col);
		edge_constraint = calculate_edge_constraint(clue, distance);
		grid[row][col] = get_num_range(edge_constraint);
	}
	
}

void	solve_edge_clue(int row, int col, char *grid[N][N])	
{
	int	min;
	int	max;
	int	clue;

	min = 1;
	max = N - 2;
	if (grid[row][col])
	{
		clue = ft_simple_atoi(grid[row][col]);
		if (clue == max)
			process_edge_clue(put_num_ascending, row, col, grid);
		else if (clue == min)
			process_edge_clue(put_num_descending, row, col, grid);
		else
			process_edge_clue(put_num_range, row, col, grid);
	}
}

void	iterate_edge_clues(void (*f)(int, int, char *[N][N]), char *grid[N][N])
{
	iterate_top_clues(f, grid);
	// iterate_bottom_clues(f, grid);
	// iterate_left_clues(f, grid);
	// iterate_right_clues(f, grid);
}

int main(int ac, char **av)
{
	static char	*grid[N][N];

	store_clues_to_grid(av, grid);
	iterate_edge_clues(solve_edge_clue, grid);
	debug_print_grid(grid);
	grid_free(grid);
	
	// char	*num_range;
	// num_range = get_num_range(5);
	// printf("%s\n", num_range);
	// free(num_range);
	return (0);
}