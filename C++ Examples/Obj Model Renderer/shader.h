#include <GLFW/glfw3.h>
#include <iostream>

class Shader {
private:
  GLuint sfo, sgo, spo, svo;
public:
  Shader(const char *vfile, const char *ffile);
  Shader(const char *vfile, const char *ffile, const char *nfile);
  void use() { glUseProgram(spo); }
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  GLuint getID() { return spo; }
};
