#include "map.h"

//utils
int32_t SLUGmaker_CountOnes(int8_t *tab, int32_t tab_size)
{
    if(tab == NULL || tab_size <= 0)
        return -1;
    int32_t count = 0;
    for(int32_t i = 0; i < tab_size; ++i)
    {
        if(tab[i] != 0)
            count += 1;
    }
    return count;
}

int8_t SLUGmaker_AllZeros(int8_t *tab, int32_t tab_size)
{
    if(tab == NULL || tab_size <= 0)
        return -1;
    for(int32_t i = 0; i < tab_size; ++i)
    {
        if(tab[i] != 0)
            return false;
    }
    return true;
}

int32_t SLUGmaker_GetIndexForPosition(int8_t *tab, int32_t tab_size, int32_t position)
{
    if(tab == NULL || tab_size <= 0)
        return -1;
    int32_t count;
    for(int32_t i = 0; i < tab_size; ++i)
    {
        if(tab[i] != 0)
        {
            count += 1;
            if(count >= position)
                return i;        
        }
    }
    return -1;
}

int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int8_t rv = remove(fpath);

    if (rv)
        perror(fpath);

    return rv;
}

int8_t SLUGmaker_RemoveDirRecursive(const char *dir)//TODO:windows
{
    if(dir == NULL)
        return -1;

    return nftw(dir, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}

//map load/unload
SLUGmaker_map* SLUGmaker_NewMap(uint32_t width, uint32_t height)
{

    SLUGmaker_map *map = (SLUGmaker_map *) malloc(sizeof(SLUGmaker_map));
    if(map == NULL)
    {
        printf("Malloc Error in map allocation.\n");
        return NULL;
    }
    
    //map name
    strcpy(map->map_name,"Untitled");
    strcpy(map->map_path,"maps/");
    
    //map size
    
    if(width < 1680)
	{
		printf("Map width too small. Extending to 1680 pixels.\n");
		map->zone.width = 1680.0f;
	}
    else
    	map->zone.width = (float) width;
    if(height < 1050)
	{
		printf("Map height too small. Extending to 1050 pixels.\n");
		map->zone.height = 1050.0f;
	}
    else
    	map->zone.height = (float) height;
    map->zone.x = 0;
    map->zone.y = 0;
    
    //map sprite init
    for(int16_t i = 0; i < MAX_SPRITES; ++i)
    {
    	map->loaded_sprites[i].id = 0;
    	map->loaded_sprites_names[i][0] = '\0';
    }
    for(int16_t i = 0; i < MAX_PLACED_SPRITES; ++i)
    {
    	map->map_sprites[i].sprite_index = -1;
    	map->map_sprites[i].zone = (Rectangle) {.x = 0, .y = 0, .width = 0, .height = 0};
    }
    map->sprite_nb = 0;
    map->selected_sprite = -1;
    
    //wall init
    for(int16_t i = 0; i < MAX_WALLS_NB; ++ i)
        map->walls[i] = (SLUGmaker_SegmentExtended) {
            .A = (Vector2) {.x = 0, .y = 0},
            .B = (Vector2) {.x = 0, .y = 0},
            .normal = (Vector2) {.x = 0, .y = 1},
            .middlepoint = (Vector2) {.x = 0, .y = 0},
            .next = NULL,
            .exists = false
        };

    map->current_wall_index = 0;
    map->wall_nb = 0;
    map->wall_line_mode = false;
    map->wall_line_origin_index = -1;
    map->wall_move_mode = -1;
    
    //player spawn point init
    map->player_spawn_point = (Vector2) {.x = map->zone.width/2, .y = map->zone.height/2};

    return map;
}

//loadMap is expected to be like : "/home/pepsiduck/Desktop/SLUG_maker/maps/map"
SLUGmaker_map* SLUGmaker_LoadMap(const char *map_dir)
{
    if(map_dir == NULL)
        return NULL;

    char loadMap[strlen(map_dir) + 1];
    if(map_dir[strlen(map_dir) - 1] != '\n')
        sprintf(loadMap,"%s/",map_dir);
    else
        sprintf(loadMap,"%s",map_dir);

    uint32_t len = strlen(loadMap);
    if(len == 0)
        return NULL;
        
    //read the map file
    char mapslug[len + 9];
    sprintf(mapslug,"%smap.slug",loadMap);
    FILE *f = fopen(mapslug, "r");
    if(f == NULL)
    {
        printf("Error while loading file.\n");
        return NULL;
    }
    
    //signature test
    unsigned char signature[7];
    if(fread((void *) signature, sizeof(unsigned char), 7, f) != 7)
    {
        printf("File incomplete or error.\n");
        return NULL;
    }
    unsigned char test[7] = {0x53, 0x4C, 0x55, 0x47, 0x4D, 0x41, 0x50};
    if(memcmp(signature, test, 7) != 0)
    {
        printf("File signature is wrong.\n");
        return NULL;
    }
    
    //reading the width & height
    uint32_t width, height;
    if(fread((void *) &width, sizeof(int32_t), 1, f) != 1)
    {
        printf("File incomplete or error.\n");
        return NULL;
    }
    if(fread((void *) &height, sizeof(int32_t), 1, f) != 1)
    {
        printf("File incomplete or error.\n");
        return NULL;
    }

    //map init
    SLUGmaker_map *map = SLUGmaker_NewMap(width,height);
    if(map == NULL)
    {
        printf("Map malloc Error\n");
        return NULL;
    }
    
    //map name
    
    //separate the path from the name;
    uint32_t index;
    for(index = strlen(loadMap) - 1; index >= 0; index--)
    {
    	if(loadMap[index] == '/')
    		break;
    }   
      
    strncpy(map->map_path, loadMap, index + 1); //"/home/pepsiduck/Desktop/SLUG_maker/maps/"
    strncpy(map->map_name, loadMap + index + 1, strlen(loadMap) - index - 2); //"map"

    //loaded sprites init
    char sprite_file_name[len + 32];
    sprintf(sprite_file_name,"%sassets/sprites/sprite_names.txt",loadMap);
    FILE *sprite_file = fopen(sprite_file_name,"r");
    if(sprite_file == NULL)
    {
        printf("No sprite name file.\n");
        SLUGmaker_UnloadMap(map);
        fclose(f);
        return NULL;
    }
    char line[256];
    char sprite_name[271 + len];
    uint16_t counter = 0;
    while(counter < MAX_SPRITES && fgets(line, sizeof(line), sprite_file))
    {   
        if(line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

        if(strlen(line) > 0)
        {
            sprintf(sprite_name,"%sassets/sprites/%s",loadMap,line);
            map->loaded_sprites[counter] = LoadTexture(sprite_name);
            if(map->loaded_sprites[counter].id > 0)
                strncpy(map->loaded_sprites_names[counter],line,256);
            else
            {
                printf("Warning : incorrect sprite file name.\n");
                map->loaded_sprites[counter] = LoadTexture("assets/sprites/missing.png");
            }
        }
        
        counter++;
    }
    
    printf("\n\n");

    fclose(sprite_file);
    
    //map sprites init
    if(fread((void *) &(map->sprite_nb), sizeof(int16_t), 1, f) != 1)
    {
        printf("File incomplete or error.\n");
        SLUGmaker_UnloadMap(map);
        fclose(f);
        return NULL;
    }
    
    if(map->sprite_nb < 0)
    {
        printf("File incomplete or error.\n");
        SLUGmaker_UnloadMap(map);
        fclose(f);
        return NULL;
    }
    
    if(map->sprite_nb > 0)
    {
    	if(fread((void *) map->map_sprites, sizeof(SLUGmaker_PlacableSprite), map->sprite_nb, f) != map->sprite_nb)
		{
		    printf("File incomplete or error.\n");
		    SLUGmaker_UnloadMap(map);
            fclose(f);
		    return NULL;
		}
    }

    //wall init
    size_t bsptree;
    if(fread((void *) &bsptree, sizeof(size_t), 1, f) != 1)
    {
        printf("File incomplete or error.\n");
        SLUGmaker_UnloadMap(map);
        fclose(f);
        return NULL;
    }

    map->wall_nb = 0;
    map->current_wall_index = 0; 
    if(bsptree != 0)
    {
        int32_t wall_nb;
        if(fread((void *) &wall_nb, sizeof(int32_t), 1, f) != 1)
        {
            printf("File incomplete or error.\n");
            SLUGmaker_UnloadMap(map);
            fclose(f);
            return NULL;
        }

        if(wall_nb <= 0)
        {
            printf("Wtf there are no walls ???\n");
            SLUGmaker_UnloadMap(map);
            fclose(f);
            return NULL;
        }

        if(wall_nb > MAX_WALLS_NB)
        {
            printf("Too many walls\n");
            SLUGmaker_UnloadMap(map);
            fclose(f);
            return NULL;
        }

        SLUG_SegmentExtended walls[wall_nb];
        if(fread((void *) walls, sizeof(SLUG_SegmentExtended), wall_nb, f) != wall_nb)
        {
            printf("File incomplete or error.\n");
            SLUGmaker_UnloadMap(map);
            fclose(f);
            return NULL;
        }

        int32_t links[wall_nb];
        if(fread((void *) links, sizeof(int32_t), wall_nb, f) != wall_nb)
        {
            printf("File incomplete or error.\n");
            SLUGmaker_UnloadMap(map);
            fclose(f);
            return NULL;
        }

        for(int32_t i = 0; i < wall_nb; ++i)
        {
            map->walls[i].A = walls[i].A;
            map->walls[i].B = walls[i].B;
            map->walls[i].middlepoint = Vector2Scale(Vector2Add(walls[i].A,walls[i].B), 0.50);
            map->walls[i].normal = walls[i].normal;
            map->walls[i].next = &(map->walls[links[i]]);
            map->walls[i].exists = true;
        }
        for(int32_t i = wall_nb; i < MAX_WALLS_NB; ++i)
        {
            map->walls[i].A = (Vector2) {.x = 0, .y = 0};
            map->walls[i].B = (Vector2) {.x = 0, .y = 0};
            map->walls[i].middlepoint = (Vector2) {.x = 0, .y = 0};
            map->walls[i].normal = (Vector2) {.x = 0, .y = 0};
            map->walls[i].next = NULL;
            map->walls[i].exists = false;
        }

        map->wall_nb = (int16_t) wall_nb;
        if(wall_nb < MAX_WALLS_NB)
            map->current_wall_index = wall_nb + 1;
        else
            map->current_wall_index = -1;
            
        //skip the BSP tree
  		int32_t elements_size;
  		if(fread((void *) &elements_size, sizeof(int32_t), 1, f) != 1)
        {
            printf("File incomplete or error.\n");
            SLUGmaker_UnloadMap(map);
            fclose(f);
            return NULL;
        }      
        
        SLUG_BSPTreeElement garbage_BSP_element;
        for(int32_t i = 0; i < elements_size; ++i)
        {
        	if(fread((void *) &garbage_BSP_element, sizeof(SLUG_BSPTreeElement), 1, f) != 1)
		    {
		        printf("File incomplete or error.\n");
		        SLUGmaker_UnloadMap(map);
                fclose(f);
		        return NULL;
		    }  
        }
    }
    else
    {
        for(int32_t i = 0; i < MAX_WALLS_NB; ++i)
        {
            map->walls[i].A = (Vector2) {.x = 0, .y = 0};
            map->walls[i].B = (Vector2) {.x = 0, .y = 0};
            map->walls[i].middlepoint = (Vector2) {.x = 0, .y = 0};
            map->walls[i].normal = (Vector2) {.x = 0, .y = 0};
            map->walls[i].next = NULL;
            map->walls[i].exists = false;
        }
    }
    
    
    map->wall_line_mode = false;
    map->wall_move_mode = -1;
    map->wall_line_origin_index = -1;
    
    //player spawn point init
    if(fread((void *) &(map->player_spawn_point), sizeof(Vector2), 1, f) != 1)
    {
        printf("File incomplete or error.\n");
        SLUGmaker_UnloadMap(map);
        fclose(f);
        return NULL;
    }

    fclose(f);

    return map; 
}

void SLUGmaker_UnloadMap(SLUGmaker_map *map)
{
    if(map != NULL)
    {
    	for(uint16_t i = 0; i < MAX_SPRITES; ++i)
    	{
    		if(map->loaded_sprites[i].id > 0)
    			UnloadTexture(map->loaded_sprites[i]);
    	}
        free(map);
        map = NULL;
    }
}

//map walls
SLUG_SegmentExtended *SLUGmaker_GetSegments(SLUGmaker_map *map)
{
    if(map == NULL)
        return NULL;
    if(map->wall_nb <= 0)
        return NULL;

    SLUG_SegmentExtended *segs = (SLUG_SegmentExtended *) malloc(map->wall_nb * sizeof(SLUG_SegmentExtended));
    if(segs == NULL)
    {
        printf("Malloc Error\n");
        return NULL;
    }

    int16_t count = 0;
    for(int16_t i = 0; i < MAX_WALLS_NB; ++i)
    {  
        if(map->walls[i].exists)
        {
            if(count >= map->wall_nb)
            {   
                printf("Array too small for segments\n");
                free(segs);
                segs = NULL;
                return NULL;
            }

            segs[count].A = map->walls[i].A;
            segs[count].B = map->walls[i].B;
            segs[count].normal = map->walls[i].normal;
            segs[count].dist = Vector2DotProduct(map->walls[i].A, map->walls[i].normal);
            count += 1;       
        }
    }
    if(count != map->wall_nb)
    {
        printf("Wall number is incorrect\n");
        free(segs);
        segs = NULL;
        return NULL;
    }

    return segs;

}

int32_t* SLUGmaker_GetWallsLinks(SLUGmaker_map *map)
{
    if(map == NULL)
        return NULL;
    if(map->wall_nb <= 0)
        return NULL;

    int32_t index[map->wall_nb];

    int16_t count = 0;
    for(int16_t i = 0; i < MAX_WALLS_NB; ++i)
    {  
        if(map->walls[i].exists)
        {
            if(count >= map->wall_nb)
            {   
                printf("Array too small for segments\n");
                return NULL;
            }

            index[count] = (int32_t) i;          
            count += 1;       
        }
    }
    if(count != map->wall_nb)
    {
        printf("WAll number is incorrect\n");
        return NULL;
    }

    int32_t *links = (int32_t *) malloc(map->wall_nb * sizeof(int32_t));
    if(links == NULL)
    {
        printf("Malloc Error\n");
        return NULL;
    }

    for(int32_t j = 0; j < map->wall_nb; j++)
    {

        if(map->walls[index[j]].next == NULL || map->walls[index[j]].next < &(map->walls[0]) || map->walls[index[j]].next >= &(map->walls[0]) + MAX_WALLS_NB)
        {
            printf("Unconnected wall\n");
            return NULL;
        }

        int32_t next_index = (int32_t) (map->walls[index[j]].next - &(map->walls[0])); //tema ce crime de guerre
        links[j] = -1;

        for(int32_t k = 0; k < map->wall_nb; k++)
        {
            if(index[k] == next_index)
            {
                links[j] = k;
                break;
            }
        }

        if(links[j] == -1)
        {
            printf("Error : wall %d doesn't exist\n", j);
            free(links);
            links = NULL;
            return NULL;
        }

    }

    return links;
}

bool SLUGmaker_MapWallsTest(SLUG_SegmentExtended *segs, int32_t seg_size)
{
    if(segs == NULL || seg_size <= 0)
        return false;

    //no single-points segments
    for(int32_t i = 0; i < seg_size; ++i)
    {
        if(segs->A.x == segs->B.x && segs->A.y == segs->B.y)
        {
            printf("Single point segment : segment %d\n",i);
            return false;
        }
    }

    //no intersections
    if(SegmentExtendedIntersectArray(segs, seg_size))
    {
        printf("Segment intersectino\n");
        return false;
    }

    return true;
}

//BSPTree
int8_t SLUGmaker_BSPTreeBuildRecursive(SLUG_BSPTree *tree,int32_t *node_nb, int8_t *elements_taken)
{
    if(tree == NULL || elements_taken == NULL || node_nb == NULL)
        return -1;

    if(*node_nb >= tree->elements_size)
    {   
        printf("Too much node split\n");
        return -1;
    }

    if(SLUGmaker_CountOnes(elements_taken, tree->tab_size) <= 0)
    {
        printf("Wtf the elements_taken array is empty!\n");
        return -1;
    }
    
    int32_t index = SLUGmaker_GetIndexForPosition(elements_taken, tree->tab_size, 1);
    int32_t varf = 2147483647;
    int32_t varb = 0;

    for(int32_t i = 0; i < tree->tab_size; ++i)
    {
        if(elements_taken[i] != 0)
        {

            int32_t varf2 = 0;
            int32_t varb2 = 0;

            for(int32_t j = 0; j < tree->tab_size; ++j)
            {
                if(elements_taken[j] != 0 && j != i)
                {
                    float a = GetSegmentExtendedSide(&(tree->tab[i]), &(tree->tab[j].A));
                    float b = GetSegmentExtendedSide(&(tree->tab[i]), &(tree->tab[j].B));

                    varf2 += a > 0 || b > 0;
                    varb2 += a < 0 || b < 0; 
                }
            }

            if((varb2 + varf2 < varb + varf) || ((varb2 + varf2 == varb + varf) && (abs(varb2 - varf2) < abs(varb - varf))))
            {
                index = i;
                varf = varf2;
                varb = varb2;
            }
        }
    }

    int8_t back_tab[tree->tab_size];
    int8_t front_tab[tree->tab_size];

    for(int32_t i = 0; i < tree->tab_size; ++i)
    {
        front_tab[i] = 0;
        back_tab[i] = 0;
        if(elements_taken[i] != 0 && i != index)
        {
            float a = GetSegmentExtendedSide(&(tree->tab[index]), &(tree->tab[i].A));
            float b = GetSegmentExtendedSide(&(tree->tab[index]), &(tree->tab[i].B));

            front_tab[i] = a > 0 || b > 0;
            back_tab[i] = a < 0 || b < 0;
        }
    }
    

    tree->elements[*node_nb].segment = index;
    tree->elements[*node_nb].children[0] = SPACE_SOLID;
    tree->elements[*node_nb].children[1] = SPACE_EMPTY;
    
    int32_t node_now = *node_nb;

    int8_t err = 0;

    if(!SLUGmaker_AllZeros(front_tab, tree->tab_size))
    {
        *node_nb += 1;
        tree->elements[node_now].children[1] = *node_nb;
        err = SLUGmaker_BSPTreeBuildRecursive(tree,node_nb, front_tab);
    }

    if(err != 0)
        return err;

    if(!SLUGmaker_AllZeros(back_tab, tree->tab_size))
    {
        *node_nb += 1;
        tree->elements[node_now].children[0] = *node_nb;
        err = SLUGmaker_BSPTreeBuildRecursive(tree,node_nb, back_tab);
    }

    if(err != 0)
        return err;
    
    return 0;
}

int8_t SLUGmaker_BSPTreeBuild(SLUG_BSPTree *tree,int32_t *node_nb)
{
    int8_t tab[tree->tab_size];
    for(uint32_t i = 0; i < tree->tab_size; ++i)
        tab[i] = 1;
    return SLUGmaker_BSPTreeBuildRecursive(tree,node_nb,tab);
}

void SLUG_BSPTreeUnload(SLUG_BSPTree *tree)
{
    if(tree != NULL)
    {
        if(tree->tab != NULL)
        {
            free(tree->tab);
            tree->tab = NULL;
        }    
        if(tree->elements != NULL)
        {
            free(tree->elements);
            tree->elements = NULL;
        }
            
        free(tree);
        tree = NULL;
    }
}

//map save
bool SLUGmaker_CheckSave(SLUGmaker_map *map)
{
    if(map == NULL)
        return false;
    if(map->wall_line_mode)
    {
        printf("Cannot save while editing walls. Please finish placing the current loop.\n");
        return false;
    }
    if(map->wall_move_mode >= 0)
    {
        printf("Cannot save while moving walls.\n");
        return false;
    }    
    if(strlen(map->map_name) == 0)
    {
        printf("Map has no name.\n");
        return false;
    }
    if(map->sprite_nb < 0)
    {
    	printf("Negative number of map sprites.\n");
    	return false;
    }
    return true;
}

int8_t SLUGmaker_WriteMap(SLUGmaker_map *map)//TODO:windows
{
    if(map == NULL)
        return -1;
    
    if(!SLUGmaker_CheckSave(map))
        return -1;
    
    char path[strlen(map->map_path) + 6];
    sprintf(path,"%s.temp",map->map_path);
    struct stat sb;
    if(stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        if(SLUGmaker_RemoveDirRecursive(path) == -1)
        {
            printf("Remove Error.\n");
            return -1;
        }
    }

    if(mkdir(path, 0777) != 0)
    {
        printf("Error on folder creation.\n");
        return -1;
    }

    
    //save sprites
    char assets[strlen(path) + 8];
    sprintf(assets,"%s/assets",path);
    if(mkdir(assets, 0777) != 0)
    {
        printf("Error on folder creation.\n");
        return -1;
    }
    
    char sprites[strlen(assets) + 9];
    sprintf(sprites,"%s/sprites",assets);
    if(mkdir(sprites, 0777) != 0)
    {
        printf("Error on folder creation.\n");
        return -1;
    }

    //save images
    char sprites_files_buffer[strlen(sprites) + 18];
    sprintf(sprites_files_buffer, "%s/sprite_names.txt", sprites);
    FILE *sprites_file = fopen(sprites_files_buffer,"w");
    if(sprites_file == NULL)
    {
        printf("Error on sprite name file creation.\n");
        return -1;
    }
    
    for(int16_t s = 0; s < MAX_SPRITES; ++s)
    {
        if(map->loaded_sprites[s].id > 0)
        {
            char sprite_name[strlen(sprites) + strlen(map->loaded_sprites_names[s]) + 2];
            sprintf(sprite_name,"%s/%s",sprites,map->loaded_sprites_names[s]);
            if(ExportImage(LoadImageFromTexture(map->loaded_sprites[s]),sprite_name))
                fwrite(map->loaded_sprites_names[s],sizeof(char),strlen(map->loaded_sprites_names[s]),sprites_file);
            else
            {
                printf("Warning : image export did not work.\n");
                fwrite("\n",sizeof(char),1,sprites_file);
            }
        }
        else
            fwrite("\n",sizeof(char),1,sprites_file);
    }
    
    fclose(sprites_file);

    //save sounds
    char sounds[strlen(assets) + 7];
    sprintf(sounds,"%s/sound",assets);
    if(mkdir(sounds, 0777) != 0)
    {
        printf("Error on folder creation.\n");
        return -1;
    }

    //map file
    char map_file_name[strlen(path) + 10];
    sprintf(map_file_name,"%s/map.slug",path);
    FILE *f = fopen(map_file_name, "w");
    if(f == NULL)
    {
        printf("Could not open file.\n");
        return -1;
    }
    
    //signature
    unsigned char slug[] = {0x53, 0x4C, 0x55, 0x47, 0x4D, 0x41, 0x50};
    if(fwrite((void *) slug, sizeof(slug), 1, f) == 0)
    {
        printf("Write Error\n");
        return -1;
    }

    //map size
    uint32_t width  = (uint32_t) map->zone.width;
    uint32_t height = (uint32_t) map->zone.height;

    if(fwrite((void *) &width, sizeof(uint32_t), 1, f) == 0)
    {
        printf("Write Error\n");
        return -1;
    }

    if(fwrite((void *) &height, sizeof(uint32_t), 1, f) == 0)
    {
        printf("Write Error\n");
        return -1;
    }

    //save sprites rect
    if(fwrite((void *) &(map->sprite_nb), sizeof(int16_t), 1, f) == 0)
    {
        printf("Write Error\n");
        return -1;
    }

	if(map->sprite_nb > 0)
	{
		if(fwrite((void *) map->map_sprites, sizeof(SLUGmaker_PlacableSprite), map->sprite_nb, f) == 0)
		{
		    printf("Write Error\n");
		    return -1;
		}
	}

    //if there are walls, save the structure
    if(map->wall_nb > 0)
    {
        //create BSP Tree
        SLUG_BSPTree *tree = (SLUG_BSPTree *) malloc(sizeof(SLUG_BSPTree));
        if(tree == NULL)
        {
            printf("Malloc error\n");
            fclose(f);
            return -1;
        }

        tree->tab_size = (int32_t) (map->wall_nb);
        tree->tab = SLUGmaker_GetSegments(map);
        if(tree->tab == NULL)
        {
            printf("Malloc error\n");
            fclose(f);
            SLUG_BSPTreeUnload(tree);
            return -1;
        }

        if(!SLUGmaker_MapWallsTest(tree->tab, tree->tab_size))
        {
            printf("Node disposition is incorrect\n");
            fclose(f);
            SLUG_BSPTreeUnload(tree);
            return -1;
        }

        int32_t node_nb = 0;
        tree->elements_size = 2 * tree->tab_size;

        tree->elements = (SLUG_BSPTreeElement *) malloc(tree->elements_size * sizeof(SLUG_BSPTreeElement));
        if(tree->elements == NULL)
        {
            printf("Malloc error\n");
            fclose(f);
            SLUG_BSPTreeUnload(tree);
            return -1;
        }
		
        if(SLUGmaker_BSPTreeBuild(tree,&node_nb) != 0)
        {
            printf("Tree Build Fail\n");
            fclose(f);
            SLUG_BSPTreeUnload(tree);
            return -1;
        }
  
        tree->elements_size = node_nb + 1;
        
        tree->elements = (SLUG_BSPTreeElement *) realloc(tree->elements, tree->elements_size * sizeof(SLUG_BSPTreeElement));
        if(tree->elements == NULL)
        {
            printf("Realloc error\n");
            fclose(f);
            SLUG_BSPTreeUnload(tree);
            return -1;
        }
             
        //write walls       
        size_t s = 2*sizeof(int32_t) + tree->tab_size * sizeof(SLUG_SegmentExtended) + tree->elements_size * sizeof(SLUG_BSPTreeElement);
        if(fwrite((void *) &s, sizeof(size_t), 1, f) == 0)
        {
            printf("Write Error\n");
            fclose(f);
            SLUG_BSPTreeUnload(tree);
            return -1;
        }

        if(fwrite((void *) &(tree->tab_size), sizeof(int32_t const), 1, f) == 0)
        {
            printf("Write Error\n");
            fclose(f);
            SLUG_BSPTreeUnload(tree);
            return -1;
        }
            
        if(fwrite((void *) tree->tab, sizeof(SLUG_SegmentExtended), tree->tab_size, f) == 0)
        {
            printf("Write Error\n");
            fclose(f);
            SLUG_BSPTreeUnload(tree);
            return -1;
        }

        //write links
        int32_t *links = SLUGmaker_GetWallsLinks(map);
        if(links == NULL)
        {
            printf("Error while writing wall linking info\n");
            fclose(f);
            SLUG_BSPTreeUnload(tree);
            return -1;
        }

        if(fwrite((void *) links, sizeof(int32_t), tree->tab_size, f) == 0)
        {
            printf("Write Error\n");
            fclose(f);
            SLUG_BSPTreeUnload(tree);
            free(links);
            links = NULL;
            return -1;
        }

        free(links);
        links = NULL;

        //write BSPTree
        if(fwrite((void *) &(tree->elements_size), sizeof(int32_t), 1, f) == 0)
        {
            printf("Write Error\n");
            fclose(f);
            SLUG_BSPTreeUnload(tree);
            return -1;
        }

        if(fwrite((void *) tree->elements, sizeof(SLUG_BSPTreeElement), tree->elements_size, f) == 0)
        {
            printf("Write Error\n");
            fclose(f);
            SLUG_BSPTreeUnload(tree);
            return -1;
        }

        SLUG_BSPTreeUnload(tree);
    }
    else
    {
        //write BSPTree size = 0
        size_t s = 0;
        if(fwrite((void *) &s, sizeof(size_t), 1, f) == 0)
        {
            printf("Write Error\n");
            fclose(f);
            return -1;
        }
    }
    
    //write player spawn point
    if(fwrite((void *) &(map->player_spawn_point), sizeof(Vector2), 1, f) == 0)
    {
        printf("Write Error\n");
        fclose(f);
        return -1;
    }

    fclose(f);

    printf("Succes ! Moving temp file.\n");

    char path_final[strlen(map->map_path) + strlen(map->map_name) + 1];
    sprintf(path_final,"%s%s",map->map_path,map->map_name);

    if(stat(path_final, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        if(SLUGmaker_RemoveDirRecursive(path_final) == -1)
        {
            printf("Remove Error.\n");
            return -1;
        }
    }

    if(rename(path,path_final) != 0)
        printf("Could not rename your file. You will find your map in .temp");
    else
        printf("Temp file renamed.\n");
    
    return 0;
}
