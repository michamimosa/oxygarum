/**
 *  converters/obj/convert_obj.c
 *
 *  (C) Copyright 2013 Michael Sippel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../../include/vertex.h"
#include "../../include/face.h"

typedef struct mat
{
    char texD[256];
    char texA[256];
    char texS[256];
    float d[3];
    float a[3];
    float s[3];
    float sh;
} mat_t;

int readstr(FILE *f, char *string)
{
    do
    {
        if(fgets(string, 255, f) == NULL)
        {
            return -1;
        }
    }
    while ((string[0] == '#') || (string[0] == '\n'));
    return 0;
}

int main(int argc, char **argv)
{
    if(argc < 3)
    {
        printf("Usage: %s [source] [output]\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *src = fopen(argv[1], "rt");
    if(src == NULL)
    {
        printf("Error.\n");
        return EXIT_FAILURE;
    }

    FILE *dest = fopen(argv[2], "wb");
    if(dest == NULL)
    {
        printf("Error.\n");
        return EXIT_FAILURE;
    }

    int i,j;

    mat_t material;
    int have_material = 0;
    int num_vertices = 0;
    int num_faces = 0;
    int num_texcoords = 0;
    int num_normals = 0;
    char line[256];

    fseek(src, SEEK_SET, 0);

    readstr(src, line);
    char mtllib[256];
    char mat_name[256];
    if(sscanf(line, "mtllib %s\n", mtllib) != NULL)
    {
        printf("Material: %s\n", mtllib);
        FILE *mtl = fopen(mtllib, "rt");
        if(mtl == NULL)
        {
            printf("Error.\n");
            return EXIT_FAILURE;
        }
        else
        {
            readstr(mtl, line);
            sscanf(line, "newmtl %s\n", mat_name);
            readstr(mtl, line);
            sscanf(line, "Ka %f %f %f\n", &material.a[0], &material.a[1], &material.a[2]);
            readstr(mtl, line);
            sscanf(line, "Kd %f %f %f\n", &material.d[0], &material.d[1], &material.d[2]);
            readstr(mtl, line);
            sscanf(line, "Ks %f %f %f\n", &material.s[0], &material.s[1], &material.s[2]);
            readstr(mtl, line);
            readstr(mtl, line);
            sscanf(line, "Ns %f\n", &material.sh);
            readstr(mtl, line);
            sscanf(line, "map_Kd %s\n", &material.texD);
            readstr(mtl, line);
            sscanf(line, "map_Ka %s\n", &material.texA);
            readstr(mtl, line);
            sscanf(line, "map_Ks %s\n", &material.texS);
            have_material = 1;
        }
    }

    while(readstr(src, line) == 0)
    {
        switch(line[0])
        {
            case 'v':
                switch(line[1])
                {
                    case 't': // texture choord
                        num_texcoords++;
                        break;
                    case 'n': // normal
                        num_normals++;
                        break;
                    default: // vertex
                        num_vertices++;
                        break;
                }
                break;
            case 'f':
                num_faces++;
                break;
        }
    }

    printf("%d vertices\n", num_vertices);
    printf("%d texture coords\n", num_texcoords);
    printf("%d normals\n", num_normals);
    printf("%d faces\n", num_faces);

    vertex3d_t *vertices = (vertex3d_t*) calloc(num_vertices, sizeof(vertex3d_t));
    uv_t *texcoords = (uv_t*) calloc(num_texcoords, sizeof(uv_t));
    int **uv_maps = (int**) calloc(num_faces, sizeof(int*));
    int *size = (int*) calloc(num_faces, sizeof(int));
    int **faces = (int**) calloc(num_faces, sizeof(int*));

    fseek(src, SEEK_SET, 0);
    int v = 0, t = 0, n = 0, f = 0, pos;
    int c,d,e;
    char buf[256];

    while(readstr(src, line) == 0)
    {
        int len = strlen(line);
        switch(line[0])
        {
            case 'v':
                switch(line[1])
                {
                    case 't':
                        sscanf(line, "vt %f %f\n", &texcoords[t].u, &texcoords[t].v);
                        t++;
                        break;
                    case 'n':
                        // ignore normals
                        n++;
                        break;
                    default:
                        sscanf(line, "v %f %f %f\n", &vertices[v].x, &vertices[v].y, &vertices[v].z);
                        v++;
                        break;
                }
                break;
            case 'f':
                size[f] = 0;
                pos = 1;
                while(pos < len)
                {
                    if(num_texcoords == 0 && num_normals == 0)
                    {
                        if(sscanf(line+pos, " %d", &c) == EOF) break;
                        sprintf(buf, " %d", c);
                        pos += strlen(buf);
                    }
                    else if(num_texcoords != 0 && num_normals == 0)
                    {
                        if(sscanf(line+pos, " %d/%d", &c, &d) == EOF) break;
                        sprintf(buf, " %d/%d", c, d);
                        pos += strlen(buf);
                    }
                    else if(num_texcoords != 0 && num_normals != 0)
                    {
                        if(sscanf(line+pos, " %d/%d/%d", &c, &d, &e) == EOF) break;
                        sprintf(buf, " %d/%d/%d", c, d, e);
                        pos += strlen(buf);
                    }
                    size[f]++;
                }
                faces[f] = (int*) calloc(size[f], sizeof(int));
                if(num_texcoords != 0)
                {
                    uv_maps[f] = (int*) calloc(size[f], sizeof(int));
                }

                pos = 1;
                for(i = 0; i < size[f]; i++)
                {
                    if(num_texcoords == 0 && num_normals == 0)
                    {
                        sscanf(line+pos, " %d", &faces[f][i]);
                        sprintf(buf, " %d", faces[f][i]);
                        pos += strlen(buf);
                    }
                    else if(num_texcoords != 0 && num_normals == 0)
                    {
                        sscanf(line+pos, " %d/%d", &faces[f][i], &uv_maps[f][i]);
                        sprintf(buf, " %d/%d", faces[f][i], uv_maps[f][i]);
                        pos += strlen(buf);
                    }
                    else if(num_texcoords != 0 && num_normals != 0)
                    {
                        sscanf(line+pos, " %d/%d/%d", &faces[f][i], &uv_maps[f][i], &e);
                        sprintf(buf, " %d/%d/%d", faces[f][i], uv_maps[f][i], e);
                        pos += strlen(buf);
                    }
                }
                f++;
                break;
            default:
                break;
        }
    }

    if(! have_material)
    {
        fprintf(dest,
                "MATERIAL\n"
                "texture.png\n"
                "AMBIENT   0.2 0.2 0.2 0.0\n"
                "DIFFUSE   1.0 1.0 1.0 0.0\n"
                "SPECULAR  0.2 0.2 0.2 0.0\n"
                "SHININESS 1.0\n\n"
               );
    }
    else
    {
        fprintf(dest, "MATERIAL\n");
        fprintf(dest, "%s\n", material.texD);
        fprintf(dest, "AMBIENT   %f %f %f %f\n", material.a[0], material.a[1], material.a[2]);
        fprintf(dest, "DIFFUSE   %f %f %f %f\n", material.d[0], material.d[1], material.d[2]);
        fprintf(dest, "SPECULAR  %f %f %f %f\n", material.s[0], material.s[1], material.s[2]);
        fprintf(dest, "SHININESS %f\n\n", material.sh);
    }

    fprintf(dest, "VERTICES %d\n", num_vertices);
    for(i = 0; i < num_vertices; i++)
    {
        fprintf(dest, "%d: %f %f %f\n", i, vertices[i].x, vertices[i].y, vertices[i].z);
    }

    fprintf(dest, "\n");

    if(num_texcoords != 0)
    {
        fprintf(dest, "UVMAPS %d\n", num_faces);
        for(i = 0; i < num_faces; i++)
        {
            fprintf(dest, "%d: SIZE %d\n", i, size[i]);
            for(j = 0; j < size[i]; j++)
            {
                fprintf(dest, "%f %f\n", texcoords[uv_maps[i][j]-1].u, texcoords[uv_maps[i][j]-1].v);
            }
        }
    }
    else
    {
        fprintf(dest,
                "UVMAPS 1\n"
                "0: SIZE 4\n"
                "0 1\n"
                "0 0\n"
                "1 0\n"
                "1 1\n\n"
               );
    }

    fprintf(dest, "\n");

    fprintf(dest, "FACES %d\n", num_faces);
    for(i = 0; i < num_faces; i++)
    {
        int uvmap = 0;
        if(num_texcoords != 0) uvmap = i;
        fprintf(dest, "%d : SIZE %d\n%d", i, size[i], uvmap);
        for(j = 0; j < size[i]; j++)
        {
            fprintf(dest, " %d", faces[i][j]-1);
        }
        fprintf(dest, "\n");
    }

    fclose(src);
    fclose(dest);

    return EXIT_SUCCESS;
}
