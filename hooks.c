/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyildiri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/14 21:27:34 by cyildiri          #+#    #+#             */
/*   Updated: 2017/01/14 21:27:36 by cyildiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include "stdlib.h"

int		mouse_press_hook(int button, int x, int y, void *param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *)param;
	if (button == 5 || button == 6)
	{
		fractol->zoom *= 1.1;
		fractol->pos.x += (x - fractol->renderer->win_x / 2) * 4.0
				/ (fractol->renderer->win_x * fractol->zoom) * (1.1 - 1);
		fractol->pos.y += (y - fractol->renderer->win_y / 2) * 4.0
				/ (fractol->renderer->win_y * fractol->zoom) * (1.1 - 1);
		fractol->renderer->last_click.x = x;
		fractol->renderer->last_click.y = y;
	}
	else if (button == 4 || button == 7)
	{
		fractol->zoom /= 1.1;
		fractol->pos.x += (x - fractol->renderer->win_x / 2)
			* 4.0 / (fractol->renderer->win_x * fractol->zoom) * (1 / 1.1 - 1);
		fractol->pos.y += (y - fractol->renderer->win_y / 2)
			* 4.0 / (fractol->renderer->win_y * fractol->zoom) * (1 / 1.1 - 1);
		fractol->renderer->last_click.x = x;
		fractol->renderer->last_click.y = y;
	}
	render2d(fractol->renderer, fractol);
	return (0);
}

int		mouse_motion_hook(int x, int y, void *param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *)param;
	x -= 250;
	y -= 250;
	if (!fractol->julia.pause)
	{
		fractol->julia.re -= (x - fractol->renderer->last_click.x)
							/ (fractol->renderer->win_x / 2 * fractol->zoom);
		fractol->julia.im -= (y - fractol->renderer->last_click.y)
							/ (fractol->renderer->win_y / 2 * fractol->zoom);
		render2d(fractol->renderer, fractol);
	}
	fractol->renderer->last_click.x = x;
	fractol->renderer->last_click.y = y;
	return (0);
}

int		key_pressed(int keycode, void *param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *)param;
	if (keycode == NUM_8 && fractol->iterations < 176)
		fractol->iterations += 16;
	else if (keycode == NUM_5 && fractol->iterations > 15)
		fractol->iterations -= 16;
	else if (keycode == R)
	{
		fractol->zoom = 1.0;
		fractol->pos.x = 0;
		fractol->pos.y = 0;
		fractol->iterations = 64;
	}
	else if (keycode == F)
		fractol->julia.pause = (fractol->julia.pause) ? 0 : 1;
	else if (keycode == ESC)
		exit(1);
	render2d(fractol->renderer, fractol);
	return (0);
}
