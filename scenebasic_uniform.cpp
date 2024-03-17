#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <sstream>

#include <iostream>
using std::cerr;
using std::endl;


#include "helper/glutils.h"
#include <glm/gtc/matrix_transform.hpp>




using glm::vec3;
using glm::mat4;


SceneBasic_Uniform::SceneBasic_Uniform() : plane(10.0f,10.0f,100,100) {
    mesh = ObjMesh::load("media/penquin.obj");
}

void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);
    model = mat4(1.0f);
    view = glm::lookAt(vec3(0.9f, 0.9f, 1.0f), vec3(0.2f, 1.9f, 0.2f), vec3(0.0f, 1.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(-35.0f), vec3(1.0f, 0.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(15.0f), vec3(0.0f, 1.0f, 0.0f));//rotate on y axis
    projection = mat4(1.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.2f, -0.8f, 0.2f));

    float x, z;
    for (int i = 0; i < 3; i++) {
        std::stringstream name;
        name << "lights[" << i << "].Position";
        x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
        z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
        prog.setUniform(name.str().c_str(), view * glm::vec4(x, 2.2f, z + 1.0f, 1.0f));
    }

    //prog.setUniform("Light.Position", view * glm::vec4(5.0f, 5.0f, 2.0f, 1.0f));
    prog.setUniform("lights[0].L", vec3(0.0f, 0.0f, 0.8f));
    prog.setUniform("lights[1].L", vec3(0.0f, 0.8f, 0.0f));
    prog.setUniform("lights[2].L", vec3(0.8f, 0.0f, 0.0f));

    prog.setUniform("lights[0].La", vec3(0.0f, 0.0f, 0.2f));
    prog.setUniform("lights[1].La", vec3(0.0f, 0.2f, 0.0f));
    prog.setUniform("lights[2].La", vec3(0.2f, 0.0f, 0.0f));



   // prog.setUniform("Light.La", vec3(0.4f, 0.4f, 0.4f));
    //prog.setUniform("Light.Ls", vec3(1.0f, 1.0f, 1.0f));


}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");

		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
    model = glm::rotate(model, glm::radians(0.1f), vec3(0.0f, 1.0f, 0.1f));
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    

    prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ka", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ks", vec3(0.8f, 0.8f, 0.8f));

    prog.setUniform("Material.Shininess", 1000.0f);


    setMatrices();
    mesh->render();
    //torus.render();

}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    
    projection = glm::perspective(glm::radians(70.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneBasic_Uniform::setMatrices() {
    mat4 mv = view*model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}