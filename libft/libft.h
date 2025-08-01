/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 11:22:10 by anemet            #+#    #+#             */
/*   Updated: 2025/07/17 13:02:27 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>

// from ft_printf.h
# include <stdarg.h>
# include <stdint.h>

// from get_next_line
# include <limits.h>
# define BUFFER_SIZE 2048
# define OPEN_MAX 1024

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

int					min(int a, int b);
int					max(int a, int b);
float				minf(float a, float b);
float				maxf(float a, float b);
int					ft_atoi(const char *nptr);
void				ft_bzero(void *s, size_t n);
void				*ft_calloc(size_t nmemb, size_t size);
int					ft_isalnum(int c);
int					ft_isalpha(int c);
int					ft_isascii(int c);
int					ft_isdigit(int c);
int					ft_isprint(int c);
void				*ft_memchr(const void *s, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memmove(void *dest, const void *src, size_t n);
void				*ft_memset(void *s, int c, size_t n);
char				*ft_strchr(const char *s, int c);
char				*ft_strdup(const char *s);
size_t				ft_strlcat(char *dst, const char *src, size_t size);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
size_t				ft_strlen(const char *s);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strnstr(const char *big, const char *little,
						size_t len);
char				*ft_strrchr(const char *s, int c);
int					ft_tolower(int c);
int					ft_toupper(int c);
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strtrim(char const *s1, char const *set);
char				**ft_split(char const *s, char c);
char				*ft_itoa(int n);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);
t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **lst, t_list *new);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *new);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));

/* ******************* Added from ft_printf.h ****************************** */

typedef struct s_flags
{
	int				minus;
	int				zero;
	int				width;
	int				dot;
	int				precision;
	int				hash;
	int				space;
	int				plus;
}					t_flags;

int					ft_printf(const char *format, ...);

// parsing and dispatching
int					ft_eval_format(va_list args, const char *format, int *i);
t_flags				ft_init_flags(void);
void				ft_parse_flags_width_prec(t_flags *flags,
						const char *format, int *i);
int					ft_dispatch(char type, t_flags flags, va_list args);

// print functions for each conversion specifier
int					ft_print_char(char c, t_flags flags);
int					ft_print_str(char *str, t_flags flags);
int					ft_print_ptr(uintptr_t ptr, t_flags flags);
int					ft_print_nbr(int n, t_flags flags);
int					ft_print_unsigned(unsigned int n, t_flags flags);
int					ft_print_hex(unsigned int n, t_flags flags, char format);

// utility functions
int					ft_print_padding(char c, int len);
char				*ft_utoa(unsigned int n);
void				ft_strrev(char *s, int len);
int					ft_ptr_len(uintptr_t num);
void				ft_put_ptr(uintptr_t num);

/* ******************* Added from get_next_line ************************* */

char				*get_next_line(int fd);
size_t				gnl_strlen(char *s);
char				*gnl_strchr(char *s, int c);
char				*gnl_strjoin(char *s1, char *s2);
char				*gnl_substr(char *s, unsigned int start, size_t len);

#endif

// grep -E '^[a-zA-Z_][a-zA-Z0-9_ \t\*]*.*ft_' *.c
