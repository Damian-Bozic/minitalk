/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbozic <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:31:16 by dbozic            #+#    #+#             */
/*   Updated: 2024/06/11 16:31:48 by dbozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include "Libft/libft.h"
# include "signal.h"

// Client Helper Functions:
char	*ft_strjoin_mod(char *str1, char *str2, size_t i, size_t j);
char	*ft_binary_itoa(unsigned char nbr);
int		ft_binary_atoi(char *str);
char	*full_binary_pid(int pid);
int		send_binary(char *str, int pid);

// Server Helper Functions:
int		ft_binary_atoi_mod(char *str);
int		pid_binary_atoi(char *str);
void	pid_event_handler(char *code);

#endif
