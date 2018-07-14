/*
 * Level.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: twiemann
 */

#include "Actor.hpp"
#include "Level.hpp"
#include "StaticRenderable.hpp"

#include <iostream>
#include <fstream>
#include <cmath>
using std::cout;
using std::endl;

#include <SDL2/SDL_image.h>


namespace jumper
{


Level::Level(SDL_Renderer* renderer, std::string filename) : StaticRenderable(renderer)
{
    // Set all default values
    m_texture       = 0;
    m_tileWidth     = 0;
    m_tileHeight    = 0;
    m_tileOffset    = 0;
    m_numRows = 0;
    m_keyR = 0;
    m_keyG = 0;
    m_keyB = 0;
    m_tilesPerRow = 0;
    m_levelWidth = 0;
    m_levelHeight = 0;


    // Read meta data from level file
    std::ifstream in(filename.c_str());
    std::string texFileName;
    std::string slopesFileName;

    int ir, ig, ib;

    if(in.good())
    {
        in >> texFileName;
        in >> slopesFileName;
        in >> m_tileWidth >> m_tileHeight >> m_tilesPerRow >> m_numRows;
        in >> m_tileOffset >> ir >> ig >> ib;
        in >> m_levelWidth >> m_levelHeight;
    }
    else
    {
        std::cout << "Unable to open file " << filename << std::endl;
    }

    SparseMatrix tiles(m_levelHeight, m_levelWidth);
    m_tiles = tiles;

    // Cast keying colors manually!
    m_keyR = (unsigned char)ir;
    m_keyG = (unsigned char)ig;
    m_keyB = (unsigned char)ib;

    // Load texture
    std::size_t found = filename.find_last_of("/\\");
    string path = filename.substr(0,found);

    m_texture = TextureFactory::instance(m_renderer).getTexture(path + "\\" + texFileName, m_keyR, m_keyG, m_keyB);

    if(!m_texture)
    {
        std::cout << "Unable to load texture " << texFileName << std::endl;
    }

    // Read tile indices
    for(int i = 0; i < m_levelHeight; i++)
    {
        for(int j = 0; j < m_levelWidth; j++)
        {
            int tileID;
            in >> tileID;
            m_tiles.insert(i, j, tileID);
        }
    }

    in.close();


    std::ifstream inSlopes(path + "\\" + slopesFileName.c_str());

    int left, right;

    if(inSlopes.good())
    {
        //13 = m_levelHeight * m_levelWidth
        for(int i = 1; i <= 169; i++)
        {
            inSlopes >> left >> right;
            struct YValues y = { left, right };
            m_slopes.insert(std::pair< int, YValues >(i, y));
        }
    }
    else
    {
        std::cout << "Unable to open file " << slopesFileName << std::endl;
    }

    inSlopes.close();
}


void Level::render()
{
    if(getRenderer() && m_texture)
    {
        int i;
        int j;
        int tile_index;
        int col, row;
        SDL_Rect target;
        SDL_Rect source;


        target.w = m_tileWidth;
        target.h = m_tileHeight;

        source.w = m_tileWidth;
        source.h = m_tileHeight;


        for(i = 0; i < m_levelHeight; i++)
        {
            for(j = 0; j < m_levelWidth; j++)
            {
                tile_index = m_tiles[i][j] - 1;
                if(tile_index >= 0)
                {
                    //Compute the position of the target on the screen
                    target.x = j * m_tileWidth - m_camera.x();
                    target.y = i * m_tileHeight - m_camera.y()  ;

                    // Don't render tiles outside the frustrum. To prevent popping,
                    // add some extra margin
                    if(target.x + target.h > -m_tileWidth && target.x + target.h < m_camera.w() + m_tileWidth)
                    {

                        row = tile_index / m_tilesPerRow;
                        col = tile_index % m_tilesPerRow;

                        source.x = col * m_tileWidth;
                        if(col > 0)
                        {
                            source.x += col * m_tileOffset;
                        }

                        source.y = row * m_tileHeight;
                        if(row > 0)
                        {
                            source.y += row * m_tileOffset;
                        }

                        // Copy pixel date to correct position
                        SDL_RenderCopy(getRenderer(), m_texture, &source, &target);
                    }
                }
            }
        }
    }
}

WorldProperty& Level::physics()
{
    return m_levelPhysics;
}

void Level::getSurroundingTiles(float x, float y, int width, int height, Vector2i *tiles)
{
    /* Determine x and y position of the sprite within the grid */
    Vector2i gridPos(floor((x + 0.5 * width) / m_tileWidth), floor((y + 0.5 * height) / m_tileHeight));

    /* Get the surrounding tiles in "priority" order, i.e., we want
     * check some collisions like left before we check the others
     */

    //upper left tile
    tiles[0].setX(gridPos.x() - 1);
    tiles[0].setY(gridPos.y() - 1);

    //upper central tile
    tiles[1].setX(gridPos.x());
    tiles[1].setY(gridPos.y() - 1);

    //upper right tile
    tiles[2].setX(gridPos.x() + 1);
    tiles[2].setY(gridPos.y() - 1);

    //middle left tile
    tiles[3].setX(gridPos.x() - 1);
    tiles[3].setY(gridPos.y());

    //middle right tile
    tiles[4].setX(gridPos.x() + 1);
    tiles[4].setY(gridPos.y());

    //lower left tile
    tiles[5].setX(gridPos.x() - 1);
    tiles[5].setY(gridPos.y() + 1);

    //lower central tile
    tiles[6].setX(gridPos.x());
    tiles[6].setY(gridPos.y() + 1);

    //lower right tile
    tiles[7].setX(gridPos.x() + 1);
    tiles[7].setY(gridPos.y() + 1);

    //central tile
    tiles[8].setX(gridPos.x());
    tiles[8].setY(gridPos.y());
}

int Level::levelHeight() const
{
    return m_levelHeight;
}

int Level::levelWidth() const
{
    return m_levelWidth;
}

SparseMatrix& Level::tiles()
{
    return m_tiles;
}

int Level::tileWidth() const
{
    return m_tileWidth;
}

int Level::tileHeight() const
{
    return m_tileHeight;
}

Level::~Level()
{
    // Free texture resources
    SDL_DestroyTexture(m_texture);
}
float Level::slopeShift(float l, float r, int w)
{
    float value = (float)w / (float)m_tileWidth;
    float big_y = sqrt(pow(m_tileWidth,2) + pow((l - r),2));
    float small_y = big_y * value;
    float y_shift = sqrt(pow(small_y,2) - pow(w,2));

    return y_shift;
}

Collision Level::resolveCollision(Actor* player)
{
    SDL_Rect tileRect;
    SDL_Rect spriteRect;
    SDL_Rect intersectionRect;
    Vector2f desiredPosition;
    Vector2i surroundingTiles[9];
    int n, i ,j;

    // set desired position to new position
    desiredPosition = player->position();

    // set position of the hitbox
    float hitboxX = ((float)desiredPosition.x() + (float)player->w() / 2.0) - ((float)player->hitboxW() / 2.0);
    float hitboxY = ((float)desiredPosition.y() + (float)player->h() / 2.0) - ((float)player->hitboxH() / 2.0);

    Vector2f hitboxVector(hitboxX, hitboxY);

    // Check if sprite intersects with one of its surrounding tiles
    getSurroundingTiles(hitboxX, hitboxY, player->hitboxW(), player->hitboxH(), surroundingTiles);

    //holder to correct unnecessary down push while sliding up on a vertical wall
    static int holder;

    Vector2i v(0, 0);

    Collision c(v);

    Vector2f hitVec(desiredPosition.x(),desiredPosition.y());

    for(n = 0; n < 9; n++)
    {
        j = surroundingTiles[n].x();
        i = surroundingTiles[n].y();

        // check, if tile coordinates are valid
        if((i >= 0) && (i < m_levelHeight) && (j >= 0) && (j < m_levelWidth) )
        {
            // if tile is not entirely transparent
            if(m_tiles[i][j] > 0)
            {
                // set changes to hitboxvector
                if(hitVec.x() - desiredPosition.x() != 0 || hitVec.y() - desiredPosition.y() != 0)
                {
                    hitboxVector.setX(hitboxVector.x() - hitVec.x() - desiredPosition.x());
                    hitboxVector.setY(hitboxVector.y() - hitVec.y() - desiredPosition.y());
                    hitVec = desiredPosition;
                }

                // Get SDL rect for current tile and sprite and check intersection
                tileRect.y = i * m_tileHeight;
                tileRect.x = j * m_tileWidth;
                tileRect.w = m_tileWidth;
                tileRect.h = m_tileHeight;

                spriteRect.x = hitboxVector.x();
                spriteRect.y = hitboxVector.y();
                spriteRect.w = player->hitboxW();
                spriteRect.h = player->hitboxH();

                if(SDL_IntersectRect(&tileRect, &spriteRect, &intersectionRect))
                {
                    // check, if tile number is valid
                    if(m_tiles[i][j] <= 169)
                    {
                        map< int, YValues >::iterator it = m_slopes.find(m_tiles[i][j]);

                        int left, right;

                        // get left and right values of the intersecting tile
                        if (it != m_slopes.end())
                        {
                            YValues x = (YValues)it->second;
                            left = x.left;
                            right = x.right;
                        }
                        else
                        {
                            cout << "!!!" << endl;
                        }

                        // if nontransparent tile, set collision-object
                        if(c.type() == NONE && ((left==right && left == 0) || left != right))
                        {
                            c = Collision(Vector2i(intersectionRect.w, intersectionRect.h));
                            c.setType(LEVEL);
                        }

                        // set Tile-Type for Collision-Object

                        if(left >= 0 && right >= 0)
                        {
                            // standard Slopes
                            if(left > right)
                            {
                                c.setTileType(SchraegeAufwaerts);
                            }
                            else if(left < right)
                            {
                                c.setTileType(SchraegeAbwaerts);
                            }
                        }
                        else
                        {
                            // inverted Slopes
                            if(left > right)
                            {
                                c.setTileType(SchraegeUmgekehrtAbwaerts);
                            }
                            else if(left < right)
                            {
                                c.setTileType(SchraegeUmgekehrtAufwaerts);
                            }
                        }

                        // Ende set Tile-Type
                        //check for equal left and right index -> if equal: tile is part of background!
                        if(left==right && left != 0)
                        {
                            float y_shift = slopeShift( left, right, intersectionRect.w);
                            float correction_piece = (float)m_tileHeight - (float)intersectionRect.h;

                            if(abs(left) < abs(right))
                            {
                                if(m_tileHeight - abs(right) > 0)
                                {
                                    correction_piece -= m_tileHeight -  abs(right);
                                }
                            }
                            else
                            {
                                if(m_tileHeight - abs(left) > 0)
                                {
                                    correction_piece -= m_tileHeight -  abs(left);
                                }
                            }

                            if(correction_piece <= y_shift)
                            {
                                if(left > 0 || right > 0)
                                {
                                    desiredPosition.setY(desiredPosition.y() - (y_shift - correction_piece));
                                }

                                else
                                {
                                    desiredPosition.setY(desiredPosition.y() + (y_shift - correction_piece));
                                }
                            }
                        }

                        //left and right
                        else if(n == 3 || n == 4)
                        {
                            /*
                             * check, if neighbouring tile shold be viewed as part of a slope
                             * if the difference in height between current tile and neighbouring tile is too high,
                             * the actor shouldn't be able to surmount it
                             */
                            if((n == 3 && m_tileHeight - abs(right) <= 4) || (n == 4 && m_tileHeight - abs(left) <= 4))
                            {
                                // calculate necessary shift in y-direction
                                float y_shift = slopeShift(left, right, intersectionRect.w);

                                float correction_piece = (float)m_tileHeight - (float)intersectionRect.h;

                                // if correction is necessary
                                if(correction_piece <= y_shift)
                                {
                                    // modify correction piece, if necessary
                                    if(n == 3 && m_tileHeight - abs(right) > 0)
                                    {
                                        correction_piece -= m_tileHeight -  abs(right);
                                    }
                                    else if(n == 4 && m_tileHeight - abs(left) > 0)
                                    {
                                        correction_piece -= m_tileHeight -  abs(left);
                                    }

                                    // if tile is a floor tile, actor needs to be pushed up
                                    else if(left > 0 || right > 0)
                                    {
                                        desiredPosition.setY(desiredPosition.y() - (y_shift - correction_piece));
                                    }
                                    // if tile is a ceiling tile, actor needs to be pushed down
                                    else
                                    {
                                        desiredPosition.setY(desiredPosition.y() + (y_shift - correction_piece));
                                    }
                                }

                            }
                            /*
                             * if neighbouring tile should not be viewed as part of a slope, but as an
                             * obstacle
                             */
                            else
                            {	
                                holder = 1;

                                /* if there is a collision with the left neighbour,
                                 * the actor should be pushed to the right
                                 */
                                if(n == 3)
                                {
                                    desiredPosition.setX(desiredPosition.x() + intersectionRect.w);
                                }
                                /* if there is a collision with the right neighbour,
                                 * the actor should be pushed to the left
                                 */
                                else // n == 4
                                {
                                    desiredPosition.setX(desiredPosition.x() - intersectionRect.w);
                                }
                            }
                            if(n == 3)
                            {
                                c.setType(LEFT);
                            }
                            else // n == 4
                            {
                                c.setType(RIGHT);
                            }
                        }

                        //down - middle - up
                        else if(n == 1 || n == 6 || n == 8)
                        {

                            // if there is a slope
                            if(left != right)
                            {
                                {
                                    float y_shift = slopeShift( left, right, intersectionRect.w);
                                    float correction_piece = (float)m_tileHeight - (float)intersectionRect.h;

                                    if(abs(left) < abs(right))
                                    {
                                        if(m_tileHeight - abs(right) > 0)
                                        {
                                            correction_piece -=m_tileHeight -  abs(right);
                                        }
                                    }
                                    else
                                    {
                                        if(m_tileHeight - abs(left) > 0)
                                        {
                                            correction_piece -=m_tileHeight -  abs(left);
                                        }
                                    }

                                    if(correction_piece <= y_shift)
                                    {
                                        if(left > 0 || right > 0)
                                        {
                                            // if tile is a floor tile, actor needs to be pushed up
                                            desiredPosition.setY(desiredPosition.y() - (y_shift - correction_piece));
                                        }

                                        else
                                        {
                                            // if tile is a ceiling tile, actor needs to be pushed down
                                            desiredPosition.setY(desiredPosition.y() + (y_shift - correction_piece));
                                        }
                                    }
                                }
                            }
                            else
                            {
                                if(n == 6)
                                {
                                    desiredPosition.setY(desiredPosition.y() - intersectionRect.h);
                                    c.setType(DOWN);
                                }
                                else
                                {
                                    desiredPosition.setY(desiredPosition.y() + intersectionRect.h);
                                    c.setType(UP);
                                }
                            }
                        }

                        //check upper left, upper right, lower left, lower right
                        else
                        {
                            //if there is an upper left - lower left or upper right - lower right collision
                            if (intersectionRect.w < intersectionRect.h)
                            {
                                // if there is a slope
                                if(left!=right)
                                {
                                    float y_shift = slopeShift( left, right, intersectionRect.w);
                                    float correction_piece = (float)m_tileHeight - (float)intersectionRect.h;

                                    if(abs(left) < abs(right))
                                    {
                                        if(m_tileHeight - abs(right) > 0)
                                        {
                                            correction_piece -= m_tileHeight -  abs(right);
                                        }
                                    }
                                    else
                                    {
                                        if(m_tileHeight - abs(left) > 0)
                                        {
                                            correction_piece -= m_tileHeight -  abs(left);
                                        }
                                    }

                                    if(correction_piece <= y_shift)
                                    {

                                        if(left > 0 || right > 0)
                                        {
                                            desiredPosition.setY(desiredPosition.y() - (y_shift - correction_piece));
                                            if(n == 5)
                                            {
                                                c.setType(LOWERLEFT);
                                            }
                                            else if(n == 7)
                                            {
                                                c.setType(LOWERRIGHT);
                                            }
                                        }

                                        else
                                        {
                                            desiredPosition.setY(desiredPosition.y() + (y_shift - correction_piece));

                                            if(n == 0)
                                            {
                                                c.setType(UPPERLEFT);
                                            }
                                            else if(n == 2)
                                            {
                                                c.setType(UPPERRIGHT);
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    if(n == 2 || n == 7)
                                    {
                                        desiredPosition.setX(desiredPosition.x() - intersectionRect.w);
                                        c.setType(RIGHT);
                                    }
                                    else //(n == 0 || n == 5)
                                    {
                                        desiredPosition.setX(desiredPosition.x() + intersectionRect.w);
                                        c.setType(LEFT);
                                    }
                                }

                            }

                            // if there is an upper left - upper right or lower left - lower right collision
                            else
                            {
                                // if there is a slope
                                if(left!=right)
                                {
                                    float y_shift = slopeShift( left, right, intersectionRect.w);
                                    float correction_piece = (float)m_tileHeight - (float)intersectionRect.h;

                                    if(abs(left) < abs(right))
                                    {
                                        if(m_tileHeight - abs(right) > 0)
                                        {
                                            correction_piece -=m_tileHeight -  abs(right);
                                        }
                                    }
                                    else
                                    {
                                        if(m_tileHeight - abs(left) > 0)
                                        {
                                            correction_piece -=m_tileHeight -  abs(left);
                                        }
                                    }

                                    if(correction_piece <= y_shift)
                                    {
                                        if(n == 5 || n == 7)
                                        {
                                            desiredPosition.setY(desiredPosition.y() - (y_shift - correction_piece));
                                        }
                                        else //(n == 0 || n == 2)
                                        {
                                            desiredPosition.setY(desiredPosition.y() + (y_shift - correction_piece));
                                        }

                                    }
                                }

                                else
                                {
                                    if(n == 5 || n == 7)
                                    {
                                        desiredPosition.setY(desiredPosition.y() - intersectionRect.h);
                                    }
                                    else //(n == 0 || n == 2)
                                    {
                                        if (holder == 1)
                                        {
                                            holder = 0;
                                        }
                                        else
                                        {
                                            desiredPosition.setY(desiredPosition.y() + intersectionRect.h);
                                        }

                                    }
                                }
                                if(n == 5 || n == 7)
                                {
                                    c.setType(DOWN);
                                }
                                else //(n == 0 || n == 2)
                                {
                                    c.setType(UP);
                                }

                            }
                        }
                    }
                }
            }
        }
        // if actor is projectile, return collision-object; no need to correct the position
        if(hitVec.x() != desiredPosition.x() || hitVec.y() != desiredPosition.y())
        {
            if(player->type() == PLAYERPROJECTILE || player->type() == BOTPROJECTILE)
            {
                return c;
            }
        }
    }

    // correct the position of the actor
    player->setPosition(Vector2f(desiredPosition.x(), desiredPosition.y()));


        return c;
    }

} /* namespace jumper */
