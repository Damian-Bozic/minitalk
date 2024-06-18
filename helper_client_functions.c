/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbozic <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 15:57:21 by dbozic            #+#    #+#             */
/*   Updated: 2024/06/18 15:57:40 by dbozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

// ft_strjoin_mod joins two strings, and returns an allocated string.
// str1 must be allocated or NULL, and is freed and set to NULL.
// str2 can be any string or NULL.
// due to size restrictions variables i and j are initialized as so.
char	*ft_strjoin_mod(char *str1, char *str2, size_t i, size_t j)
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

// ft_binary_itoa takes in a char, then returns a binary string.
// a -> 97 -> 01100001
char	*ft_binary_itoa(unsigned char nbr)
{
	char	*rtn;
	int		i;

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

// ft_binary_atoi takes in a binary string, then returns its base 10 value.
// 01100001 -> 97 -> 'a'
int	ft_binary_atoi(char *str)
{
	int	i;
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

// full_binary_pid takes in a process id then returns it as a binary string
// encoded as follows: 11111000 10XXXXXX 10XXXXXX 10XXXXXX 10XXXXXX
// the first byte states that this is a PID and that it will be followed
// by 4 more bytes of data that hold the PID
char	*full_binary_pid(int pid)
{
	char	*rtn;
	int		i;
	int		j;

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

// send_binary takes a binary string and sends each bit one by one to the pid
// provided using UNIX signals
int	send_binary(char *str, int pid)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
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
