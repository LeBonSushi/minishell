/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 05:18:28 by macorso           #+#    #+#             */
/*   Updated: 2025/03/06 07:19:43 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	tokens_size(t_token *tokens)
{
	t_token	*curr;
	int		i;

	curr = tokens;
	i = 0;
	while (curr)
	{
		i++;
		curr = curr->next;
	}
	return (i);
}

char	**tokens_to_tab(t_token *tokens)
{
	char	**tab;
	t_token	*curr;
	int		i;

	if (!tokens)
		return (NULL);
	curr = tokens;
	tab = malloc(sizeof(char *) * (tokens_size(tokens) + 1));
	if (!tab)
		return (NULL);
	i = 0;
	while (curr)
	{
		tab[i] = ft_strdup(curr->value);
		i++;
		curr = curr->next;
	}
	tab[i] = NULL;
	return (tab);
}

char	**get_paths(t_all *all, char **env)
{
	int		i;
	char	**paths;

	i = 0;
	paths = NULL;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			paths = ft_split(env[i] + 5, ':');
			break ;
		}
		i++;
	}
	if (!paths)
	{
		ft_fprintf(2, "$PATH not found in env.\n");
		all->status = 127;
		return (NULL);
	}
	return (paths);
}

char	*find_exec_path(char *cmd, char **paths)
{
	int		i;
	char	*exec_path;
	char	*tmp;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/", 0, 0);
		exec_path = ft_strjoin(tmp, cmd, 1, 0);
		if (!access(exec_path, X_OK))
			return (exec_path);
		free(exec_path);
		exec_path = NULL;
		i++;
	}
	return (ft_strdup(cmd));
}

bool	is_dir(t_all *all, char *cmd)
{
	int	fd;
	char	*error;

	fd = open(cmd, O_RDONLY);
	error = ft_strjoin("ministress: ", cmd, 0, 0);
	if (fd < 0 && errno == EISDIR)
	{
		perror(error);
		all->status = 1;
	}
	free(error);
	close(fd);
	return (false);
}

char	*get_cmd_path(t_all *all, char *cmd, char **env)
{
	char	**paths;
	char	*exe_path;

	if (!cmd)
		return (NULL);
	if (!access(cmd, X_OK) && !is_dir(all, cmd))
		return (ft_strdup(cmd));
	if (!*env)
	{
		perror(cmd);
		return (NULL);
	}
	paths = get_paths(all, env);
	exe_path = find_exec_path(cmd, paths);
	free_tab(paths);
	return (exe_path);
}

void	pipe_error(t_all *all, int *fd)
{
	close(fd[0]);
	close(fd[1]);
	perror("pipe");
	all->status = 1;
}

void	fork_error(t_all *all, int *fd)
{
	close(fd[0]);
	close(fd[1]);
	perror("fork");
	all->status = 1;
}

void	child_process(t_all *all, t_cmd *cmd, int *fd, char **env)
{
	if (cmd->infile_fd == -2)
		cmd->infile_fd = open("/dev/null", O_RDONLY);
	close(fd[0]);
	dup2(cmd->infile_fd, STDIN_FILENO);
	if (!cmd->next)
		dup2(cmd->outfile_fd, STDOUT_FILENO);
	else
		dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	close(cmd->infile_fd);
	execve(cmd->path, cmd->args, env);
	ft_fprintf(2, "%s: Command not found\n", cmd->args[0]);
	if (errno == EACCES)
		all->status = 126;
	else if (errno == ENOENT)
		all->status = 127;
	else
		all->status = 1;
}

void	parent_process(t_cmd *curr, int *fd)
{
	close(fd[1]);
	close(curr->infile_fd);
	curr->infile_fd = dup(fd[0]);
	close(fd[0]);
}

void	waitall(t_all *all, t_cmd *cmds)
{
	t_cmd	*cmd;
	int		tmp;

	cmd = cmds;
	while (cmd)
	{
		waitpid(cmd->pid, &tmp, 0);
		cmd = cmd->next;
	}
	if (WIFEXITED(tmp))
		if (WEXITSTATUS(tmp) > all->status)
			all->status = WEXITSTATUS(tmp);
}

void	exec_cmds(t_all *all, t_cmd *cmds)
{
	t_cmd	*curr;
	char	**env;
	int		fd[2];

	env = env_to_tab(all->env);
	curr = cmds;
	while (curr)
	{
		curr->args = tokens_to_tab(curr->tokens);
		curr->path = get_cmd_path(all, curr->args[0], env);
		if (!curr->path)
			if (*curr->args)
				curr->path = ft_strdup(curr->args[0]);
		if (curr->path && !curr->skip_cmd)
		{
			if (pipe(fd) == -1)
				pipe_error(all, fd);
			curr->pid = fork();
			if (curr->pid == -1)
				fork_error(all, fd);
			if (curr->pid == 0)
				child_process(all, curr, fd, env);
			else
				parent_process(curr, fd);
		}
		if (curr->skip_cmd && curr->next)
			curr->next->infile_fd = open("/dev/null", O_RDONLY);
		curr = curr->next;
	}
	// debug_print_cmdlist(cmds);
	waitall(all, cmds);
	return (free_tab(env));
}
