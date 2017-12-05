#include "drawfield.h"

DrawField::DrawField()
{

}


void DrawField::draw()
{

    for (unsigned int var = 0; var < fields.size(); ++var) {
        ScalarField2 & hg = fields[var];
        for (int i = 0; i < hg.h-1; i++) {
            for (int j = 0; j < hg.w-1; j++) {


                Vector2 pos = hg.get(i, j);
                //pos = pos + Vector2(-0.5, -0.5);
                glColor3f(hg.field[hg.pos(i, j)], hg.field[hg.pos(i, j)], hg.field[hg.pos(i, j)]);
                glBegin(GL_TRIANGLES);
                glVertex3f(pos.x, pos.y, hg.field[hg.pos(i, j)]);


                pos = hg.get(i, j+1);
                //pos = pos + Vector2(-0.5, -0.5);
                glColor3f(hg.field[hg.pos(i, j+1)], hg.field[hg.pos(i, j+1)], hg.field[hg.pos(i, j+1)]);
                glVertex3f(pos.x, pos.y, hg.field[hg.pos(i, j+1)]);

                pos = hg.get(i+1, j);
                //pos = pos + Vector2(-0.5, -0.5);
                glColor3f(hg.field[hg.pos(i+1, j)], hg.field[hg.pos(i+1, j)], hg.field[hg.pos(i+1, j)]);
                glVertex3f(pos.x, pos.y, hg.field[hg.pos(i+1, j)]);

                ////////////////////////////////////////

                pos = hg.get(i+1, j);
                //pos = pos + Vector2(-0.5, -0.5);
                glColor3f(hg.field[hg.pos(i+1, j)], hg.field[hg.pos(i+1, j)], hg.field[hg.pos(i+1, j)]);
                glVertex3f(pos.x, pos.y, hg.field[hg.pos(i+1, j)]);

                pos = hg.get(i, j+1);
                //pos = pos + Vector2(-0.5, -0.5);
                glColor3f(hg.field[hg.pos(i, j+1)], hg.field[hg.pos(i, j+1)], hg.field[hg.pos(i, j+1)]);
                glBegin(GL_TRIANGLES);
                glVertex3f(pos.x, pos.y, hg.field[hg.pos(i, j+1)]);


                pos = hg.get(i+1, j+1);
                //pos = pos + Vector2(-0.5, -0.5);
                glColor3f(hg.field[hg.pos(i+1, j+1)], hg.field[hg.pos(i+1, j+1)], hg.field[hg.pos(i+1, j+1)]);
                glVertex3f(pos.x, pos.y, hg.field[hg.pos(i+1, j+1)]);
                glEnd();
            }
        }

    }

}


void DrawField::drawInterpol(int size, int idf){
    double pasX = (fields[idf].b.x - fields[idf].a.x)/(float)size;
    double pasY = (fields[idf].b.y - fields[idf].a.y)/(float)size;
    //glBegin(GL_POINTS);
    for (int i = 0; i < size-1; ++i) {
        for (int j = 0; j < size-1; ++j) {
            double z = 0;
            glBegin(GL_TRIANGLES);
            HeightField & hf = static_cast<HeightField&>(fields[idf]);
            Vector2 p = fields[idf].a + Vector2(i*pasX, j*pasY);
            Vector2 p1 = fields[idf].a + Vector2((i+1)*pasX, j*pasY);
            Vector2 p2 = fields[idf].a + Vector2(i*pasX, (j+1)*pasY);
            Vector2 p3 = fields[idf].a + Vector2((i+1)*pasX, (j+1)*pasY);
            hf.Bilineaire(p, z);
            glColor3f(z, z, z);
            glVertex3f(p.x, p.y, z);
            hf.Bilineaire(p1, z);
            glColor3f(z, z, z);
            glVertex3f(p1.x, p1.y, z);
            hf.Bilineaire(p2, z);
            glColor3f(z, z, z);
            glVertex3f(p2.x, p2.y, z);

            glEnd();

            glBegin(GL_TRIANGLES);
            hf.Bilineaire(p2, z);
            glColor3f(z, z, z);
            glVertex3f(p2.x, p2.y, z);

            hf.Bilineaire(p1, z);
            glColor3f(z, z, z);
            glVertex3f(p1.x, p1.y, z);
            hf.Bilineaire(p3, z);
            glColor3f(z, z, z);
            glVertex3f(p3.x, p3.y, z);

            glEnd();
        }

    }
    //glEnd();

}
