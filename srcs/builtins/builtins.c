/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprojean <cprojean@42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 17:22:48 by cprojean          #+#    #+#             */
/*   Updated: 2023/07/21 01:16:05 by cprojean         ###   ########.fr       */
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
	if (runner == ft_strlen(str))
		return (1);
	return (0);
}

void	ft_env(t_list **env)
{
	t_list	*runner;

	runner = *env;
	while (runner != NULL)
	{
		if (ft_equal_size(runner->content) != 0)
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
	return (path);
}

void	ft_cd(t_list **my_env, t_parse *parse)
{
	char	*oldpwd;
	int		runner;
	int		count;
	int		flag;

	count = 0;
	flag = 0;
	runner = 0;
	g_errno = 0;
	while (parse[runner++].str)
	{
		if (parse[runner].type == CMD_ARG)
		{
			count++;
			flag = count;
		}
	}
	if (count > 1)
	{
		ft_error(ERROR_23, parse[runner].str, 1);
		return ;
	}
	oldpwd = ft_strjoin4("OLDPWD=", ft_pwd(0));
	do_ft_export(my_env, oldpwd);
	next_cd(my_env, parse[flag].str);
}
