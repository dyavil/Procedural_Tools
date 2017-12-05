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

void DrawField::prepareInterpol(int size1, int idf){
    double pasX = (fields[idf].b.x - fields[idf].a.x)/(float)size1;
    double pasY = (fields[idf].b.y - fields[idf].a.y)/(float)size1;
    //glBegin(GL_POINTS);
    triangles.clear();
    int size = size1;
    HeightField & hf = static_cast<HeightField&>(fields[idf]);
    for (int i = 0; i < size1; ++i) {
        for (int j = 0; j < size1; ++j) {
            double z = 0;
            bool ok = true;
            Vector2 p = fields[idf].a + Vector2(i*pasX, j*pasY);
            Vector2 p1 = fields[idf].a + Vector2((i+1)*pasX, j*pasY);
            Vector2 p2 = fields[idf].a + Vector2(i*pasX, (j+1)*pasY);
            Vector2 p3 = fields[idf].a + Vector2((i+1)*pasX, (j+1)*pasY);
            //std::cout << "test" <<std::endl;
            hf.Bilineaire(p, z);
            vertices.push_back(Vector3(p, z));

            if(p.x == fields[idf].a.x || p.y == fields[idf].a.y || p.x == fields[idf].b.x || p.y == fields[idf].b.y) ok = false;
            if(p1.x == fields[idf].a.x || p1.y == fields[idf].a.y || p1.x == fields[idf].b.x || p1.y == fields[idf].b.y) ok = false;
            if(p2.x == fields[idf].a.x || p2.y == fields[idf].a.y || p2.x == fields[idf].b.x || p2.y == fields[idf].b.y) ok = false;
            if(ok) triangles.push_back(Triangle((i*size+j), ((i+1)*size+j), (i*size+(j+1))));

            ok = true;

            if(p3.x == fields[idf].a.x || p3.y == fields[idf].a.y || p3.x == fields[idf].b.x || p3.y == fields[idf].b.y) ok = false;
            if(p1.x == fields[idf].a.x || p1.y == fields[idf].a.y || p1.x == fields[idf].b.x || p1.y == fields[idf].b.y) ok = false;
            if(p2.x == fields[idf].a.x || p2.y == fields[idf].a.y || p2.x == fields[idf].b.x || p2.y == fields[idf].b.y) ok = false;
            if(ok) triangles.push_back(Triangle((i*size+(j+1)), ((i+1)*size+j), ((i+1)*size+(j+1))));

        }

    }
    std::cout << triangles.size() << std::endl;
}


void DrawField::drawInterpol(){

    for (int i = 0; i < triangles.size(); ++i) {
        glBegin(GL_TRIANGLES);
        double z = vertices[triangles[i].vertices[0]].z;
        glColor3f(z, z, z);
        glVertex3f(vertices[triangles[i].vertices[0]].x, vertices[triangles[i].vertices[0]].y, z);
        z = vertices[triangles[i].vertices[1]].z;
        glColor3f(z, z, z);
        glVertex3f(vertices[triangles[i].vertices[1]].x, vertices[triangles[i].vertices[1]].y, z);

        z = vertices[triangles[i].vertices[2]].z;
        glColor3f(z, z, z);
        glVertex3f(vertices[triangles[i].vertices[2]].x, vertices[triangles[i].vertices[2]].y, z);
        glEnd();
    }

}
