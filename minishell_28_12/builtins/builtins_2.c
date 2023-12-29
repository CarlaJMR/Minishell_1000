/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mneves-l <mneves-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:06:56 by mneves-l          #+#    #+#             */
/*   Updated: 2023/12/04 20:47:41 by mneves-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_unset(t_cmd *cmd, t_env **env)
{
	int	i;

	i = 0;
	if (!cmd->comand[1])
		return ;
	while (cmd->comand[i])
	{
		unset_utils(cmd->comand[i], env);
		i++;
	}
}
