/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakubcz <ajakubcz@42Lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 17:22:21 by ajakubcz          #+#    #+#             */
/*   Updated: 2023/06/20 11:00:19 by ajakubcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_get_path(void);
static int	is_in_path(char *home, char *path);
static char	*replace_in_path(char *home, char *path);

char	*ft_get_prompt(void)
{
	char	*prompt;
	char	*user;
	char	*path;

	user = getenv("USER");
	path = ft_get_path();
	prompt = ft_strjoin2(ft_strdup(user), ft_strdup(":"));
	prompt = ft_strjoin2(prompt, path);
	prompt = ft_strjoin2(prompt, ft_strdup("$ "));
	return (prompt);
}

static char	*ft_get_path(void)
{
	char	*path;
	char	*home;

	path = getenv("PWD");
	home = getenv("HOME");
	if (is_in_path(home, path))
		path = replace_in_path(home, path);
	return (path);
}

static int	is_in_path(char *home, char *path)
{
	int	i;

	i = 0;
	while (home[i] && path[i] && home[i] == path[i])
		i++;
	if (home[i] == '\0')
		return (1);
	else
		return (0);
}

static char	*replace_in_path(char *home, char *path)
{
	int		i;
	char	*new_path;

	i = 0;
	while (home[i] && path[i] && home[i] == path[i])
		i++;
	new_path = ft_strjoin("~", &path[i]);
	return (new_path);
}
