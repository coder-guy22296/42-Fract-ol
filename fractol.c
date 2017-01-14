/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyildiri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/11 01:22:18 by cyildiri          #+#    #+#             */
/*   Updated: 2017/01/11 01:22:20 by cyildiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgraphics.h"
#include "stdlib.h"
#include <stdio.h> //REMOVE

typedef struct s_julia
{
	float re;
	float im;
	int pause;
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

int julia_set(float i, float j, t_fractol *fractol)
{
	int cur_iter;
	float x;
	float y;
	float t;

	x = 4 * (i - fractol->renderer->win_x / 2)
		/ (fractol->zoom * fractol->renderer->win_x) + fractol->pos.x;
	y = 4 * (j - fractol->renderer->win_y / 2)
		/ (fractol->zoom * fractol->renderer->win_y) + fractol->pos.y;

	cur_iter = 0;
	while (x*x + y*y < 2*2 && cur_iter < fractol->iterations)
	{
		t = x*x - y*y + fractol->julia.re;
		y = 2*x*y + fractol->julia.im;
		x = t;
		cur_iter++;
	}
	return (cur_iter);
}

int mendelbrot(float x, float y, t_fractol *fractol)
{
	int cur_iter;
	float x0;
	float y0;
	float xtemp;

	x0 = 4 * (x - fractol->renderer->win_x / 2)
		 / (fractol->zoom * fractol->renderer->win_x) + fractol->pos.x;
	y0 = 4 * (y - fractol->renderer->win_y / 2)
		 / (fractol->zoom * fractol->renderer->win_y) + fractol->pos.y;
	x = 0.0;
	y = 0.0;
	cur_iter = 0;
	while (x*x + y*y < 2*2 &&  cur_iter < fractol->iterations)
	{
		xtemp = x*x - y*y + x0;
		y = 2*x*y + y0;
		//y = 5*x*y + y0;
		x = xtemp;
		cur_iter++;
	}
	return (cur_iter);
}

int blend3(int a, int b, int c, float percent)
{
	int color;
//printf("color scale: %f\n", percent);//REMOVE
	if (percent >= 0 && percent < 0.33)
		color = blend(a, b, percent * 3);
	else if (percent >= 0.33 && percent < 0.66)
		color = blend(b, c, (percent - 0.33) * 3);
	else
		color = blend(c, a, (percent - 0.67) * 3);
	return (color);
}



void render_fractal(t_fractol *fractol, t_renderer *renderer)
{
	int x;
	int y;
	float iter;
	int color;

	y = 0;
	while (y < renderer->win_y)
	{
		x = 0;
		while (x < renderer->win_x)
		{
			iter = (float)fractol->fractal(x, y, fractol);
			color = blend3(0x000000FF, 0x00FFFFFF, 0x00000000, (iter / fractol->iterations));
//			color = blend3(0x00FF7000, 0x00FF00DD, 0x0000FFFF,
//												(iter / fractol->iterations));
			frame_pixel_put(renderer->scene, vec2fc(x, y, color));
			x++;
		}
		y++;
	}
}


void render2d(t_renderer *renderer, t_fractol *fractol)
{
	mlx_clear_window(renderer->mlx,renderer->window);
	renderer->scene->cur_frame.id = mlx_new_image(renderer->mlx,
												  renderer->win_x,
												  renderer->win_y);
	if (!renderer->scene->cur_frame.color_depth)
		renderer->scene->cur_frame.image = mlx_get_data_addr(
				renderer->scene->cur_frame.id,
				&(renderer->scene->cur_frame.color_depth),
				&(renderer->scene->cur_frame.line_size),
				&(renderer->scene->cur_frame.endien));
	render_fractal(fractol, renderer);

	mlx_put_image_to_window(renderer->mlx, renderer->window,
							renderer->scene->cur_frame.id, 0, 0);
	mlx_destroy_image(renderer->mlx, renderer->scene->cur_frame.id);
}

int		mouse_press_hook(int button, int x, int y, void *param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *)param;
	if (button && x && y)
		ft_putchar('\0');
	if (button == 5 || button == 6)
	{
		fractol->zoom *= 1.1;
		fractol->pos.x += (x - fractol->renderer->win_x / 2) * 4.0 / (fractol->renderer->win_x * fractol->zoom) * (1.1 - 1);
		fractol->pos.y += (y - fractol->renderer->win_y / 2) * 4.0 / (fractol->renderer->win_y * fractol->zoom) * (1.1 - 1);
	}
	else if (button == 4 || button == 7)
	{
		fractol->zoom /= 1.1;
		fractol->pos.x += (x - fractol->renderer->win_x / 2) * 4.0 / (fractol->renderer->win_x * fractol->zoom) * (1 / 1.1 - 1);
		fractol->pos.y += (y - fractol->renderer->win_y / 2) * 4.0 / (fractol->renderer->win_y * fractol->zoom) * (1 / 1.1 - 1);
	}
	fractol->renderer->last_click.x = x;
	fractol->renderer->last_click.y = y;
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
		fractol->julia.re -= (x - fractol->renderer->last_click.x) / (fractol->renderer->win_x / 2 * fractol->zoom);
		fractol->julia.im -= (y - fractol->renderer->last_click.y) / (fractol->renderer->win_y / 2 * fractol->zoom);
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

void select_fractal(int argc, char **argv, t_fractol *fractol)
{
	fractol->pos.x = 0;
	fractol->pos.y = 0;
	fractol->zoom = 1.0;
	fractol->iterations = 64;

	if (argc != 2)
	{
		ft_putstr("usage: ./fractol <fractal number>\n");
		ft_putstr("***********\nFractals: *\n***********\n");
		ft_putstr("1) Mendelbrot\n2) Julia Set\n3) Surprise\n");
		exit(1);
	}
	if (ft_strcmp(argv[1], "1\0") == 0)
		fractol->fractal = mendelbrot;
	else if (ft_strcmp(argv[1], "2\0") == 0)
	{
		fractol->fractal = julia_set;
		fractol->julia.re = -0.75f;
		fractol->julia.im = 0.0;
	}
	else if (ft_strcmp(argv[1], "3\0") == 0)
		fractol->fractal = julia_set;
	else
		exit(1);
}

int main(int argc, char **argv)
{
	t_fractol fractol;
	t_scene		*scene1;
	int winx;
	int winy;

	winx = 500;
	winy = 500;
	fractol.julia.pause = 0;
	select_fractal(argc, argv, &fractol);
	fractol.renderer = new_renderer(render_scene);
	add_window(fractol.renderer, winx, winy, "cyildiri's fract'ol");
	scene1 = new_scene(perspective_projection, winx, winy);
	fractol.renderer->scene = scene1;
	render2d(fractol.renderer, &fractol);
	mlx_hook(fractol.renderer->window, 2, 0, key_pressed, &fractol);
	mlx_hook(fractol.renderer->window, 4, 0, mouse_press_hook, &fractol);
	mlx_hook(fractol.renderer->window, 6, 0, mouse_motion_hook, &fractol);
	mlx_loop(fractol.renderer->mlx);
	return (0);
}