#include "map.h"

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

SLUGmaker_map* SLUGmaker_NewMap(const char *filename)
{
    SLUGmaker_map *map = (SLUGmaker_map *) malloc(sizeof(SLUGmaker_map));
    if(map == NULL)
    {
        printf("Malloc Error in map allocation.\n");
        return NULL;
    }
    map->fixed_sprite = LoadTexture(filename);
    if(map->fixed_sprite.id <= 0)
    {
        printf("Error while loading new map sprite.\n");
        free(map);
        map = NULL;
        return NULL;
    }
    SetTextureWrap(map->fixed_sprite, 1);
    map->zone.width = map->fixed_sprite.width;
    map->zone.height = map->fixed_sprite.height;
    map->zone.x = 0;
    map->zone.y = 0;
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
    return map;
}

SLUGmaker_map* SLUGmaker_LoadMap(const char *loadMap)
{
    if(loadMap == NULL)
        return NULL;

    uint32_t len = strlen(loadMap);
    if(len == 0)
        return NULL;

    SLUGmaker_map *map = (SLUGmaker_map*) malloc(sizeof(SLUGmaker_map));
    if(map == NULL)
    {
        printf("Malloc Error\n");
        return NULL;
    }

    char fixed_sprite[len + 37];
    strcpy(fixed_sprite, loadMap);
    strcat(fixed_sprite, "/assets/sprites/map_fixed_sprite.png");
    map->fixed_sprite = LoadTexture(fixed_sprite);
    if(map->fixed_sprite.id <= 0)
    {
        printf("Error while loading map sprite.\n");
        free(map);     
        map = NULL;   
        return NULL;
    }
    SetTextureWrap(map->fixed_sprite, 1);
    map->zone.width = map->fixed_sprite.width;
    map->zone.height = map->fixed_sprite.height;
    map->zone.x = 0;
    map->zone.y = 0;


    char mapslug[len + 10];
    strcpy(mapslug, loadMap);
    strcat(mapslug, "/map.slug");
    FILE *f = fopen(mapslug, "r");
    if(f == NULL)
    {
        printf("Error while loading file.\n");
        free(map);
        map = NULL;
        return NULL;
    }
    
    unsigned char signature[7];
    if(fread((void *) signature, sizeof(unsigned char), 7, f) != 7)
    {
        printf("File incomplete or error.\n");
        free(map);
        map = NULL;
        return NULL;
    }
    unsigned char test[7] = {0x53, 0x4C, 0x55, 0x47, 0x4D, 0x41, 0x50};
    if(memcmp(signature, test, 7) != 0)
    {
        printf("File signature is wrong.\n");
        free(map);
        map = NULL;
        return NULL;
    }

    size_t bsptree;
    if(fread((void *) &bsptree, sizeof(size_t), 1, f) != 1)
    {
        printf("File incomplete or error.\n");
        free(map);
        map = NULL;
        return NULL;
    }

    map->wall_nb = 0;
    map->current_wall_index = 0; 
    printf("%lu\n", bsptree); 
    if(bsptree != 0)
    {
        int32_t wall_nb;
        if(fread((void *) &wall_nb, sizeof(int32_t), 1, f) != 1)
        {
            printf("File incomplete or error.\n");
            free(map);
            map = NULL;
            return NULL;
        }

        if(wall_nb <= 0)
        {
            printf("Wtf there are no walls ???\n");
            free(map);
            map = NULL;
            return NULL;
        }

        if(wall_nb > MAX_WALLS_NB)
        {
            printf("Too many walls\n");
            free(map);
            map = NULL;
            return NULL;
        }

        SLUG_SegmentExtended walls[wall_nb];
        if(fread((void *) walls, sizeof(SLUG_SegmentExtended), wall_nb, f) != wall_nb)
        {
            printf("File incomplete or error.\n");
            free(map);
            map = NULL;
            return NULL;
        }

        int32_t links[wall_nb];
        if(fread((void *) links, sizeof(int32_t), wall_nb, f) != wall_nb)
        {
            printf("File incomplete or error.\n");
            free(map);
            map = NULL;
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

    return map; 
}

void SLUGmaker_UnloadMap(SLUGmaker_map *map)
{
    if(map != NULL)
    {
        if(map->fixed_sprite.id > 0)
            UnloadTexture(map->fixed_sprite);
        free(map);
        map = NULL;
    }
}

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

    int8_t err;

    if(!SLUGmaker_AllZeros(front_tab, tree->tab_size))
    {
        *node_nb += 1;
        tree->elements[*node_nb].children[1] = *node_nb;
        err = SLUGmaker_BSPTreeBuildRecursive(tree,node_nb, front_tab);
    }

    if(err != 0)
        return err;

    if(!SLUGmaker_AllZeros(back_tab, tree->tab_size))
    {
        *node_nb += 1;
        tree->elements[*node_nb].children[0] = *node_nb;
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
    return true;
}

int8_t SLUGmaker_WriteMap(SLUGmaker_map *map)
{
    
    if(!SLUGmaker_CheckSave(map))
        return -1;
    

    printf("Enter Map Name : \n");
    char buff[100];
    scanf("%s", buff);
    while ((getchar()) != '\n');
    struct stat sb;
    if(stat(buff, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        printf("Directory exists. Do you want to delete it [y/n]?\n");
        char resp = (char) getchar();
        printf("%c\n",resp);
        if(resp != 'y' && resp != 'Y')
        {
            printf("Save aborted.\n");
            return 0;
        }
        else
        {
            char cmd[107] = "rm -rf ";
            strcat(cmd, buff);
            if(system(cmd) != 0)
            {
                printf("Error on folder deletion.\n");
                return -1;
            }        
        }
    }

    if(mkdir(buff, 0777) != 0)
    {
        printf("Error on folder creation.\n");
        return -1;
    }

    char buff2[256];
    strcpy(buff2, buff);
    strcat(buff2, "/assets");
    if(mkdir(buff2, 0777) != 0)
    {
        printf("Error on folder creation.\n");
        return -1;
    }
    
    strcpy(buff2, buff);
    strcat(buff2, "/assets/sprites");
    if(mkdir(buff2, 0777) != 0)
    {
        printf("Error on folder creation.\n");
        return -1;
    }
    strcat(buff2, "/map_fixed_sprite.png");
    if(!ExportImage(LoadImageFromTexture(map->fixed_sprite), buff2))
    {
        printf("Error on image export.\n");
        return -1;
    }

    strcpy(buff2, buff);
    strcat(buff2, "/assets/sound");
    if(mkdir(buff2, 0777) != 0)
    {
        printf("Error on folder creation.\n");
        return -1;
    }

    
    strcpy(buff2, buff);
    strcat(buff2, "/map.slug");
    FILE *f = fopen(buff2, "w");
    if(f == NULL)
    {
        printf("Could not open file.\n");
        return -1;
    }
    
    unsigned char slug[] = {0x53, 0x4C, 0x55, 0x47, 0x4D, 0x41, 0x50};
    if(fwrite((void *) slug, sizeof(slug), 1, f) == 0)
    {
        printf("Write Error\n");
        return -1;
    }

    if(map->wall_nb > 0)
    {
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

        tree->elements_size = node_nb;
        tree->elements = (SLUG_BSPTreeElement *) realloc(tree->elements, tree->elements_size * sizeof(SLUG_BSPTreeElement));
        if(tree->elements == NULL)
        {
            printf("Realloc error\n");
            fclose(f);
            SLUG_BSPTreeUnload(tree);
            return -1;
        }
                
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
        size_t s = 0;
        if(fwrite((void *) &s, sizeof(size_t), 1, f) == 0)
        {
            printf("Write Error\n");
            fclose(f);
            return -1;
        }
    }

    fclose(f);
    
    return 0;
}
