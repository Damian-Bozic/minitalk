/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbozic <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:25:55 by dbozic            #+#    #+#             */
/*   Updated: 2024/06/14 15:03:27 by dbozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

int	error_exit(int	error)
{
	ft_printf("ERROR %d\n", error);
	if (error == 0)
		ft_printf("Malloc failed to allocate\n");
	else if (error == 1)
		ft_printf("Failed to identify given PID\n");
	else if (error == 2)
		ft_printf("Server has not responded after message was sent\n");
	exit(error);
}

char *ft_binary_itoa(unsigned char nbr)
{
	char *rtn;
	int	i;

	i = 7;
	rtn = malloc(9);
	if (!rtn)
		return (NULL);
	rtn[8] = 0;
	while (i >= 0)
	{
		rtn[i] = (nbr % 2) + '0';
		nbr = nbr / 2;
		i--;
	}
	return (rtn);
}

int	ft_binary_atoi(char *str)
{
	int i;
	int	rtn;
	int	power;

	i = ft_strlen(str);
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

int	send_binary(char *str, int pid)
{
	int	i;

	i = 0;
	if (!str)
		error_exit(1);
	while (str[i])
	{
		if (str[i] == '0')
		{
			if (kill(pid, SIGUSR1) == -1)
			{
				free(str);
				return (0);
			}
		}
		else if (kill(pid, SIGUSR2) == -1)
		{
			free(str);
			return (0);
		}
		i++;
		usleep(100);
	}
    free(str);
	return (1);
}

char *full_binary_pid(int pid)
{
	char	*rtn;
	int	i;
	int	j;

	i = 31;
	rtn = malloc(33);
	if (!rtn)
		return (NULL);
	rtn[32] = 0;
	while (i >= 0)
	{
		j = 6;
		while (j > 0)
		{
			rtn[i] = (pid % 2) + '0';
			pid = pid / 2;
			i--;
			j--;
		}
		rtn[i] = '0';
		i--;
		rtn[i] = '1';
		i--;
	}
	return (rtn);
}

void	send_pid(int sender, int reciever)
{
	char	*raw_pid;
	char	*final;
	int		i;

	i = 0;
	raw_pid = full_binary_pid(sender);
	if (!raw_pid)
		error_exit(0);
	final = ft_strjoin ("11111000", raw_pid);
	free (raw_pid);
	if (!final)
		error_exit(0);
	if (!send_binary(final, reciever))
		error_exit(1);
}

void	send_message(int pid, char *message)
{
    int         i;
    char        *byte;

    send_pid(getpid(), pid);
    i = 0;
    while (message[i])
    {
        byte = ft_binary_itoa(message[i]);
        if (!byte)
            error_exit(0);
        if(!send_binary(byte, pid))
            error_exit(1);
        byte = NULL;
        i++;
    }
    byte = ft_strdup("11111100");
    if (!byte)
        exit(0);
    send_binary(byte, pid);
    sleep(5);
    error_exit(2);
}

void reciever(int signal)
{
    if (signal == SIGUSR1)
        ft_printf("Kicked from server\n");
    else if (signal == SIGUSR2)
        ft_printf("Server recieved message successfully\n");
    exit(1);
}

int	main(int argc, char	**argv)
{
	int	pid;

    signal(SIGUSR1, reciever);
    signal(SIGUSR2, reciever);
	if (argc != 3)
	{
		ft_printf("Invalid input\nTry again using the format: ");
		ft_printf("./client Server_PID %cMessage%c\n", '"', '"');
		return (0);
	}
	pid = ft_atoi(argv[1]);
	send_message(pid, argv[2]);
	pause();
}

// UTF-8
// 1-byte: 0XXXXXXX
// 2-byte: 110XXXXX 10XXXXXX
// 3-byte: 1110XXXX 10XXXXXX 10XXXXXX
// 4-byte: 11110XXX 10XXXXXX 10XXXXXX 10XXXXXX

// CODE GRAVEYARD:

/* char	*full_binary_itoa2(long nbr, int sign, int i)
{
	char	*rtn;
	
	rtn = malloc (i + 1);
	if (!rtn)
		return (NULL);
	rtn[i + 1] = '\0';
	i--;
	while((i >= 0 && sign == 1) || (i > 0 && sign == -1))
	{
		rtn[i] = (nbr % 2) + '0';
		nbr = nbr / 2;
		i--;
	}
	if (sign == -1)
		rtn[0] = '-';
	return (rtn);
}

char	*full_binary_itoa(int nbr)
{
	int		i;
	int		sign;
	long	j;

	i = 0;
	sign = 1;
	j = nbr;
	if (nbr < 0)
	{
		sign = -1;
		j = -j;
		i++;
	}
	while (nbr != 0)
	{
		nbr = nbr / 2;
		i++;
	}
	return(full_binary_itoa2(j, sign, i));
} */