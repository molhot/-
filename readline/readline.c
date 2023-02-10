#include "../minishell.h"
#include <stdio.h>

/*__attribute__((destructor))
static void destructor()
{
	system("leaks -q minishell");
}*/

int main()
{
	char	  *line;
	t_token	*tok;
  t_node  *node;

	rl_outstream = stderr;
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break;
		if (*line)
			add_history(line);
		if (line[0] == '/' || line[0] == '.')
			abusolute_path(line);
		else
		{
			tok = tokenizer(line);
			//tok には空白区切りで全てのものが格納されている
			expand(tok);
			node = parse(tok);
			node->command->now_in = STDIN_FILENO;
			node->command->now_out = STDOUT_FILENO;
			t_node *test;
			test = node;
			//if (tok->kind == EOF)
			//	;
			// if (syntax_error)
			//	continue;
			/*
			t_token *t = tok;
			while(t->next != NULL)
			{
				printf("%s\n", t->word);
				t = t->next;
			}
			*/
			exec(node->command);
			t_redirect *redirect;
			redirect = *(node->command->redirect);
			// while (redirect->next != NULL)
			// 	redirect = redirect->next;
			//  while (redirect != NULL)
			// {
			// 	if (redirect->type == IN)
			// 	{
			// 		close(node->command->now_in);
			// 		dup2(redirect->stash_fd, node->command->now_in);
			// 	}
			// 	else
			// 	{
			// 		close(node->command->now_out);
			// 		dup2(redirect->stash_fd, node->command->now_out);
			// 	}
			// 	redirect = redirect->before;
			// }
			// printf("now 'now in' is >%d\n", node->command->now_in);
			// printf("now 'now out' is >%d\n", node->command->now_out);
			if (tok != NULL)
				free_token(tok);
		}
		free(line);
	}
	exit(0);
}
