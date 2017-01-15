/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyildiri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/14 21:29:41 by cyildiri          #+#    #+#             */
/*   Updated: 2017/01/14 21:29:42 by cyildiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "libgraphics.h"

typedef struct	s_julia
{
	float		re;
	float		im;
	int			pause;
}				t_julia;

typedef struct	s_fractol
{
	t_renderer	*renderer;
	t_vec2fc	pos;
	float		zoom;
	int			iterations;
	t_julia		julia;
	int			(*fractal)(float x, float y, struct s_fractol *fractol);
}				t_fractol;

int				spacefury(float i, float j, t_fractol *fractol);
int				space_boot(float i, float j, t_fractol *fractol);
int				julia_set(float i, float j, t_fractol *fractol);
int				mendelbrot(float x, float y, t_fractol *fractol);
int				blend3(int a, int b, int c, float percent);
void			render_fractal(t_fractol *fractol, t_renderer *renderer);
void			render2d(t_renderer *renderer, t_fractol *fractol);
int				mouse_press_hook(int button, int x, int y, void *param);
int				mouse_motion_hook(int x, int y, void *param);
int				key_pressed(int keycode, void *param);
void			select_fractal(int argc, char **argv, t_fractol *fractol);

#endif
