#include "Framebuffer.h"
#include "Model.h"
#include "Shader.h"


void Model::Draw()
{
	Shader::Draw(m_vb);
}

bool Model::Load(const std::string& filename)
{
	// open file using ifstream (input file stream)
	std::ifstream stream(filename);
	// check if stream is_open
	if (!stream.is_open())
	{
		// TODO: report error to cerr and return false
		std::cerr << "stream not open" << filename << std::endl;
		return false;
	}

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::string line;
	while (std::getline(stream, line))
	{
		// read in vertex positions
		// https://cplusplus.com/reference/string/string/substr/
		if (line.substr(0, 2) == "v ")
		{
			// read position of vertex
			std::istringstream sstream{ line.substr(2) };
			glm::vec3 position;
			sstream >> position.x >> position.y >> position.z;

			// TODO: add position to vertices vector
			vertices.push_back(position);

		}
		else if (line.substr(0, 3) == "vn ")
		{
			unsigned int index[3] = { 0, 0, 0 }; // 0 = position, 1 = normal, 2 = uv

			// read position of vertex
			std::istringstream sstream{ line.substr(3) };
			glm::vec3 normal;
			sstream >> normal.x >> normal.y >> normal.z;

			// TODO: add position to vertices vector
			normals.push_back(normal);

			if (index[2] && index[1])
			{
				vertex_t vertex;
				vertex.position = vertices[index[0] - 1];
				vertex.normal = vertices[index[1] - 1];
			}

		}
		// read in faces (triangles)
		else if (line.substr(0, 2) == "f ")
		{
			// read face (triangle), index of vertex position in vertices array [1, 2, 3]
			std::istringstream sstream{ line.substr(2) };
			std::string str;
			while (std::getline(sstream, str, ' '))
			{
				std::istringstream sstream(str);
				std::string indexString;

				// read each triangle index (position, normal, uv)
				size_t i = 0;
				unsigned int index[3] = { 0, 0, 0 }; // 0 = position, 1 = normal, 2 = uv
				while (std::getline(sstream, indexString, '/'))
				{
					if (!indexString.empty())
					{
						std::istringstream indexStream{ indexString };
						indexStream >> index[i];
					}
					i++;
				}

				// check if index 0 (position) is valid
				if (index[0] != 0 && index[0] - 1 < vertices.size())
				{
					// get vertex at index position
					// index is 1 based, need to subtract one for array
					vertex_t vp;
					vp.position = vertices[index[0] - 1];
					vp.normal = normals[index[2] - 1];

					m_vb.push_back(vp);
				}
			}
		}	
	}
	stream.close();
	
	return true;
}

