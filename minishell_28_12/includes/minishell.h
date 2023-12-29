/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mneves-l <mneves-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 15:12:25 by cjoao-me          #+#    #+#             */
/*   Updated: 2023/12/29 18:30:46 by mneves-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/wait.h>
# include <signal.h>
# include <stdarg.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct s_cmd
{
	struct s_cmd	*next;
	struct s_cmd	*prev;
	char			**comand;
	int				redir[2];
	int				fd_in;
	int				fd_out;
	int 			n_cmds;
}					t_cmd;

typedef struct s_env
{
	char			*name;
	char			*content;
	struct s_env	*next;
}					t_env;

typedef struct s_data
{
	t_cmd			*cmds;
	t_env			*env;
	int				store_std[2];
	int				flag_pipe;
	int 			flag_exec;
	int				status;
}					t_data;

//main - minishell.c
void		print_cmd_list(t_cmd *c);
void		init_data(t_data *sh, char **env);
int			handle_input(char *l);

//main - signals.c
void		set_signals(void);
void		signal_handler(int sig);
void		signals_here_doc(int sig);
int			set_exit_code(int i, int flag);

//parser - syntax.c
int			check_syntax(char *line, int i);
int			check_quotes(char *line, int *i);
int			check_redir(char *line, int *i);
int			check_pipe(char *line, int *i);

//parser - syntax_utils.c
int			ft_is_space(char c);
void		skip_spaces(char *s, int *i);
void		print_syntax_error(char c);
int			matrixlen(char **str);
int			check_mixed_redir(char *line, char c, int *i);

//parser - redirections.c
void		manage_redirections(char **line, int *i, t_cmd *cm, t_data sh);
int			redir_type(char *str);

//parser - heredoc.c
int			ft_here_doc(char *limiter, t_data sh);
char		*expander_heredoc(char *str, t_data sh);
void		ft_hd_child(char *limiter, int fd, t_data sh);

//parser - quote_utils.
void		skip_quotes(char *line, int *i, char c);
void		delete_char(char *line, int i);
void		delete_quotes(char *line);
void		copy_quotes(char *line, int *i, char *newline, int *j);

//parser - parse.c
t_cmd		*parsing(char **first, t_data sh);
void		set_comand_list(char **line, t_cmd *cm, t_data sh);
char		*prepare_2nd_split(char *line);
void		replace_spaces(char *line);
void		treat_great_less(char *line, int *i, char *newline, int *j);

//parser - command_list_utils.c
t_cmd		*new_comand(void);
t_cmd		*last_comand(t_cmd *c);
void		add_comand(t_cmd **lst, t_cmd *new);
int			number_comands(t_cmd *lst);

//parser - expander.c
void		expand_comands(t_cmd **list, t_data sh);
char		*expander(char *str, t_data sh);
void		expand_loop(char *str, char *final, int *j, t_data sh);
char		*get_var_value(char *str, int *i, t_data sh);
char		*get_var_name(char *str, int *i);

//parser - expander_utils.c
void		copy_var_value(char *s1, int *j, char *s2);
char		*get_env_value(char *str, int *i, t_env *ev);

//utils - env.c
void		get_env(char **env, t_env **ev);
int			g_size(char *s, int a);
t_env		*ev_lstnew(char *name, char *content);
void		ev_lstadd_back(t_env **lst, t_env *new);
int			ev_lstsize(t_env *lst);

//utils - free_utils.c
void		free_split(char **str);
void		free_env(t_env **ev);
void		free_comand(t_cmd **lst);

//utils - get_next_line.c
char		*get_next_line(int fd);

//builtins - builtins.c
void		choose_builtin(t_data *data, t_cmd *cmd, int *pids);
void		do_echo(t_cmd *cmd);
void		print_env(t_env **stack, t_cmd *cmd);
void		do_pwd(t_env **stack, t_cmd *cmd);
void		do_export(t_env *env, t_cmd *cmd, t_data *data);

//builtins - do_cd.c
void		do_cd(t_env *env, t_cmd *cmd);
void		*get_path(t_env *env, t_cmd *cmd);
void		update_dirs(char *newdir, t_env *env);
char		*get_env_content(char *name, t_env *env);
t_env		*get_env_node(char *name, t_env *env);

//builtins - do_exit.c
void		do_exit(t_cmd *cmd);
int			check_exit_arg(char *str);
int			get_code(char *str);

//builtins - export.c
void		check_var(char *cmd, t_env *env);
int			check_repeat(char *cmd, t_env *env);
void		set_variable(t_env *env, char *cmd);
void		print_export(t_env *env, t_data *data, t_cmd *cmd);
void		bubble_sort(t_env *env);

//builtins - builtins_2.c
void		do_unset(t_cmd *cmd, t_env **env);

//builtins - utils.c
void		echo_util(t_cmd *cmd, int flag);
void		do_swap(t_env *env);
void		unset_utils(char *s, t_env **head);
void		print_cd_error(char *s);

//executor - executor.c
void		executor(t_data sh);
void		do_execve(t_data *data, t_cmd *cmd, int flag, int *pids);
char		**env_to_matrix(t_env *env);

//executor - utils_executor.c
char		*to_path(char *cmd, char **envp);
char		*path_to_cmd(char *cmd, char **path);
void		execute_cmd(char **cmd, char **envp, int *pids);

//executor - pipes.c
void    	pipe_process(t_cmd *cmd, t_data data);
int     	child_process(t_data data, t_cmd *cmd, int *pids);
void    	error(char *s);
void    	last_process(t_cmd *cmd, t_data data, int *pids);
void    	do_child(t_data data, t_cmd *cmd, int *pids, int *pipefd);
void    	wait_all(t_cmd *cmd, int *pids);

#endif
