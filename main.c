/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 17:35:13 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/06/21 11:51:51 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



int global_sig = 0;




void restory_window(t_cmd *cmd)
{
	dup2(cmd->fd[0], 0);
	dup2(cmd->fd[1], 1);
	close(cmd->fd[0]);
	close(cmd->fd[1]);
}



void handel_signal(int sig)
{
	if (sig == SIGINT)
	{
		global_sig = 2;
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else 
		global_sig = sig;
}

void excute_redrction(t_cmd **cmd, t_env *env_list)
{
		if((*cmd)->redirs)
		{
			if((*cmd)->redirs->fd[0] == -1)
			{
				(*cmd)->data.exit_status = get_or_set(SET, 1);
				restory_window((*cmd));
				return ;
			}
			ft_redircte((*cmd)->redirs, env_list, *cmd);
		}
}

void cleanup_all_heredocs(t_cmd *cmd)
{
	t_cmd *tmp = cmd;
	t_redir *tmp_redir;
	
	while (tmp)
	{
		tmp_redir = tmp->redirs;
		while (tmp_redir)
		{
			if (tmp_redir->type == 3)
			{
				close_heredoc_fds(tmp_redir);
			}
			tmp_redir = tmp_redir->next;
		}
		tmp = tmp->next;
	}
}

void check_line(t_cmd **command, t_env **env_list, char *env[])
{
	t_cmd	*cmd;
	cmd = *command;

	(*command)->fd[0] = dup(0);
	(*command)->fd[1] = dup(1);
    if ((*command)->redirs != NULL)
    {
        if ((*command)->flag == 1)
        {
			cleanup_all_heredocs(cmd);
            restory_window(*command);
            return;
        }
    }
	if (cmd->pipe_out)
	{
		ft_excute_mult_pipe(cmd, *env_list, env);
		cleanup_all_heredocs(cmd);
		get_or_set(SET, cmd->data.exit_status);
		restory_window(cmd);
		return ;
	}
	else
	{
		excute_redrction(command, *env_list);
		execute_single_command(*command, env_list, env);	
	}
	restory_window(cmd);
}



void handl_sig_herdoc()
{
	ft_putstr_fd("\n", 1);
	exit(12);
}
void wait_parent_children(t_cmd *cmd, pid_t child_pid)
{
	int status;

	status = 0;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(child_pid, &status, 0);
	cmd->data.exit_status = WEXITSTATUS(status);
		if (WIFSIGNALED(status))
		{
			int sig = WTERMSIG(status);
			if (sig == SIGQUIT)
				ft_putstr_fd("Quit: 3\n", 1);
			else if (sig == SIGINT)
			{
				cmd->flag = 1;
				cmd->data.exit_status = get_or_set(SET, 1);
			}
			else
			{
				cmd->data.exit_status = get_or_set(SET, 0);
			}
		}
}
void hb(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}


void check_sig(t_cmd *cmd)
{
	if(global_sig != 0)
	{
		if (global_sig == 2)
			global_sig = 1;
		else
			global_sig = 0;
		cmd->data.exit_status = get_or_set(SET, global_sig);
	}
}




//------------main--------------//
void shell_mode(void)
{
	if(!isatty(1) || !isatty(0))
		exit(1);
}

t_shell_var *init_shell(char **env)
{
    t_shell_var *var;
    
    var = malloc(sizeof(t_shell_var));
    if (!var)
        return (NULL);
	var->env_struct = NULL;
    var->env_struct = env_maker(env, &var->env_struct);
    if (!var->env_struct)
        var->env_struct = env_null();
    var->env = env;
    var->exit_status = 0;
    add_one_shlvl(var->env_struct);
    tcgetattr(1, &var->infos);
    return (var);
}

void setup_signals_and_terminal(void)
{
    desable_echo_term();
    signal(SIGINT, handel_signal);
    signal(SIGQUIT, SIG_IGN);
}

char *get_user_input(t_cmd *cmd)
{
    char *input;
    
    input = readline("minishell $> ");
    if (!input)
    {
        printf("exit\n");
        if (!cmd || !cmd->data.exit_status)
            exit(0);
        exit(cmd->data.exit_status);
    }
    add_history(input);
    return input;
}

char *process_input(char *input, int *should_continue)
{
    char *preprocessed_input;
    
    if (check_quotes(input))
    {
        get_or_set(SET, 258);
        free(input);
        *should_continue = 1;
        return (NULL);
    }
    preprocessed_input = preprocess_command(input);
    if (!preprocessed_input)
    {
        free(input);
        *should_continue = 1;
        return (NULL);
    }
    free(input);
    *should_continue = 0;
    return (preprocessed_input);
}


t_token *create_token_list(char *preprocessed_input)
{
    char *new_input;
    t_token *token_list;
    
    new_input = change_space(preprocessed_input);
    token_list = tokin_list_maker(new_input);
    free(preprocessed_input);
    return (token_list);
}

void execute_command(t_token *token_list, t_shell_var *state)
{
    t_cmd *cmd;
    
    cmd = parser(token_list);
    split_stoper(cmd);
    free_token_list(token_list);
    check_sig(cmd);
    expand_handle(cmd, state->env_struct, get_or_set(GET, 0));
    ambiguous_finder(cmd);
    process_quotes_for_cmd(cmd, 1);
    change_back_cmd(cmd);
    file_opener(cmd, state->env_struct);
    print_ambiguous_redir_errors(cmd);
    check_line(&cmd, &state->env_struct, state->env);
    free_cmd_list(cmd);
    global_sig = 0;
}


void process_tokens(t_token *token_list,  t_shell_var *state)
{
    int has_pipe_error;
    int has_syntax_error;
    
    if (!token_list)
        return;
    
    has_pipe_error = error_pipi(token_list);
    has_syntax_error = check_syntax_errors(token_list);
    
    if (!has_pipe_error && !has_syntax_error)
        execute_command(token_list, state);
    else if (has_pipe_error || has_syntax_error)
    {
        get_or_set(SET, 258);
        free_token_list(token_list);
    }
    else
        free_token_list(token_list);
}


void minishell_loop(t_shell_var *state)
{
    char *input;
    char *preprocessed_input;
    t_token *token_list;
    int should_continue;
    t_cmd *cmd = NULL;
    
    while (1)
    {
        setup_signals_and_terminal();
        input = get_user_input(cmd);
        preprocessed_input = process_input(input, &should_continue);
        if (should_continue)
            continue;
        token_list = create_token_list(preprocessed_input);
        process_tokens(token_list, state);
        tcsetattr(1, TCSANOW, &state->infos);
    }
}


void cleanup_minishell(t_shell_var *state)
{
    if (state)
    {
        free_env_struct(state->env_struct);
        free(state);
    }
}

int main(int argc, char *argv[], char *env[])
{
    t_shell_var *var;
    
    (void)argc;
    (void)argv;
    shell_mode();
    var = init_shell(env);
    if (!var)
        return (1);
    minishell_loop(var);
    cleanup_minishell(var);
    return (0);
}
