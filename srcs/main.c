/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:24:05 by ntan-wan          #+#    #+#             */
/*   Updated: 2023/04/01 08:45:39 by ntan-wan         ###   ########.fr       */
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
	@brief Find the distance from edge to the cell.
	@param clue_pos[2] Row and col index of the clue.
	@param row The cell's row index.
	@param col The cell's col index.
 */
int	find_distance(int clue_pos[2], int row, int col)
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
		dst[dst_content_len++] = src[i];
		i++;
	}
	dst[dst_content_len] = '\0';
}

char	*get_num_range(int max_range)
{
	int		i;
	char	*num;
	char	*num_range;
	size_t	num_range_size;

	i = 0;
	num_range = NULL;
	num_range_size = max_range * 2 + 1;
	while (++i < max_range && i <= N - 2)
	{
		if (i == 1)
			num_range = ft_calloc(num_range_size, sizeof(char));
		num = ft_simple_itoa(i);
		ft_simple_strcat(num_range, num, num_range_size);
		if (i != N - 2 && i != max_range - 1)
			ft_simple_strcat(num_range, ",", num_range_size);
		free(num);
	}
	return (num_range);
}

char	*ft_simple_substr(char *str, unsigned int start_index, size_t len)
{
	int		i;
	char	*substr;
	int		substr_len;
	
	if (!str || !len)
		return (NULL);
	substr_len = ft_strlen(str) - start_index;
	substr = malloc((substr_len + 1) * sizeof(char));
	i = -1;
	while (++i < len && str[start_index])
		substr[i] = str[start_index++];
	substr[i] = '\0';
	return (substr);
}

char	*ft_strchr(char *str, int c)
{
	while (*str)
	{
		if (*str == c)	
			return (str);
		str++;
	}
	return (NULL);
}

int	find_word_count(char *str, char *delimiters)
{
	int	i;
	int	word_count;

	i = -1;
	word_count = 0;
	while (str[++i])
	{
		if (i == 0)
			word_count = 1;
		if (ft_strchr(delimiters, str[i]))
			word_count++;
	}
	return (word_count);
}

char	**ft_split(char *str, char *delimiters)
{
	int		i;
	int		j;
	char	**arr;
	int		word_start_index;

	i = -1;
	j = -1;
	word_start_index = 0;
	while (str[++i])
	{
		if (i == 0)
			arr = malloc((find_word_count(str, delimiters) + 1) * sizeof(char *));
		if (ft_strchr(delimiters, str[i]))
		{
			arr[++j] = ft_simple_substr(str, word_start_index, i - word_start_index);
			word_start_index = i + 1;
		}
		if (i + 1 == ft_strlen(str))
			arr[++j] = ft_simple_substr(str, word_start_index, ++i - word_start_index);
	}
	arr[++j] = NULL;
	return (arr);
}

int	ft_strncmp(char *str1, char *str2, size_t n)
{
	size_t	i;		
	
	i = 0;
	if (n == 0)
		return (0);
	while (i < n - 1 && str1[i] && str2[i])
	{
		if (str1[i] != str2[i])
			break ;
		i++;
	}
	return ((unsigned char)str1[i] - (unsigned char)str2[i]);
}

void	filter_num_range(char *num_range, int num)
{
	int	i;	

	i = -1;
}

int	get_max_range(int clue, int clue_pos[2], int row, int col)
{	
	return ((N - 2) - clue + 2 + find_distance(clue_pos, row, col));
}


void	put_num_range(int row, int col, char *grid[N][N])
{
	static int	clue;
	static int	clue_pos[2];
	char		*num_range;

	if (is_edge_clue(row, col))
	{
		clue = ft_simple_atoi(grid[row][col]);
		clue_pos[0] = row;
		clue_pos[1] = col;
	}
	else if (ft_strlen(grid[row][col]) != 1)
	{
		num_range = get_num_range(get_max_range(clue, clue_pos, row, col));
		if (ft_strlen(grid[row][col]) == 0)
			grid[row][col] = num_range;
		else if (ft_strlen(grid[row][col]) > ft_strlen(num_range))
		{
			free(grid[row][col]);
			grid[row][col] = num_range;
		}
		else
			free(num_range);
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
	iterate_bottom_clues(f, grid);
	iterate_left_clues(f, grid);
	iterate_right_clues(f, grid);
}

void	rm_array_item(char **arr, int index)
{
	while (arr[index])	
	{
		free(arr[index]);
		if (arr[index + 1])
			arr[index] = ft_strdup(arr[index + 1]);
		else
			arr[index] = NULL;
		index++;
	}
}

char 	*arr_str_concat(char **arr, char *delimiter)
{
	int		i;
	char	*str;
	char	*to_del;

	i = 0;
	str = arr[0];
	while (arr[++i])
	{
		str = ft_strjoin(str, delimiter);
		to_del = str;
		str = ft_strjoin(str, arr[i]);
		free(to_del);
	}
	return (str);
}

void	arr_str_free(char ***array)
{
	int		i;
	char	**arr;

	i = -1;
	arr = *array;
	if (!arr)
		return ;
	while (arr[++i])
		free(arr[i]);
	free(arr);
	*array = NULL;
}

char	*rm_num(char *num_range, char *num)
{
	int		i;
	char	**arr;
	char	*result;

	i = -1;
	arr = ft_split(num_range, ",");
	while (arr[++i])
	{
		if (ft_strncmp(arr[i], num, ft_strlen(num)) == 0)
		{
			rm_array_item(arr, i);
			break ;
		}
	}
	result = arr_str_concat(arr, ",");
	arr_str_free(&arr);
	return (result);
}

int main(int ac, char **av)
{
	static char	*grid[N][N];

	store_clues_to_grid(av, grid);
	iterate_edge_clues(solve_edge_clue, grid);
	debug_print_grid(grid);
	grid_free(grid);
	
	
	// rm_array_item(ft_split("hello,man,world", ","), 2);

	// char	**arr;
	// arr = ft_split("1,2,3,4", ",");
	// rm_array_item(arr, 3);
	// for (int i = 0; arr[i]; i++)
	// {
	// 	printf("%s\n", arr[i]);
	// 	free(arr[i]);
	// }
	// free(arr);
	
	// int	count;
	// printf("%d\n", find_word_count("hello,world", ","));
	// char *arr[] = {"hello", "world",NULL};
	// char *str = arr_str_concat(arr);
	// printf("%s\n", str);
	// free(str);

	// char *s = ft_strjoin("hello", "world");
	// printf("%s\n", s);
	// free(s);
	// printf("%d\n", ft_strncmp("2", "1", 1));
	// char *num = rm_num("1", "1");
	// printf("test %s\n", num);
	// free(num);
	return (0);
}