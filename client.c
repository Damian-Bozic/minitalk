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

// error_exit takes in an int that represents the occured error, prints the
// error, then closes the program.
static int	error_exit(int error)
{
	ft_printf("ERROR %d\n", error);
	if (error == 0)
		ft_printf("Malloc failed to allocate\n");
	else if (error == 1)
		ft_printf("Invalid server PID, please check if given PID is valid\n");
	else if (error == 2)
		ft_printf("Server has not responded after message was sent\n");
	else if (error == 3)
		ft_printf("Connection with server lost unexpectedly");
	exit(error);
}

// send_pid sends the process id of the sender program to the
// reciever program in binary
static void	send_pid(int sender, int reciever)
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

// send_message sends the programs PID, text message, and exit byte to the
// process id (the server) using signals (1's and 0's).
static void	send_message(int pid, char *message)
{
	int		i;
	char	*byte;

	send_pid(getpid(), pid);
	i = 0;
	while (message[i])
	{
		byte = ft_binary_itoa(message[i]);
		if (!byte)
			error_exit(0);
		if (!send_binary(byte, pid))
			error_exit(3);
		byte = NULL;
		i++;
	}
	byte = ft_strdup("11111100");
	send_binary(byte, pid);
	sleep(5);
	error_exit(2);
}

// reciever runs when the program recieves a signal, prints an exit
// message and then closes the program.
static void	reciever(int signal)
{
	if (signal == SIGUSR1)
		ft_printf("Kicked from server\n");
	else if (signal == SIGUSR2)
		ft_printf("Server recieved message successfully\n");
	exit(1);
}

// main opens two signal channels that wait for a signal from the
// server, calls send_message, then waits indefinetly for exit().
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
// 1-byte: 		0XXXXXXX
// 2-byte: 		110XXXXX 10XXXXXX
// 3-byte: 		1110XXXX 10XXXXXX 10XXXXXX
// 4-byte: 		11110XXX 10XXXXXX 10XXXXXX 10XXXXXX
// PID:    		11111000 10XXXXXX 10XXXXXX 10XXXXXX 10XXXXXX
// last byte:	11111100

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