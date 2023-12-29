/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjoao-me <cjoao-me@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 15:11:40 by cjoao-me          #+#    #+#             */
/*   Updated: 2023/12/29 18:01:36 by cjoao-me         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expander_heredoc(char *str, t_data sh)
{
	char	*final;
	char	*var;
	int		j;
	int		i;

	var = NULL;
	final = malloc(2048);
	if (!final)
		return (NULL);
	j = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] != '$' || ((str[i] == '$' && (str[i + 1] == ' ' \
			|| str[i + 1] == '\0' || str[i + 1] == '"' || str[i + 1] == '\''))))
			final[j++] = str[i++];
		else
		{
			var = get_var_value(str, &i, sh);
			copy_var_value(final, &j, var);
		}
	}
	final[j] = 0;
	free(str);
	return (final);
}

void	ft_hd_child(char *limiter, int fd, t_data sh)
{
	char	*line;

	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		write(1, "> ", 2);
		line = get_next_line(0);
		if (!line)
		{
			printf("warning: here-doc delimited by ");
			printf("end-of-file (wanted '%s')\n", limiter);
			break ;
		}
		if ((ft_strncmp(line, limiter, ft_strlen(limiter)) == 0) && \
			(ft_strlen(line) - 1 == ft_strlen(limiter)))
			break ;
		line = expander_heredoc(line, sh);
		write(fd, line, ft_strlen(line));
		free(line);
	}
	close(fd);
	exit(0);
}

int	ft_here_doc(char *limiter, t_data sh)
{
	pid_t	pid;
	int		file;
	int		status;

	status = 0;
	file = open("heredoc", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (file < 0)
		return (-1);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, signals_here_doc);
	pid = fork();
	if (pid < 0)
	{
		set_exit_code(1, 1);
		return (-1);
	}
	if (pid == 0)
		ft_hd_child(limiter, file, sh);
	signal(SIGINT, SIG_IGN);
	waitpid(0, &status, 0);
	set_exit_code(0, 0);
	return (open("heredoc", R_OK));
}
