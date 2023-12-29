/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjoao-me <cjoao-me@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 10:55:55 by mneves-l          #+#    #+#             */
/*   Updated: 2023/12/29 12:03:01 by cjoao-me         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    executor(t_data sh)
{
	//int     fd[2];

	if(sh.flag_pipe == 0)
	{
		sh.cmds->fd_in = sh.cmds->redir[0];
		sh.cmds->fd_out = sh.cmds->redir[1];
		choose_builtin(&sh);
	}
}

void    do_execve(t_data *data)
{
	pid_t   child;
	char ** our_env;

	our_env = env_to_matrix(data->env);
	
	child = fork();
	if(child < 0)
		perror("error: fork");
	if(child == 0)
	{
		dup2(data->cmds->fd_in, STDIN_FILENO);
		dup2(data->cmds->fd_out, STDOUT_FILENO);
		execute_cmd(data->cmds->comand, our_env, NULL);
		exit(EXIT_FAILURE);
	}
	waitpid(child, NULL, 0);
	free_split(our_env);
}

char    **env_to_matrix(t_env *env)
{
	char    **matriz;
	t_env   *tmp;
	int         i;
	char    *name;
	char    *content;

	tmp = env;
	matriz = (char **)malloc((ev_lstsize(env) + 1) * sizeof(char *));
	i = 0;

	while(tmp)
	{
		name = ft_strjoin(tmp->name, "=");
		content = ft_strjoin(name, tmp->content);
		free(name);
		matriz[i] = ft_strdup(content);
		free(content);
		i++;
		tmp = tmp->next;
	}
	matriz[i] = NULL;
	return(matriz);
}