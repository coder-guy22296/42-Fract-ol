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
#include <stdio.h> //REMOVE

typedef struct	s_fractol
{
	t_renderer	*renderer;
	t_vec2fc	pos;
	float		zoom;
}				t_fractol;


int get_iterations(float x, float y, int max_iterations, t_fractol *fractol)
{
	int cur_iter;
	float x0;
	float y0;
	float xtemp;



	x0 = 1.5 * (x - 1000 / 2) / (0.5 * fractol->zoom * 1000) + fractol->pos.x;
	y0 = (y - 1000 / 2) / (0.5 * fractol->zoom * 1000) + fractol->pos.y;
	x = 0.0;
	y = 0.0;
	cur_iter = 0;
	while (x*x + y*y < 2*2  &&  cur_iter < max_iterations - 1)
	{
		xtemp = x*x - y*y + x0;
		y = 2*x*y + y0;
		x = xtemp;
		cur_iter++;
	}
	return (cur_iter);
}

void render2d(t_renderer *renderer, t_fractol *fractol)
{
	int x;
	int y;


	renderer->scene->cur_frame.id = mlx_new_image(renderer->mlx,
												  renderer->win_x,
												  renderer->win_y);
	if (!renderer->scene->cur_frame.color_depth)
		renderer->scene->cur_frame.image = mlx_get_data_addr(
				renderer->scene->cur_frame.id,
				&(renderer->scene->cur_frame.color_depth),
				&(renderer->scene->cur_frame.line_size),
				&(renderer->scene->cur_frame.endien));
	// construct frame
	y = 0;
	while (y < renderer->win_y)
	{
		x = 0;
		while (x < renderer->win_x)
		{
			float iter = (float)get_iterations(x, y, 256, fractol);

			//printf("iter: %f\n", iter);//REMOVE
			//float percent = iter / 256;
			float percent;
			int color;
			percent = iter / 256;
			//percent = (float)x / 1000.0 + iter*0;
			//percent = (float)(x % 333) / 333.0 + iter*0;
			/*if (x < 333 && x >= 0)
				color = blend(0x00FF0000, 0x000000FF, percent);
			else if (x < 666 && x >= 333)
				color = blend(0x000000FF, 0x0000FF00, percent);
			else*/
				color = blend(0x00FF5000, 0x00005FFF, percent);

			//printf("color scale: %f\n", percent);//REMOVE
			frame_pixel_put(renderer->scene, vec2fc(x, y, color));
			x++;
		}
		y++;
	}
	//
	mlx_put_image_to_window(renderer->mlx, renderer->window,
							renderer->scene->cur_frame.id, 0, 0);
	mlx_destroy_image(renderer->mlx, renderer->scene->cur_frame.id);
}

int main()
{
	t_fractol fractol;
	t_scene		*scene1;

	fractol.renderer = new_renderer(render_scene);
	fractol.pos.x = -0.5;
	fractol.pos.y = 0;
	fractol.zoom = 4.0;
	add_window(fractol.renderer, 1000, 1000, "cyildiri's fract'ol");
	scene1 = new_scene(perspective_projection, 1000, 1000);
	fractol.renderer->scene = scene1;
	render2d(fractol.renderer, &fractol);
	mlx_loop(fractol.renderer->mlx);
	return (0);
}