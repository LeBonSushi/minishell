/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 00:41:51 by macorso           #+#    #+#             */
/*   Updated: 2025/02/17 08:49:42 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_tree  *find_highest_operator(t_token *start, t_token *end)
{
    t_token *curr;
    t_token *highest_op;
    int     paren_count;

    highest_op = NULL;
    curr = start;
    paren_count = 0;
    while (curr && curr != end)
    {
        if (curr->token_type == OPEN_PARENTHESIS)
            paren_count++;
        else if (curr->token_type == CLOSE_PARENTHESIS)
            paren_count--;
        else if (paren_count == 0 && (curr->token_type == OR 
            || curr->token_type == AND))
        {
            if (!highest_op || curr->token_type >= highest_op->token_type)
                highest_op = curr;
        }
        curr = curr->next;
    }
    if (!highest_op)
        return (NULL);
    return (create_operator_node(highest_op->token_type));
}

t_token *find_matching_parenthesis(t_token *start)
{
    int     count;
    t_token *curr;

    count = 1;
    curr = start->next;
    while (curr && count > 0)
    {
        if (curr->token_type == OPEN_PARENTHESIS)
            count++;
        else if (curr->token_type == CLOSE_PARENTHESIS)
            count--;
        if (count == 0)
            return (curr);
        curr = curr->next;
    }
    return (NULL);
}

t_tree  *build_command_node(t_token *start, t_token *end)
{
    t_tree          *node;
    t_token         *curr;
    t_string_list   *args;
    t_string_list   *redir_in;
    t_string_list   *redir_out;

    node = malloc(sizeof(t_tree));
    if (!node)
        return (NULL);
    node->node_type = CMD;
    node->left = NULL;
    node->right = NULL;
    args = NULL;
    redir_in = NULL;
    redir_out = NULL;
    curr = start;
    while (curr && curr != end)
    {
        if (curr->token_type == WORD)
            string_list_add(&args, curr->value);
        else if (curr->token_type == REDIR_IN)
            string_list_add(&redir_in, curr->next->value);
        else if (curr->token_type == REDIR_OUT || curr->token_type == REDIR_APPEND)
            string_list_add(&redir_out, curr->next->value);
        curr = curr->next;
    }
    node->args = args;
    node->redir_in_names = redir_in;
    node->redir_out_names = redir_out;
    return (node);
}

t_tree  *build_tree(t_token *start, t_token *end)
{
    t_tree  *root;
    t_token *close_paren;

    if (!start || start == end)
        return (NULL);

    root = find_highest_operator(start, end);
    if (root)
    {
        t_token *op = start;
        while (op != end && (op->token_type != OR && op->token_type != AND))
            op = op->next;
        root->left = build_tree(start, op);
        root->right = build_tree(op->next, end);
        return (root);
    }

    if (start->token_type == OPEN_PARENTHESIS)
    {
        close_paren = find_matching_parenthesis(start);
        if (!close_paren)
            return (NULL);
        return (build_tree(start->next, close_paren));
    }

    return (build_command_node(start, end));
}

static void handle_redirections(t_string_list *redir_in, t_string_list *redir_out)
{
    t_string_list   *curr;
    int             fd;

    if (redir_in)
    {
        curr = redir_in;
        fd = open(curr->str, O_RDONLY);
        if (fd == -1)
            exit(1);
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    if (redir_out)
    {
        curr = redir_out;
        fd = open(curr->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
            exit(1);
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

static int  execute_command_node(t_tree *node, t_all *all)
{
    pid_t   pid;
    int     status;
    char    **args;

    args = string_list_to_array(node->args);
    if (!args)
        return (1);
    pid = fork();
    if (pid == -1)
    {
        free_array(args);
        return (1);
    }
    if (pid == 0)
    {
        handle_redirections(node->redir_in_names, node->redir_out_names);
        execve(args[0], args, all->env);
        exit(127);
    }
    free_array(args);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    return (1);
}

int execute_tree(t_tree *root, t_all *all)
{
    int status;

    if (!root)
        return (0);

    if (root->node_type == CMD)
        return (execute_command_node(root, all));

    if (root->node_type == AND)
    {
        status = execute_tree(root->left, all);
        if (status == 0)
            return (execute_tree(root->right, all));
        return (status);
    }

    if (root->node_type == OR)
    {
        status = execute_tree(root->left, all);
        if (status != 0)
            return (execute_tree(root->right, all));
        return (status);
    }

    return (1);
}

void    process_parenthesis(t_token *tokens, t_all *all)
{
    t_tree  *root;
    int     status;

    if (!tokens)
        return;
    root = build_tree(tokens, NULL);
    if (root)
    {
        status = execute_tree(root, all);
        all->status = status;
		free_tree(root);
    }
}