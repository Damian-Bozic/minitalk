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

static char	*ft_strjoin_mod(char *str1, char *str2, size_t i, size_t j)
{
	char		*rtn;

	if (!str1)
		str1 = ft_strdup("");
	if (!str1)
		return (0);
	rtn = malloc(ft_strlen(str1) + ft_strlen(str2) + 1);
	if (!rtn)
		free (str1);
	if (!rtn)
		return (0);
	while (str1 && str1[i])
	{
		rtn[i] = str1[i];
		i++;
	}
	while (str2 && str2[j])
	{
		rtn[i + j] = str2[j];
		j++;
	}
	rtn[i + j] = '\0';
	free(str1);
	str1 = NULL;
	return (rtn);
}

int	error_exit(int	error)
{
	ft_printf("ERROR %d\n", error);
	if (error == 0)
	    ft_printf("Malloc failed to allocate\n");
	else if (error == 1)
	    ft_printf("PID failed to send\n");
    else if (error == 2)
    {
        ft_printf("Client failed to send entire message\n");
    }
    exit(error);
}
int	ft_binary_atoi_mod(char *str)
{
	int i;
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

int pid_binary_atoi(char *str)
{
    int i;
    int j;
    int power;
    int rtn;

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

void    pid_event_handler(char *code)
{
    static int  client = 0;

    if (client == 0 && ft_strncmp(code, "11111000", 8) == 0)
    {
        client = pid_binary_atoi(code);
        ft_printf("\nSuccessfully connection with %d\n", client);
    }
    else if (client != 0 && ft_strncmp(code, "11111100", 8) == 0)
    {
        ft_printf("\nEnd of %d's message.\n", client);
        if (kill(client, SIGUSR2) == -1)
            error_exit(1);
        client = 0;
    }
}

char *handle_special_char(char *chr)
{
    unsigned char byte_array[4];
    int i;
    int j;

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

void print_char(char *byte)
{
    static char *chr = NULL;
    static int  flag = 0;

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

void bit_intake(int bit)
{
    static int  bytepos = 0;
    static char *byte = NULL;

    if (!byte && bytepos == 0)
    {
        byte = malloc(9);
        if (!byte)
            error_exit(0);
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

void    signal_handler(int signal)
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