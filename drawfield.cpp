#include "drawfield.h"

DrawField::DrawField() { idStartTree = 0;}


void DrawField::prepare()
{
    std::vector<double>::iterator result;
    result = std::max_element(fields.field.begin(), fields.field.end());
    double zm = *result+(*result/10.0);
    result = std::min_element(fields.field.begin(), fields.field.end());
    double minn = *result;
    std::cout << "max : " << zm << std::endl;
    result = std::max_element(fields.field.begin(), fields.field.end());

    treeVertices.resize(trees.size());
    treeTranslations.resize(trees.size());
    treeColors.resize(trees.size());
    larg.resize(trees.size());
    for (unsigned int i = 0; i < trees.size(); ++i) {
        larg[i]= trees[i].widthT*1.4;
    }

    ScalarField2 & hg = fields;
    for (int i = 0; i < hg.h; i++) {
        for (int j = 0; j < hg.w; j++) {
            Vector2 pos = hg.get(i, j);
            /*double wid = fields.b.x - fields.a.x;
            double hgt = fields.b.y - fields.a.y;*/
            vertices.push_back(Vector3(pos, hg.field[hg.pos(i, j)]));
            colors.push_back(Vector3((hg.field[hg.pos(i, j)]-minn)/zm+0.1, (hg.field[hg.pos(i, j)]-minn)/zm+0.1, (hg.field[hg.pos(i, j)]-minn)/zm+0.1));
            if((j+1) < hg.w && (i+1)<hg.h) triangles.push_back(Triangle((i*hg.w+j), (i*hg.w+(j+1)), ((i+1)*hg.w+j)));
            if((j+1) < hg.w && (i+1)<hg.h) triangles.push_back(Triangle((i*hg.w+(j+1)), ((i+1)*hg.w+(j+1)), ((i+1)*hg.w+j)));
        }
    }
}


void DrawField::addRivers(const ScalarField2 &sf){
    std::vector<double>::const_iterator result;
    //std::cout << colors.size() << ", " << sf.h*sf.w << std::endl;
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


void DrawField::addVeget(vegetationField &sf){
    std::vector<double>::const_iterator result;
    //std::cout << colors.size() << ", " << sf.h*sf.w << std::endl;
    //result = std::max_element(sf.field.begin(), sf.field.end());
    //double zm = sqrt(*result);

    //double upp = 0.0;
    idStartTree = triangles.size();
    for (int i = 0; i < sf.h; i++) {
        for (int j = 0; j < sf.w; j++) {
            if(sf.hasTree[sf.pos(i, j)]) {
                Vector2 tmpp= sf.get(i, j);

                std::pair<int, int> ij = fields.inside(Vector3(tmpp, 0.0));
                //colors[fields.pos(ij.first, ij.second)].y += (sqrt(sf.field[sf.pos(i, j)])/zm);

                double tz = fields.field[fields.pos(ij.first, ij.second)];
                treeTranslations[(int)sf.field[sf.pos(i, j)]].push_back(Vector3(tmpp.x, tmpp.y, tz));

            }
        }
    }
}



void DrawField::loadTreeObj(QString path, int pos){
    std::string inputfile = path.toStdString();
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    VAR_TYPE chang = pos/10.0;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());

    if (!err.empty()) { // `err` may contain warning message.
      std::cerr << err << std::endl;
    }

    if (!ret) {
      exit(1);
    }

    std::vector<tinyobj::real_t>::const_iterator result;
    //std::cout << colors.size() << ", " << sf.h*sf.w << std::endl;
    result = std::max_element(attrib.vertices.begin(), attrib.vertices.end());


    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
      // Loop over faces(polygon)
      Vector3 colorr = Vector3(0.0, 0.392157+chang, 0.0);
      size_t index_offset = 0;
      for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
        unsigned int fv = shapes[s].mesh.num_face_vertices[f];

        // Loop over vertices in the face.
        for (size_t v = 0; v < fv; v++) {
          // access to vertex
          tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
          tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
          tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
          tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];
          treeVertices[pos].push_back(Vector3(vx, vy, vz));
          /*tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
          tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
          tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];
          tinyobj::real_t tx = attrib.texcoords[2*idx.texcoord_index+0];
          tinyobj::real_t ty = attrib.texcoords[2*idx.texcoord_index+1];*/
          // Optional: vertex colors
          /*tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
          tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
          tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];*/
          if(vz > (*result - *result*0.82) )treeColors[pos].push_back(colorr);
          else treeColors[pos].push_back(Vector3(0.647059+chang, 0.164706+chang, 0.164706+chang));
        }
        index_offset += fv;

        // per-face material
        shapes[s].mesh.material_ids[f];
      }
    }
    //std::cout << shapes.size() << std::endl;
}

void DrawField::prepareInterpol(int size1){
    std::vector<double>::iterator result;
    result = std::max_element(fields.field.begin(), fields.field.end());
    double zm = *result;
    result = std::min_element(fields.field.begin(), fields.field.end());
    double minn = *result;
    for (int i = 0; i < fields.h*fields.w; ++i) {
        fields.field[i] -= minn;
    }

    for (int i = 0; i < fields.h*fields.w; ++i) {
        fields.field[i] /= (zm-minn);
    }
    //std::cout << zm << std::endl;
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

            hf.Bilineaire(p, z);
            //hf.Barycentrique(p, z);

            double wid = fields.b.x - fields.a.x;
            double hgt = fields.b.y - fields.a.y;
            p.x = ((p.x-fields.a.x)/wid)*2.0;
            p.y = ((p.y-fields.a.y)/hgt)*2.0;
            vertices.push_back(Vector3(p+Vector2(-1, -1), z));
            //std::cout << z <<"  " << p <<std::endl;
            colors.push_back(Vector3(z, z, z));
            if(!testPoint(Vector3(p, 0), size1)) ok = false;
            if(!testPoint(Vector3(p1, 0), size1)) ok = false;
            if(!testPoint(Vector3(p2, 0), size1)) ok = false;
            if(ok) triangles.push_back(Triangle((i*size+j), ((i+1)*size+j), (i*size+(j+1))));

            if(!testPoint(Vector3(p3, 0), size1)) ok = false;
            if(ok) triangles.push_back(Triangle((i*size+(j+1)), ((i+1)*size+j), ((i+1)*size+(j+1))));
        }
    }
}


bool DrawField::testPoint(const Vector3 &v3, int size){
    double marginx = (fields.b.x - fields.a.x)/(float)size;
    double marginy = (fields.b.y - fields.a.y)/(float)size;
    if((v3.x <= fields.a.x+marginx) || v3.y <= fields.a.y+marginy || v3.x >= fields.b.x-marginx || v3.y >= fields.b.y-marginy) return false;
    return true;
}


void DrawField::draw(bool showTree){

    unsigned int endLoop = triangles.size();

    if (idStartTree != 0 && !showTree) endLoop = idStartTree;
    for (unsigned int i = 0; i < endLoop; ++i) {
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

    if(showTree && treeTranslations.size() > 0){
        //std::cout << triangles.size() << ", " << idStartTree << ", " << std::endl;
        for (unsigned int l = 0; l < treeVertices.size(); ++l) {

            for (unsigned int i = 0; i < treeTranslations[l].size(); ++i) {

                for (unsigned int k = 0; k < treeVertices[l].size(); k+=3) {
                    glBegin(GL_TRIANGLES);
                    glColor3f(treeColors[l][k].x, treeColors[l][k].y, treeColors[l][k].z);
                    glVertex3f((treeTranslations[l][i].x+treeVertices[l][k].x/5.0*larg[l]), (treeTranslations[l][i].y+treeVertices[l][k].y/5.0*larg[l]), (treeTranslations[l][i].z+treeVertices[l][k].z/5.0*larg[l]));

                    glColor3f(treeColors[l][k+1].x, treeColors[l][k+1].y, treeColors[l][k+1].z);
                    glVertex3f((treeTranslations[l][i].x+treeVertices[l][k+1].x/5.0*larg[l]), (treeTranslations[l][i].y+treeVertices[l][k+1].y/5.0*larg[l]), (treeTranslations[l][i].z+treeVertices[l][k+1].z/5.0*larg[l]));


                    glColor3f(treeColors[l][k+2].x, treeColors[l][k+2].y, treeColors[l][k+2].z);
                    glVertex3f((treeTranslations[l][i].x+treeVertices[l][k+2].x/5.0*larg[l]), (treeTranslations[l][i].y+treeVertices[l][k+2].y/5.0*larg[l]), (treeTranslations[l][i].z+treeVertices[l][k+2].z/5.0*larg[l]));

                    glEnd();
                }
            }
        }
    }
}
