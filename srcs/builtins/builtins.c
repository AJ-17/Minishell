/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprojean <cprojean@42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 17:22:48 by cprojean          #+#    #+#             */
/*   Updated: 2023/06/23 13:08:05 by cprojean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(char **array)
{
	int	runner;
	int	type;

	runner = 1;
	type = 0;
	if (ft_strncmp(array[1], "-n", 2) == 0)
	{
		type = -1;
		runner++;
	}
	while (array[runner])
	{
		ft_printf("%s ", array[runner]);
		runner++;
	}
	if (type != -1)
		ft_printf("\n");
}

void	ft_env(t_list **env)
{
	t_list	*runner;

	runner = *env;
	while (runner != NULL)
	{
		ft_printf("%s\n", runner->content);
		runner = runner->next;
	}
}

char	*ft_pwd()
{
	char	*path;
	int		size;

	size = 2048;
	path = malloc(sizeof(char) * (size));
	getcwd(path, size);
	return (path);
}

void	ft_cd(char *directory, t_list **my_env)
{
	char	*oldpwd;
	char	*pwd;
	
	oldpwd = ft_strjoin4("OLDPWD=", ft_pwd());
	ft_export(my_env, oldpwd);
	if (!directory)
		chdir("/nfs/homes");
	chdir(directory);
	pwd = ft_strjoin4("PWD=", ft_pwd());
	ft_export(my_env, pwd);
}
