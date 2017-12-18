#include "drawfield.h"

DrawField::DrawField()
{

}


void DrawField::prepare()
{

    std::vector<double>::iterator result;
    result = std::max_element(fields.field.begin(), fields.field.end());
    double zm = *result;
    result = std::min_element(fields.field.begin(), fields.field.end());
    double minn = *result;
    if(minn < 0.0)
    {
        for (int i = 0; i < fields.h*fields.w; ++i) {
            fields.field[i] += minn;
        }
    }
    if(zm > 1.0){
        for (int i = 0; i < fields.h*fields.w; ++i) {
            fields.field[i] /= zm;
        }
    }
    ScalarField2 & hg = fields;
    for (int i = 0; i < hg.h; i++) {
        for (int j = 0; j < hg.w; j++) {


            Vector2 pos = hg.get(i, j);
            vertices.push_back(Vector3(pos, hg.field[hg.pos(i, j)]));
            colors.push_back(Vector3(hg.field[hg.pos(i, j)], hg.field[hg.pos(i, j)], hg.field[hg.pos(i, j)]));
            if((j+1) < hg.w && (i+1)<hg.h) triangles.push_back(Triangle((i*hg.w+j), (i*hg.w+(j+1)), ((i+1)*hg.w+j)));
            if((j+1) < hg.w && (i+1)<hg.h) triangles.push_back(Triangle((i*hg.w+(j+1)), ((i+1)*hg.w+(j+1)), ((i+1)*hg.w+j)));

        }
    }

}

void DrawField::addRivers(const ScalarField2 &sf){
    std::vector<double>::const_iterator result;
    std::cout << colors.size() << ", " << sf.h*sf.w << std::endl;
    result = std::max_element(sf.field.begin(), sf.field.end());
    double zm = sqrt(*result);
    for (int i = 0; i < fields.h; i++) {
        for (int j = 0; j < fields.w; j++) {
            if((sqrt(sf.field[sf.pos(i, j)])/zm) > 0.0) {
                colors[sf.pos(i, j)].z += (sqrt(sf.field[sf.pos(i, j)])/zm);
            }
        }
    }
}

void DrawField::prepareInterpol(int size1){
    std::vector<double>::iterator result;
    result = std::max_element(fields.field.begin(), fields.field.end());
    double zm = *result;
    result = std::min_element(fields.field.begin(), fields.field.end());
    double minn = *result;
    if(minn < 0.0)
    {
        for (int i = 0; i < fields.h*fields.w; ++i) {
            fields.field[i] += minn;
        }
    }
    if(zm > 1.0){
        for (int i = 0; i < fields.h*fields.w; ++i) {
            fields.field[i] /= zm;
        }
    }
    double pasX = (fields.b.x - fields.a.x)/(float)size1;
    double pasY = (fields.b.y - fields.a.y)/(float)size1;
    //glBegin(GL_POINTS);
    triangles.clear();
    int size = size1;
    HeightField & hf = static_cast<HeightField&>(fields);
    for (int i = 0; i < size1; ++i) {
        for (int j = 0; j < size1; ++j) {
            double z = 0;
            bool ok = true;
            Vector2 p = fields.a + Vector2(i*pasX, j*pasY);
            Vector2 p1 = fields.a + Vector2((i+1)*pasX, j*pasY);
            Vector2 p2 = fields.a + Vector2(i*pasX, (j+1)*pasY);
            Vector2 p3 = fields.a + Vector2((i+1)*pasX, (j+1)*pasY);
            //std::cout << "test" <<std::endl;
            hf.Bilineaire(p, z);
            //std::cout << "in" << std::endl;
            //hf.Barycentrique(p, z);
            vertices.push_back(Vector3(p, z));
            colors.push_back(Vector3(z, z, z));
            if(!testPoint(Vector3(p, 0), size1)) ok = false;
            if(!testPoint(Vector3(p1, 0), size1)) ok = false;
            if(!testPoint(Vector3(p2, 0), size1)) ok = false;
            if(ok) triangles.push_back(Triangle((i*size+j), ((i+1)*size+j), (i*size+(j+1))));

            if(!testPoint(Vector3(p3, 0), size1)) ok = false;
            if(ok) triangles.push_back(Triangle((i*size+(j+1)), ((i+1)*size+j), ((i+1)*size+(j+1))));

        }

    }
    std::cout << triangles.size() << std::endl;
}


bool DrawField::testPoint(const Vector3 &v3, int size){
    double marginx = (fields.b.x - fields.a.x)/(float)size;
    double marginy = (fields.b.y - fields.a.y)/(float)size;
    if((v3.x <= fields.a.x+marginx) || v3.y <= fields.a.y+marginy || v3.x >= fields.b.x-marginx || v3.y >= fields.b.y-marginy) return false;
    return true;
}


void DrawField::draw(){

    for (unsigned int i = 0; i < triangles.size(); ++i) {
        glBegin(GL_TRIANGLES);
        double z = vertices[triangles[i].vertices[0]].z;
        glColor3f(colors[triangles[i].vertices[0]].x, colors[triangles[i].vertices[0]].y, colors[triangles[i].vertices[0]].z);
        glVertex3f(vertices[triangles[i].vertices[0]].x, vertices[triangles[i].vertices[0]].y, z);
        z = vertices[triangles[i].vertices[1]].z;
        glColor3f(colors[triangles[i].vertices[1]].x, colors[triangles[i].vertices[1]].y, colors[triangles[i].vertices[1]].z);
        glVertex3f(vertices[triangles[i].vertices[1]].x, vertices[triangles[i].vertices[1]].y, z);

        z = vertices[triangles[i].vertices[2]].z;
        glColor3f(colors[triangles[i].vertices[2]].x, colors[triangles[i].vertices[2]].y, colors[triangles[i].vertices[2]].z);
        glVertex3f(vertices[triangles[i].vertices[2]].x, vertices[triangles[i].vertices[2]].y, z);
        glEnd();
    }

}
