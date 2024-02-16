/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 17:27:09 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/02/16 13:31:49 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	ft_get_map_height(char *file)
{
	char	buffer[1];
	int		height;
	int		fd;

	height = 0;
	fd = open(file, O_RDONLY);
	while (read(fd, buffer, 1) > 0)
		height += ft_strchr(buffer, '\n') != NULL;
	close(fd);
	return (height);
}

void	ft_free(void **ptr)
{
	void	**hold;

	hold = ptr;
	if (!ptr)
		return ;
	while (*ptr)
		free(*ptr++);
	free(hold);
}

int	ft_default_color(void)
{
	return (0x00ffffff);
}

int	ft_parse_value(t_point *pos, char *value)
{
	char	**parsed_value;

	if (ft_strchr(value, ','))
	{
		parsed_value = ft_split(value, ',');
		if (!parsed_value)
			return (-1);
		pos->color = ft_atoi_hex(parsed_value[1]);
		pos->z = ft_atoi(parsed_value[0]);
		ft_free((void **)parsed_value);
	}
	else
	{
		pos->color = ft_default_color();
		pos->z = ft_atoi(value);
	}
	return (0);
}

static t_point	*ft_get_row(char **input, int height, t_map *map)
{
	char	**values;
	t_point	*pos;
	int		i;

	i = 0;
	if (!input)
		return (NULL);
	pos = (t_point *)malloc(map->width * sizeof(t_point));
	if (!pos)
		return (NULL);
	while (i < map->width)
	{
		pos[i].x = i;
		pos[i].y = height;
		if (ft_parse_value(&pos[i], input[i]) < 0)
			return (NULL);
		i++;
	}
	return (pos);
}

int	ft_get_len(char **ptr)
{
	int	i;

	i = 0;
	if (!ptr)
		return (0);
	while (*(ptr + i))
		i++;
	return (i);
}

char	**ft_parse(int fd, t_map *map)
{
	char	*line;
	char	**split_line;

	line = get_next_line(fd);
	if (!line)
		return (NULL);
	split_line = ft_split(line, ' ');
	free(line);
	if (!split_line)
		return (NULL);
	if (!map->width)
		map->width = ft_get_len(split_line);
	if (map->width != ft_get_len(split_line))
	{
		ft_putstr_fd("Inconsistent map width.\n", 2);
		return (NULL);
	}
	return (split_line);
}

void	ft_get_map(char *file, t_map *map)
{
	char	**column;
	int		fd;
	int		i;

	i = 0;
	map->height = ft_get_map_height(file);
	map->map = (t_point **)malloc(map->height * sizeof(t_point *));
	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (perror(file));
	while (i < map->height)
	{
		column = ft_parse(fd, map);
		map->map[i] = ft_get_row(column, i, map);
		ft_free((void **)column);
		if (!map->map[i])
		{
			ft_free((void **)map);
			close(fd);
			exit(1);
		}
		i++;
	}
	close(fd);
}
