#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include <string>
#include <vector>

class aw_t {
public:
	std::vector<glm::vec3 >       verts;
	std::vector<unsigned int >    vi;
	std::string                   group;
	//type 0: v, type 1: v/vt, type 2: v//vn, type 3: v/vt/vn
	int type;
	void pack(GLfloat[]);
	aw_t(const char* ofile);
	glm::vec3 getCenter();
	int getSize();
	int getType() { return type; }
	std::vector<glm::vec3 > faceNormals();
	void computeNormals();
};
