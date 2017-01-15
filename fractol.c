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

#include "fractol.h"
#include "stdlib.h"

int		blend3(int a, int b, int c, float percent)
{
	int	color;

	if (percent >= 0 && percent < 0.33)
		color = blend(a, b, percent * 3);
	else if (percent >= 0.33 && percent < 0.66)
		color = blend(b, c, (percent - 0.33) * 3);
	else
		color = blend(c, a, (percent - 0.67) * 3);
	return (color);
}

void	render_fractal(t_fractol *fractol, t_renderer *renderer)
{
	int		x;
	int		y;
	float	iter;
	int		color;

	y = 0;
	while (y < renderer->win_y)
	{
		x = 0;
		while (x < renderer->win_x)
		{
			iter = (float)fractol->fractal(x, y, fractol);
			color = blend3(0x000000FF, 0x00FFFFFF, 0x00000000,
							(iter / fractol->iterations));
			if (fractol->fractal == space_boot || fractol->fractal == spacefury)
				color = blend3(0x00000000, 0x00FF0000, 0x009900FF,
								(iter / fractol->iterations));
			else if (fractol->fractal == julia_set)
				color = blend3(0x00000000, 0x00FF5000, 0x007000FF,
								(iter / fractol->iterations));
			frame_pixel_put(renderer->scene, vec2fc(x, y, color));
			x++;
		}
		y++;
	}
}

void	render2d(t_renderer *renderer, t_fractol *fractol)
{
	mlx_clear_window(renderer->mlx, renderer->window);
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

void	select_fractal(int argc, char **argv, t_fractol *fractol)
{
	fractol->pos.x = 0;
	fractol->pos.y = 0;
	fractol->zoom = 1.0;
	fractol->iterations = 64;
	fractol->julia.re = -0.75f;
	fractol->julia.im = 0.0;
	if (argc != 2 || (argv[1][0] < '1' || argv[1][0] > '4'))
	{
		ft_putstr("usage: ./fractol <fractal number>\n");
		ft_putstr("***********\nFractals: *\n***********\n");
		ft_putstr("1) Mendelbrot\n2) Julia Set\n3) SpaceBoot\n");
		ft_putstr("4) Spacefury Set\n");
		exit(1);
	}
	if (ft_strcmp(argv[1], "1\0") == 0)
		fractol->fractal = mendelbrot;
	else if (ft_strcmp(argv[1], "2\0") == 0)
		fractol->fractal = julia_set;
	else if (ft_strcmp(argv[1], "3\0") == 0)
		fractol->fractal = space_boot;
	else if (ft_strcmp(argv[1], "4\0") == 0)
		fractol->fractal = spacefury;
	else
		exit(1);
}

int		main(int argc, char **argv)
{
	t_fractol	fractol;
	t_scene		*scene1;
	int			winx;
	int			winy;

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
