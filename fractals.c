/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractals.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyildiri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/14 21:26:55 by cyildiri          #+#    #+#             */
/*   Updated: 2017/01/14 21:26:57 by cyildiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	spacefury(float i, float j, t_fractol *fractol)
{
	int		cur_iter;
	float	x;
	float	y;
	float	t;

	x = 4 * (i - fractol->renderer->win_x / 2)
		/ (fractol->zoom * fractol->renderer->win_x) + fractol->pos.x;
	y = 4 * (j - fractol->renderer->win_y / 2)
		/ (fractol->zoom * fractol->renderer->win_y) + fractol->pos.y;
	cur_iter = 0;
	while (x * x + y * y < 4 && cur_iter < fractol->iterations)
	{
		t = x * x - y * y - cos(y) + fractol->julia.re;
		y = 2 * x * y + sin(x) + fractol->julia.im;
		x = t;
		cur_iter++;
	}
	return (cur_iter);
}

int	space_boot(float i, float j, t_fractol *fractol)
{
	int		cur_iter;
	float	x;
	float	y;
	float	t;

	x = 4 * (i - fractol->renderer->win_x / 2)
		/ (fractol->zoom * fractol->renderer->win_x) + fractol->pos.x;
	y = 4 * (j - fractol->renderer->win_y / 2)
		/ (fractol->zoom * fractol->renderer->win_y) + fractol->pos.y;
	cur_iter = 0;
	while (x * x + y * y < 4 && cur_iter < fractol->iterations)
	{
		t = x * x - y * y - cos(y) + 1.57001;
		y = 2 * x * y + sin(x) + -0.804000;
		x = t;
		cur_iter++;
	}
	return (cur_iter);
}

int	julia_set(float i, float j, t_fractol *fractol)
{
	int		cur_iter;
	float	x;
	float	y;
	float	t;

	x = 4 * (i - fractol->renderer->win_x / 2)
		/ (fractol->zoom * fractol->renderer->win_x) + fractol->pos.x;
	y = 4 * (j - fractol->renderer->win_y / 2)
		/ (fractol->zoom * fractol->renderer->win_y) + fractol->pos.y;
	cur_iter = 0;
	while (x * x + y * y < 4 && cur_iter < fractol->iterations)
	{
		t = x * x - y * y + fractol->julia.re;
		y = 2 * x * y + fractol->julia.im;
		x = t;
		cur_iter++;
	}
	return (cur_iter);
}

int	mendelbrot(float x, float y, t_fractol *fractol)
{
	int		cur_iter;
	float	x0;
	float	y0;
	float	xtemp;

	x0 = 4 * (x - fractol->renderer->win_x / 2)
		/ (fractol->zoom * fractol->renderer->win_x) + fractol->pos.x;
	y0 = 4 * (y - fractol->renderer->win_y / 2)
		/ (fractol->zoom * fractol->renderer->win_y) + fractol->pos.y;
	x = 0.0;
	y = 0.0;
	cur_iter = 0;
	while (x * x + y * y < 2 * 2 && cur_iter < fractol->iterations)
	{
		xtemp = x * x - y * y + x0;
		y = 2 * x * y + y0;
		x = xtemp;
		cur_iter++;
	}
	return (cur_iter);
}
