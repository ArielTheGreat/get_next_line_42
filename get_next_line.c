#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "get_next_line.h"
#define BUFFER_SIZE 6

char *_set_line(char *line_buffer)
{
    int counter;
    char *str;
    
    counter = 0;
    while(line_buffer[counter] != '\0' && line_buffer[counter] != '\n')
        counter++;
    if (line_buffer[counter] == '\n')
    {
        line_buffer[counter] = '\0';
        str = ft_strdup(line_buffer + counter + 1);
    }
    else
        str = NULL;
    return (str);
}

char *_fill_line_buffer(int fd, char **left_c, char *buffer)
{
    ssize_t bytes_read;
    char *str;
    char *str2;

    str2 = NULL;
    if (*left_c != NULL)
    {
        str = ft_strdup(*left_c);
        free(*left_c);
        *left_c = NULL;
    }
    else
        str = ft_strdup("");
    while(ft_strchr(str,'\n') == NULL)
    {
        bytes_read = read(fd, (void *)buffer, BUFFER_SIZE);
        if (bytes_read <= 0)
            break; 
        buffer[bytes_read] = '\0';
        if (!str)
            str = ft_strdup((char *)buffer);
        else
        {
            str2 = ft_strdup(str);
            free(str);
            str = ft_strjoin(str2,(char *)buffer);
            free(str2);
        }
    }
    *left_c = _set_line(str);
    return (str);
}

char *get_next_line(int fd)
{
    char buffer[BUFFER_SIZE + 1];
    static char *left_c;
    char *str;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);

    str = _fill_line_buffer(fd, &left_c, buffer);

    if (str == NULL || str[0] == '\0')
    {
        free(str);
        return NULL;
    }

    return (str);
}

int main()
{
    int fd = open("test.txt", O_RDONLY);
    char *str;


    while ((str = get_next_line(fd)) != NULL)
    {
        printf("%s\n", str);
        free(str);
    }

    close(fd);
    return 0;
}