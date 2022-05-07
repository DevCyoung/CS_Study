 #include <mlx.h> 
 #include <stdio.h>
  //RENDER
   #define PLAYER 0x00000001 
   #define TILE   0x00000002 
   #define WALL   0x00000004 
   #define EXIT   0x00000008

 #define TITLE "SO_LONG"
 #define MOVE_SPEED 4

 #define IMAGE_MAX_SIZE 30

 # define X_EVENT_KEY_PRESS          2
 # define X_EVENT_KEY_RELEASE        3

 # define KEY_ESC        53

 # define KEY_W          13
 # define KEY_A          0
 # define KEY_S          1
 # define KEY_D          2


 char *str = "1111111111\n1000000001\n1000000001\n1111111111";

 //right down left up
 //
 int dir[4] = {0,};

 typedef struct s_graphic
 {
     int layer;
     int x;
     int y;
     int width;
     int height;
     void *image;
 } t_graphic;

 typedef struct s_animation
 {
     int idx;
     int max_size;
     void *images[IMAGE_MAX_SIZE];
 } t_animation;


 void add_anim(t_animation *anim, void *image)
 {
     if (anim == 0)
         return;
     if (anim -> idx >= IMAGE_MAX_SIZE)
         return;
     (anim -> images)[anim -> idx] = image;
     ++(anim -> idx);
 };

 typedef struct s_object
 {
     int class_id;
     int instance_id;
     void *mlx;
     void *win;
     int pos_x;
     int pos_y;
     t_animation anim;
     t_graphic graph;
 } t_object;

 t_object maps[10000];

 int is_collide(t_object *a, t_object *b)
 {
	 if(a->pos_x < b->pos_x + b->graph.width &&
        a->pos_x + a->graph.width > b->pos_x &&
        a->pos_y < b->pos_y + b->graph.height &&
        a->pos_y + a->graph.height > b->pos_y)
		 return 1;
     return 0;
 }
 void object_render(t_object *object, void *mlx, void *win);
 int map_render(char *map, void *mlx, void *win);

 int deltatime()
 {
     static int time = 0;
     ++time;
     return time;
 }


 int key_press(int keycode, t_object *object)
 {
     if (keycode == KEY_D)
         dir[0] = 1;
     if (keycode == KEY_S)
         dir[1] = 1;
     if (keycode == KEY_A)
         dir[2] = 1;
     if (keycode == KEY_W)
         dir[3] = 1;
     return (0);
 }

 int key_release(int keycode, t_object *object)
 {

     if (keycode == KEY_D)
         dir[0] = 0;
     if (keycode == KEY_S)
         dir[1] = 0;
     if (keycode == KEY_A)
         dir[2]  =0;
     if (keycode == KEY_W)
         dir[3] = 0;
     return 1;
 }

 int map_render(char *map, void *mlx, void *win)
 {
     int width = 0;
     int height = 0;
     int x = 0;
     int y = 0;
	 int idx = 0;
     int a;
     int b;
     void *img = mlx_xpm_file_to_image(mlx, "./wall.xpm", &width, &height);
     void *img2 = mlx_xpm_file_to_image(mlx, "./grass.xpm", &a, &b);
     while (*map)
     {
         mlx_put_image_to_window(mlx, win, img2, x, y);
         if (*map == '1')
		 {
             mlx_put_image_to_window(mlx, win, img, x, y);
			 //maps[idx].
		 }
		 
         if (*map == '\n')
         {
             y += height;
             x = 0;
         }
         else
             x += width;
         ++map;
     }
     y += height;
     printf("SCREEM_____SIZE____ x : %d y : %d\n",x, y) ;
     return 1;
 
}

 void screen_init(char *map, void **mlx, void **win)
 {
     int img_width;
     int img_height;
     int screen_sizex = 0;
     int screen_sizey = 0;
     void *wall = mlx_xpm_file_to_image(mlx, "./wall.xpm", &img_width, &img_height);
     if (wall == 0)
     {
         printf("ERROR \n");
     }
     while (*map != '\n')
     {
         ++map;
         screen_sizex += img_width;
     }
     while (*map)
     {
         if (*map == '\n')
             screen_sizey += img_height;
         ++map;
     }
     screen_sizey += img_height;


     *mlx = mlx_init();
     *win = mlx_new_window(*mlx, screen_sizex, screen_sizey, TITLE);
 }

 void object_render(t_object *object, void *mlx, void *win)
 {
     mlx_put_image_to_window(mlx, win, (object->graph).image, object->pos_x, object->pos_y);
 }

 void animation_render()
 {

 }


 int main_loop(t_object *player)
 {
	 int pos_x = player -> pos_x;
	 int pos_y = player -> pos_y;

     if (dir[0])
        pos_x += MOVE_SPEED;
     if (dir[1])
         pos_y += MOVE_SPEED;
     if (dir[2])
         pos_x -= MOVE_SPEED;
     if (dir[3])
         pos_y -= MOVE_SPEED;

	 int i = 0;
	 //for (int i = 0; i < idx; i++)
	 //{
	//	 if ( is_collide( &maps[i], player))
	//	 {
	//		 pos_x = 0;
	//		 pos_y = 0;
	//	 }
	//	 break;
	 //}
	 player -> pos_x = pos_x;
	 player -> pos_y = pos_y;
     map_render(str, player->mlx, player->win);
     object_render(player, player->mlx, player->win);
     return 1;
 }

 int main(void)
 {

     void *mlx;
     void *win;
     int img_width = 0;
     int img_height = 0;
     screen_init(str, &mlx, &win);

     //player init.
     t_object player;
     player.mlx = mlx;
     player.win = win;
     player.graph.image = mlx_xpm_file_to_image(mlx, "./player.xpm", &img_width, &img_height);
     player.pos_x = 0;
     player.pos_y = 0;

     map_render(str, mlx, win);
     object_render(&player, mlx, win);
     //mlx_hook(win, X_EVENT_KEY_RELEASE, 0, &key_press, &player);
     //X_EVENT_KEY_PRESS
     mlx_hook(win, X_EVENT_KEY_PRESS,   0, &key_press, &player);
     mlx_hook(win, X_EVENT_KEY_RELEASE, 0, &key_release, &player);
     mlx_loop_hook(mlx, main_loop, &player);

     mlx_loop(mlx);
     printf("mlx loop");
   return (0);
 }
