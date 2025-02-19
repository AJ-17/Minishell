/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprojean <cprojean@42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 14:11:04 by ajakubcz          #+#    #+#             */
/*   Updated: 2023/07/29 15:35:34 by cprojean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_errno;

void	handler(int sig);
void	init_data(t_exec *data, char **env);
void	minishell(t_list **my_env, char **env, t_exec *data);
void	dup_entrys(t_exec *data);

int	main(int ac, char **av, char **env)
{
	t_list	*my_env;
	t_exec	data;

	(void) ac;
	(void) av;
	g_errno = 0;
	my_env = NULL;
	if (!env || !env[0])
		ft_create_env(&my_env);
	else
		ft_dup_env(env, &my_env);
	minishell(&my_env, env, &data);
	ft_lstclear(&my_env, free);
	rl_clear_history();
}

void	minishell(t_list **my_env, char **env, t_exec *data)
{
	char	*cmd;
	char	*prompt;

	cmd = NULL;
	while (42)
	{
		init_data(data, env);
		prompt = ft_get_prompt(my_env);
		signal(SIGINT, handler);
		signal(SIGQUIT, SIG_IGN);
		cmd = readline(prompt);
		ft_free(prompt, NULL);
		if (!cmd)
			break ;
		if (ft_strlen(cmd) != 0)
			add_history(cmd);
		parse_and_exec(cmd, my_env, data);
	}
	if (!cmd)
	{
		ft_printf("exit\n");
		rl_clear_history();
	}
	free(cmd);
}
//if CTRL-D

void	handler(int sig)
{
	(void) sig;
	g_errno = 130;
	ft_printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	init_data(t_exec *data, char **env)
{
	data->fd_in = -1;
	data->fd_out = -1;
	data->mode = 0;
	data->env = env;
	data->all_limiter = NULL;
	data->str_heredoc = NULL;
}

void	free_all(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i])
			free(str[i]);
		i++;
	}
	if (str)
		free(str);
}
