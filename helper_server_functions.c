/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_server_functions.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbozic <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 16:04:15 by dbozic            #+#    #+#             */
/*   Updated: 2024/06/18 16:04:17 by dbozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

// ft_binary_atoi_mod takes in a binary string the size of a byte, then returns
// an int. 01100001 -> 97 -> 'a'
int	ft_binary_atoi_mod(char *str)
{
	int	i;
	int	rtn;
	int	power;

	i = 8;
	rtn = str[i - 1] - '0';
	i = i - 2;
	power = 2;
	while (i >= 0)
	{
		rtn = rtn + ((str[i] - '0') * power);
		power = power * 2;
		i--;
	}
	return (rtn);
}

// pid_binary_atoi takes in a binary string that represents a PID and
// decodes its format. (11111000 10XXXXXX 10XXXXXX 10XXXXXX 10XXXXXX)
int	pid_binary_atoi(char *str)
{
	int	i;
	int	j;
	int	power;
	int	rtn;

	i = 38;
	power = 2;
	rtn = (str[39] - '0');
	j = 1;
	while (i > 9)
	{
		while (j < 6)
		{
			rtn = rtn + ((str[i] - '0') * power);
			power = power * 2;
			i--;
			j++;
		}
		i = i - 2;
		j = 0;
	}
	return (rtn);
}

// pid_event_handler takes in a binary string.
// if the first 8 bits are 11111000, then *code is sent to pid_binary_atoi()
//  and then that pid is stored as the clients process id.
// if the first 8 bits are 11111100, then a "message recieved" signal gets
//  sent to the client process id stored from before.
void	pid_event_handler(char *code)
{
	static int	client = 0;

	if (client == 0 && ft_strncmp(code, "11111000", 8) == 0)
	{
		client = pid_binary_atoi(code);
		ft_printf("\nSuccessfully connected with %d\n", client);
		ft_printf("Start of %d's message.\n---\n", client);
	}
	else if (client != 0 && ft_strncmp(code, "11111100", 8) == 0)
	{
		usleep(1000);
		ft_printf("\n---\nEnd of %d's message.\n", client);
		kill(client, SIGUSR2);
		client = 0;
	}
}
