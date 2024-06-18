/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbozic <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:25:55 by dbozic            #+#    #+#             */
/*   Updated: 2024/06/11 16:25:57 by dbozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

// handle_special_char takes in a byte in the form of a str, then depending on
// its type (start of a UTF-8 char, PID, or end of message byte) it will either 
// print a UTF-8 char, or send the collected binary str to pid_event_handler()
// ENCODING FORMAT:
// 1-byte: 		0XXXXXXX (ascii)
// 2-byte: 		110XXXXX 10XXXXXX (utf-8)
// 3-byte: 		1110XXXX 10XXXXXX 10XXXXXX (utf-8)
// 4-byte: 		11110XXX 10XXXXXX 10XXXXXX 10XXXXXX (utf-8)
// PID:    		11111000 10XXXXXX 10XXXXXX 10XXXXXX 10XXXXXX (custom)
// last byte:	11111100 (custom)
static char	*handle_special_char(char *chr)
{
	unsigned char	byte_array[4];
	int				i;
	int				j;

	i = 0;
	while (chr[i] == '1')
		i++;
	if (i <= 4)
	{
		j = (i - 1) * 8;
		while (i > 0)
		{
			byte_array[i - 1] = ft_binary_atoi_mod(&chr[j]);
			j = j - 8;
			i--;
		}
		write(1, byte_array, sizeof(byte_array));
	}
	if (i > 4)
		pid_event_handler(chr);
	free(chr);
	return (NULL);
}

// print_char takes in a binary string if that str is an ascii char it 
// will print the char, if it is a special str (UTF-8 char or Special 
// code) it will wait until all extra bytes are collected, then it will
// send that mutli byte binary str to handle_special_char().
static void	print_char(char *byte)
{
	static char	*chr = NULL;
	static int	flag = 0;

	if (ft_strncmp(byte, "11111100", 8) == 0)
		pid_event_handler(byte);
	else if (ft_strncmp(byte, "0", 1) == 0 && flag == 0 && !chr)
		ft_printf("%c", ft_binary_atoi_mod(byte));
	else if (ft_strncmp(byte, "11", 2) == 0 && !chr)
	{
		while (byte[flag + 1] == '1' && flag <= 4)
			flag++;
		chr = ft_strdup(byte);
	}
	else if (ft_strncmp(byte, "10", 2) == 0 && flag != 0 && chr)
	{
		flag--;
		chr = ft_strjoin_mod(chr, byte, 0, 0);
	}
	if (chr && flag == 0)
		chr = handle_special_char(chr);
}

// bit_intake takes in one bit at a time, then creates a binary string
// that once holds 8 bits, gets sent to print_char.
static void	bit_intake(int bit)
{
	static int	bytepos = 0;
	static char	*byte = NULL;

	if (!byte && bytepos == 0)
	{
		byte = malloc(9);
		if (!byte)
		{
			ft_printf("ERROR 0\nMalloc failed to allocate\n");
			exit(0);
		}
		byte[8] = 0;
	}
	byte[bytepos] = bit + '0';
	bytepos++;
	if (bytepos >= 8)
	{
		print_char(byte);
		free(byte);
		byte = NULL;
		bytepos = 0;
	}
}

// signal_handler is ran when a signal is recieved. It can recieve only
// two types of signal, 1 or 0, which is then sent to bit_intake.
static void	signal_handler(int signal)
{
	if (signal == SIGUSR1)
	{
		bit_intake(0);
	}
	else if (signal == SIGUSR2)
	{
		bit_intake(1);
	}
}

// main opens two signal channels that listen for signals,
// then waits indefinitely.
int	main(void)
{
	ft_printf("Server PID: %d\n", getpid());
	signal(SIGUSR1, signal_handler);
	signal(SIGUSR2, signal_handler);
	while (1)
	{
	}
	return (1);
}
