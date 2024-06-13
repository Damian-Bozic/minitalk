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

int	error_exit(int	error)
{
	ft_printf("ERROR %d\n", error);
	if (error == 0)
		ft_printf("malloc failed to allocate\n");
	else if (error == 1)
		ft_printf("PID failed to send\n");
	else if (error == 2)
		ft_printf("\n");
	exit(error);
}

void    print_char(char *data, int size)
{
    //if size is larger than 1 byte check if it isnt corrupted
    //then print
}

int printer(int bit)
{
    static int  bytepos = 0;
    static char *byte = NULL;

    // find how many bytes there will be from the first byte
    if (!byte && bit == 0 && bytepos == 0)
    {
        byte = malloc(41);
        if (!byte)
            return(0);
        ft_bzero(byte, 41);
    }
    byte[bytepos] = bit + '0';
    if (bytepos >= 7)
    {
        if (utf8_char(byte) = 1)
            // HUGE WIP
        else
        {
            print_char(byte, 1);
            bytepos = 0;
        }
    }
    bytepos++;
    return(1);
}

void    signal_handler(int signal)
{
    static long  flag = 0;
    static char *client = NULL;

    if (!client)
    {
        client = malloc(41);
        ft_bzero(client, 41);
    }
    if (signal == SIGUSR1 && flag >= 40)
        printer(0);
    else if (signal == SIGUSR2 && flag >= 40)
        printer(1);
    if (flag < 40 && signal == SIGUSR1)
        client[flag] = '0';
    else if (flag < 40 && signal == SIGUSR2)
        client[flag] = '1';
    if (flag == 39)
        ft_printf("%s\n", client);
    flag++;
}

int	main(void)
{
	ft_printf("Server PID: %d\n", getpid());
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);
    while (1)
        pause();
	return (1);
}

/* Certainly! To send a signal from a server to a client in C, 
you can use the kill() function. Here’s a basic example:

Server Side:
First, obtain the client’s process ID (PID).
Then, use kill() to send a signal to the client process.
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    pid_t clientPID; // Get the client's PID (e.g., through command line arguments)
    // ...
    // Some server logic
    // ...

    // Send a signal to the client
    kill(clientPID, SIGUSR1); // You can use any signal you want (e.g., SIGUSR1)
    return 0;
}

Client Side:
Handle the signal in the client process using a signal handler.
For example, you can print a message when the signal is received.
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handleSignal(int signum) {
    if (signum == SIGUSR1) {
        printf("Received signal from server!\n");
        // Handle the signal as needed
    }
}

int main() {
    // Set up the signal handler
    signal(SIGUSR1, handleSignal);

    // ...
    // Some client logic
    // ...

    // Keep the client process running
    while (1) {
        // ...
        // Your client code here
        // ...
    }
    return 0;
}

Remember to replace the placeholders with your actual server and client logic.
 Additionally, choose an appropriate signal (e.g., SIGUSR1) based on your
  requirements1. Happy coding! 🚀 */