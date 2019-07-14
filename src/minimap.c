/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoyle <adoyle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 15:54:43 by jsteuber          #+#    #+#             */
/*   Updated: 2019/07/14 17:03:06 by adoyle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include <stdlib.h>
#include <stdio.h>

static void	draw_objects(t_core *cr)
{
	int	i;

	i = 0;
	cr->vs->vcolor = 0xffffff;
	while (i < cr->spritesnum)
	{
		draw_rectangle(cr, cr->objarr[i].x *
			cr->vs->gridsize + cr->vs->x_offset - 3,
				cr->objarr[i].y * cr->vs->gridsize + cr->vs->y_offset - 3,
					cr->vs->gridsize / 3, cr->vs->gridsize / 3);
		i++;
	}
}

static void	draw_player(t_core *cr)
{
	int pp;

	pp = cr->vs->gridsize / 3;
	cr->vs->vcolor = 0xffffff;
	cr->vs->x_i = cr->player.x * cr->vs->gridsize + cr->vs->x_offset;
	cr->vs->y_i = cr->player.y * cr->vs->gridsize + cr->vs->y_offset;
	cr->vs->x2_i = cr->player.x * cr->vs->gridsize + cr->dir.x /
		cr->dir.len * 2 * cr->vs->gridsize + cr->vs->x_offset;
	cr->vs->y2_i = cr->player.y * cr->vs->gridsize + cr->dir.y /
		cr->dir.len * 2 * cr->vs->gridsize + cr->vs->y_offset;
	bresenham(cr);
	cr->vs->vcolor = 0x00ff00;
	draw_rectangle(cr, cr->player.x * cr->vs->gridsize +
		cr->vs->x_offset - pp / 2,
		cr->player.y * cr->vs->gridsize + cr->vs->y_offset - pp / 2, pp, pp);
}

static void	draw_walls(t_core *cr)
{
	int		xlen;
	int		ylen;

	ylen = cr->map_h;
	cr->vs->vcolor = 0x909497;
	while (ylen--)
	{
		xlen = cr->map_w;
		while (xlen--)
		{
			if (cr->tiles[ylen][xlen] != 0)
				draw_rectangle(cr, xlen * cr->vs->gridsize, \
				ylen * cr->vs->gridsize, cr->vs->gridsize, cr->vs->gridsize);
		}
	}
}

static void	draw_grid(t_core *cr)
{
	int		map_h;
	int		map_w;
	int		yp;

	yp = cr->vs->gridsize;
	cr->vs->vcolor = 0x909497;
	map_h = cr->map_h;
	map_w = cr->map_w - 1;
	while (map_h--)
	{
		cr->vs->x_i = 0;
		cr->vs->y_i = yp - 1;
		cr->vs->x2_i = cr->vs->gridsize * (cr->map_w - 1) - 1;
		cr->vs->y2_i = yp - 1;
		bresenham(cr);
		// printf("%d %d %d %d %d\n", yp, cr->vs->mmsize, cr->map_w, cr->map_h, cr->vs->gridsize);
		yp += cr->vs->gridsize;
	}
	yp = cr->vs->gridsize;
	while (map_w--)
	{
		cr->vs->x_i = yp - 1;
		cr->vs->y_i = 0;
		cr->vs->x2_i = yp - 1;
		cr->vs->y2_i = cr->vs->gridsize * (cr->map_h - 1) - 1;
		bresenham(cr);
		// printf("%d %d %d %d %d\n", yp, cr->vs->mmsize, cr->map_w, cr->map_h, cr->vs->gridsize);
		yp += cr->vs->gridsize;
	}
}

void		draw_rectangle(t_core *cr, int x, int y, int xlen, int ylen)
{
	while (ylen--)
	{
		cr->vs->x_i = x;
		cr->vs->y_i = y + ylen;
		cr->vs->x2_i = x + xlen;
		cr->vs->y2_i = y + ylen;
		bresenham(cr);
	}
}

void		minimap(t_core *cr)
{
		// draw_rectangle(cr, cr->vs->x_offset, cr->vs->y_offset, cr->vs->gridsize * (cr->map_w - 1), cr->vs->gridsize * (cr->map_h - 1));
		// draw_grid(cr);
		// draw_walls(cr);
		mlx_put_image_to_window(cr->mlx, cr->win, cr->mm_img_ptr, cr->vs->x_offset, cr->vs->y_offset);
		//
		cr->print_func = &pxl_put_wrap;
		cr->img_switcher = cr->mm_image;
		draw_objects(cr);
		draw_player(cr);
}

void		minimap_init(t_core *cr)
{
	cr->vs->mmsize = 300;
	cr->vs->gridsize = cr->map_w > cr->map_h ? (double)cr->vs->mmsize / cr->map_w : (double)cr->vs->mmsize / cr->map_h;
	cr->vs->mmsize_y = cr->vs->gridsize * cr->map_h;
	cr->vs->x_offset = WIN_WIDTH - cr->vs->gridsize * (cr->map_w);
	cr->vs->y_offset = 0;
	cr->vs->vcolor = MM_COLOR;
	// printf("->%d\n", cr->vs->mmsize);
	// fflush(stdout);
	img_minimap_new(cr);
	cr->print_func = &img_pxl;
	cr->img_switcher = cr->mm_image;
	draw_rectangle(cr, 0, 0, cr->vs->gridsize * (cr->map_w) - 1, cr->vs->gridsize * (cr->map_h) - 1);
	draw_grid(cr);
	draw_walls(cr);
}
