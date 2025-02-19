/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprojean <cprojean@42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 17:22:48 by cprojean          #+#    #+#             */
/*   Updated: 2023/07/26 01:59:12 by cprojean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_echos_flag(char *str);

void	ft_echo(t_parse *parse)
{
	int	index;
	int	type;
	int	flag;

	flag = 0;
	type = 0;
	index = 0;
	while (parse[index].str && parse[index++].type != PIPEE)
	{
		if (parse[index].type == CMD_ARG)
		{
			if (is_echos_flag(parse[index].str) == 0 || flag == 1)
			{
				if (flag == 1)
					ft_printf(" ");
				ft_printf("%s", parse[index].str);
				flag = 1;
			}
			else if (flag != 1)
				type = 1;
		}
	}
	if (!type)
		ft_printf("\n");
	g_errno = 0;
}

int	is_echos_flag(char *str)
{
	size_t	runner;

	runner = 0;
	if (str[runner] == '-')
	{
		runner++;
		while (str[runner] && str[runner] == 'n')
			runner++;
	}
	if (runner == ft_strlen(str) && str[0])
		return (1);
	return (0);
}

void	ft_env(t_list **env, t_parse *parse)
{
	t_list	*runner;
	int		index;

	index = 0;
	runner = *env;
	while (parse[index].str && parse[index].type != PIPEE)
	{
		if (parse[index].type == CMD_ARG)
		{
			ft_error(ERROR_13, parse[index].str, 127);
			return ;
		}
		index++;
	}
	while (runner != NULL)
	{
		if (ft_equal_ind(runner->content) != 0)
			ft_printf("%s\n", runner->content);
		runner = runner->next;
	}
	g_errno = 0;
}

char	*ft_pwd(int mode)
{
	char	*path;

	if (mode != 0)
		g_errno = 0;
	path = getcwd(NULL, 0);
	if (!path && mode != 0)
		ft_printf("LOST\n");
	return (path);
}
